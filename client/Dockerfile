FROM grpc/cxx:1.12.0

WORKDIR /data

ENV FOOD_NAME="milk"
ENV SERVICE_URL="localhost:50051"

COPY client/client.cc ./client/
COPY proto/foodfinder.proto ./proto/
COPY Makefile ./

RUN make food-finder-client && make clean

CMD ["sh", "-c", "./food-finder-client $FOOD_NAME $SERVICE_URL"]

