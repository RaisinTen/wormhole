#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

cp -R "$VENDOR/openssl" "$BUILD"
cd "$BUILD/openssl"
./config --prefix="$PREFIX" --libdir="$PREFIX"/lib
make -j"$(nproc)"
make install_sw
