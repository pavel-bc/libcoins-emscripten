# libcoins-emscripten

Emscripten playground for libcoins project.

## Pre-requisites

- [protobuf](https://github.com/protocolbuffers/protobuf)
- [nanopb](https://github.com/nanopb/nanopb)
- [emscripten](https://github.com/emscripten-core/emscripten)

## Building

- Ensure correct `NANOPB_DIR` is set in [Makefile](Makefile)
- Source Emscripten SDK:

    ```sh
    $ source $HOME/sandbox/emsdk/emsdk_env.sh
    ```

- Build protobuf & wasm artifacts:

    ```sh
    $ make proto build
    ```

- Serve HTML files:

    ```sh
    $ make serve
    ```
