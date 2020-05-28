#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "foodfinder.grpc.pb.h"

using foodfinder::VendorInfo;
using foodfinder::InventoryInfo;
using foodfinder::SupplyInfo;

InventoryInfo MakeInventory(float price, int quantity) {
  InventoryInfo i;
  i.set_price(price);
  i.set_quantity(quantity);
  return i;
};

VendorInfo MakeVendor(std::string name, std::string url) {
  VendorInfo v;
  v.set_name(name);
  v.set_url(url);
  return v;
};

SupplyInfo MakeSupply(VendorInfo v_info, InventoryInfo i_info) {
	SupplyInfo s;

	VendorInfo* v_ptr = s.mutable_vendor();
	*v_ptr = v_info;

	InventoryInfo* i_ptr = s.mutable_inventory();
	*i_ptr = i_info;

	return s;
}