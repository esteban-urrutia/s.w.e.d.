const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const humidifierOfGrowSpace = {
  on: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_humidifierOfGrowSpace, 'e')),
  off: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_humidifierOfGrowSpace, 'd')),
};

module.exports = {
  humidifierOfGrowSpace,
};
