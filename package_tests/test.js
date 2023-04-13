const {
  deepStrictEqual,
  ok,
  strictEqual,
  rejects,
} = require('node:assert');

const wormhole = require('..');

describe('Basic HTTP test', async () => {
  let response;
  let server;
  let port;

  before(async () => {
    // HTTP server setup code from the Node.js `http` module documentation.
    // Refs: https://nodejs.org/api/http.html#httpcreateserveroptions-requestlistener
    const http = require('node:http');

    server = http.createServer((req, res) => {
      res.writeHead(200, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({
        data: 'Hello World!',
      }));
    });

    await (new Promise((resolve) => {
      server.listen(0, () => {
        resolve();
      });
    }));
    ok(server.listening);

    ({ port } = server.address());
  });

  it('request', async () => {
    response = await wormhole.request(`http://localhost:${port}`);
  });

  it('code', () => {
    strictEqual(response.code, 200);
  });

  it('body', () => {
    strictEqual(response.body, JSON.stringify({
      data: 'Hello World!',
    }));
  });

  after(() => {
    server.close();
  });
});

describe('Basic HTTPS test', async () => {
  let response;

  it('request', async () => {
    response = await wormhole.request('https://postman-echo.com/get', {
      headers: {
        hello: 'world',
        a: 'b',
        x: 'y',
      }
    });
  }).timeout(6_000);

  it('code', () => {
    strictEqual(response.code, 200);
  });

  const responseJSON = JSON.parse(response.body);

  it('headers', () => {
    strictEqual(typeof responseJSON.headers, 'object');
    strictEqual(responseJSON.headers.hello, 'world');
    strictEqual(responseJSON.headers.a, 'b');
    strictEqual(responseJSON.headers.x, 'y');
  });

  it('args', () => {
    deepStrictEqual(responseJSON.args, {});
  });

  it('url', () => {
    strictEqual(responseJSON.url, 'https://postman-echo.com/get');
  });
});

// TODO(RaisinTen): When this test is moved to the bottom of this file, for some
// reason, the request call fails with a `Error: Couldn't connect to server`
// exception.
describe('Basic HTTP2 test', async () => {
  let response;
  let server;
  let port;
  let ca;

  before(async () => {
    // HTTP2 server setup code from the Node.js `http2` module documentation.
    // Refs: https://nodejs.org/api/http2.html#server-side-example
    const http2 = require('node:http2');
    const fs = require('node:fs');
    const path = require('node:path');

    ca = path.join(__dirname, 'fixtures', 'http2-test-certificate', 'localhost-cert.pem');

    server = http2.createSecureServer({
      key: fs.readFileSync(path.join(__dirname, 'fixtures', 'http2-test-certificate', 'localhost-privkey.pem')),
      cert: fs.readFileSync(ca),
    });

    server.on('stream', (stream) => {
      // stream is a Duplex
      stream.respond({
        'content-type': 'text/html; charset=utf-8',
        ':status': 200,
      });
      stream.end('Hello, world!');
    });

    await (new Promise((resolve) => {
      server.listen(0, () => {
        resolve();
      });
    }));
    ok(server.listening);

    ({ port } = server.address());
  });

  it('request', async () => {
    response = await wormhole.request(`https://localhost:${port}`, { ca });
  });

  it('code', () => {
    strictEqual(response.code, 200);
  });

  it('body', () => {
    strictEqual(response.body, 'Hello, world!');
  });

  after(() => {
    server.close();
  });
});

describe('Basic HTTP3 test', async () => {
  let response

  it('request', async () => {
    // Found this url in https://bagder.github.io/HTTP3-test/.
    response = await wormhole.request('https://quic.aiortc.org/', { http: 'v3' });
  }).timeout(6_000);

  it('code', () => {
    strictEqual(response.code, 200);
  });

  it('content', () => {
    ok(/Congratulations, you loaded this page using HTTP\/3/.test(response.body));
  });
});

describe('404 not found', async () => {
  let response;

  it('request', async () => {
    response = await wormhole.request('https://example.com/404-not-found');
  });

  it('code', () => {
    strictEqual(response.code, 404);
  });
});

describe('Illegal URL format', async () => {
  it('request', async () => {
    await rejects(async () => {
      await wormhole.request('abc');
    }, {
      name: 'Error',
      message: 'Couldn\'t resolve host name'
    });
  });
});

describe('POST request', async () => {
  let response;

  it('request', async () => {
    response = await wormhole.request('https://postman-echo.com/post', { method: 'POST' });
  }).timeout(6_000);

  it('code', () => {
    strictEqual(response.code, 200);
  });

  const responseJSON = JSON.parse(response.body);

  it('headers', () => {
    strictEqual(typeof responseJSON.headers, 'object');
  });

  it('args', () => {
    deepStrictEqual(responseJSON.args, {});
  });

  it('url', () => {
    strictEqual(responseJSON.url, 'https://postman-echo.com/post');
  });
});

describe('Basic GBK encoding test', async () => {
  let response;

  it('request', async () => {
    // Third most popular website from https://w3techs.com/technologies/details/en-gbk.
    response = await wormhole.request('https://www.69shu.com');
  }).timeout(6_000);

  it('code', () => {
    strictEqual(response.code, 200);
  });

  it('body', () => {
    // TODO(RaisinTen): Test the entire title string:
    // `<title>69书吧_更新最快_无弹窗广告_无错小说阅读网</title>`.
    // Currently, encoding isn't handled correctly but at least this doesn't
    // crash or throw.
    ok(/<title>69/.test(response.body));
  });
});
