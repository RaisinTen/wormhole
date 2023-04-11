#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset


# This script makes libwormhole and the libraries it depends on relocatable, so that
# it is possible to move the directory containing these libraries to another
# location and still be able to link to these and use these correctly.

RELOCATABLE_LIBWORMHOLE_DIR="relocatable_libwormhole_$(uname -m)"

# It will copy the dependency tree of libwormhole into the `relocatable_libwormhole_<arch>` directory.
cmake --install cmake_build --prefix "$RELOCATABLE_LIBWORMHOLE_DIR"

# The install name of the entrypoint library is also changed into a path relative to the `@rpath`
# path. The assumed `@rpath` here is the `relocatable_libwormhole_<arch>` directory.
install_name_tool -id @rpath/lib/libwormhole.dylib "$RELOCATABLE_LIBWORMHOLE_DIR/lib/libwormhole.dylib"

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
codesign --remove-signature "$RELOCATABLE_LIBWORMHOLE_DIR/lib/libwormhole.dylib"
codesign --sign - "$RELOCATABLE_LIBWORMHOLE_DIR/lib/libwormhole.dylib"
