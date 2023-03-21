#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

cp -R "$VENDOR/ngtcp2" "$BUILD"
cd "$BUILD/ngtcp2"
autoreconf -fi
./configure --prefix=$PREFIX PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig" --enable-lib-only --with-openssl
make install
