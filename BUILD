load("@io_opencensus_cpp//opencensus:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

licenses(["notice"])  # Apache License 2.0

proto_library(
	name = "ff_proto",
	srcs = ["proto/foodfinder.proto"]
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
	name = "exporters",
	srcs = ["exporters.cc"],
	copts = DEFAULT_COPTS,
	deps = [
        "@io_opencensus_cpp//opencensus/exporters/trace/stackdriver:stackdriver_exporter",
        "@io_opencensus_cpp//opencensus/exporters/trace/stdout:stdout_exporter",
        "@com_google_absl//absl/strings",
	],
)

cc_binary(
	name = "vendor_server",
	srcs = ["vendor/vendor.cc"],
	copts = DEFAULT_COPTS,
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
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
	]
)

cc_binary(
	name = "supplier_server",
	srcs = ["supplier/supplier.cc"],
	copts = DEFAULT_COPTS,
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
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
	]
)

cc_binary(
	name = "food_finder_server",
	srcs = ["food-finder/food-finder.cc"],
	copts = DEFAULT_COPTS,
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
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
	]
)