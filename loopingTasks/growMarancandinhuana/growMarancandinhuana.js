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

async function manageGrowSpaceTemperatureAndHumidity(miniDB, semaphoreMiniDB) {
  const { temperature, humidity } = await temperatureAndHumidityOfGrowSpace.get();

  const photoperiod = getPhotoperiod();







  if (
    temperature
    <= miniDB
      .growMarancandinhuanaParams
      .growSpace
      .temperature
      .values[photoperiod]
      .min
  ) {
    await airHeaterOfGrowSpace.on();
    miniDB.growMarancandinhuanaParams.growSpace.temperature.status = 'heating';
    await saveMiniDB(semaphoreMiniDB, miniDB);
  // eslint-disable-next-line brace-style
  }
  else if (
    temperature
    >= miniDB
      .growMarancandinhuanaParams
      .growSpace
      .temperature
      .values[photoperiod]
      .max
  ) {
    await airHeaterOfGrowSpace.off();
    miniDB.growMarancandinhuanaParams.growSpace.temperature.status = 'cooling';
    await saveMiniDB(semaphoreMiniDB, miniDB);
  }
  else {
    await airHeaterOfGrowSpace.off();
    miniDB.growMarancandinhuanaParams.growSpace.temperature.status = null;
    await saveMiniDB(semaphoreMiniDB, miniDB);
  }






  if (
    humidity
    <= miniDB
      .growMarancandinhuanaParams
      .growSpace
      .humidity
      .values[photoperiod]
      .min
  ) {
    await humidifierOfGrowSpace.on();
    miniDB.growMarancandinhuanaParams.growSpace.humidity.status = 'humidifying';
    await saveMiniDB(semaphoreMiniDB, miniDB);
  }
  else if (
    humidity
    >= miniDB
      .growMarancandinhuanaParams
      .growSpace
      .humidity
      .values[photoperiod]
      .max
  ) {
    await humidifierOfGrowSpace.off();
    miniDB.growMarancandinhuanaParams.growSpace.humidity.status = null;
    await saveMiniDB(semaphoreMiniDB, miniDB);
  }
  else {
    await humidifierOfGrowSpace.off();
    miniDB.growMarancandinhuanaParams.growSpace.humidity.status = null;
    await saveMiniDB(semaphoreMiniDB, miniDB);
  }



  
  return true;
}

/**
 * growMarancandinhuana -> Manages:
 *   growSpace
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
  await manageGrowSpaceTemperatureAndHumidity(miniDB, semaphoreMiniDB);
  await manageGrowSpaceHumidity(miniDB, semaphoreMiniDB);
  await manageGrowSpaceAirRenew(miniDB, semaphoreMiniDB);
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
