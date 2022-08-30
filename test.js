/* eslint-disable no-console */
/* eslint-disable max-len */
/* eslint-disable no-unused-vars */
const { airHeaterOfGrowSpace } = require('./peripherals/airHeaters');
const { airPumpOfNutrientSolution } = require('./peripherals/airPumps');
const { humidifierOfGrowSpace } = require('./peripherals/humidifiers');
const { ventilatorOfGrowSpace } = require('./peripherals/ventilators');
const { waterHeaterOfNutrientSolution } = require('./peripherals/waterHeaters');
const { waterPumpForRecirculationOfNutrientSolution } = require('./peripherals/waterPumps');
const { waterPumpForSampleOfNutrientSolution } = require('./peripherals/waterPumps');
const { waterPumpForNFTsystem } = require('./peripherals/waterPumps');
const { waterPumpForSampleOfNFTsystem } = require('./peripherals/waterPumps');
const { waterValveForIrrigationOfFrontGarden } = require('./peripherals/waterValves');
const { waterValveForIrrigationOfBackGarden } = require('./peripherals/waterValves');
const { sleep } = require('./utils/utils');
const { temperatureAndHumidityOfGrowSpace } = require('./sensors/temperature-humidity');

// Solid State Relay
// 1
/*
console.info('airHeaterOfGrowSpace.on()');
airHeaterOfGrowSpace.on()
.then(async () => {
    await sleep(2);
    console.info('airHeaterOfGrowSpace.off()\n\n');
    await airHeaterOfGrowSpace.off()
    .then(async () => {

// 2
        console.info('airPumpOfNutrientSolution.on()');
        airPumpOfNutrientSolution.on()
        .then(async () => {
            await sleep(2);
            console.info('airPumpOfNutrientSolution.off()\n\n');
            await airPumpOfNutrientSolution.off()
            .then(async () => {

// 3
                console.info('humidifierOfGrowSpace.on()');
                humidifierOfGrowSpace.on()
                .then(async () => {
                    await sleep(2);
                    console.info('humidifierOfGrowSpace.off()\n\n');
                    await humidifierOfGrowSpace.off()
                    .then(async () => {

// 4
                        console.info('humidifierOfGrowSpace.on()');
                        ventilatorOfGrowSpace.on()
                        .then(async () => {
                            await sleep(2);
                            console.info('humidifierOfGrowSpace.off()\n\n');
                            await ventilatorOfGrowSpace.off()
                            .then(async () => {

// 5
                                console.info('waterHeaterOfNutrientSolution.on()');
                                waterHeaterOfNutrientSolution.on()
                                .then(async () => {
                                    await sleep(2);
                                    console.info('waterHeaterOfNutrientSolution.off()\n\n');
                                    await waterHeaterOfNutrientSolution.off()
                                    .then(async () => {

// 6
                                        console.info('waterPumpForRecirculationOfNutrientSolution.on()');
                                        waterPumpForRecirculationOfNutrientSolution.on()
                                        .then(async () => {
                                            await sleep(2);
                                            console.info('waterPumpForRecirculationOfNutrientSolution.off()\n\n');
                                            await waterPumpForRecirculationOfNutrientSolution.off()
                                            .then(async () => {

// 7
                                                console.info('waterPumpForSampleOfNutrientSolution.on()');
                                                waterPumpForSampleOfNutrientSolution.on()
                                                .then(async () => {
                                                    await sleep(2);
                                                    console.info('waterPumpForSampleOfNutrientSolution.off()\n\n');
                                                    await waterPumpForSampleOfNutrientSolution.off()
                                                    .then(async () => {

// 8
                                                        console.info('waterPumpForNFTsystem.on()');
                                                        waterPumpForNFTsystem.on()
                                                        .then(async () => {
                                                            await sleep(2);
                                                            console.info('waterPumpForNFTsystem.off()\n\n');
                                                            await waterPumpForNFTsystem.off()
                                                            .then(async () => {

// 9
                                                                console.info('waterPumpForSampleOfNFTsystem.on()');
                                                                waterPumpForSampleOfNFTsystem.on()
                                                                .then(async () => {
                                                                    await sleep(2);
                                                                    console.info('waterPumpForSampleOfNFTsystem.off()\n\n');
                                                                    await waterPumpForSampleOfNFTsystem.off()
                                                                    .then(async () => {

// 10
                                                                        console.info('waterValveForIrrigationOfFrontGarden.on()');
                                                                        waterValveForIrrigationOfFrontGarden.on()
                                                                        .then(async () => {
                                                                            await sleep(2);
                                                                            console.info('waterValveForIrrigationOfFrontGarden.off()\n\n');
                                                                            await waterValveForIrrigationOfFrontGarden.off()
                                                                            .then(async () => {

// 11
                                                                                console.info('waterValveForIrrigationOfBackGarden.on()');
                                                                                waterValveForIrrigationOfBackGarden.on()
                                                                                .then(async () => {
                                                                                    await sleep(2);
                                                                                    console.info('waterValveForIrrigationOfBackGarden.off()\n\n');
                                                                                    await waterValveForIrrigationOfBackGarden.off()
                                                                                    .then(async () => {
                                                                                        console.info('Solid State Relay test success');
                                                                                    })
                                                                                })
                                                                            })
                                                                        })
                                                                    })
                                                                })
                                                            })
                                                        })
                                                    })
                                                })
                                            })
                                        })
                                    })
                                })
                            })
                        })
                    })
                })
            })
        })
    })
})
.catch((error) => {
    console.error(error);
})
*/

// Dht22
// 1
temperatureAndHumidityOfGrowSpace.get()
  .then((response) => {
    console.info(response);
  })
  .catch((response) => {
    console.info(response);
  });
