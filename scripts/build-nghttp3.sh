#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

cp -R "$VENDOR/nghttp3" "$BUILD"
cd "$BUILD/nghttp3"
autoreconf -fi
./configure --prefix="$PREFIX" PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig" --enable-lib-only
make -j"$(nproc)"
make install
