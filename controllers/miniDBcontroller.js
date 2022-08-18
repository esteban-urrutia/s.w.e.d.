/* eslint-disable no-param-reassign */
/* eslint-disable no-prototype-builtins */
/* eslint-disable prefer-promise-reject-errors */
/* eslint-disable no-async-promise-executor */
const fs = require('fs');
const growFrontAndBackGardenParams = require('../loopingTasks/growFrontAndBackGarden/params.json');
const growNFTsystemParams = require('../loopingTasks/growNFTsystem/params.json');
const growMarancandinhuanaParams = require('../loopingTasks/growMarancandinhuana/params.json');

const miniDBtemplate = {
  growMarancandinhuanaParams,
  growNFTsystemParams,
  growFrontAndBackGardenParams,
};

async function readMiniDB(semaphoreMiniDB) {
  return new Promise(async (resolve, reject) => {
    semaphoreMiniDB.take(async () => {
      let miniDB;
      try {
        // if miniDB.json exist
        if (fs.existsSync('miniDB.json')) {
          // read and parse miniDB.json
          miniDB = await JSON.parse((fs.readFileSync('miniDB.json')).toString());
        } else {
          miniDB = miniDBtemplate;
          fs.writeFileSync('miniDB.json', JSON.stringify(miniDB, null, 4));
        }

        semaphoreMiniDB.leave();
        resolve(miniDB);
      } catch (error) {
        semaphoreMiniDB.leave();
        reject({
          data: 'miniDB.json',
          message: (error.hasOwnProperty('stack') ? error.stack : error),
          stack: 'miniDBcontroller -> readMiniDB',
        });
      }
    });
  });
}

async function saveMiniDB(semaphoreMiniDB, miniDB) {
  return new Promise(async (resolve, reject) => {
    semaphoreMiniDB.take(async () => {
      try {
        fs.writeFileSync('miniDB.json', JSON.stringify(miniDB, null, 4));

        semaphoreMiniDB.leave();
        resolve(true);
      } catch (error) {
        semaphoreMiniDB.leave();
        reject({
          data: miniDB,
          message: (error.hasOwnProperty('stack') ? error.stack : error),
          stack: 'miniDBcontroller -> saveMiniDB',
        });
      }
    });
  });
}

module.exports = {
  readMiniDB,
  saveMiniDB,
};
