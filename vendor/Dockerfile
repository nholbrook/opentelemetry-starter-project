FROM grpc/cxx:1.12.0

WORKDIR /data

COPY vendor/vendor.cc ./vendor/
COPY proto/foodfinder.proto ./proto/
COPY Makefile helpers.cc ./

RUN make vendor-server

CMD ["./vendor-server"]