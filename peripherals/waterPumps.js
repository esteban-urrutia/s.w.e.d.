const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const waterPumpForRecirculationOfNutrientSolution = {
  on: (async (semaphoreI2cController) => {
    return new Promise(async (resolve, reject) => {
      semaphoreI2cController.take(async () => {
        try{
          await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForRecirculationOfNutrientSolution, 'e');
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
          await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForRecirculationOfNutrientSolution, 'd');
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

const waterPumpForSampleOfNutrientSolution = {
  on: (async (semaphoreI2cController) => {
    return new Promise(async (resolve, reject) => {
      semaphoreI2cController.take(async () => {
        try{
          await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNutrientSolution, 'e');
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
          await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNutrientSolution, 'd');
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

const waterPumpForNFTsystem = {
  on: (async (semaphoreI2cController) => {
    return new Promise(async (resolve, reject) => {
      semaphoreI2cController.take(async () => {
        try{
          await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForNFTsystem, 'e');
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
          await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForNFTsystem, 'd');
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

const waterPumpForSampleOfNFTsystem = {
  on: (async (semaphoreI2cController) => {
    return new Promise(async (resolve, reject) => {
      semaphoreI2cController.take(async () => {
        try{
          await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNFTsystem, 'e');
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
          await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNFTsystem, 'd');
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
  waterPumpForRecirculationOfNutrientSolution,
  waterPumpForSampleOfNutrientSolution,
  waterPumpForNFTsystem,
  waterPumpForSampleOfNFTsystem,
};
