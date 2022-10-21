/* eslint-disable max-len */
/* eslint-disable brace-style */
/* eslint-disable no-param-reassign */
/* eslint-disable no-await-in-loop */
/* eslint-disable radix */
/* eslint-disable no-plusplus */
const { lightingOfGrowSpace } = require('../../peripherals/lighting');
const { airHeaterOfGrowSpace } = require('../../peripherals/airHeaters');
const { airPumpOfNutrientSolution } = require('../../peripherals/airPumps');
const { humidifierOfGrowSpace } = require('../../peripherals/humidifiers');
const { bigVentilatorOfGrowSpace } = require('../../peripherals/ventilators');
const { airExtractorOfGrowSpace } = require('../../peripherals/airExtractors');
const { waterHeaterOfNutrientSolution } = require('../../peripherals/waterHeaters');
const { waterPumpForRecirculationOfNutrientSolution } = require('../../peripherals/waterPumps');
const { temperatureAndHumidityOfGrowSpace } = require('../../sensors/temperature-humidity');
const { temperatureOfNutrientSolution } = require('../../sensors/waterTemperature');
const { saveMiniDB } = require('../../controllers/miniDBcontroller');
const log = require('../../controllers/logController').getInstance();

/**
 * getPhotoperiod: get photoperiod day based on start/finish time
 * @returns {String}
 */
function getPhotoperiod(miniDB) {
  const now = new Date();

  const startTime = new Date();
  const startHour = parseInt(miniDB.growMarancandinhuanaParams.training.photoperiodDay.start.substring(0, 2));
  const startMinute = parseInt(miniDB.growMarancandinhuanaParams.training.photoperiodDay.start.substring(3, 5));
  startTime.setHours(startHour);
  startTime.setMinutes(startMinute);
  startTime.setSeconds(0);
  startTime.setMilliseconds(0);

  const finishTime = new Date();
  const finishHour = parseInt(miniDB.growMarancandinhuanaParams.training.photoperiodDay.finish.substring(0, 2));
  const finishMinute = parseInt(miniDB.growMarancandinhuanaParams.training.photoperiodDay.finish.substring(3, 5));
  finishTime.setHours(finishHour);
  finishTime.setMinutes(finishMinute);
  finishTime.setSeconds(0);
  finishTime.setMilliseconds(0);

  if (startTime.getTime() <= now.getTime()
  && now.getTime() <= finishTime.getTime()) {
    return 'day';
  }
  return 'night';
}

async function manageGrowSpaceLight(miniDB, semaphoreMiniDB, semaphoreI2cController) {
  if (miniDB.growMarancandinhuanaParams.growSpace.light.events.length > 0) {
    for (let i = 0; i < miniDB.growMarancandinhuanaParams.growSpace.light.events.length; i++) {
      const { start, finish } = miniDB.growMarancandinhuanaParams.growSpace.light.events[i];

      const now = new Date();
      const startHour = parseInt(start.substring(0, 2));
      const startMinute = parseInt(start.substring(3, 5));
      const finishHour = parseInt(finish.substring(0, 2));
      const finishMinute = parseInt(finish.substring(3, 5));

      if (startHour === now.getHours()
      && startMinute === now.getMinutes()) {
        await lightingOfGrowSpace.on(semaphoreI2cController);
        miniDB.growMarancandinhuanaParams.growSpace.light.status = 'lighting';
      }
      else if (finishHour === now.getHours()
      && finishMinute === now.getMinutes()) {
        await lightingOfGrowSpace.off(semaphoreI2cController);
        miniDB.growMarancandinhuanaParams.growSpace.light.status = null;
      }
    }
    await saveMiniDB(semaphoreMiniDB, miniDB);
  }

  return true;
}

async function manageGrowSpaceTemperature(miniDB, semaphoreMiniDB, semaphoreI2cController, temperatureOfGrowSpace) {
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
    await airHeaterOfGrowSpace.on(semaphoreI2cController);
    await bigVentilatorOfGrowSpace.off(semaphoreI2cController);
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
    await airHeaterOfGrowSpace.off(semaphoreI2cController);
    await bigVentilatorOfGrowSpace.on(semaphoreI2cController);
    miniDB.growMarancandinhuanaParams.growSpace.temperature.status = 'cooling';
  }
  // min < temperatureOfGrowSpace < max
  else {
    await airHeaterOfGrowSpace.off(semaphoreI2cController);
    await bigVentilatorOfGrowSpace.off(semaphoreI2cController);
    miniDB.growMarancandinhuanaParams.growSpace.temperature.status = null;
  }

  await saveMiniDB(semaphoreMiniDB, miniDB);
  return true;
}

