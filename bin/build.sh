#!/bin/bash
set -e

# Configure
/bin/cmake --no-warn-unused-cli \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++ \
    -S$PWD \
    -B$PWD/build \
    -G Ninja

# Build
cmake --build $PWD/build --config Debug --target all -j $(nproc) --