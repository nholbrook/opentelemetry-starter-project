#include <iostream>
#include <memory>
#include <string>
#include <cfloat>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "opencensus/trace/span.h"

#ifdef BAZEL_BUILD
#include "proto/foodfinder.grpc.pb.h"
#else
#include "foodfinder.grpc.pb.h"
#endif

class VendorClient {
  public:
    VendorClient(std::shared_ptr<grpc_impl::Channel> channel);

    foodfinder::InventoryResponse RequestInventoryList(
    	const foodfinder::SupplyRequest& request,
    	const opencensus::trace::Span& parentSpan);

  private:
    std::unique_ptr<foodfinder::VendorService::Stub> stub_;
};