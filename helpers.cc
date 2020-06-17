#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#ifdef BAZEL_BUILD
#include "proto/foodfinder.grpc.pb.h"
#else
#include "foodfinder.grpc.pb.h"
#endif

using foodfinder::Vendor;
using foodfinder::Item;
using foodfinder::SupplyResponse;

Item MakeItem(std::string name, float price, int quantity) {
  Item i;
  i.set_name(name);
  i.set_price(price);
  i.set_quantity(quantity);
  return i;
};

Vendor MakeVendor(std::string name, std::string url) {
  Vendor v;
  v.set_name(name);
  v.set_url(url);
  return v;
};

SupplyResponse MakeSupply(Vendor v_info, Item i_info) {
	SupplyResponse s;

	Vendor* v_ptr = s.mutable_vendor();
	*v_ptr = v_info;

	Item* i_ptr = s.mutable_inventory();
	*i_ptr = i_info;

	return s;
}