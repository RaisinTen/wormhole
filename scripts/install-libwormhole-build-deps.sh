#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

command -v cmake || brew install cmake
