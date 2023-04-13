#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

command -v clang-format || brew install clang-format
