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

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "foodfinder.grpc.pb.h"
#include "helpers.cc"

using std::string;
using std::vector;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using grpc::StatusCode;
using foodfinder::SupplierService;
using foodfinder::SupplyRequest;
using foodfinder::Vendor;
using foodfinder::VendorResponse;
using foodfinder::Empty;

// TEMP: Temporary list of vendors. This will evebtually be moved to a MySQL DB.
vector<Vendor> vendors;

class SupplierImpl final : public SupplierService::Service {
  Status RequestVendorList(ServerContext* context, const SupplyRequest* request,
		VendorResponse* response) override {
    std::cout << "Request for Vendors with " << request->name()
      << " in stock" << std::endl;

    if (vendors.empty()) {
      return Status(StatusCode::NOT_FOUND, "No candidate vendors found");
    } else {
      for (const auto& vendor : vendors) {
        Vendor* current_vendor = response->add_vendors();
        current_vendor->set_url(vendor.url());
        current_vendor->set_name(vendor.name());
      }
      return Status::OK;
    }
  }

  Status RegisterVendor(ServerContext* context, const Vendor* request,
    Empty* response) {
      vendors.push_back(*request);
      std::cout << "Successfully registered " << request->name()
        << " as a new vendor at " << request->url() << std::endl;
      return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50052");
  SupplierImpl service;

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
  std::cout << "Supplier server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
