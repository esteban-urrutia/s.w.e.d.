const I2C = require('i2c-bus');
const env = require('dotenv').config().parsed;

function sendMessage(message, destinataryAddress){
  message = Buffer.from(message);

  const i2c = I2C.openSync(1);
  i2c.i2cWriteSync(parseInt(destinataryAddress), message.length, message);
  i2c.closeSync();
}

function receiveMessage(senderAddress){
  let message = Buffer.alloc(parseInt(env.i2c_message_length));

  const i2c = I2C.openSync(1);
  i2c.i2cReadSync(parseInt(senderAddress), message.length,  message)
  i2c.closeSync();

  message = message.toString();
  return message
}





  sendMessage("porro", env.i2c_arduinoMega_address);
  console.log( receiveMessage(env.i2c_arduinoMega_address) );




  
module.exports = {
  sendMessage,
  receiveMessage,
};