// eslint-disable-next-line no-unused-vars
async function manageGrowSpaceHumidity(miniDB, semaphoreMiniDB, semaphoreI2cController, humidityOfGrowSpace) {
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
    await humidifierOfGrowSpace.on(semaphoreI2cController);
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
    await humidifierOfGrowSpace.off(semaphoreI2cController);
    miniDB.growMarancandinhuanaParams.growSpace.humidity.status = null;
  }
  // min < humidityOfGrowSpace < max
  else {
    await humidifierOfGrowSpace.off(semaphoreI2cController);
    miniDB.growMarancandinhuanaParams.growSpace.humidity.status = null;
  }

  await saveMiniDB(semaphoreMiniDB, miniDB);
  return true;
}

async function manageGrowSpaceAirRenew(miniDB, semaphoreMiniDB, semaphoreI2cController) {
  if (miniDB.growMarancandinhuanaParams.growSpace.airRenew.events.length > 0) {
    for (let i = 0; i < miniDB.growMarancandinhuanaParams.growSpace.airRenew.events.length; i++) {
      const { start, finish } = miniDB.growMarancandinhuanaParams.growSpace.airRenew.events[i];

      const now = new Date();
      const startHour = parseInt(start.substring(0, 2));
      const startMinute = parseInt(start.substring(3, 5));
      const finishHour = parseInt(finish.substring(0, 2));
      const finishMinute = parseInt(finish.substring(3, 5));

      if (startHour === now.getHours()
      && startMinute === now.getMinutes()) {
        await airExtractorOfGrowSpace.on(semaphoreI2cController);
        miniDB.growMarancandinhuanaParams.growSpace.airRenew.status = 'airRenewing';
      }
      else if (finishHour === now.getHours()
      && finishMinute === now.getMinutes()) {
        await airExtractorOfGrowSpace.off(semaphoreI2cController);
        miniDB.growMarancandinhuanaParams.growSpace.airRenew.status = null;
      }
    }
    await saveMiniDB(semaphoreMiniDB, miniDB);
  }

  return true;
}

async function manageNutritiveSolutionTemperature(miniDB, semaphoreMiniDB, semaphoreI2cController) {
  const photoperiod = getPhotoperiod(miniDB);

  const temperatureOfNutSol = await temperatureOfNutrientSolution.get();

  if (temperatureOfNutSol !== 'readingError') {
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
      await waterHeaterOfNutrientSolution.on(semaphoreI2cController);
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
      await waterHeaterOfNutrientSolution.off(semaphoreI2cController);
      miniDB.growMarancandinhuanaParams.growSpace.temperature.status = null;
    }
    // min < temperatureOfNutSol < max
    else {
      await waterHeaterOfNutrientSolution.off(semaphoreI2cController);
      miniDB.growMarancandinhuanaParams.nutritiveSolution.temperature.status = null;
    }

    await saveMiniDB(semaphoreMiniDB, miniDB);
  }
  return {
    sensorsData2: {
      temperatureOfNutSol,
    },
  };
}

async function manageNutritiveSolutionRecirculation(miniDB, semaphoreMiniDB, semaphoreI2cController) {
  if (miniDB.growMarancandinhuanaParams.nutritiveSolution.recirculation.events.length > 0) {
    for (let i = 0; i < miniDB.growMarancandinhuanaParams.nutritiveSolution.recirculation.events.length; i++) {
      const { start, finish } = miniDB.growMarancandinhuanaParams.nutritiveSolution.recirculation.events[i];

      const now = new Date();
      const startHour = parseInt(start.substring(0, 2));
      const startMinute = parseInt(start.substring(3, 5));
      const finishHour = parseInt(finish.substring(0, 2));
      const finishMinute = parseInt(finish.substring(3, 5));

      if (startHour === now.getHours()
      && startMinute === now.getMinutes()) {
        await waterPumpForRecirculationOfNutrientSolution.on(semaphoreI2cController);
        miniDB.growMarancandinhuanaParams.nutritiveSolution.recirculation.status = 'recirculating';
      }
      else if (finishHour === now.getHours()
      && finishMinute === now.getMinutes()) {
        await waterPumpForRecirculationOfNutrientSolution.off(semaphoreI2cController);
        miniDB.growMarancandinhuanaParams.nutritiveSolution.recirculation.status = null;
      }
    }
    await saveMiniDB(semaphoreMiniDB, miniDB);
  }

  return true;
}

