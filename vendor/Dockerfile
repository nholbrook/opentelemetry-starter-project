FROM grpc/cxx:1.12.0

WORKDIR /data

ENV VENDOR_NAME="Aldi"
ENV VENDOR_ADDRESS="localhost:50060"
ENV SUPPLIER_ADDRESS="localhost:50052"

COPY vendor/vendor.cc ./vendor/
COPY proto/foodfinder.proto ./proto/
COPY Makefile helpers.cc food-finder/supplier-client.cc ./

RUN make vendor-server

CMD ["./vendor-server"]
CMD ["sh", "-c", "./vendor-server $VENDOR_NAME $VENDOR_ADDRESS $SUPPLIER_ADDRESS"]