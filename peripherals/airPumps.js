const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const airPumpOfNutrientSolution = {
  on: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_airPumpOfNutrientSolution, 'e')),
  off: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_airPumpOfNutrientSolution, 'd')),
};

module.exports = {
  airPumpOfNutrientSolution,
};
