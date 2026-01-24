FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y \
    binutils-mips-linux-gnu \
    gcc-mips-linux-gnu \
    python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
