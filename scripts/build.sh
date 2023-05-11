#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

ARCH="$(uname -m)"
RELOCATABLE_LIBCURL_DIR="relocatable_libcurl_$ARCH"
FORCE_REINSTALL=false
if [ $# -gt 0 ]; then
    case "$1" in
        --force)
            FORCE_REINSTALL=true
            ;;
        *)
    esac
fi

# Exit if the directory already exists or the user does not want to reinstall.
if [ -d "$RELOCATABLE_LIBCURL_DIR" ] && [ "$FORCE_REINSTALL" = false ]; then
  echo "The directory $RELOCATABLE_LIBCURL_DIR already exists."
  echo "Please remove it first if you want to reinstall or use --force option."
  exit 0
fi

# The default deployment target is read from this environment variable. 
# This option only affects Darwin targets.
if [ "$ARCH" = 'arm64' ]
then
  # The Apple silicon support is added starting macos 11
  export MACOSX_DEPLOYMENT_TARGET="11.0"
else
  export MACOSX_DEPLOYMENT_TARGET="10.15"
fi

# NOTE: This detects the script directory, so that this script can be run from
# any directory in the file system.
# Refs: https://stackoverflow.com/q/59895/11395352
CWD="$PWD"
CDPATH='' cd -- "$(dirname -- "$0")"
SCRIPT_DIRECTORY="$(pwd -P)"
cd "$CWD"

PROJECT_ROOT="$(dirname "$SCRIPT_DIRECTORY")"
export PROJECT_ROOT
export VENDOR="$PROJECT_ROOT/vendor"
export BUILD="$PROJECT_ROOT/curl_build/vendor"
export PREFIX="$PROJECT_ROOT/curl_build/vendor_install"

mkdir -p "$BUILD"
mkdir -p "$PREFIX"

"$SCRIPT_DIRECTORY/build-openssl.sh"
"$SCRIPT_DIRECTORY/build-nghttp3.sh"
"$SCRIPT_DIRECTORY/build-ngtcp2.sh"
"$SCRIPT_DIRECTORY/build-nghttp2.sh"
"$SCRIPT_DIRECTORY/build-curl.sh"

"$SCRIPT_DIRECTORY/make-relocatable-libcurl.sh"
