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
using std::vector;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using grpc::StatusCode;
using foodfinder::Supplier;
using foodfinder::SupplyRequest;
using foodfinder::VendorInfo;

// TEMP: Temporary list of vendors. This will evebtually be moved to a MySQL DB.
vector<VendorInfo> vendors;

VendorInfo MakeVendor(string name, string url) {
  VendorInfo v;
  v.set_name(name);
  v.set_url(url);
  return v;
};

class SupplierImpl final : public Supplier::Service {
  Status RequestVendorInfo(ServerContext* context, const SupplyRequest* request,
		ServerWriter<VendorInfo>* writer) override {

    if (vendors.begin() == vendors.end()) {
      return Status(StatusCode::NOT_FOUND, "No candidate vendors found");
    } else {
      for (auto ptr = vendors.begin(); ptr != vendors.end(); ++ptr) {
        writer->Write(*ptr);
      }
      return Status::OK;
    }
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
  // TEMP: Populate temporary list of vendors. This will evebtually be moved to a MySQL DB.
  vendors.push_back(MakeVendor("Aldi", "localhost:50060"));
  vendors.push_back(MakeVendor("Trader Joe's", "localhost:50061"));
  vendors.push_back(MakeVendor("Whole Foods", "localhost:50062"));
  vendors.push_back(MakeVendor("Publix", "localhost:50063"));
  vendors.push_back(MakeVendor("Kroger", "localhost:50064"));
  vendors.push_back(MakeVendor("Meijer", "localhost:50065"));

  RunServer();

  return 0;
}
