# wormhole

A custom networking layer for Postman.

## Use

### C++ library

Compile with the wormhole shared library present in `relocatable_libwormhole`:

```sh
clang++ \
  <entry/point.cc> \
  -I<path/to/relocatable_libwormhole/include> \
  -L<path/to/relocatable_libwormhole/lib> \
  -lwormhole \
  -rpath @executable_path/<relative/path/to/relocatable_libwormhole>
```

```cc
#include <request.h>

...
  wormhole::Response res = wormhole::request("https://quic.aiortc.org/");
  std::string response_string(res.data, res.size);
  std::cout << response_string << std::endl;
...

// Output:
// ...
// <p>
//   Congratulations, you loaded this page using HTTP/3!
// </p>
// ...
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
//   body: '{\n' +
//     '  "args": {},\n' +
//     '  "headers": {\n' +
//     '    "x-forwarded-proto": "https",\n' +
//     '    "x-forwarded-port": "443",\n' +
//     '    "host": "postman-echo.com",\n' +
//     '    "x-amzn-trace-id": "Root=1-64351173-097ce76b03cb782e1576ba51",\n' +
//     '    "accept": "*/*"\n' +
//     '  },\n' +
//     '  "url": "https://postman-echo.com/get"\n' +
//     '}',
//   code: 200
// }
```

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
