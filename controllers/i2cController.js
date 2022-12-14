/* eslint-disable no-lonely-if */
/* eslint-disable no-else-return */
/* eslint-disable brace-style */
/* eslint-disable no-param-reassign */
/* eslint-disable radix */
const I2C = require('i2c-bus');
const env = require('dotenv').config().parsed;
const { isDigit } = require('../utils/utils');

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

  // first attempt
  await sendMessage(address, messageToSend);
  let receivedMessage = await receiveMessage(address);

  if (messageToSend === receivedMessage) {
    return true;
  }
  else if (receivedMessage === 'undef') {
    throw new Error(`message sended(${messageToSend}) was not recognized`);
  }
  else {
    // second attempt
    await sendMessage(address, messageToSend);
    receivedMessage = await receiveMessage(address);

    if (messageToSend === receivedMessage) {
      return true;
    }
    else if (receivedMessage === 'undef') {
      throw new Error(`message sended(${messageToSend}) was not recognized`);
    }
    else {
      // third attempt
      if (messageToSend === receivedMessage) {
        return true;
      }
      else if (receivedMessage === 'undef') {
        throw new Error(`message sended(${messageToSend}) was not recognized`);
      }
      else {
        throw new Error(`message sended(${messageToSend}) is different from message received(${receivedMessage})`);
      }
    }
  }
}

async function get(address, device) {
  const messageToSend = `get${device}`;

  // first attempt
  await sendMessage(address, messageToSend);
  let receivedMessage = await receiveMessage(address);

  if (receivedMessage.charAt(0) === device.charAt(0)
   && receivedMessage.charAt(1) === device.charAt(1)
   && isDigit(receivedMessage.charAt(2))
   && isDigit(receivedMessage.charAt(3))
   && isDigit(receivedMessage.charAt(4))) {
    return parseInt(receivedMessage.substring(2));
  }
  else if (receivedMessage === 'undef') {
    throw new Error(`message sended(${messageToSend}) was not recognized`);
  }
  else {
    // second attempt
    await sendMessage(address, messageToSend);
    receivedMessage = await receiveMessage(address);

    if (receivedMessage.charAt(0) === device.charAt(0)
     && receivedMessage.charAt(1) === device.charAt(1)
     && isDigit(receivedMessage.charAt(2))
     && isDigit(receivedMessage.charAt(3))
     && isDigit(receivedMessage.charAt(4))) {
      return parseInt(receivedMessage.substring(2));
    }
    else if (receivedMessage === 'undef') {
      throw new Error(`message sended(${messageToSend}) was not recognized`);
    }
    else {
      // third attempt
      await sendMessage(address, messageToSend);
      receivedMessage = await receiveMessage(address);

      if (receivedMessage.charAt(0) === device.charAt(0)
       && receivedMessage.charAt(1) === device.charAt(1)
       && isDigit(receivedMessage.charAt(2))
       && isDigit(receivedMessage.charAt(3))
       && isDigit(receivedMessage.charAt(4))) {
        return parseInt(receivedMessage.substring(2));
      }
      else if (receivedMessage === 'undef') {
        throw new Error(`message sended(${messageToSend}) was not recognized`);
      }
      else {
        throw new Error(`message sended(${messageToSend}) is not coherent with message received(${receivedMessage})`);
      }
    }
  }
}

module.exports = {
  sendMessage,
  receiveMessage,
  post,
  get,
};
