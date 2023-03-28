brew "cmake"
brew "node"
brew "clang-format"
brew "shellcheck"

brew "automake" # Used directly in `./scripts` to build curl and its dependencies.
brew "coreutils" # So that `realpath` can be used.
brew "libtool" # Nghttp3 needs it while running automake.
brew "pkg-config" # Nghttp3 needs it while running the configure script. Refs: https://github.com/nghttp2/nghttp2/issues/620#issuecomment-541536650.

# These were available locally on my system but not on CI.
brew "libidn2"
brew "zstd"
