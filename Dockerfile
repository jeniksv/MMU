FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
    curl \
    vim \
    expect \
    make \
    cmake \
    zip \
    unzip \
    libtinfo5 \
    g++ \
    g++-11 \
    libstdc++-11-dev \
    python3 \
    libusb-1.0-0-dev


WORKDIR /app

COPY src/ /app/src
COPY lib/ /app/lib
COPY scripts/ /app/scripts
COPY CMakeLists.txt /app
COPY tests/ /app/tests
COPY ccs.zip /

RUN cd / && unzip ccs.zip
RUN cd /app/ && /app/scripts/docker_path_correction.py
