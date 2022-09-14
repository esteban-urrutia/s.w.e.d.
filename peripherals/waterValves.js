const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const waterValveForIrrigationOfFrontGarden = {
  on: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfFrontGarden, 'e');
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
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfFrontGarden, 'd');
        semaphoreI2cController.leave();
      }
      catch(error){
        semaphoreI2cController.leave();
        throw error;
      }
    });
  }),
};

const waterValveForIrrigationOfBackGarden = {
  on: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfBackGarden, 'e');
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
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfBackGarden, 'd');
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
  waterValveForIrrigationOfFrontGarden,
  waterValveForIrrigationOfBackGarden,
};
