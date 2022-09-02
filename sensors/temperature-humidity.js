/* eslint-disable radix */
const env = require('dotenv').config().parsed;
const dht22 = require('node-dht-sensor').promises;
const {execSync} = require('child_process');
const { sleep } = require('../utils/utils');

const temperatureAndHumidityOfGrowSpace = {
  get: (async () => {

  // turn On sensor, wait 1 second, read sensor, wait 1 second, turn Off sensor
  execSync('raspi-gpio set '+env.power_GPIO_sensors_temperatureAndHumidityOfGrowSpace+' op dh');
  await sleep(1);

    const {
      temperature: temperatureOfGrowSpace1,
      humidity: humidityOfGrowSpace1,
    } = dht22.readSync(22, parseInt(env.pin_sensor_temperatureAndHumidityOfGrowSpace1));
    if (!temperatureOfGrowSpace1 || !humidityOfGrowSpace1) {
      throw new Error(`error: malfunction on dht22 connected at pin ${env.pin_sensor_temperatureAndHumidityOfGrowSpace1}`);
    }

    const {
      temperature: temperatureOfGrowSpace2,
      humidity: humidityOfGrowSpace2,
    } = dht22.readSync(22, parseInt(env.pin_sensor_temperatureAndHumidityOfGrowSpace2));
    if (!temperatureOfGrowSpace2 || !humidityOfGrowSpace2) {
      throw new Error(`error: malfunction on dht22 connected at pin ${env.pin_sensor_temperatureAndHumidityOfGrowSpace2}`);
    }

    const {
      temperature: temperatureOfGrowSpace3,
      humidity: humidityOfGrowSpace3,
    } = dht22.readSync(22, parseInt(env.pin_sensor_temperatureAndHumidityOfGrowSpace3));
    if (!temperatureOfGrowSpace3 || !humidityOfGrowSpace3) {
      throw new Error(`error: malfunction on dht22 connected at pin ${env.pin_sensor_temperatureAndHumidityOfGrowSpace3}`);
    }

    const temperatureOfGrowSpace = (
      (temperatureOfGrowSpace1
      + temperatureOfGrowSpace2
      + temperatureOfGrowSpace3) / 3
    );

    // need better rel. humidity sensor
    const humidityOfGrowSpace = humidityOfGrowSpace2;

    await sleep(1);
    execSync('raspi-gpio set '+env.power_GPIO_sensors_temperatureAndHumidityOfGrowSpace+' op dl');

    return {
      temperatureOfGrowSpace,
      humidityOfGrowSpace,
    };
  }),
};

module.exports = {
  temperatureAndHumidityOfGrowSpace,
};
