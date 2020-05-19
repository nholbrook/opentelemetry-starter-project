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
using grpc::Status;
using foodfinder::SupplyRequest;
using foodfinder::SupplyResponse;
using foodfinder::FoodFinder;

class FoodFinderClient {
  public:
    FoodFinderClient(std::shared_ptr<Channel> channel)
      : stub_(FoodFinder::NewStub(channel)) {}

    std::string RequestSupplyInfo(const std::string& name) {
      SupplyRequest request;
      request.set_name(name);

      SupplyResponse response;
      ClientContext context;

      Status status = stub_->RequestSupplyInfo(&context, request, &response);

      if (status.ok()) {
        return response.test();
      } else {
	std::cout << status.error_details() << std::endl;
        std::cout << status.error_code() << ": " << status.error_message()
		<< std::endl;
	return "RPC failed";
      }
    }

  private:
    std::unique_ptr<FoodFinder::Stub> stub_;
};

int main(int argc, char** argv) {
  FoodFinderClient client(grpc::CreateChannel(
			  "localhost:50051", grpc::InsecureChannelCredentials()));
  std::string name("flour");
  std::string response = client.RequestSupplyInfo(name);
  std::cout << "Client received: " << response << std::endl;

  return 0;
}
