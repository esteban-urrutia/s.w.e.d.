const env = require('dotenv').config().parsed;
const dht22 = require('node-dht-sensor').promises;

const temperatureAndHumidityOfGrowSpace = {
  get: (async () => {
    const { 
      temperature: temperatureOfGrowSpace1,
      humidity: humidityOfGrowSpace1
    } = dht22.readSync(22, parseInt(env.pin_temperatureAndHumidityOfGrowSpace1));
    const { 
      temperature: temperatureOfGrowSpace2,
      humidity: humidityOfGrowSpace2
    } = dht22.readSync(22, parseInt(env.pin_temperatureAndHumidityOfGrowSpace2));
    const { 
      temperature: temperatureOfGrowSpace3,
      humidity: humidityOfGrowSpace3
    } = dht22.readSync(22, parseInt(env.pin_temperatureAndHumidityOfGrowSpace3));

    const temperatureOfGrowSpace = ((temperatureOfGrowSpace1+temperatureOfGrowSpace2+temperatureOfGrowSpace3)/3);
    const humidityOfGrowSpace = ((humidityOfGrowSpace1+humidityOfGrowSpace2+humidityOfGrowSpace3)/3);
    
    return {
      temperatureOfGrowSpace,
      humidityOfGrowSpace
    }
  }),
};

module.exports = {
  temperatureAndHumidityOfGrowSpace,
};
