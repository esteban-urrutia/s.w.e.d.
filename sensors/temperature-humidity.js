/* eslint-disable radix */
const env = require('dotenv').config().parsed;
const dht22 = require('node-dht-sensor').promises;
const {
  sleep,
  execute,
} = require('../utils/utils');

const temperatureAndHumidityOfGrowSpace = {
  get: (async () => {
  // turn On sensor, wait 1 second, read sensor, wait 1 second, turn Off sensor
    await execute(`raspi-gpio set ${env.power_GPIO_sensors_temperatureAndHumidityOfGrowSpace} op dh`);
    await sleep(1);

    const {
      temperature: temperatureOfGrowSpace1,
      humidity: humidityOfGrowSpace1,
    } = await dht22.read(22, parseInt(env.pin_sensor_temperatureAndHumidityOfGrowSpace1));
    if (typeof temperatureOfGrowSpace1 !== 'number' || typeof humidityOfGrowSpace1 !== 'number') {
      throw new Error(`error: malfunction on dht22 connected at pin ${env.pin_sensor_temperatureAndHumidityOfGrowSpace1}`);
    }

    const {
      temperature: temperatureOfGrowSpace2,
      humidity: humidityOfGrowSpace2,
    } = await dht22.read(22, parseInt(env.pin_sensor_temperatureAndHumidityOfGrowSpace2));
    if (typeof temperatureOfGrowSpace2 !== 'number' || typeof humidityOfGrowSpace2 !== 'number') {
      throw new Error(`error: malfunction on dht22 connected at pin ${env.pin_sensor_temperatureAndHumidityOfGrowSpace2}`);
    }

    const {
      temperature: temperatureOfGrowSpace3,
      humidity: humidityOfGrowSpace3,
    } = await dht22.read(22, parseInt(env.pin_sensor_temperatureAndHumidityOfGrowSpace3));
    if (typeof temperatureOfGrowSpace2 !== 'number' || typeof humidityOfGrowSpace2 !== 'number') {
      throw new Error(`error: malfunction on dht22 connected at pin ${env.pin_sensor_temperatureAndHumidityOfGrowSpace3}`);
    }

    const temperatureOfGrowSpace = (
      (temperatureOfGrowSpace1
      + temperatureOfGrowSpace2
      + temperatureOfGrowSpace3) / 3
    );

    const humidityOfGrowSpace = (
      (humidityOfGrowSpace1
      + humidityOfGrowSpace2
      + humidityOfGrowSpace3) / 3
    );

    await sleep(1);
    await execute(`raspi-gpio set ${env.power_GPIO_sensors_temperatureAndHumidityOfGrowSpace} op dl`);

    return {
      temperatureOfGrowSpace,
      humidityOfGrowSpace,
      sensorsData1: {
        temperatureOfGrowSpace1,
        humidityOfGrowSpace1,
        temperatureOfGrowSpace2,
        humidityOfGrowSpace2,
        temperatureOfGrowSpace3,
        humidityOfGrowSpace3,
      },
    };
  }),
};

module.exports = {
  temperatureAndHumidityOfGrowSpace,
};
