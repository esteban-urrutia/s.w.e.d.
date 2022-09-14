const env = require('dotenv').config().parsed;
const i2c = require('../controllers/i2cController');

const waterValveForIrrigationOfFrontGarden = {
  on: (async () => {
    i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfFrontGarden, 'e');
  }),
  off: (async () => {
    i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfFrontGarden, 'd');
  }),
};

const waterValveForIrrigationOfBackGarden = {
  on: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfBackGarden, 'e');

      semaphoreI2cController.leave();
    });
  }),
  off: (async (semaphoreI2cController) => {
    semaphoreI2cController.take(async () => {
      await i2c.post(env.i2c_arduinoMega_address, 'SR', env.pin_waterValveForIrrigationOfBackGarden, 'd');

      semaphoreI2cController.leave();
    });
  }),
};

module.exports = {
  waterValveForIrrigationOfFrontGarden,
  waterValveForIrrigationOfBackGarden,
};
