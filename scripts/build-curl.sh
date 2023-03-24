#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

cp -R "$VENDOR/curl" "$BUILD"
cd "$BUILD/curl"
autoreconf -fi
./configure \
  --prefix=$PREFIX \
  --with-openssl=$PREFIX \
  PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig" \
  --with-ngtcp2="$PREFIX" \
  --enable-warnings \
  --enable-werror \
  --enable-debug \
  --with-test-nghttpx="$PREFIX/bin/nghttpx"
make CFLAGS="-mmacosx-version-min=10.15 -Wno-deprecated-declarations" -j$(nproc)
make install
