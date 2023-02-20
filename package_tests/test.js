const { test } = require('node:test');
const {
  deepStrictEqual,
  strictEqual,
} = require('node:assert');

const wormhole = require('..');

test('Basic HTTP test', async (t) => {
  const response = wormhole.request('https://postman-echo.com/get');
  const responseJSON = JSON.parse(response);

  await t.test('headers', (t) => {
    strictEqual(typeof responseJSON.headers, 'object');
  });

  await t.test('args', (t) => {
    deepStrictEqual(responseJSON.args, {});
  });

  await t.test('url', (t) => {
    strictEqual(responseJSON.url, 'https://postman-echo.com/get');
  });
});
