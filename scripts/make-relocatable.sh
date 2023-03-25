#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

# This script makes libcurl and the libraries it depends on relocatable, so that
# it is possible to move the directory containing these libraries to another
# location and still be able to link to these and use these correctly.
# This is done by manually checking the output of `otool -l` on each of these
# libraries and replacing the absolute paths of the libraries into relative
# paths with the `@loader_path` symbol, which is the path of the directory that
# contains the library, by using the `install_name_tool` tool. The install name
# of the entrypoint library is also changed into a path relative to the `@rpath`
# path. The assumed `@rpath` here is the `relocatable` directory.

cd "$PROJECT_ROOT"

rm -rf relocatable
mkdir relocatable
cd relocatable

# Copy the includes directory.
cp -R "$PREFIX/include" .

mkdir lib
cd lib

# Copy libraries by resolving the symlinks, so that `-lcurl` could be passed
# to the linker instead of `-lcurl.4`.
cp "$PREFIX/lib/libcrypto.dylib" .
cp "$PREFIX/lib/libcurl.dylib" .
cp "$PREFIX/lib/libnghttp3.dylib" .
cp "$PREFIX/lib/libngtcp2.dylib" .
cp "$PREFIX/lib/libngtcp2_crypto_openssl.dylib" .
cp "$PREFIX/lib/libssl.dylib" .

# Set the install name of the entrypoint dylib - libcurl.
# So here, the requirement is that whatever dylib / executable that attempts to
# load this should have an rpath pointing to the `relocatable` directory.
install_name_tool -id "@rpath/lib/libcurl.dylib" "libcurl.dylib"

# Set the install names of the dylibs. This step is totally optional because
# these libraries are currently only loaded by libraries in the
# `relocatable/lib` directory and not from any client application which would
# load these indirectly through an rpath. This is being because of stylistic
# reasons so that `otool -l` on these libraries don't contain the original paths
# of these libraries which is only sensible on the host system where these were
# built.
install_name_tool -id "@rpath/lib/libcrypto.dylib" "libcrypto.dylib"
install_name_tool -id "@rpath/lib/libnghttp3.dylib" "libnghttp3.dylib"
install_name_tool -id "@rpath/lib/libngtcp2.dylib" "libngtcp2.dylib"
install_name_tool -id "@rpath/lib/libngtcp2_crypto_openssl.dylib" "libngtcp2_crypto_openssl.dylib"
install_name_tool -id "@rpath/lib/libssl.dylib" "libssl.dylib"

# Change the dependent shared library install names in libcurl
install_name_tool -change "$PREFIX/lib/libnghttp3.3.dylib" "@loader_path/libnghttp3.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libngtcp2_crypto_openssl.4.dylib" "@loader_path/libngtcp2_crypto_openssl.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libngtcp2.10.dylib" "@loader_path/libngtcp2.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libssl.81.3.dylib" "@loader_path/libssl.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libcrypto.81.3.dylib" "@loader_path/libcrypto.dylib" "libcurl.dylib"

# Change the dependent shared library install names in libngtcp2_crypto_openssl
install_name_tool -change "$PREFIX/lib/libngtcp2.10.dylib" "@loader_path/libngtcp2.dylib" "libngtcp2_crypto_openssl.dylib"
install_name_tool -change "$PREFIX/lib/libssl.81.3.dylib" "@loader_path/libssl.dylib" "libngtcp2_crypto_openssl.dylib"
install_name_tool -change "$PREFIX/lib/libcrypto.81.3.dylib" "@loader_path/libcrypto.dylib" "libngtcp2_crypto_openssl.dylib"

# Change the dependent shared library install names in libssl
install_name_tool -change "$PREFIX/lib/libcrypto.81.3.dylib" "@loader_path/libcrypto.dylib" "libssl.dylib"

# TODO(RaisinTen): Also include the system dependencies in the `relocatable/lib`
# directory as relocatable libraries.
