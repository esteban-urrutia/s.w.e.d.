const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const waterHeaterOfNutrientSolution = {
  on: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      try{
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterHeaterOfNutrientSolution, 'e');
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
        await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterHeaterOfNutrientSolution, 'd');
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
  waterHeaterOfNutrientSolution,
};
