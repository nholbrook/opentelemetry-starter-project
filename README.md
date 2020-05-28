# Opentelemetry Starter Project - Nick Holbrook

# Build
*Run all build commands from the root dir.*
- **Client:** docker build -t food-finder-client -f client/Dockerfile .
- **Food Finder:** docker build -t food-finder-server -f server/Dockerfile .
- **Supplier:** docker build -t supplier-server -f supplier/Dockerfile .
- **Vendor:** docker build -t vendor-server -f vendor/Dockerfile .

# Run
*Run all build commands from the root dir.*
- **Client:** docker run --rm --name food-finder-client --network="host" food-finder-client
- **Food Finder:** docker run --rm --network="host" --name food-finder-server food-finder-server
- **Supplier:** docker run --rm --network="host" --name supplier-server supplier-server
- **Vendor:** docker run --rm --network="host" --name vendor-server vendor-server
