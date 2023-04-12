const {
  deepStrictEqual,
  ok,
  strictEqual,
  rejects,
} = require('node:assert');

const wormhole = require('..');

describe('Basic HTTP test', async () => {
  let response;

  it('request', async () => {
    response = await wormhole.request('https://postman-echo.com/get');
  });

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
    strictEqual(responseJSON.url, 'https://postman-echo.com/get');
  });
});

describe('Basic HTTP3 test', async () => {
  let response

  it('request', async () => {
    // Found this url in https://bagder.github.io/HTTP3-test/.
    response = await wormhole.request('https://quic.aiortc.org/');
  });

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
      message: 'URL using bad/illegal format or missing URL'
    });
  });
});

describe('POST request', async () => {
  let response;

  it('request', async () => {
    response = await wormhole.request('https://postman-echo.com/post', { method: 'POST' });
  });

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
