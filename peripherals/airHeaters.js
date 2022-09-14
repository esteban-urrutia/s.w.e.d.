const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const airHeaterOfGrowSpace = {
  on: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_airHeaterOfGrowSpace, 'e');
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
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_airHeaterOfGrowSpace, 'd');
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
  airHeaterOfGrowSpace,
};
