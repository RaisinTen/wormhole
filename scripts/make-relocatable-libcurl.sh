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
# path. The assumed `@rpath` here is the `relocatable_libcurl` directory.

cd "$PROJECT_ROOT"

rm -rf relocatable_libcurl
mkdir relocatable_libcurl
cd relocatable_libcurl

# Copy the includes directory.
cp -R "$PREFIX/include" .

mkdir lib
cd lib

# Copy libraries by resolving the symlinks, so that `-lcurl` could be passed
# to the linker instead of `-lcurl.4`.

# Copy built libraries.
cp "$PREFIX/lib/libcrypto.dylib" .
cp "$PREFIX/lib/libcurl.dylib" .
cp "$PREFIX/lib/libnghttp3.dylib" .
cp "$PREFIX/lib/libngtcp2.dylib" .
cp "$PREFIX/lib/libngtcp2_crypto_openssl.dylib" .
cp "$PREFIX/lib/libssl.dylib" .

# Copy other installed dependency libraries.
# These are all libcurl dependencies.
cp "/usr/local/opt/libnghttp2/lib/libnghttp2.dylib" .
cp "/usr/local/opt/libidn2/lib/libidn2.dylib" .
cp "/usr/local/opt/zstd/lib/libzstd.dylib" .
cp "/usr/local/opt/brotli/lib/libbrotlidec.dylib" .

# These are all libidn2 dependencies.
cp "/usr/local/opt/libunistring/lib/libunistring.dylib" .
cp "/usr/local/opt/gettext/lib/libintl.dylib" .

# These are all libbrotlidec dependencies.
cp "/usr/local/opt/brotli/lib/libbrotlicommon.dylib" .

# Set the identification name of the entrypoint dylib - libcurl.
# So here, the requirement is that whatever dylib / executable that attempts to
# load this should have an rpath pointing to the `relocatable_libcurl`
# directory.
install_name_tool -id "@rpath/lib/libcurl.dylib" "libcurl.dylib"

# Set the identification names of the dylibs. This step is totally optional
# because these libraries are currently only loaded by libraries in the
# `relocatable_libcurl/lib` directory and not from any client application which
# would load these indirectly through an rpath. This is being because of
# stylistic reasons so that `otool -l` on these libraries don't contain the
# original paths of these libraries which is only sensible on the host system
# where these were built.

# Change identification names of built libraries.
install_name_tool -id "@rpath/lib/libcrypto.dylib" "libcrypto.dylib"
install_name_tool -id "@rpath/lib/libnghttp3.dylib" "libnghttp3.dylib"
install_name_tool -id "@rpath/lib/libngtcp2.dylib" "libngtcp2.dylib"
install_name_tool -id "@rpath/lib/libngtcp2_crypto_openssl.dylib" "libngtcp2_crypto_openssl.dylib"
install_name_tool -id "@rpath/lib/libssl.dylib" "libssl.dylib"

# Change identification names of installed libraries.
install_name_tool -id "@rpath/lib/libnghttp2.dylib" "libnghttp2.dylib"
install_name_tool -id "@rpath/lib/libidn2.dylib" "libidn2.dylib"
install_name_tool -id "@rpath/lib/libunistring.dylib" "libunistring.dylib"
install_name_tool -id "@rpath/lib/libintl.dylib" "libintl.dylib"
install_name_tool -id "@rpath/lib/libzstd.dylib" "libzstd.dylib"
install_name_tool -id "@rpath/lib/libbrotlidec.dylib" "libbrotlidec.dylib"
install_name_tool -id "@rpath/lib/libbrotlicommon.dylib" "libbrotlicommon.dylib"

# Change the dependent shared library install names in built libraries

# Change the dependent shared library install names in libcurl

# Change the dependent built shared library install names in libcurl
install_name_tool -change "$PREFIX/lib/libnghttp3.3.dylib" "@loader_path/libnghttp3.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libngtcp2_crypto_openssl.4.dylib" "@loader_path/libngtcp2_crypto_openssl.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libngtcp2.10.dylib" "@loader_path/libngtcp2.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libssl.81.3.dylib" "@loader_path/libssl.dylib" "libcurl.dylib"
install_name_tool -change "$PREFIX/lib/libcrypto.81.3.dylib" "@loader_path/libcrypto.dylib" "libcurl.dylib"

# Change the dependent installed shared library install names in libcurl

# TODO(RaisinTen): Add assertions to make sure that the old path in
# `install_name_tool -change old new file` command exists. Currently, if it
# doesn't exist the command doesn't indicate that in anyway.

# The `stat -f "%Y" <symlink/path>` command follows a symlink just once. This
# has been done so that we can use the library paths without hardcoding the
# version suffixes. No need to do this for built libraries for now because we
# are in full control of those version numbers.
# Refs: https://unix.stackexchange.com/a/138658
install_name_tool -change "/usr/local/opt/libnghttp2/lib/$(stat -f "%Y" "/usr/local/opt/libnghttp2/lib/libnghttp2.dylib")" "@loader_path/libnghttp2.dylib" "libcurl.dylib"
install_name_tool -change "/usr/local/opt/libidn2/lib/$(stat -f "%Y" "/usr/local/opt/libidn2/lib/libidn2.dylib")" "@loader_path/libidn2.dylib" "libcurl.dylib"
install_name_tool -change "/usr/local/opt/zstd/lib/$(stat -f "%Y" "/usr/local/opt/zstd/lib/libzstd.dylib")" "@loader_path/libzstd.dylib" "libcurl.dylib"
install_name_tool -change "/usr/local/opt/brotli/lib/$(stat -f "%Y" "/usr/local/opt/brotli/lib/libbrotlidec.dylib")" "@loader_path/libbrotlidec.dylib" "libcurl.dylib"

# Change the dependent shared library install names in libngtcp2_crypto_openssl
install_name_tool -change "$PREFIX/lib/libngtcp2.10.dylib" "@loader_path/libngtcp2.dylib" "libngtcp2_crypto_openssl.dylib"
install_name_tool -change "$PREFIX/lib/libssl.81.3.dylib" "@loader_path/libssl.dylib" "libngtcp2_crypto_openssl.dylib"
install_name_tool -change "$PREFIX/lib/libcrypto.81.3.dylib" "@loader_path/libcrypto.dylib" "libngtcp2_crypto_openssl.dylib"

# Change the dependent shared library install names in libssl
install_name_tool -change "$PREFIX/lib/libcrypto.81.3.dylib" "@loader_path/libcrypto.dylib" "libssl.dylib"

# Change the dependent shared library install names in installed libraries

# Change the dependent shared library install names in libidn2
install_name_tool -change "/usr/local/opt/libunistring/lib/$(stat -f "%Y" "/usr/local/opt/libunistring/lib/libunistring.dylib")" "@loader_path/libunistring.dylib" "libidn2.dylib"
install_name_tool -change "/usr/local/opt/gettext/lib/$(stat -f "%Y" "/usr/local/opt/gettext/lib/libintl.dylib")" "@loader_path/libintl.dylib" "libidn2.dylib"

# Change the dependent shared library install names in libbrotlidec
# Note that unlike the other cases, the install name of libbrotlicommon in
# libbrotlidec is not an absolute path but rather a relative path using
# `@loader_path`.
install_name_tool -change "@loader_path/$(stat -f "%Y" /usr/local/opt/brotli/lib/libbrotlicommon.dylib)" "@loader_path/libbrotlicommon.dylib" "libbrotlidec.dylib"
