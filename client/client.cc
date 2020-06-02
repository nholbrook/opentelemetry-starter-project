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
using foodfinder::FoodFinderService;
using foodfinder::SupplyRequest;
using foodfinder::SupplyResponse;
using foodfinder::Vendor;

class FoodFinderClient {
  public:
    FoodFinderClient(std::shared_ptr<Channel> channel)
      : stub_(FoodFinderService::NewStub(channel)) {}

    Status RequestSupplyInfo(const std::string& name, SupplyResponse& s_i) {
      SupplyRequest request;
      request.set_name(name);

      SupplyResponse response;
      ClientContext context;

      Status status = stub_->RequestSupplyInfo(&context, request, &response);

      if (status.ok()) {
        s_i = response;
      } else {
	      std::cout << status.error_details() << std::endl;
        std::cout << status.error_code() << ": " << status.error_message()
          << std::endl;
      }
      return status;
    }

  private:
    std::unique_ptr<FoodFinderService::Stub> stub_;
};

int main(int argc, char** argv) {
  std::string food_str = "milk";
  std::string target_str = "localhost:50051";

  FoodFinderClient client(grpc::CreateChannel(
			  target_str, grpc::InsecureChannelCredentials()));
  SupplyResponse s_i;
  Status status = client.RequestSupplyInfo(food_str, s_i);
  if (status.ok()) {
    std::cout << "Best Price for " << food_str << " found at "
      << s_i.vendor().name() << " for a price of $" << s_i.inventory().price()
      << std::endl;
  } else {
    std::cout << "Could not find " << food_str << " at any indexed stores"
      << std::endl;
  }
  return 0;
}