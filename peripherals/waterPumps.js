const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const waterPumpForRecirculationOfNutrientSolution = {
  on: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForRecirculationOfNutrientSolution, 'e');
        semaphoreI2cController.leave();
      }
      catch(error){
        semaphoreI2cController.leave();
        throw error;
      }
    });
  }),
  off: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForRecirculationOfNutrientSolution, 'd');
        semaphoreI2cController.leave();
      }
      catch(error){
        semaphoreI2cController.leave();
        throw error;
      }
    });
  }),
};

const waterPumpForSampleOfNutrientSolution = {
  on: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNutrientSolution, 'e');
        semaphoreI2cController.leave();
      }
      catch(error){
        semaphoreI2cController.leave();
        throw error;
      }
    });
  }),
  off: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNutrientSolution, 'd');
        semaphoreI2cController.leave();
      }
      catch(error){
        semaphoreI2cController.leave();
        throw error;
      }
    });
  }),
};

const waterPumpForNFTsystem = {
  on: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForNFTsystem, 'e');
        semaphoreI2cController.leave();
      }
      catch(error){
        semaphoreI2cController.leave();
        throw error;
      }
    });
  }),
  off: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForNFTsystem, 'd');
        semaphoreI2cController.leave();
      }
      catch(error){
        semaphoreI2cController.leave();
        throw error;
      }
    });
  }),
};

const waterPumpForSampleOfNFTsystem = {
  on: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNFTsystem, 'e');
        semaphoreI2cController.leave();
      }
      catch(error){
        semaphoreI2cController.leave();
        throw error;
      }
    });
  }),
  off: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterPumpForSampleOfNFTsystem, 'd');
        semaphoreI2cController.leave();
      }
      catch(error){
        semaphoreI2cController.leave();
        throw error;
      }
    });
  }),
};

module.exports = {
  waterPumpForRecirculationOfNutrientSolution,
  waterPumpForSampleOfNutrientSolution,
  waterPumpForNFTsystem,
  waterPumpForSampleOfNFTsystem,
};
