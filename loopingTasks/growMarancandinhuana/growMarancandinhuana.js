/* eslint-disable max-len */
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
const { waterPumpForRecirculationOfNutrientSolution } = require('../../peripherals/waterPumps');
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

async function manageNutritiveSolutionRecirculation(miniDB, semaphoreMiniDB) {
  for (let i = 0; i < miniDB.growMarancandinhuanaParams.nutritiveSolution.recirculation.events.length; i++) {
    const { start, finish } = miniDB.growMarancandinhuanaParams.nutritiveSolution.recirculation.events[i];

    const now = new Date();
    const startHour = parseInt(start.substring(0, 2));
    const startMinute = parseInt(start.substring(3, 5));
    const finishHour = parseInt(finish.substring(0, 2));
    const finishMinute = parseInt(finish.substring(3, 5));

    if (startHour === now.getHours()
    && startMinute === now.getMinutes()) {
      await waterPumpForRecirculationOfNutrientSolution.on();
      miniDB.growMarancandinhuanaParams.nutritiveSolution.recirculation.status = 'recirculating';
    }
    else if (finishHour === now.getHours()
    && finishMinute === now.getMinutes()) {
      await waterPumpForRecirculationOfNutrientSolution.off();
      miniDB.growMarancandinhuanaParams.nutritiveSolution.recirculation.status = null;
    }
  }

  await saveMiniDB(semaphoreMiniDB, miniDB);
  return true;
}

async function manageNutritiveSolutionOxygenation(miniDB, semaphoreMiniDB) {
  for (let i = 0; i < miniDB.growMarancandinhuanaParams.nutritiveSolution.oxygenation.events.length; i++) {
    const { start, finish } = miniDB.growMarancandinhuanaParams.nutritiveSolution.oxygenation.events[i];

    const now = new Date();
    const startHour = parseInt(start.substring(0, 2));
    const startMinute = parseInt(start.substring(3, 5));
    const finishHour = parseInt(finish.substring(0, 2));
    const finishMinute = parseInt(finish.substring(3, 5));

    if (startHour === now.getHours()
    && startMinute === now.getMinutes()) {
      await airPumpOfNutrientSolution.on();
      miniDB.growMarancandinhuanaParams.nutritiveSolution.oxygenation.status = 'oxygenating';
    }
    else if (finishHour === now.getHours()
    && finishMinute === now.getMinutes()) {
      await airPumpOfNutrientSolution.off();
      miniDB.growMarancandinhuanaParams.nutritiveSolution.oxygenation.status = null;
    }
  }

  await saveMiniDB(semaphoreMiniDB, miniDB);
  return true;
}

/*
  growMarancandinhuana -> Manages:
    growSpace
    x   light
    ✓   temperature
   ✓/2  humidity
    x   airRenew
  nutritiveSolution
   ✓/2  temperature
    ✓   recirculation
    x   ec
   ✓/2   ph
    ✓   oxygenation
  training
    x  topping
    x  pruning
    x  defoliation
 */
async function growMarancandinhuana(miniDB, semaphoreMiniDB) {
  console.log("growMarancandinhuana")
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

  // await manageNutritiveSolutionEc(miniDB, semaphoreMiniDB);

  // await manageNutritiveSolutionPh(miniDB, semaphoreMiniDB);

  await manageNutritiveSolutionOxygenation(miniDB, semaphoreMiniDB);

  // await manageTrainingTopping(miniDB, semaphoreMiniDB);

  // await manageTrainingPruning(miniDB, semaphoreMiniDB);

  // await manageTrainingDefoliation(miniDB, semaphoreMiniDB);

  return true;
}

module.exports = {
  growMarancandinhuana,
};
