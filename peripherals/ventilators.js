const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const ventilatorOfGrowSpace = {
  on: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_ventilatorOfGrowSpace, 'e')),
  off: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_ventilatorOfGrowSpace, 'd')),
};

module.exports = {
  ventilatorOfGrowSpace,
};
