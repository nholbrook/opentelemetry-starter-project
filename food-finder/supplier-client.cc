#include <iostream>
#include <memory>
#include <string>
#include <cfloat>

#include "food-finder/supplier-client.h"

#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "opencensus/trace/span.h"

#ifdef BAZEL_BUILD
#include "proto/foodfinder.grpc.pb.h"
#else
#include "foodfinder.grpc.pb.h"
#endif

using google::protobuf::Empty;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using foodfinder::SupplierService;
using foodfinder::SupplyRequest;
using foodfinder::VendorResponse;
using foodfinder::Vendor;


SupplierClient::SupplierClient(std::shared_ptr<Channel> channel)
  : stub_(SupplierService::NewStub(channel)) {}

VendorResponse SupplierClient::RequestVendorList(const SupplyRequest& request,
  const opencensus::trace::Span& parentSpan) {
  auto span = opencensus::trace::Span::StartSpan(
    "Fulfilling Vendor List Request", &parentSpan);
  ClientContext context;
  VendorResponse vendors;
  Status status = stub_->RequestVendorList(&context, request, &vendors);

  if (status.ok()) {
    span.End();
    return vendors;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
      << std::endl;
      span.End();
      return vendors;
  }
}

Empty SupplierClient::RegisterVendor(const Vendor& request) {
  ClientContext context;
  Empty e;
  Status status = stub_->RegisterVendor(&context, request, &e);

  if (!status.ok()) {
    std::cout << status.error_code() << ": " << status.error_message()
      << std::endl;
  }
  return e;
}