FROM grpc/cxx:1.12.0

WORKDIR /data

COPY client/client.cc ./client/
COPY proto/foodfinder.proto ./proto/
COPY Makefile ./

RUN make food-finder-client && make clean

CMD ["./food-finder-client"]

