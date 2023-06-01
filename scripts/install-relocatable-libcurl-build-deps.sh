#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

brew install automake # Used directly in `./scripts/build-*.sh` to build curl and its dependencies.
brew install coreutils # So that `realpath` can be used.
brew install libtool # Nghttp3 needs it while running automake.
brew install pkg-config # Nghttp3 needs it while running the configure script. Refs: https://github.com/nghttp2/nghttp2/issues/620#issuecomment-541536650.
