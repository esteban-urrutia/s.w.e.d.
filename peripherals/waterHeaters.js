const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const waterHeaterOfNutrientSolution = {
  on: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterHeaterOfNutrientSolution, 'e');

      semaphoreI2cController.leave();
    });
  }),
  off: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterHeaterOfNutrientSolution, 'd');

      semaphoreI2cController.leave();
    });
  }),
};

module.exports = {
  waterHeaterOfNutrientSolution,
};
