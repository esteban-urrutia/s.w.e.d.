/* eslint-disable brace-style */
/* eslint-disable no-shadow */
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

      // if miniDB.json exist
      if (fs.existsSync('miniDB.json')) {
        // read and parse miniDB.json
        fs.readFile('miniDB.json', async (err, data) => {
          if (err) {
            semaphoreMiniDB.leave();
            reject({
              message: (err.hasOwnProperty('stack') ? err.stack : err),
              stack: 'miniDBcontroller -> readMiniDB -> readFile',
            });
          }

          try {
            miniDB = await JSON.parse(data.toString());
            semaphoreMiniDB.leave();
            resolve(miniDB);
          } catch (error) {
            try {
              await execute('sudo rm miniDB.json');

              miniDB = miniDBtemplate;
              fs.writeFile('miniDB.json', JSON.stringify(miniDB, null, 4), (err) => {
                semaphoreMiniDB.leave();
                if (err) { reject({
                  message: (err.hasOwnProperty('stack') ? err.stack : err),
                  stack: 'miniDBcontroller -> readMiniDB -> parsing error -> writeFile',
                }); }

                resolve(miniDB);
              });
            } catch (error2) {
              semaphoreMiniDB.leave();

              reject({
                message: (error2.hasOwnProperty('stack') ? error2.stack : error2),
                stack: 'miniDBcontroller -> readMiniDB -> parsing error -> deleting error',
              });
            }
          }
        });
      }
      // if miniDB.json don't exist
      else {
        miniDB = miniDBtemplate;
        fs.writeFile('miniDB.json', JSON.stringify(miniDB, null, 4), (err) => {
          semaphoreMiniDB.leave();

          if (err) { reject({
            message: (err.hasOwnProperty('stack') ? err.stack : err),
            stack: 'miniDBcontroller -> readMiniDB -> writeFile',
          }); }

          resolve(miniDB);
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
          semaphoreMiniDB.leave();

          if (err) { reject({
            data: miniDB,
            message: (err.hasOwnProperty('stack') ? err.stack : err),
            stack: 'miniDBcontroller -> saveMiniDB',
          }); }

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
