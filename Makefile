# Overrides
EXPORT_NAME = LibCoins
NANOPB_DIR = $(HOME)/sandbox/nanopb
PROTOC = $(NANOPB_DIR)/generator/protoc
PROTOC_OPTS = --plugin=protoc-gen-nanopb=$(NANOPB_DIR)/generator/protoc-gen-nanopb

SRC_DIR   = src
PROTO_DIR = proto
BUILD_DIR = build

# Flags
CSRC  = $(SRC_DIR)/proto/*.c
CSRC += $(SRC_DIR)/nanopb/*.c
CSRC += $(SRC_DIR)/trezor/*.c
CSRC += $(SRC_DIR)/trezor/ed25519-donna/*.c

CFLAGS = -O3 --closure 1 -s MODULARIZE=1 -s 'EXPORT_NAME="$(EXPORT_NAME)"'

INCLUDE_FLAGS  = -I$(SRC_DIR)
INCLUDE_FLAGS += -I$(SRC_DIR)/nanopb
INCLUDE_FLAGS += -I$(SRC_DIR)/trezor

OBJECTS  = $(BUILD_DIR)/main.o
OBJECTS += $(BUILD_DIR)/HexEncoding.o
OBJECTS += $(BUILD_DIR)/HDWallet.o

# Commands
proto:
	mkdir -p $(BUILD_DIR)
	$(PROTOC) $(PROTOC_OPTS) --nanopb_out=$(SRC_DIR) $(PROTO_DIR)/libcoins.proto

build:
	emcc --bind -c $(SRC_DIR)/main.cpp $(INCLUDE_FLAGS) $(CFLAGS) -o $(BUILD_DIR)/main.o
	emcc --bind -c $(SRC_DIR)/HexEncoding.cpp $(INCLUDE_FLAGS) $(CFLAGS) -o $(BUILD_DIR)/HexEncoding.o
	emcc --bind -c $(SRC_DIR)/HDWallet.cpp $(INCLUDE_FLAGS) $(CFLAGS) -o $(BUILD_DIR)/HDWallet.o
	emcc --bind $(CSRC) $(INCLUDE_FLAGS) $(OBJECTS) $(CFLAGS) -o $(BUILD_DIR)/libcoins.js

clean:
	rm -rf $(SRC_DIR)/proto
	rm -rf $(BUILD_DIR)

serve:
	emrun --verbose --no_browser --port 8080 .
	
.PHONY: proto build clean serve