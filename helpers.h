#ifdef BAZEL_BUILD
#include "proto/foodfinder.grpc.pb.h"
#else
#include "foodfinder.grpc.pb.h"
#endif

#include <string>

foodfinder::Item MakeItem(std::string name, float price, int quantity);

foodfinder::Vendor MakeVendor(std::string name, std::string url);

foodfinder::SupplyResponse MakeSupply(foodfinder::Vendor v_info,
									  foodfinder::Item i_info);