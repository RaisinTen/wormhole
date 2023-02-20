# wormhole

A custom networking layer for Postman.

## C++ library

### Install dependencies

```sh
brew bundle
```

### Configure

```sh
cmake -S . -B cmake
```

### Build

```sh
cmake --build cmake
```

### Test

```sh
cmake/test/request_unit_test
```

## NPM package

### Install dependencies

```sh
npm ci
```
### Build

```sh
npm run build
```

### Test

```sh
npm test
```
