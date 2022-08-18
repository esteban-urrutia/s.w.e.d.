/* eslint-disable brace-style */
/* eslint-disable no-param-reassign */
/* eslint-disable no-await-in-loop */
/* eslint-disable radix */
/* eslint-disable no-plusplus */
const { airHeaterOfGrowSpace } = require('../../peripherals/airHeaters');
const { airPumpOfNutrientSolution } = require('../../peripherals/airPumps');
const { humidifierOfGrowSpace } = require('../../peripherals/humidifiers');
const { ventilatorOfGrowSpace } = require('../../peripherals/ventilators');
const { waterHeaterOfNutrientSolution } = require('../../peripherals/waterHeaters');
const {
  waterPumpForSampleOfNutrientSolution,
  waterPumpForRecirculationOfNutrientSolution,
} = require('../../peripherals/waterPumps');
const { ECofNutrientSolution } = require('../../sensors/ec');
const { PHofNutrientSolution } = require('../../sensors/ph');
const { temperatureAndHumidityOfGrowSpace } = require('../../sensors/temperature-humidity');
const { temperatureOfNutrientSolution } = require('../../sensors/waterTemperature');
const { saveMiniDB } = require('../../controllers/miniDBcontroller');
const { getPhotoperiod } = require('../../utils/utils');

async function manageGrowSpaceTemperature(miniDB, semaphoreMiniDB, temperatureOfGrowSpace) {
  const photoperiod = getPhotoperiod(miniDB);

  // temperatureOfGrowSpace <= min
  if (
    temperatureOfGrowSpace
    <= miniDB
      .growMarancandinhuanaParams
      .growSpace
      .temperature
      .values[photoperiod]
      .min
  ) {
    await airHeaterOfGrowSpace.on();
    await ventilatorOfGrowSpace.off();
    miniDB.growMarancandinhuanaParams.growSpace.temperature.status = 'heating';
  }
  // temperatureOfGrowSpace >= max
  else if (
    temperatureOfGrowSpace
    >= miniDB
      .growMarancandinhuanaParams
      .growSpace
      .temperature
      .values[photoperiod]
      .max
  ) {
    await airHeaterOfGrowSpace.off();
    await ventilatorOfGrowSpace.on();
    miniDB.growMarancandinhuanaParams.growSpace.temperature.status = 'cooling';
  }
  // min < temperatureOfGrowSpace < max
  else {
    await airHeaterOfGrowSpace.off();
    await ventilatorOfGrowSpace.off();
    miniDB.growMarancandinhuanaParams.growSpace.temperature.status = null;
  }

  await saveMiniDB(semaphoreMiniDB, miniDB);
  return true;
}

async function manageGrowSpaceHumidity(miniDB, semaphoreMiniDB, humidityOfGrowSpace) {
  const photoperiod = getPhotoperiod(miniDB);

  // humidityOfGrowSpace <= min
  if (
    humidityOfGrowSpace
    <= miniDB
      .growMarancandinhuanaParams
      .growSpace
      .humidity
      .values[photoperiod]
      .min
  ) {
    await humidifierOfGrowSpace.on();
    miniDB.growMarancandinhuanaParams.growSpace.humidity.status = 'humidifying';
  }
  // humidityOfGrowSpace >= max
  else if (
    humidityOfGrowSpace
    >= miniDB
      .growMarancandinhuanaParams
      .growSpace
      .humidity
      .values[photoperiod]
      .max
  ) {
    await humidifierOfGrowSpace.off();
    miniDB.growMarancandinhuanaParams.growSpace.humidity.status = null;
  }
  // min < humidityOfGrowSpace < max
  else {
    await humidifierOfGrowSpace.off();
    miniDB.growMarancandinhuanaParams.growSpace.humidity.status = null;
  }

  await saveMiniDB(semaphoreMiniDB, miniDB);
  return true;
}

async function manageNutritiveSolutionTemperature(miniDB, semaphoreMiniDB) {
  const photoperiod = getPhotoperiod(miniDB);

  const temperatureOfNutSol = await temperatureOfNutrientSolution.get();

  // temperatureOfNutSol <= min
  if (
    temperatureOfNutSol
    <= miniDB
      .growMarancandinhuanaParams
      .nutritiveSolution
      .temperature
      .values[photoperiod]
      .min
  ) {
    await waterHeaterOfNutrientSolution.on();
    miniDB.growMarancandinhuanaParams.nutritiveSolution.temperature.status = 'heating';
  }
  // temperatureOfNutSol >= max
  else if (
    temperatureOfNutSol
    >= miniDB
      .growMarancandinhuanaParams
      .nutritiveSolution
      .temperature
      .values[photoperiod]
      .max
  ) {
    await waterHeaterOfNutrientSolution.off();
    miniDB.growMarancandinhuanaParams.growSpace.temperature.status = null;
  }
  // min < temperatureOfNutSol < max
  else {
    await waterHeaterOfNutrientSolution.off();
    miniDB.growMarancandinhuanaParams.nutritiveSolution.temperature.status = null;
  }

  await saveMiniDB(semaphoreMiniDB, miniDB);
  return true;
}

/**
 * growMarancandinhuana -> Manages:
 *   growSpace
 *     light
 *     temperature
 *     humidity
 *     airRenew
 *   nutritiveSolution
 *     temperature
 *     recirculation
 *     ec
 *     pc
 *     oxygenation
 *   training
 *     topping
 *     pruning
 *     defoliation
 */
async function growMarancandinhuana(miniDB, semaphoreMiniDB) {
  // await manageGrowSpaceLight(miniDB, semaphoreMiniDB);
  const {
    temperatureOfGrowSpace,
    humidityOfGrowSpace,
  } = await temperatureAndHumidityOfGrowSpace.get();

  await manageGrowSpaceTemperature(miniDB, semaphoreMiniDB, temperatureOfGrowSpace);

  await manageGrowSpaceHumidity(miniDB, semaphoreMiniDB, humidityOfGrowSpace);

  // await manageGrowSpaceAirRenew(miniDB, semaphoreMiniDB);

  await manageNutritiveSolutionTemperature(miniDB, semaphoreMiniDB);

  await manageNutritiveSolutionRecirculation(miniDB, semaphoreMiniDB);

  await manageNutritiveSolutionEc(miniDB, semaphoreMiniDB);

  await manageNutritiveSolutionPh(miniDB, semaphoreMiniDB);

  await manageNutritiveSolutionOxygenation(miniDB, semaphoreMiniDB);

  await manageTrainingTopping(miniDB, semaphoreMiniDB);

  await manageTrainingPruning(miniDB, semaphoreMiniDB);

  await manageTrainingDefoliation(miniDB, semaphoreMiniDB);

  return true;
}

module.exports = {
  growMarancandinhuana,
};
