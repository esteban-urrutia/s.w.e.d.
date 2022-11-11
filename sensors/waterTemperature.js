const env = require('dotenv').config().parsed;
const ds18b20 = require('ds18x20');
const {
  sleep,
  execute,
} = require('../utils/utils');

function getDs18b20Temp(oneWireAddress) {
  return new Promise((resolve) => {
    ds18b20.get(oneWireAddress, (error, temp) => {
      if (error || typeof temp !== 'number' || temp > 80 || temp === 0) {
        resolve('readingError');
      }
      resolve((Math.round(temp * 10) / 10));
    });
  });
}

const temperatureOfNutrientSolution = {
  get: (async () => {
    // turn On sensor, wait 1 second, read sensor, wait 1 second, turn Off sensor
    await execute(`raspi-gpio set ${env.power_GPIO_sensors_temperatureOfNutrientSolution} op dh`);
    await sleep(1);

    const temperatureOfNutrientSolution1 = await getDs18b20Temp(
      env.oneWire_address_sensor_temperatureOfNutrientSolution,
    );

    await sleep(1);
    await execute(`raspi-gpio set ${env.power_GPIO_sensors_temperatureOfNutrientSolution} op dl`);

    return temperatureOfNutrientSolution1;
  }),
};

module.exports = {
  temperatureOfNutrientSolution,
};
