# libcoins-emscripten

Emscripten playground for libcoins project.

## Pre-requisites

- [protobuf](https://github.com/protocolbuffers/protobuf) (also install `pip` version)
- [nanopb](https://github.com/nanopb/nanopb)
- [emscripten](https://github.com/emscripten-core/emscripten)

## Building

- Ensure correct `NANOPB_DIR` is set in [bootstrap.sh](bootstrap.sh)
- Source Emscripten SDK:

    ```sh
    $ source $HOME/sandbox/emsdk/emsdk_env.sh
    ```

- Build everything:

    ```sh
    $ ./bootstrap.sh
    ```

- Serve HTML files:

    ```sh
    $ emrun --port 8080 .
    ```
