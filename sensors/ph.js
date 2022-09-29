/* eslint-disable no-async-promise-executor */
const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const PHofNutrientSolution = {
  get: (async (semaphoreI2cController) => new Promise(async (resolve, reject) => {
    semaphoreI2cController.take(async () => {
      try {
        const analogPHreading = await i2c.get(env.i2c_arduinoMega_address, 'PH');
        semaphoreI2cController.leave();

        const PHvalue = ((-56.48 * analogPHreading) * (5.0 / 1023.0) + 222.49)/10;

        resolve(PHvalue);
      } catch (error) {
        semaphoreI2cController.leave();
        reject(error);
      }
    });
  })),
};

module.exports = {
  PHofNutrientSolution,
};

// float calibracion_ph_Y = 222.49;
// float calibracion_ph_M = -56.48;
