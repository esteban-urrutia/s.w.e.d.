/* eslint-disable no-async-promise-executor */
const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const PHofNutrientSolution = {
  get: (async (miniDB, semaphoreI2cController) => new Promise(async (resolve, reject) => {
    semaphoreI2cController.take(async () => {
      try {
        const analogPhVoltageReading = await i2c.get(env.i2c_arduinoMega_address, 'PH');
        semaphoreI2cController.leave();

        const PHofNutSol = (
          ((analogPhVoltageReading * miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.m)
          * (5.0 / 1023.0))
          + miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.y
        ) / 10;

        resolve({ PHofNutSol, analogPhVoltageReading });
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
