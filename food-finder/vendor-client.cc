#include <iostream>
#include <memory>
#include <string>
#include <cfloat>

#include "vendor-client.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "opencensus/trace/span.h"

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

VendorClient::VendorClient(std::shared_ptr<Channel> channel)
      : stub_(VendorService::NewStub(channel)) {}

InventoryResponse VendorClient::RequestInventoryList(
  const SupplyRequest& request, const opencensus::trace::Span& parentSpan) {
  auto span = opencensus::trace::Span::StartSpan(
    "Fulfilling Inventory List Request", &parentSpan);
  ClientContext context;
  InventoryResponse inventory;
  Status status = stub_->RequestInventoryList(&context, request, &inventory);

  if (status.ok()) {
    span.End();
    return inventory;
  } else {
    std::cout << status.error_code() << ": " << status.error_message() 
      << std::endl;
    span.End();
    return inventory;
  }
}