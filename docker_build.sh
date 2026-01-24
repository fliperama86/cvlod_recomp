#!/bin/bash
set -e

# Build Docker image if needed
docker build -t cvlod-build .

# Run build inside Docker
docker run --rm -v "$(pwd)":/build cvlod-build bash -c "
    cd /build
    python3 tools/build.py
"
