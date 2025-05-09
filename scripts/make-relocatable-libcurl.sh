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
# path. The assumed `@rpath` here is the `relocatable_libcurl_<arch>` directory.

cd "$PROJECT_ROOT"

RELOCATABLE_LIBCURL_DIR="relocatable_libcurl_$(uname -m)"

rm -rf "$RELOCATABLE_LIBCURL_DIR"
mkdir "$RELOCATABLE_LIBCURL_DIR"
cd "$RELOCATABLE_LIBCURL_DIR"

# Copy the includes directory.
mkdir include
cp -R "$PREFIX/include/curl" include

mkdir lib
cd lib

# Copy libraries by resolving the symlinks, so that `-lcurl` could be passed
# to the linker instead of `-lcurl.4`.

# Copy built libraries.
cp "$PREFIX/lib/libcrypto.dylib" .
cp "$PREFIX/lib/libcurl.dylib" .
cp "$PREFIX/lib/libnghttp3.dylib" .
cp "$PREFIX/lib/libnghttp2.dylib" .
cp "$PREFIX/lib/libngtcp2.dylib" .
cp "$PREFIX/lib/libngtcp2_crypto_openssl.dylib" .
cp "$PREFIX/lib/libssl.dylib" .

# Set the identification name of the entrypoint dylib - libcurl.
# So here, the requirement is that whatever dylib / executable that attempts to
# load this should have an rpath pointing to the `relocatable_libcurl_<arch>`
# directory.
install_name_tool -id "@rpath/lib/libcurl.dylib" "libcurl.dylib"

# Set the identification names of the dylibs. This step is totally optional
# because these libraries are currently only loaded by libraries in the
# `relocatable_libcurl_<arch>/lib` directory and not from any client application which
# would load these indirectly through an rpath. This is being because of
# stylistic reasons so that `otool -l` on these libraries don't contain the
# original paths of these libraries which is only sensible on the host system
# where these were built.

# Change identification names of built libraries.
install_name_tool -id "@rpath/lib/libcrypto.dylib" "libcrypto.dylib"
install_name_tool -id "@rpath/lib/libnghttp3.dylib" "libnghttp3.dylib"
install_name_tool -id "@rpath/lib/libnghttp2.dylib" "libnghttp2.dylib"
install_name_tool -id "@rpath/lib/libngtcp2.dylib" "libngtcp2.dylib"
install_name_tool -id "@rpath/lib/libngtcp2_crypto_openssl.dylib" "libngtcp2_crypto_openssl.dylib"
install_name_tool -id "@rpath/lib/libssl.dylib" "libssl.dylib"

# Change the dependent built shared library install names in libcurl
install_name_tool -change "$PREFIX/lib/libnghttp3.3.dylib" "@loader_path/libnghttp3.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libnghttp2.14.dylib" "@loader_path/libnghttp2.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libngtcp2_crypto_openssl.4.dylib" "@loader_path/libngtcp2_crypto_openssl.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libngtcp2.12.dylib" "@loader_path/libngtcp2.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libssl.81.3.dylib" "@loader_path/libssl.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libcrypto.81.3.dylib" "@loader_path/libcrypto.dylib" "libcurl.dylib"

# TODO(RaisinTen): Add assertions to make sure that the old path in
# `install_name_tool -change old new file` command exists. Currently, if it
# doesn't exist the command doesn't indicate that in anyway.

# Change the dependent shared library install names in libngtcp2_crypto_openssl
install_name_tool -change "$PREFIX/lib/libngtcp2.12.dylib" "@loader_path/libngtcp2.dylib" "libngtcp2_crypto_openssl.dylib"
install_name_tool -change "$PREFIX/lib/libssl.81.3.dylib" "@loader_path/libssl.dylib" "libngtcp2_crypto_openssl.dylib"
install_name_tool -change "$PREFIX/lib/libcrypto.81.3.dylib" "@loader_path/libcrypto.dylib" "libngtcp2_crypto_openssl.dylib"

# Change the dependent shared library install names in libssl
install_name_tool -change "$PREFIX/lib/libcrypto.81.3.dylib" "@loader_path/libcrypto.dylib" "libssl.dylib"

# Replace the invalidated signature.
# NOTE: We tried removing the signatures before running the `install_name_tool`
# command but `install_name_tool` was failing with the error discussed in
# https://stackoverflow.com/q/48324224:
# install_name_tool: fatal error: file not in an order that can be processed (link edit information does not fill the __LINKEDIT segment)
# Instead of using MachOView to fiddle with `__LINKEDIT`'s `File Size`, this is
# using another approach that has been discussed in
# https://bytemeta.vip/repo/ezQuake/ezquake-source/issues/624
# i.e., to remove the invalidated signature after manipulating the binary and
# resign it.
find . -type f -name "*.dylib" -exec codesign --remove-signature "{}" \;
find . -type f -name "*.dylib" -exec codesign --sign - "{}" \;
