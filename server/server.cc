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

using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using foodfinder::SupplyRequest;
using foodfinder::SupplyResponse;
using foodfinder::VendorRequest;
using foodfinder::VendorResponse;
using foodfinder::InventoryRequest;
using foodfinder::InventoryResponse;
using foodfinder::FoodFinder;

class SupplierClient {
  public:
    SupplierClient(std::shared_ptr<Channel> channel)
      : stub_(FoodFinder::NewStub(channel)) {}

    std::string RequestVendorInfo(const std::string& name) {
      VendorRequest request;
      request.set_name(name);

      VendorResponse response;
      ClientContext context;

      Status status = stub_->RequestVendorInfo(&context, request, &response);

      if (status.ok()) {
        return response.vendors().Get(0).url();
      } else {
        std::cout << status.error_code() << ": " << status.error_message()
		<< std::endl;
	return "RPC failed";
      }
    }

  private:
    std::unique_ptr<FoodFinder::Stub> stub_;
};

class FoodFinderImpl final : public FoodFinder::Service {
  Status RequestSupplyInfo(ServerContext* context, const SupplyRequest* request,
		 SupplyResponse* response) override {
    
    SupplierClient supplier(grpc::CreateChannel(
			    "localhost:50051", grpc::InsecureChannelCredentials()));
    std::string name("flour");
    std::string vendor_info = supplier.RequestVendorInfo(name);
    std::cout << "Vendors recieved: " << vendor_info << std::endl;  

    response->set_test("test response");
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
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}