async function manageNutritiveSolutionOxygenation(miniDB, semaphoreMiniDB, semaphoreI2cController) {
  if (miniDB.growMarancandinhuanaParams.nutritiveSolution.oxygenation.events.length > 0) {
    for (let i = 0; i < miniDB.growMarancandinhuanaParams.nutritiveSolution.oxygenation.events.length; i++) {
      const { start, finish } = miniDB.growMarancandinhuanaParams.nutritiveSolution.oxygenation.events[i];

      const now = new Date();
      const startHour = parseInt(start.substring(0, 2));
      const startMinute = parseInt(start.substring(3, 5));
      const finishHour = parseInt(finish.substring(0, 2));
      const finishMinute = parseInt(finish.substring(3, 5));

      if (startHour === now.getHours()
      && startMinute === now.getMinutes()) {
        await airPumpOfNutrientSolution.on(semaphoreI2cController);
        miniDB.growMarancandinhuanaParams.nutritiveSolution.oxygenation.status = 'oxygenating';
      }
      else if (finishHour === now.getHours()
      && finishMinute === now.getMinutes()) {
        await airPumpOfNutrientSolution.off(semaphoreI2cController);
        miniDB.growMarancandinhuanaParams.nutritiveSolution.oxygenation.status = null;
      }
    }
    await saveMiniDB(semaphoreMiniDB, miniDB);
  }

  return true;
}

/*
  growMarancandinhuana -> Manages:
    growSpace
    ✓   light
    ✓   temperature
    x   humidity
    ✓   airRenew
  nutritiveSolution
   ✓/2  temperature
    ✓   recirculation
   ✓/2  ec
   ✓/2  ph
    ✓   oxygenation
  training
    x  topping
    x  pruning
    x  defoliation
 */
async function growMarancandinhuana(miniDB, semaphoreMiniDB, semaphoreI2cController) {
  await manageGrowSpaceLight(miniDB, semaphoreMiniDB, semaphoreI2cController);

  // eslint-disable-next-line no-unused-vars
  const { temperatureOfGrowSpace, humidityOfGrowSpace, sensorsData1 } = await temperatureAndHumidityOfGrowSpace.get();
  await manageGrowSpaceTemperature(miniDB, semaphoreMiniDB, semaphoreI2cController, temperatureOfGrowSpace);

  // await manageGrowSpaceHumidity(miniDB, semaphoreMiniDB, semaphoreI2cController, humidityOfGrowSpace);

  // await manageGrowSpaceAirRenew(miniDB, semaphoreMiniDB);

  await manageGrowSpaceAirRenew(miniDB, semaphoreMiniDB, semaphoreI2cController);

  const { sensorsData2 } = await manageNutritiveSolutionTemperature(miniDB, semaphoreMiniDB, semaphoreI2cController);

  await manageNutritiveSolutionRecirculation(miniDB, semaphoreMiniDB, semaphoreI2cController);

  // await manageNutritiveSolutionEc(miniDB, semaphoreMiniDB);

  // await manageNutritiveSolutionPh(miniDB, semaphoreMiniDB);

  await manageNutritiveSolutionOxygenation(miniDB, semaphoreMiniDB, semaphoreI2cController);

  // await manageTrainingTopping(miniDB, semaphoreMiniDB);

  // await manageTrainingPruning(miniDB, semaphoreMiniDB);

  // await manageTrainingDefoliation(miniDB, semaphoreMiniDB);

  await log.save({
    temperatureOfGrowSpace1: sensorsData1.temperatureOfGrowSpace1,
    humidityOfGrowSpace1: sensorsData1.humidityOfGrowSpace1,
    temperatureOfGrowSpace2: sensorsData1.temperatureOfGrowSpace2,
    humidityOfGrowSpace2: sensorsData1.humidityOfGrowSpace2,
    temperatureOfGrowSpace3: sensorsData1.temperatureOfGrowSpace3,
    humidityOfGrowSpace3: sensorsData1.humidityOfGrowSpace3,
    temperatureOfNutSol: sensorsData2.temperatureOfNutSol,
  }, 'scheduled-stats');

  return true;
}

module.exports = {
  growMarancandinhuana,
};
