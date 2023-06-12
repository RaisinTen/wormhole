const { EventEmitter } = require('node:events');
const {
  request,
  websocketRequest: _websocketRequest,
} = require('./build/Release/wormhole.node');

function websocketRequest(url) {
  const handle = new EventEmitter();

  let webSocket;
  const req = _websocketRequest(url, (_webSocket, message) => {
    // TODO(RaisinTen): Add support for 'open' event once the upstream
    // discussion in the curl-library mailing list:
    // https://curl.se/mail/lib-2023-06/0006.html
    // gets resolved.
    webSocket = _webSocket;
    handle.emit('message', message);
  });

  req.then(() => {
    handle.emit('close');
  });

  req.catch((err) => {
    handle.emit('error', err);
  });

  handle.disconnect = function disconnect() {
    if (!webSocket) {
      throw new Error('Connection not established yet.');
    }
    webSocket.disconnect();
  };

  handle.send = function send(message) {
    if (!webSocket) {
      throw new Error('Connection not established yet.');
    }
    webSocket.send(message);
  };

  return handle;
}

module.exports = { request, websocketRequest };
