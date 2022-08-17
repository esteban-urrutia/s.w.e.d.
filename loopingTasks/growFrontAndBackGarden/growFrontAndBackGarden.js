/* eslint-disable no-param-reassign */
/* eslint-disable no-await-in-loop */
/* eslint-disable radix */
/* eslint-disable no-plusplus */
const {
  waterValveForIrrigationOfFrontGarden,
  waterValveForIrrigationOfBackGarden,
} = require('../../peripherals/waterValves');
const { saveMiniDB } = require('../../controllers/miniDBcontroller');

async function manageIrrigation(miniDB, semaphoreMiniDB) {
  for (let i = 0; i < miniDB.growFrontAndBackGardenParams.growSpace.irrigation.events.length; i++) {
    const { start, finish } = miniDB.growFrontAndBackGardenParams.growSpace.irrigation.events[i];

    const now = new Date();
    const startHour = parseInt(start.substring(0, 2));
    const startMinute = parseInt(start.substring(3, 5));
    const finishHour = parseInt(finish.substring(0, 2));
    const finishMinute = parseInt(finish.substring(3, 5));

    if (startHour === now.getHours()
    && startMinute === now.getMinutes()) {
      await waterValveForIrrigationOfFrontGarden.on();
      await waterValveForIrrigationOfBackGarden.on();
      miniDB.growFrontAndBackGardenParamsParams.growSpace.irrigation.status = 'irrigating';
      await saveMiniDB(semaphoreMiniDB, miniDB);
    // eslint-disable-next-line no-trailing-spaces, brace-style
    }
    else if (finishHour === now.getHours()
    && finishMinute === now.getMinutes()) {
      await waterValveForIrrigationOfFrontGarden.off();
      await waterValveForIrrigationOfBackGarden.off();
      miniDB.growFrontAndBackGardenParamsParams.growSpace.irrigation.status = null;
      await saveMiniDB(semaphoreMiniDB, miniDB);
    }
  }
  return true;
}

/**
 * growFrontAndBackGarden: - irrigates front and back garden
 */
async function growFrontAndBackGarden(miniDB, semaphoreMiniDB) {
  await manageIrrigation(miniDB, semaphoreMiniDB);

  return true;
}

module.exports = {
  growFrontAndBackGarden,
};
