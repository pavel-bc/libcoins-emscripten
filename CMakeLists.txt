cmake_minimum_required(VERSION 3.0.0)

project(LibCoins)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(EMSDK_ROOT $ENV{EMSCRIPTEN})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${EMSDK_ROOT}/cmake/Modules")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${EMSDK_ROOT}/cmake/Modules")

# Configure dependencies
add_subdirectory(trezor-crypto)

# Configure core library
file(GLOB_RECURSE sources src/*.c src/*.cc src/*.cpp src/*.h)
set(LIBCOINS_SRC ${sources})

add_library(CoinsCore ${LIBCOINS_SRC})
target_link_libraries(CoinsCore PRIVATE TrezorCrypto)
target_include_directories(CoinsCore
    PRIVATE
        src
        src/nanopb
)

# Configure JS executable
add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
        src/nanopb
)

target_link_libraries(${PROJECT_NAME}
    # Link to core library & dependencies
    TrezorCrypto
    CoinsCore

    # Emscripten compiler options
    "-o ${CMAKE_CURRENT_SOURCE_DIR}/dist/libcoins.js"
    "--bind"
    "-O3"
    "--closure 1"
    "-s WASM=1"
    "-s MODULARIZE=1"
    "-s EXPORT_NAME=${PROJECT_NAME}"
)