# Contributing to wormhole

## Building

### Curl with HTTP3 support

#### Install dependencies

```sh
scripts/install-relocatable-libcurl-build-deps.sh
```

#### Build

```sh
./scripts/build.sh
```

#### Test

```sh
./curl_build/vendor_install/bin/curl --http3 https://postman-echo.com/get
```

### C++ library

#### Install dependencies

```sh
scripts/install-libwormhole-build-deps.sh
```

#### Build

```sh
npm run cmake:build
```

#### Test

```sh
npm run cmake:test
```

#### Format

```sh
npm run cmake:format
```

### NPM package

#### Install dependencies

```sh
scripts/install-npm-library-build-deps.sh
```

#### Build

```sh
npm run build
```

#### Test

```sh
npm test
```

#### Format

```sh
npm run format
```

#### Lint

```sh
npm run lint
```
