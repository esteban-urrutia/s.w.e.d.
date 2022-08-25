const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const waterValveForIrrigationOfFrontGarden = {
  on: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfFrontGarden, 'e')),
  off: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfFrontGarden, 'd')),
};

const waterValveForIrrigationOfBackGarden = {
  on: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfBackGarden, 'e')),
  off: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfBackGarden, 'd')),
};

module.exports = {
  waterValveForIrrigationOfFrontGarden,
  waterValveForIrrigationOfBackGarden,
};
