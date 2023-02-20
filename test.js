const wormhole = require('.');
const response = wormhole.request('https://postman-echo.com/get');
console.log(response);
