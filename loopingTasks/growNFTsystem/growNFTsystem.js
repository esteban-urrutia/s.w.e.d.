/* eslint-disable brace-style */
/* eslint-disable no-param-reassign */
/* eslint-disable no-await-in-loop */
/* eslint-disable radix */
/* eslint-disable no-plusplus */
const {
  waterPumpForNFTsystem,
} = require('../../peripherals/waterPumps');
const { saveMiniDB } = require('../../controllers/miniDBcontroller');

async function manageIrrigation(miniDB, semaphoreMiniDB, semaphoreI2cController) {
  for (let i = 0; i < miniDB.growNFTsystemParams.growSpace.irrigation.events.length; i++) {
    const { start, finish } = miniDB.growNFTsystemParams.growSpace.irrigation.events[i];

    const now = new Date();
    const startHour = parseInt(start.substring(0, 2));
    const startMinute = parseInt(start.substring(3, 5));
    const finishHour = parseInt(finish.substring(0, 2));
    const finishMinute = parseInt(finish.substring(3, 5));

    if (startHour === now.getHours()
    && startMinute === now.getMinutes()) {
      await waterPumpForNFTsystem.on(semaphoreI2cController);
      miniDB.growNFTsystemParams.growSpace.irrigation.status = 'irrigating';
    }
    else if (finishHour === now.getHours()
    && finishMinute === now.getMinutes()) {
      await waterPumpForNFTsystem.off(semaphoreI2cController);
      miniDB.growNFTsystemParams.growSpace.irrigation.status = null;
    }
  }

  await saveMiniDB(semaphoreMiniDB, miniDB);
  return true;
}

/**
 * growNFTsystem: - irrigates NFT system
 */
async function growNFTsystem(miniDB, semaphoreMiniDB, semaphoreI2cController) {
  await manageIrrigation(miniDB, semaphoreMiniDB, semaphoreI2cController);

  return true;
}

module.exports = {
  growNFTsystem,
};
