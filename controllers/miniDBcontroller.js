/* eslint-disable no-param-reassign */
/* eslint-disable no-prototype-builtins */
/* eslint-disable prefer-promise-reject-errors */
/* eslint-disable no-async-promise-executor */
const fs = require('fs');
const growFrontAndBackGardenParams = require('../loopingTasks/growFrontAndBackGarden/params.json');
const growNFTsystemParams = require('../loopingTasks/growNFTsystem/params.json');
const growMarancandinhuanaParams = require('../loopingTasks/growMarancandinhuana/params.json');
const { execute } = require('../utils/utils');

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
          fs.readFile('miniDB.json', async (err, data) => {
            if (err) throw err;
            miniDB = await JSON.parse(data.toString());
            semaphoreMiniDB.leave();
            resolve(miniDB);
          });
        } else {
          miniDB = miniDBtemplate;
          fs.writeFile('miniDB.json', JSON.stringify(miniDB, null, 4), (err) => {
            if (err) throw err;
            semaphoreMiniDB.leave();
            resolve(miniDB);
          });
        }
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
        fs.writeFile('miniDB.json', JSON.stringify(miniDB, null, 4), (err) => {
          if (err) throw err;
          semaphoreMiniDB.leave();
          resolve(true);
        });
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

async function deleteMiniDB(semaphoreMiniDB) {
  return new Promise(async (resolve, reject) => {
    semaphoreMiniDB.take(async () => {
      try {
        await execute('sudo rm miniDB.json');

        semaphoreMiniDB.leave();
        resolve(true);
      } catch (error) {
        semaphoreMiniDB.leave();
        reject({
          message: (error.hasOwnProperty('stack') ? error.stack : error),
          stack: 'miniDBcontroller -> deleteMiniDB',
        });
      }
    });
  });
}

module.exports = {
  readMiniDB,
  saveMiniDB,
  deleteMiniDB,
};
