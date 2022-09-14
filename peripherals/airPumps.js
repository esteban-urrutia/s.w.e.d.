const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const airPumpOfNutrientSolution = {
  on: (async (semaphoreI2cController) => {
    return new Promise(async (resolve, reject) => {
      semaphoreI2cController.take(async () => {
        try{
          await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_airPumpOfNutrientSolution, 'e');
          semaphoreI2cController.leave();
          resolve(true);
        }
        catch(error){
          semaphoreI2cController.leave();
          reject(error);
        }
      });
    })
  }),
  off: (async (semaphoreI2cController) => {
    return new Promise(async (resolve, reject) => {
      semaphoreI2cController.take(async () => {
        try{
          await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_airPumpOfNutrientSolution, 'd');
          semaphoreI2cController.leave();
          resolve(true);
        }
        catch(error){
          semaphoreI2cController.leave();
          reject(error);
        }
      });
    })
  }),
};

module.exports = {
  airPumpOfNutrientSolution,
};
