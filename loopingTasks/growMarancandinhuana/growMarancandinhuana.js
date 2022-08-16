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
      miniDB.growNFTsystem.irrigationStatus = false;
      await saveMiniDB(semaphoreMiniDB, miniDB);
    }
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
async function growMarancandinhuana(miniDB, semaphoreMiniDB, telegram) {
  await manageIrrigation(miniDB, semaphoreMiniDB);

  return true;
}

module.exports = {
  growMarancandinhuana,
};
