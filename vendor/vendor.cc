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
#include <fstream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#ifdef BAZEL_BUILD
#include "proto/foodfinder.grpc.pb.h"
#else
#include "foodfinder.grpc.pb.h"
#endif

#include "food-finder/supplier-client.h"
#include "helpers.h"

using google::protobuf::Empty;
using std::string;
using std::vector;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::StatusCode;
using foodfinder::VendorService;
using foodfinder::SupplyRequest;
using foodfinder::Vendor;
using foodfinder::Item;
using foodfinder::InventoryResponse;

vector<Item> inventory;

class VendorImpl final : public VendorService::Service {
  Status RequestInventoryList(ServerContext* context, 
    const SupplyRequest* request, InventoryResponse* response) override {
      std::cout << "Request for Inventory for " << request->name() << std::endl;

      for (const auto& i : inventory) {
        Item* current_item = response->add_inventory();
        current_item->set_name(i.name());
        current_item->set_price(i.price());
        current_item->set_quantity(i.quantity());
      }
      return Status::OK;
  } 
};

void RunServer(Vendor v, std::string vendor_port, std::string supplier_address) {
  VendorImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(vendor_port, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Vendor server listening on " << v.url() << std::endl;

  std::cout << "Attempting connect to " << supplier_address << std::endl;
  SupplierClient supplier(grpc::CreateChannel(
    supplier_address, grpc::InsecureChannelCredentials()));
  std::cout << "Finish attempting connect to " << supplier_address << std::endl;

  Empty e = supplier.RegisterVendor(v);

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  std::string vendor_name = argv[1];
  std::string vendor_address = argv[2];
  std::string vendor_port = argv[3];
  std::string supplier_address = argv[4];

  Vendor v = MakeVendor(vendor_name, vendor_address);

  std::ifstream inventoryFile ("../volumes/inventory.txt");
  std::string name, price, quantity;

  inventoryFile >> name;

  while (inventoryFile >> name) {
    inventoryFile >> price;
    inventoryFile >> quantity;
    inventory.push_back(MakeItem(name, std::stof(price), std::stoi(quantity)));
  }
  inventoryFile.close();

  if (inventory.empty()) {
    std::cout << "CUR NAME: " << name << std::endl;
    std::cout << "ERROR: unable to read inventory" << std::endl;
    // return 0;
  }

  RunServer(v, vendor_port, supplier_address);

  return 0;
}
