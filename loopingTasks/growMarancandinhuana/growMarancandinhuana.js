/* eslint-disable no-param-reassign */
/* eslint-disable no-await-in-loop */
/* eslint-disable radix */
/* eslint-disable no-plusplus */
const {
  waterPumpForNFTsystem,
} = require('../../peripherals/waterPumps');
const { saveMiniDB } = require('../../controllers/miniDBcontroller');

async function manageIrrigation(miniDB, semaphoreMiniDB) {
  for (let i = 0; i < miniDB.growNFTsystem.irrigationEvents.length; i++) {
    const { startTime, finishTime } = miniDB.growNFTsystem.irrigationEvents[i];

    const now = new Date();
    const startHour = parseInt(startTime.substring(0, 2));
    const startMinute = parseInt(startTime.substring(3, 5));
    const finishHour = parseInt(finishTime.substring(0, 2));
    const finishMinute = parseInt(finishTime.substring(3, 5));

    if (startHour === now.getHours()
    && startMinute === now.getMinutes()) {
      await waterPumpForNFTsystem.on();
      miniDB.growNFTsystem.irrigationStatus = true;
      await saveMiniDB(semaphoreMiniDB, miniDB);
    // eslint-disable-next-line no-trailing-spaces, brace-style
    } 
    else if (finishHour === now.getHours()
    && finishMinute === now.getMinutes()) {
      await waterPumpForNFTsystem.off();
      await saveMiniDB(semaphoreMiniDB, miniDB);
    }
  }
  return true;
}

/**
 * growMarancandinhuana -> Manages:
 * 1.-  air temperature
 * 2.-  air humidity
 * 3.-  air renew on grow space
 * 4.-  nutSol temperature
 * 5.-  nutSol recirculation
 * 6.-  nutSol EC
 * 7.-  nutSol PH
 * 8.-  nutSol oxygenation
 * 9.-  topping
 * 10.- pruning
 * 11.- defoliation
 * 
 * 
 */
async function growMarancandinhuana(miniDB, semaphoreMiniDB, telegramController) {
  await manageIrrigation(miniDB, semaphoreMiniDB);

  return true;
}

module.exports = {
  growMarancandinhuana,
};
