#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

brew install cmake

# TODO(RaisinTen): These would go into a separate script when we decouple the
# linter checks in https://github.com/postmanlabs/wormhole/issues/64.
brew install clang-format
brew install shellcheck
