# wormhole

A custom networking layer for Postman.

## Use

### C++ library

Compile with the wormhole static library, `cmake/libwormhole.a`, and the custom
build of the libcurl dynamic library in
`curl_build/vendor_install/relocatable_lib` using:

```sh
clang++ \
  <entry/point.cc> \
  <path/to/native_build/libwormhole.a> \
  -I<path/to/wormhole/include/directory> \
  -L<path/to/curl_build/vendor_install/relocatable_lib> \
  -lcurl
```

```cc
#include <request.h>

...
  wormhole::Response res = wormhole::request("https://postman-echo.com/get");
  std::string response_string(res.data, res.size);
  std::cout << response_string << std::endl;
...

// Output:
// {
//   "args": {},
//   "headers": {
//     "x-forwarded-proto": "https",
//     "x-forwarded-port": "443",
//     "host": "postman-echo.com",
//     "x-amzn-trace-id": "Root=1-63f373f3-73f715716ba031264ea53935",
//     "accept": "*/*"
//   },
//   "url": "https://postman-echo.com/get"
// }
```

### NPM package

Download the package:

```sh
npm i @postman/wormhole
```

Use the package in your script:

```js
const wormhole = require('@postman/wormhole');
const response = wormhole.request('https://postman-echo.com/get');
console.log(response);

// Output:
// {
//   "args": {},
//   "headers": {
//     "x-forwarded-proto": "https",
//     "x-forwarded-port": "443",
//     "host": "postman-echo.com",
//     "x-amzn-trace-id": "Root=1-63f373f3-73f715716ba031264ea53935",
//     "accept": "*/*"
//   },
//   "url": "https://postman-echo.com/get"
// }
```

## Building

### Curl with HTTP3 support

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
brew bundle
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
npm ci
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
