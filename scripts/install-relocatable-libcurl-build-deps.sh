#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

brew install automake # Used directly in `./scripts/build-*.sh` to build curl and its dependencies.
brew install brotli # For Brotli support.
brew install coreutils # So that `realpath` can be used.
brew install libidn2 # For Idn2 support.
brew install libtool # Nghttp3 needs it while running automake.
brew install nghttp2 # For HTTP2 support.
brew install pkg-config # Nghttp3 needs it while running the configure script. Refs: https://github.com/nghttp2/nghttp2/issues/620#issuecomment-541536650.
brew install zstd # For Zstd support.
