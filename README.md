# wormhole

A custom networking layer for Postman.

## Use

### C++ library

Compile with the static library, `cmake/libwormhole.a`, and libcurl, `-lcurl`,
using:

```sh
clang++ \
  <entry/point.cc> \
  -I<path/to/wormhole/include/directory> \
  <path/to/wormhole/cmake/libwormhole.a> \
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

```js
const wormhole = require('.');
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

### C++ library

#### Install dependencies

```sh
brew bundle
```

#### Configure

```sh
cmake -S . -B native_build
```

#### Build

```sh
cmake --build native_build
```

#### Test

```sh
native_build/test/request_unit_test
```

#### Format

```sh
cmake --build native_build --target clang_format
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
