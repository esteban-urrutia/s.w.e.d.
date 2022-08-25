const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const airHeaterOfGrowSpace = {
  on: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_airHeaterOfGrowSpace, 'e')),
  off: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_airHeaterOfGrowSpace, 'd')),
};

module.exports = {
  airHeaterOfGrowSpace,
};
