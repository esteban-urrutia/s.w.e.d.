/* eslint-disable no-else-return */
/* eslint-disable brace-style */
/* eslint-disable no-param-reassign */
/* eslint-disable radix */
const I2C = require('i2c-bus');
const env = require('dotenv').config().parsed;

async function sendMessage(destinataryAddress, message) {
  message = Buffer.from(message);

  const i2c = await I2C.openPromisified(1);
  await i2c.i2cWrite(parseInt(destinataryAddress), message.length, message);
  await i2c.close();
}

async function receiveMessage(senderAddress) {
  let message = Buffer.alloc(parseInt(env.i2c_message_length));

  const i2c = await I2C.openPromisified(1);
  await i2c.i2cRead(parseInt(senderAddress), message.length, message);
  await i2c.close();

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
