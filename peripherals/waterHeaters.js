const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const waterHeaterOfNutrientSolution = {
  on: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterHeaterOfNutrientSolution, 'e')),
  off: (async () => i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterHeaterOfNutrientSolution, 'd')),
};

module.exports = {
  waterHeaterOfNutrientSolution,
};
