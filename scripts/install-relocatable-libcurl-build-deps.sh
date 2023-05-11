#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

ARCH=$(uname -m)
RELOCATABLE_LIBCURL_DIR="relocatable_libcurl_$ARCH"
FORCE_REINSTALL=false
if [ $# -gt 0 ]; then
    case "$1" in
        --force)
            FORCE_REINSTALL=true
            ;;
        *)
    esac
fi

if [ -d "$RELOCATABLE_LIBCURL_DIR" ] && [ "$FORCE_REINSTALL" = false ]; then
  echo "$RELOCATABLE_LIBCURL_DIR already exists. Skip installing dependencies."
  echo "Please remove it first if you want to reinstall or use --force option."
  exit 0
fi

brew install automake   # Used directly in `./scripts/build-*.sh` to build curl and its dependencies.
brew install coreutils  # So that `realpath` can be used.
brew install libtool    # Nghttp3 needs it while running automake.
brew install pkg-config # Nghttp3 needs it while running the configure script. Refs: https://github.com/nghttp2/nghttp2/issues/620#issuecomment-541536650.
