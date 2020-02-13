#include <emscripten/bind.h>

#include "HexEncoding.hpp"
#include "HDWallet.hpp"

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::register_vector<uint8_t>("VectorUint8");

    emscripten::class_<libcoins::HexEncoding>("HexEncoding")
        .class_function("from_hex", &libcoins::HexEncoding::from_hex)
        .class_function("to_hex", &libcoins::HexEncoding::to_hex)
    ;

    emscripten::class_<libcoins::HDWallet>("HDWallet")
        .class_function("hdnode_new", &libcoins::HDWallet::hdnode_new)
        .class_function("serialize_xprv", &libcoins::HDWallet::serialize_xprv)
    ;    
}