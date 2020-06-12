load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

# OpenCensus
http_archive(
    name = "io_opencensus_cpp",
    strip_prefix = "opencensus-cpp-master",
    urls = ["https://github.com/census-instrumentation/opencensus-cpp/archive/master.zip"],
)

maybe(
    http_archive,
    name = "rules_cc",
    strip_prefix = "rules_cc-master",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/master.zip"],
)

# OpenCensus -> Abseil
maybe(
    http_archive,
    name = "com_google_absl",
    strip_prefix = "abseil-cpp-master",
    urls = ["https://github.com/abseil/abseil-cpp/archive/master.zip"]
)

# Google API
maybe(
    http_archive,
    name = "com_google_googleapis",
    strip_prefix = "googleapis-master",
    urls = ["https://github.com/googleapis/googleapis/archive/master.zip"],
)

load("@com_google_googleapis//:repository_rules.bzl", "switched_rules_by_language")

switched_rules_by_language(
    name = "com_google_googleapis_imports",
    cc = True,
    grpc = True,
)

http_archive(
    name = "com_google_protobuf",
    strip_prefix = "protobuf-master",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/master.zip"],
)

# gRPC
maybe(
    http_archive,
    name = "com_github_grpc_grpc",
    sha256 = "d6af0859d3ae4693b1955e972aa2e590d6f4d44baaa82651467c6beea453e30e",
    strip_prefix = "grpc-1.26.0-pre1",
    urls = [
        "https://github.com/grpc/grpc/archive/v1.26.0-pre1.tar.gz",
    ],
)


load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")

grpc_extra_deps()