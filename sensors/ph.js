const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const PHofNutrientSolution = {
  get: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      const analogPHreading = await i2c.get(env.i2c_arduinoMega_address, 'PH');

      console.log(analogPHreading);

      const PHvalue = (-56.48 * analogPHreading) * (5.0 / 1023.0) + 222.49;

      console.log(PHvalue);

      semaphoreI2cController.leave();

      return PHvalue;
    });
  }),
};

module.exports = {
  PHofNutrientSolution,
};

// float calibracion_ph_Y = 222.49;
// float calibracion_ph_M = -56.48;
