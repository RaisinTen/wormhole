# wormhole

A custom networking layer for Postman.

## Use

### C++ library

Compile with the wormhole shared library present in `relocatable_libwormhole_<arch>`
(`arch` is `x86_64` or `arm64`):

```sh
clang++ \
  -std=c++17 \
  <entry/point.cc> \
  -I<path/to/relocatable_libwormhole_<arch>/include> \
  -L<path/to/relocatable_libwormhole_<arch>/lib> \
  -lwormhole \
  -rpath @executable_path/<relative/path/to/relocatable_libwormhole_<arch>>
```

Use the library in your program:

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

Check out the [C++ API documentation](docs/C++-API.md).

### NPM package

Download the package:

```sh
npm i @postman/wormhole
```

Use the package in your script:

```js
const wormhole = require('@postman/wormhole');
await wormhole.request('https://postman-echo.com/get');

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

Check out the [JS API documentation](docs/JS-API.md).

## Major features

* HTTP/3 (QUIC protocol) support
* HTTP/2 support
* OpenSSL as the backend for cryptography
