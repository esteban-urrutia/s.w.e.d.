const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const waterPumpForRecirculationOfNutrientSolution = {
  on: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForRecirculationOfNutrientSolution, 'e')),
  off: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForRecirculationOfNutrientSolution, 'd')),
};

const waterPumpForSampleOfNutrientSolution = {
  on: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNutrientSolution, 'e')),
  off: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNutrientSolution, 'd')),
};

const waterPumpForNFTsystem = {
  on: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForNFTsystem, 'e')),
  off: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForNFTsystem, 'd')),
};

const waterPumpForSampleOfNFTsystem = {
  on: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNFTsystem, 'e')),
  off: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNFTsystem, 'd')),
};

module.exports = {
  waterPumpForRecirculationOfNutrientSolution,
  waterPumpForSampleOfNutrientSolution,
  waterPumpForNFTsystem,
  waterPumpForSampleOfNFTsystem,
};
