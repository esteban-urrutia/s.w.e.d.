/* eslint-disable max-len */
/* eslint-disable brace-style */
/* eslint-disable no-param-reassign */
/* eslint-disable no-await-in-loop */
/* eslint-disable radix */
/* eslint-disable no-plusplus */
const {
  waterValveForIrrigationOfFrontGarden,
  waterValveForIrrigationOfBackGarden,
} = require('../../peripherals/waterValves');
const { saveMiniDB } = require('../../controllers/miniDBcontroller');
const { getTimeStamp } = require('../../utils/utils');

async function manageIrrigation(miniDB, semaphoreMiniDB, semaphoreI2cController) {
  if (getTimeStamp() > miniDB.growFrontAndBackGardenParams.growSpace.irrigation.suspendedUntil
  && miniDB.growFrontAndBackGardenParams.growSpace.irrigation.events.length > 0) {
    const now = new Date();

    for (let i = 0; i < miniDB.growFrontAndBackGardenParams.growSpace.irrigation.events.length; i++) {
      const { start, finish } = miniDB.growFrontAndBackGardenParams.growSpace.irrigation.events[i];

      const startHour = parseInt(start.substring(0, 2));
      const startMinute = parseInt(start.substring(3, 5));
      const finishHour = parseInt(finish.substring(0, 2));
      const finishMinute = parseInt(finish.substring(3, 5));

      if (startHour === now.getHours()
      && startMinute === now.getMinutes()) {
        await waterValveForIrrigationOfFrontGarden.on(semaphoreI2cController);
        await waterValveForIrrigationOfBackGarden.on(semaphoreI2cController);
        miniDB.growFrontAndBackGardenParams.growSpace.irrigation.status = 'irrigating';
        i = miniDB.growFrontAndBackGardenParams.growSpace.irrigation.events.length;
      }
      else if (finishHour === now.getHours()
      && finishMinute === now.getMinutes()) {
        await waterValveForIrrigationOfFrontGarden.off(semaphoreI2cController);
        await waterValveForIrrigationOfBackGarden.off(semaphoreI2cController);
        miniDB.growFrontAndBackGardenParams.growSpace.irrigation.status = null;
        i = miniDB.growFrontAndBackGardenParams.growSpace.irrigation.events.length;
      }
    }

    await saveMiniDB(semaphoreMiniDB, miniDB);
  }

  return true;
}

/**
 * growFrontAndBackGarden: - irrigates front and back garden
 */
async function growFrontAndBackGarden(miniDB, semaphoreMiniDB, semaphoreI2cController) {
  await manageIrrigation(miniDB, semaphoreMiniDB, semaphoreI2cController);

  return true;
}

module.exports = {
  growFrontAndBackGarden,
};
