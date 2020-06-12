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

using grpc::Channel;
using grpc::ClientContext;
using grpc::Server;
using grpc::Status;
using foodfinder::VendorService;
using foodfinder::SupplyRequest;
using foodfinder::InventoryResponse;

class VendorClient {
  public:
    VendorClient(std::shared_ptr<Channel> channel)
      : stub_(VendorService::NewStub(channel)) {}

    InventoryResponse RequestInventoryList(const SupplyRequest& request) {
      ClientContext context;
      InventoryResponse inventory;
      Status status = stub_->RequestInventoryList(&context, request, &inventory);

      if (status.ok()) {
        return inventory;
      } else {
        std::cout << status.error_code() << ": " << status.error_message() 
          << std::endl;
        return inventory;
      }
    }

  private:
    std::unique_ptr<VendorService::Stub> stub_;
};