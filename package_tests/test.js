const {
  deepStrictEqual,
  ok,
  strictEqual,
} = require('node:assert');

const wormhole = require('..');

describe('Basic HTTP test', async () => {
  const response = wormhole.request('https://postman-echo.com/get');
  const responseJSON = JSON.parse(response);

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
  // Found this url in https://bagder.github.io/HTTP3-test/.
  const response = wormhole.request('https://quic.aiortc.org/');

  it('content', () => {
    ok(/Congratulations, you loaded this page using HTTP\/3/.test(response));
  });
});
