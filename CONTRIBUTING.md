# Contributing to wormhole

## Building

### Curl with HTTP3 support

#### Install dependencies

```sh
scripts/install-relocatable-libcurl-build-deps.sh
```

> Note: Once relocatable-libcurl is build subsequent runs
> will skip installation, use the `--force` option if you want to install.

#### Lint

```sh
npm run lint:sh
```

#### Build

```sh
scripts/build.sh
```

> Note: Once the executable is generated, subsequent runs
> will skip the build, use the `--force` option if you want to build.

#### Test

```sh
curl_build/vendor_install/bin/curl --http3 https://quic.aiortc.org
# Output:
# ...
#         <p>
#             Congratulations, you loaded this page using HTTP/3!
#         </p>
# ...
```

### C++ library

#### Install dependencies

```sh
scripts/install-libwormhole-build-deps.sh
```

#### Format

```sh
npm run format:cpp
```

#### Build

```sh
npm run build:cmake
```

#### Test

```sh
npm run test:cmake
```

### NPM package

#### Install dependencies

```sh
# We are passing `--ignore-scripts` here, so that the installation doesn't
# trigger `node-gyp rebuild` to run because that depends on some of the binaries
# in `curl_build/vendor_install`, which is not available until
# `scripts/build.js` is run.
npm ci --ignore-scripts
```

Or if you don't have Node.js installed:

```sh
scripts/install-npm-library-build-deps.sh
```

#### Lint

```sh
npm run lint:js
```

#### Format

```sh
npm run format:js
```

#### Build

```sh
npm run build
```

#### Test

```sh
npm test
```

#### Clean

```sh
npm run clean
```
