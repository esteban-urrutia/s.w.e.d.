const i2c = require('../controllers/i2cController');
const env = require('dotenv').config().parsed;

const airHeaterOfGrowSpace = {
  on: (async () => {
    const messageToSend = 'SR'+env.pin_airHeater+'e';

    i2c.sendMessage(messageToSend, env.i2c_arduinoMega_address);
    const receivedMessage = i2c.receiveMessage(env.i2c_arduinoMega_address);

    if(messageToSend === receivedMessage){
      return true;
    }
    else if(receivedMessage === 'undef'){
      throw 'message sended('+messageToSend+') was now recognized';
    }
    else{
      throw 'message sended('+messageToSend+') is different from message received('+receivedMessage+')';
    }

  }),
  off: (async () => {
    const messageToSend = 'SR'+env.pin_airHeater+'d';

    i2c.sendMessage(messageToSend, env.i2c_arduinoMega_address);
    const receivedMessage = i2c.receiveMessage(env.i2c_arduinoMega_address);

    if(messageToSend === receivedMessage){
      return true;
    }
    else if(receivedMessage === 'undef'){
      throw 'message sended('+messageToSend+') was now recognized';
    }
    else{
      throw 'message sended('+messageToSend+') is different from message received('+receivedMessage+')';
    }

  }),
};

module.exports = {
  airHeaterOfGrowSpace,
};