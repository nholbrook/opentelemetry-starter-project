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

using std::string;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ClientReader;
using grpc::Status;
using foodfinder::FoodFinder;
using foodfinder::Supplier;
using foodfinder::Vendor;
using foodfinder::SupplyRequest;
using foodfinder::SupplyInfo;
using foodfinder::VendorInfo;
using foodfinder::InventoryInfo;

class SupplierClient {
  public:
    SupplierClient(std::shared_ptr<Channel> channel)
      : stub_(Supplier::NewStub(channel)) {}

    VendorInfo RequestVendorInfo(const std::string& name) {
      SupplyRequest request;
      request.set_name(name);

      ClientContext context;
      VendorInfo info;

      std::unique_ptr<ClientReader<VendorInfo>> reader (
        stub_->RequestVendorInfo(&context, request));

      while (reader->Read(&info)) {
        std::cout << "Potential vendor found" << info.name() << std::endl;
      }

      Status status = reader->Finish();

      if (status.ok()) {
        return info;
      } else {
        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
        return info;
      }
    }

  private:
    std::unique_ptr<Supplier::Stub> stub_;
};

class FoodFinderImpl final : public FoodFinder::Service {
  Status RequestSupplyInfo(ServerContext* context, const SupplyRequest* request,
     SupplyInfo* response) {
    
    SupplierClient supplier(grpc::CreateChannel(
         "localhost:50052", grpc::InsecureChannelCredentials()));
    VendorInfo vendor_info = supplier.RequestVendorInfo(request->name());

    VendorInfo* v_ptr = response->mutable_vendor();
    *v_ptr = vendor_info;
    std::cout << "Vendor recieved: " << response->vendor().name() << std::endl;
    return Status::OK;
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
  std::cout << "Food Finder server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
