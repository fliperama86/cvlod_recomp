#!/bin/bash
set -e
make -C build -j$(sysctl -n hw.ncpu)
./build/LodRecomp "$@"
