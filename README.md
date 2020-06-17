# Opentelemetry Starter Project - Nick Holbrook

# Build
*Run all build commands from the root dir.*
- **All:** `bazel build :all`

# Run
*Run all build commands from the root dir.*
- **Client:** ./bazel-bin/food_finder_client milk localhost:50051
- **Food Finder:** ./bazel-bin/food_finder_server
- **Supplier:** ./bazel-bin/supplier_server
- **Vendor:** ./bazel-bin/vendor_server Meijer localhost:50061 0.0.0.0:50061 localhost:50052
