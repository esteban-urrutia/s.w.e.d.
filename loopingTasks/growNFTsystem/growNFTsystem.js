/* eslint-disable no-param-reassign */
/* eslint-disable no-await-in-loop */
/* eslint-disable radix */
/* eslint-disable no-plusplus */
const {
  waterPumpForNFTsystem,
} = require('../../peripherals/waterPumps');
const { saveMiniDB } = require('../../controllers/miniDBcontroller');

async function manageIrrigation(miniDB, semaphoreMiniDB) {
  for (let i = 0; i < miniDB.growNFTsystem.events.length; i++) {
    const { start, finish } = miniDB.growNFTsystem.events[i];

    const now = new Date();
    const startHour = parseInt(start.substring(0, 2));
    const startMinute = parseInt(start.substring(3, 5));
    const finishHour = parseInt(finish.substring(0, 2));
    const finishMinute = parseInt(finish.substring(3, 5));

    if (startHour === now.getHours()
    && startMinute === now.getMinutes()) {
      await waterPumpForNFTsystem.on();
      miniDB.growNFTsystem.status = true;
      await saveMiniDB(semaphoreMiniDB, miniDB);
    // eslint-disable-next-line no-trailing-spaces, brace-style
    } 
    else if (finishHour === now.getHours()
    && finishMinute === now.getMinutes()) {
      await waterPumpForNFTsystem.off();
      miniDB.growNFTsystem.status = false;
      await saveMiniDB(semaphoreMiniDB, miniDB);
    }
  }
  return true;
}

/**
 * growNFTsystem: - irrigates NFT system
 */
async function growNFTsystem(miniDB, semaphoreMiniDB, telegram) {
  await manageIrrigation(miniDB, semaphoreMiniDB);

  return true;
}

module.exports = {
  growNFTsystem,
};
