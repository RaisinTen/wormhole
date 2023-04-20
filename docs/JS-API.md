# JavaScript API

## The `request(url[, options])` API

* `url`: `<string>` The requested URL string.
* `options`: `<Object>` Configuration settings of the outgoing request.
  * `httpVersion`: `<string>` The request HTTP version. It can be one of `1.0`,
    `1.1`, `2.0`, `2-TLS`, `2-prior-knowledge`, `3`, `3-only`.
    **Default:** `2-TLS`. See [`HTTPVersion` enum](C++-API.md#httpversion-enum).
  * `method`: `<string>` The request method. It can be one of `'GET'`, `'HEAD'`,
    `'POST'`, `'PUT'`, `'DELETE'`, `'CONNECT'`, `'OPTIONS'`, `'TRACE'` and
    `'PATCH'`. **Default:** `GET`.
  * `headers`: `<Object>` The request headers object with string keys and string
    values. **Default:** `{}`.
  * `ca`: `<string>` The certificate authority bundle file path.
  * `body`: `<string>` The request body.
* Returns: `Promise<Object>` The response object.
  * `body`: `<string>` The response body.
  * `code`: `<number>` The response status code.
  * `headers`: `<Object>` The response headers object with string keys and
    string values.

The `request()` API sends an HTTP request and returns the response.

```javascript
const wormhole = require('@postman/wormhole');

(async () => {
  const response = await wormhole.request('https://postman-echo.com/post', {
    method: 'POST',
    body: 'Wormhole body',
    headers: {
      hello: 'world',
      a: 'b',
      x: 'y',
    },
  });
  console.log(response);
})();

// Output:
// {
//   body: '{\n' +
//     '  "args": {},\n' +
//     '  "data": {},\n' +
//     '  "files": {},\n' +
//     '  "form": {},\n' +
//     '  "headers": {\n' +
//     '    "x-forwarded-proto": "https",\n' +
//     '    "x-forwarded-port": "443",\n' +
//     '    "host": "postman-echo.com",\n' +
//     '    "x-amzn-trace-id": "Root=1-643824d9-1ae0722129d51d5e4914a23d",\n' +
//     '    "accept": "*/*",\n' +
//     '    "a": "b",\n' +
//     '    "hello": "world",\n' +
//     '    "x": "y",\n' +
//     '    "content-type": "application/json"\n' +
//     '  },\n' +
//     '  "json": {\n' +
//     '    "Wormhole body": ""\n' +
//     '  },\n' +
//     '  "url": "https://postman-echo.com/post"\n' +
//     '}',
//   code: 200,
//   headers: {
//     'content-length': '411',
//     'content-type': 'application/json; charset=utf-8',
//     date: 'Thu, 13 Apr 2023 15:50:49 GMT',
//     etag: 'W/"19b-Xn5yLV12T6rdJQsbelXWjnl+xkg"',
//     'set-cookie': 'sails.sid=s%3AKRJtFWPvKMurL0xgG5rJr3cGUo_U7NZT.ItROs47R2FQ7kGQbRtK1hXN7AkOoWOsdoZcPsChLxZA; Path=/; HttpOnly'
//   }
// }
```
