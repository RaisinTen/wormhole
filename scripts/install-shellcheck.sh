#!/bin/sh

set -o xtrace
set -o errexit
set -o nounset

command -v shellcheck || brew install shellcheck
