# C++ API

## `HTTPVersion` enum

The supported HTTP verions. Possible values:

* `HTTPVersion::v1_0` - Enforce HTTP 1.0 requests.
* `HTTPVersion::v1_1` - Enforce HTTP 1.1 requests.
* `HTTPVersion::v2_0` - Attempt HTTP 2 requests. It will fall back to HTTP 1.1
  if HTTP 2 cannot be negotiated with the server.
* `HTTPVersion::v2TLS` (default) - Attempt HTTP 2 over TLS (HTTPS) only. It will fall back
  to HTTP 1.1 if HTTP 2 cannot be negotiated with the HTTPS server.
* `HTTPVersion::v2_PRIOR_KNOWLEDGE` - Issue non-TLS HTTP requests using HTTP/2
  without HTTP/1.1 Upgrade. It requires prior knowledge that the server supports
  HTTP/2 straight away. HTTPS requests will still do HTTP/2 the standard way
  with negotiated protocol version in the TLS handshake.
* `HTTPVersion::v3` - Attempt HTTP/3 requests. It will fall back to earlier HTTP
  versions if needed.
* `HTTPVersion::v3ONLY` - Attempt HTTP/3 only. It will not fall back to earlier
  HTTP version if the server does not support HTTP/3.

## `Method` enum

The supported method types. Possible values:

* `Method::GET` (default)
* `Method::HEAD`
* `Method::POST`
* `Method::PUT`
* `Method::DELETE`
* `Method::CONNECT`
* `Method::OPTIONS`
* `Method::TRACE`
* `Method::PATCH`

## `RequestOptionsBuilder` class

The `RequestOptionsBuilder` class provides an API using which a `RequestOptions`
instance can be created.

### `RequestOptionsBuilder &set_method(Method method)` method

Setter for the method.

### `RequestOptionsBuilder &set_headers(std::map<std::string, std::string> headers)` method

Setter for the headers.

### `RequestOptionsBuilder &set_ca_bundle(std::filesystem::path ca_bundle)` method

Setter for the CA bundle path.

### `RequestOptions build()` method

Builds the `RequestOptions` object that can be used for sending the request.

## `RequestOptions` class

The `RequestOptions` class is used to build an object that contains the options
using which a request can be configured. An instance of this class can be
constructed by the `RequestOptionsBuilder` class.

### `Method method() const` method

The getter for the method type.

### `Method method() const` method

The getter for the headers.

### `std::optional<std::filesystem::path> ca_bundle() const` method

The getter for the CA bundle file path.

Example usage:

```cc
RequestOptionsBuilder().build()
RequestOptionsBuilder()
    .set_method(Method::POST)
    .set_ca_bundle("../package_tests/fixtures/http2-test-certificate/localhost-cert.pem")
    .build());
```

## `Response` class

This is the type of the object that is returned by the `request()` function.

### `std::ostringstream body` member variable

The response body.

### `long code` member variable

The HTTP status code.

### `std::optional<std::string> error` member variable

This optionally contains a string in case an error happened while sending the
request.

## `request()` function

* `const std::string_view url` parameter - The URL.
* `RequestOptions options` optional parameter - The request options.

Returns a `Response` object.

Example:

```cc
#include <request.h>
#include <iostream>

using namespace wormhole;

int main() {
  Response res = wormhole::request(
      "https://postman-echo.com/post",
      RequestOptionsBuilder()
          .set_method(Method::POST)
          .set_headers({{"hello", "world"}, {"a", "b"}, {"x", "y"}})
          .build());
  if (res.error.has_value()) {
    std::cerr << "Error: " << res.error.value() << std::endl;
  } else {
    std::cout << "Response: " << res.body.str() << std::endl;
  }

  return 0;
}

// Output:
// Response: {
//   "args": {},
//   "data": {},
//   "files": {},
//   "form": {},
//   "headers": {
//     "x-forwarded-proto": "https",
//     "x-forwarded-port": "443",
//     "host": "postman-echo.com",
//     "x-amzn-trace-id": "Root=1-643816d2-7cb6d06146a5d8456dbb9247",
//     "accept": "*/*",
//     "a": "b",
//     "hello": "world",
//     "x": "y",
//     "content-type": "application/json"
//   },
//   "json": null,
//   "url": "https://postman-echo.com/post"
// }
```
