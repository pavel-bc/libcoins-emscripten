#!/usr/bin/env bash

# Config
NANOPB_DIR=$HOME/sandbox/nanopb
PROTOC=$NANOPB_DIR/generator/protoc
PROTOC_OPTS=--plugin=protoc-gen-nanopb=$NANOPB_DIR/generator/protoc-gen-nanopb

# Build & artifact directories
echo "📁 Creating directories..."
rm -rf build dist src/proto
mkdir build dist src/proto

# Compile Protobuf
echo "🚀 Compiling protocol buffers..."
$PROTOC $PROTOC_OPTS --nanopb_out=src proto/libcoins.proto

# Generate project build files
echo "📄 Generating build files..."
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Modules/Platform/Emscripten.cmake

# Build
echo "👷 Building project..."
cmake --build .
