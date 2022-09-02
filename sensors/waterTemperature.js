const env = require('dotenv').config().parsed;
var ds18b20 = require('ds18x20');
const {execSync} = require('child_process');
const { sleep } = require('../utils/utils');

const temperatureOfNutrientSolution = {
  get: (async () => {

    // turn On sensor, wait 1 second, read sensor, wait 1 second, turn Off sensor
    execSync('raspi-gpio set '+env.power_GPIO_sensors_temperatureOfNutrientSolution+' op dh');
    await sleep(1);
  
    const temperatureOfNutrientSolution = ds18b20.get(env.oneWire_address_sensor_temperatureOfNutrientSolution);

    await sleep(1);
    execSync('raspi-gpio set '+env.power_GPIO_sensors_temperatureOfNutrientSolution+' op dl');

    return {
      temperatureOfNutrientSolution,
    };
  }),
};

module.exports = {
  temperatureOfNutrientSolution,
};
