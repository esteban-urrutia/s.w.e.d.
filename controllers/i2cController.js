/* eslint-disable no-else-return */
/* eslint-disable brace-style */
/* eslint-disable no-param-reassign */
/* eslint-disable radix */
const I2C = require('i2c-bus');
const env = require('dotenv').config().parsed;

async function sendMessage(destinataryAddress, message) {
  message = Buffer.from(message);

  const i2c = I2C.openSync(1);
  i2c.i2cWriteSync(parseInt(destinataryAddress), message.length, message);
  i2c.closeSync();
}

async function receiveMessage(senderAddress) {
  let message = Buffer.alloc(parseInt(env.i2c_message_length));

  const i2c = I2C.openSync(1);
  i2c.i2cReadSync(parseInt(senderAddress), message.length, message);
  i2c.closeSync();

  message = message.toString();
  return message;
}

async function post(address, device, pin, status) {
  const messageToSend = `${device}${pin}${status}`;

  await sendMessage(address, messageToSend);
  const receivedMessage = await receiveMessage(address);

  if (messageToSend === receivedMessage) {
    return true;
  }
  else if (receivedMessage === 'undef') {
    throw new Error(`message sended(${messageToSend}) was now recognized`);
  }
  else {
    throw new Error(`message sended(${messageToSend}) is different from message received(${receivedMessage})`);
  }
}

module.exports = {
  sendMessage,
  receiveMessage,
  post,
};
