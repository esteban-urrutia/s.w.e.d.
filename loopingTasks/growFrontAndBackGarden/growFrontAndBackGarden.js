/* eslint-disable no-param-reassign */
/* eslint-disable no-await-in-loop */
/* eslint-disable radix */
/* eslint-disable no-plusplus */
const {
  waterValveForIrrigationOfFrontGarden,
  waterValveForIrrigationOfBackGarden,
} = require('../../peripherals/waterValves');
const { saveMiniDB } = require('../../controllers/miniDBcontroller');

async function growFrontAndBackGarden(miniDB, semaphoreMiniDB, telegramController) {
  for (let i = 0; i < miniDB.growFrontAndBackGarden.irrigationEvents.length; i++) {
    const { startTime, finishTime } = miniDB.growFrontAndBackGarden.irrigationEvents[i];

    const now = new Date();
    const startHour = parseInt(startTime.substring(0, 2));
    const startMinute = parseInt(startTime.substring(3, 5));
    const finishHour = parseInt(finishTime.substring(0, 2));
    const finishMinute = parseInt(finishTime.substring(3, 5));

    if (startHour === now.getHours() && startMinute === now.getMinutes()) {
      await waterValveForIrrigationOfFrontGarden.on();
      await waterValveForIrrigationOfBackGarden.on();
      miniDB.growFrontAndBackGarden.irrigationStatus = true;
      await saveMiniDB(semaphoreMiniDB, miniDB);
    }
    if (finishHour === now.getHours() && finishMinute === now.getMinutes()) {
      await waterValveForIrrigationOfFrontGarden.off();
      await waterValveForIrrigationOfBackGarden.off();
      miniDB.growFrontAndBackGarden.irrigationStatus = false;
      await saveMiniDB(semaphoreMiniDB, miniDB);
    }
  }

  return true;
}

module.exports = {
  growFrontAndBackGarden,
};
