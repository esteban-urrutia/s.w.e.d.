/* eslint-disable no-async-promise-executor */
const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const lightingOfGrowSpace = {
  on: (async (semaphoreI2cController) => new Promise(async (resolve, reject) => {
    semaphoreI2cController.take(async () => {
      try {
        await i2c.post(env.i2c_arduinoMega_address, 'UR', env.pin_lightingOfGrowSpace, 'e');
        semaphoreI2cController.leave();
        resolve(true);
      } catch (error) {
        semaphoreI2cController.leave();
        reject(error);
      }
    });
  })),
  off: (async (semaphoreI2cController) => new Promise(async (resolve, reject) => {
    semaphoreI2cController.take(async () => {
      try {
        await i2c.post(env.i2c_arduinoMega_address, 'UR', env.pin_lightingOfGrowSpace, 'd');
        semaphoreI2cController.leave();
        resolve(true);
      } catch (error) {
        semaphoreI2cController.leave();
        reject(error);
      }
    });
  })),
};

module.exports = {
  lightingOfGrowSpace,
};
