#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

command -v node || brew install node

# We are passing `--ignore-scripts` here, so that the installation doesn't
# trigger `node-gyp rebuild` to run because that depends on some of the binaries
# in `curl_build/vendor_install`, which is not available until
# `scripts/build.js` is run.
npm ci --ignore-scripts
