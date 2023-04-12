# JavaScript API

## The `request(url[, options])` API

* `url`: `<string>` The requested URL string.
* `options`: `<Object>` Configuration settings of the outgoing request.
  * `method`: `<string>` The request method. It can be one of `'GET'`, `'HEAD'`,
    `'POST'`, `'PUT'`, `'DELETE'`, `'CONNECT'`, `'OPTIONS'`, `'TRACE'` and
    `'PATCH'`. **Default:** `GET`.
  * `ca`: `<string>` The certificate authority bundle file path.
* Returns: `Promise<Object>` The response object.
  * `body`: `<string>` The response body.
  * `code`: `<number>` The response status code.

The `request()` API sends an HTTP request and returns the response.

```javascript
const wormhole = require('@postman/wormhole');

(async () => {
  const response = await wormhole.request('https://postman-echo.com/post', {
    method: 'POST'
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
//     '    "x-amzn-trace-id": "Root=1-6436bfdd-272e6cee5c1499570495f557",\n' +
//     '    "accept": "*/*",\n' +
//     '    "content-type": "application/json"\n' +
//     '  },\n' +
//     '  "json": null,\n' +
//     '  "url": "https://postman-echo.com/post"\n' +
//     '}',
//   code: 200
// }
```
