FROM ubuntu:bionic

RUN apt-get update && \
	apt-get install -y git build-essential autoconf libtool pkg-config cmake

ENV GRPC_RELEASE_TAG v1.29.0
ENV BUILD_PATH /var/local/dev/grpc

