#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

cp -R "$VENDOR/nghttp2" "$BUILD"
cd "$BUILD/nghttp2"
autoreconf -fi
./configure --prefix="$PREFIX" PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig" --enable-http3
make -j"$(nproc)"
make install
