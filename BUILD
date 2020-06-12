load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_proto_library", "cc_library")

licenses(["notice"])  # Apache License 2.0

package(default_visibility = ["//visibility:public"])

proto_library(
	name = "ff_proto",
	srcs = ["proto/foodfinder.proto"],
    deps = ["@com_google_protobuf//:empty_proto"]
)

cc_proto_library(
	name = "ff_cc_proto",
	deps = [":ff_proto"],
)

cc_grpc_library(
    name = "ff_cc_grpc",
    srcs = [":ff_proto"],
    grpc_only = True,
    deps = [":ff_cc_proto"],
)

cc_library(
    name = "helpers",
    srcs = ["helpers.cc"],
    hdrs = ["helpers.h"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":ff_cc_grpc",
        "@io_opencensus_cpp//opencensus/exporters/trace/stackdriver:stackdriver_exporter",
        "@io_opencensus_cpp//opencensus/exporters/trace/stdout:stdout_exporter",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc_client_channel",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "@com_google_absl//absl/strings"
    ]
)

cc_library(
	name = "exporters",
	srcs = ["exporters.cc"],
    hdrs = ["exporters.h"],
	deps = [
        "@io_opencensus_cpp//opencensus/exporters/trace/stackdriver:stackdriver_exporter",
        "@io_opencensus_cpp//opencensus/exporters/trace/stdout:stdout_exporter",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc_client_channel",
        "@com_google_absl//absl/strings"
	],
)

cc_library(
    name = "supplier_client",
    defines = ["BAZEL_BUILD"],
    srcs = ["food-finder/supplier-client.cc"],
    hdrs = ["supplier-client.h"],
    deps = [
        ":helpers",
        ":exporters",
        ":ff_cc_grpc",
        ":ff_cc_proto",
        "@io_opencensus_cpp//opencensus/tags",
        "@io_opencensus_cpp//opencensus/tags:context_util",
        "@io_opencensus_cpp//opencensus/tags:with_tag_map",
        "@io_opencensus_cpp//opencensus/trace",
        "@io_opencensus_cpp//opencensus/trace:context_util",
        "@io_opencensus_cpp//opencensus/trace:with_span",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc_opencensus_plugin",
        "@com_github_grpc_grpc//:grpc_client_channel",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
    ]
)

cc_library(
    name = "vendor_client",
    defines = ["BAZEL_BUILD"],
    srcs = ["food-finder/vendor-client.cc"],
    hdrs = ["vendor-client.h"],
    deps = [
        ":exporters",
        ":ff_cc_grpc",
        ":ff_cc_proto",
        "@io_opencensus_cpp//opencensus/tags",
        "@io_opencensus_cpp//opencensus/tags:context_util",
        "@io_opencensus_cpp//opencensus/tags:with_tag_map",
        "@io_opencensus_cpp//opencensus/trace",
        "@io_opencensus_cpp//opencensus/trace:context_util",
        "@io_opencensus_cpp//opencensus/trace:with_span",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc_opencensus_plugin",
        "@com_github_grpc_grpc//:grpc_client_channel",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
    ]
)

cc_binary(
	name = "vendor_server",
	srcs = ["vendor/vendor.cc"],
    defines = ["BAZEL_BUILD"],
	deps = [
        ":exporters",
        ":helpers",
        ":ff_cc_grpc",
        ":ff_cc_proto",
        ":supplier_client",
        ":vendor_client",
        "@io_opencensus_cpp//opencensus/tags",
        "@io_opencensus_cpp//opencensus/tags:context_util",
        "@io_opencensus_cpp//opencensus/tags:with_tag_map",
        "@io_opencensus_cpp//opencensus/trace",
        "@io_opencensus_cpp//opencensus/trace:context_util",
        "@io_opencensus_cpp//opencensus/trace:with_span",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc_opencensus_plugin",
        "@com_github_grpc_grpc//:grpc_client_channel",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
	]
)

cc_binary(
	name = "supplier_server",
	srcs = ["supplier/supplier.cc"],
    defines = ["BAZEL_BUILD"],
	deps = [
        ":exporters",
        ":helpers",
        ":ff_cc_grpc",
        ":ff_cc_proto",
        ":vendor_client",
        ":supplier_client",
        "@io_opencensus_cpp//opencensus/tags",
        "@io_opencensus_cpp//opencensus/tags:context_util",
        "@io_opencensus_cpp//opencensus/tags:with_tag_map",
        "@io_opencensus_cpp//opencensus/trace",
        "@io_opencensus_cpp//opencensus/trace:context_util",
        "@io_opencensus_cpp//opencensus/trace:with_span",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc_opencensus_plugin",
        "@com_github_grpc_grpc//:grpc_client_channel",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
	]
)

cc_binary(
	name = "food_finder_server",
	srcs = ["food-finder/food-finder.cc"],
    defines = ["BAZEL_BUILD"],
	deps = [
        ":exporters",
        ":helpers",
        ":ff_cc_grpc",
        ":ff_cc_proto",
        ":vendor_client",
        ":supplier_client",
        "@io_opencensus_cpp//opencensus/tags",
        "@io_opencensus_cpp//opencensus/tags:context_util",
        "@io_opencensus_cpp//opencensus/tags:with_tag_map",
        "@io_opencensus_cpp//opencensus/trace",
        "@io_opencensus_cpp//opencensus/trace:context_util",
        "@io_opencensus_cpp//opencensus/trace:with_span",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc_opencensus_plugin",
        "@com_github_grpc_grpc//:grpc_client_channel",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
	]
)

cc_binary(
    name = "food_finder_client",
    srcs = ["client/client.cc"],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":exporters",
        ":helpers",
        ":ff_cc_grpc",
        ":ff_cc_proto",
        "@io_opencensus_cpp//opencensus/tags",
        "@io_opencensus_cpp//opencensus/tags:context_util",
        "@io_opencensus_cpp//opencensus/tags:with_tag_map",
        "@io_opencensus_cpp//opencensus/trace",
        "@io_opencensus_cpp//opencensus/trace:context_util",
        "@io_opencensus_cpp//opencensus/trace:with_span",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_grpc_grpc//:grpc_opencensus_plugin",
        "@com_github_grpc_grpc//:grpc_client_channel",
        "@com_github_grpc_grpc//:grpc++_reflection",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
    ]
)