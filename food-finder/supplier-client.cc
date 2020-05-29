#include <iostream>
#include <memory>
#include <string>
#include <cfloat>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using foodfinder::SupplierService;
using foodfinder::SupplyRequest;
using foodfinder::VendorResponse;
using foodfinder::Vendor;
using foodfinder::Empty;

class SupplierClient {
  public:
    SupplierClient(std::shared_ptr<Channel> channel)
      : stub_(SupplierService::NewStub(channel)) {}

    VendorResponse RequestVendorList(const SupplyRequest& request) {
      ClientContext context;
      VendorResponse vendors;
      Status status = stub_->RequestVendorList(&context, request, &vendors);

      if (status.ok()) {
        return vendors;
      } else {
        std::cout << status.error_code() << ": " << status.error_message()
          << std::endl;
          return vendors;
      }
    }

    Empty RegisterVendor(const Vendor& request) {
      ClientContext context;
      Empty e;
      Status status = stub_->RegisterVendor(&context, request, &e);

      if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message()
          << std::endl;
      }
      return e;
    }

  private:
    std::unique_ptr<SupplierService::Stub> stub_;
};