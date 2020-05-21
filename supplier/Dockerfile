FROM grpc/cxx:1.12.0

WORKDIR /data

COPY supplier/supplier.cc ./supplier/
COPY proto/foodfinder.proto ./proto/
COPY Makefile ./

RUN make supplier-server && make clean

CMD ["./supplier-server"]