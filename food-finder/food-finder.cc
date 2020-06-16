/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <cfloat>

#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#ifdef BAZEL_BUILD
#include "proto/foodfinder.grpc.pb.h"
#else
#include "foodfinder.grpc.pb.h"
#endif

#include "helpers.h"
#include "exporters.h"
#include "food-finder/supplier-client.h"
#include "food-finder/vendor-client.h"

#include "opencensus/trace/sampler.h"
#include "opencensus/trace/span.h"

using std::string;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ClientReader;
using grpc::Status;
using grpc::StatusCode;
using foodfinder::FoodFinderService;
using foodfinder::SupplierService;
using foodfinder::VendorService;
using foodfinder::SupplyRequest;
using foodfinder::SupplyResponse;
using foodfinder::VendorResponse;
using foodfinder::InventoryResponse;
using foodfinder::Vendor;
using foodfinder::Item;

class FoodFinderImpl final : public FoodFinderService::Service {

  Status RequestSupplyInfo(ServerContext* context, const SupplyRequest* request,
     SupplyResponse* response) {

    std::cout << "pre sampler" << std::endl;

    static opencensus::trace::AlwaysSampler sampler;

    std::cout << "post sampler" << std::endl;

    // Start Span
    auto span = opencensus::trace::Span::StartSpan("RequestSupplyInfo", nullptr,
      {&sampler});
    
    SupplierClient supplier(grpc::CreateChannel(
      "localhost:50052", grpc::InsecureChannelCredentials()));

    // Start Span
    span.AddAnnotation("Requesting Vendor List");
    VendorResponse vendors = supplier.RequestVendorList(*request, span);
    // End Span

    Item candidate_item = MakeItem("", FLT_MAX, 0);
    Vendor candidate_vendor = MakeVendor("", "");

    for (size_t v_i = 0; v_i < (size_t)vendors.vendors_size(); ++v_i) {
      Vendor current_vendor = vendors.vendors(v_i);
      VendorClient vendor(grpc::CreateChannel(
        current_vendor.url(), grpc::InsecureChannelCredentials()));
      // Start Span
      span.AddAnnotation("Requesting Inventory List From: "
        + current_vendor.name());
      InventoryResponse inventory = vendor.RequestInventoryList(
        *request, span);
      // End Span
      for (size_t i_i = 0; i_i < (size_t)inventory.inventory_size(); ++i_i) {
        Item current_item = inventory.inventory(i_i);
        std::cout << current_vendor.name() << " - has " 
          << current_item.quantity() << " of " << current_item.name() <<
          " in stock at a price of " << current_item.price() << std::endl;
        if (current_item.name() == request->name()
          && current_item.quantity() >= request->quantity() 
          && current_item.price() < candidate_item.price()) {
          candidate_item = current_item;
          candidate_vendor = current_vendor;
        }
      }
    }

    if (candidate_item.quantity() != 0) {
      Vendor* v_ptr = response->mutable_vendor();
      *v_ptr = candidate_vendor;

      Item* i_ptr = response->mutable_inventory();
      *i_ptr = candidate_item;

      // End Span
      span.End();
      return Status::OK;
    } else {
      span.End();
      return Status(StatusCode::NOT_FOUND, "No candidate vendors found");
    }
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  FoodFinderImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Food Finder server listening on " << server_address
    << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RegisterExporters();

  opencensus::trace::TraceConfig::SetCurrentTraceParams(
      {128, 128, 128, 128, opencensus::trace::ProbabilitySampler(1.0)});

  RunServer();

  return 0;
}
