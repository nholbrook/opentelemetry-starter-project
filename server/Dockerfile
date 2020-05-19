FROM grpc/cxx:1.12.0

WORKDIR /data

COPY server/server.cc ./server/
COPY proto/foodfinder.proto ./proto/
COPY Makefile ./

RUN make food-finder && make clean

CMD ["./food-finder"]

