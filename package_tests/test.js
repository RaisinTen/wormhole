const {
  deepStrictEqual,
  strictEqual,
} = require('node:assert');

const wormhole = require('..');

describe('Basic HTTP test', async (t) => {
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
