# Opentelemetry Starter Project - Nick Holbrook

# Build
*Run all build commands from the root dir.*
- **Client:** docker build -t food-finder-client -f client/Dockerfile .
- **Food Finder:** docker build -t food-finder-server -f server/Dockerfile .
- **Supplier:** docker build -t supplier-server -f supplier/Dockerfile .

# Run
*Run all build commands from the root dir.*
- **Client:** docker run --rm --name food-finder-client --network="host" food-finder-client
- **Food Finder:** docker run --rm --network="host" --name food-finder-server food-finder-server
- **Supplier:** docker run --rm --network="host" --name supplier-server supplier-server