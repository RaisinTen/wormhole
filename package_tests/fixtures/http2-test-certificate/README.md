# HTTP2 test certificate

This certificate has been created to test HTTP2 requests.
It has been created by running the OpenSSL command documented in
https://nodejs.org/api/http2.html#server-side-example:

```sh
openssl req -x509 -newkey rsa:2048 -nodes -sha256 -subj '/CN=localhost' \
  -keyout localhost-privkey.pem -out localhost-cert.pem
```
