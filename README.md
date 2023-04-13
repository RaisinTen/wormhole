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

### No additional system depencencies

The wormhole project uses libcurl in the networking layer. However, it doesn't
require libcurl to be installed on the end user's system for use because
wormhole ships with a standalone, relocatable build of libcurl.

### HTTP/3 (QUIC protocol) support

Requests can be sent to QUIC endpoints like https://quic.aiortc.org.

```js
const wormhole = require('@postman/wormhole');

(async () => {
  const response = await wormhole.request('https://quic.aiortc.org');
  console.log(response);
})();

// Output:
// ...
// <p>
//   Congratulations, you loaded this page using HTTP/3!
// </p>
// ...
```

### HTTP/2 support

The HTTP/2 protocol is supported. To verify, set up the HTTP/2-only server
documented in https://nodejs.org/api/http2.html#server-side-example and run:

```js
const wormhole = require('@postman/wormhole');

(async () => {
  const response = await wormhole.request('https://localhost:8443', {
    ca: 'localhost-cert.pem'
  });
  console.log(response);
})();

// Output:
// { body: '<h1>Hello World</h1>', code: 200 }
```

### OpenSSL as the backend for cryptography

The wormhole project uses the quictls fork of OpenSSL -
https://github.com/quictls/openssl for SSL.
