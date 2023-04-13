# C++ API

## `Method` enum

The supported method types. Possible values:

* `Method::GET`
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
      RequestOptionsBuilder().set_method(Method::POST).build());
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
//     "x-amzn-trace-id": "Root=1-6436becc-5b64b8d43621de60770c4691",
//     "accept": "*/*",
//     "content-type": "application/json"
//   },
//   "json": null,
//   "url": "https://postman-echo.com/post"
// }
```
