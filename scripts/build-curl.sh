#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

cp -R "$VENDOR/curl" "$BUILD"
cd "$BUILD/curl"
autoreconf -fi
./configure \
  --prefix="$PREFIX" \
  --with-openssl="$PREFIX" \
  PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig" \
  --with-ngtcp2="$PREFIX" \
  --enable-warnings \
  --enable-werror \
  --enable-debug \
  --enable-websockets \
  --with-test-nghttpx="$PREFIX/bin/nghttpx" \
  --without-brotli \
  --without-libidn2 \
  --without-zstd
make CFLAGS="-Wno-deprecated-declarations" -j"$(nproc)"
make install
