FROM grpc/cxx:1.12.0

WORKDIR /data

COPY food-finder/food-finder.cc ./food-finder/
COPY proto/foodfinder.proto ./proto/
COPY Makefile ./

RUN make food-finder-server

CMD ["./food-finder-server"]

