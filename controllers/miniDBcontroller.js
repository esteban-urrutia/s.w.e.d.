/* eslint-disable no-param-reassign */
/* eslint-disable no-prototype-builtins */
/* eslint-disable prefer-promise-reject-errors */
/* eslint-disable no-async-promise-executor */
const fs = require('fs');
const growFrontAndBackGarden = require('../loopingTasks/growFrontAndBackGarden/params.json');
const growNFTsystem = require('../loopingTasks/growNFTsystem/params.json');

const miniDBtemplate = {
  growFrontAndBackGarden,
  growNFTsystem,
};

async function readMiniDB(semaphoreMiniDB) {
  return new Promise(async (resolve, reject) => {
    semaphoreMiniDB.take(async () => {
      let miniDB;
      try {
        // if miniDB.json exist
        if (await fs.existsSync('miniDB.json')) {
          // read and parse miniDB.json
          miniDB = await JSON.parse((await fs.readFileSync('miniDB.json')).toString());
        } else {
          miniDB = miniDBtemplate;
          await fs.writeFileSync('miniDB.json', JSON.stringify(miniDB, null, 4));
        }

        semaphoreMiniDB.leave();
        resolve(miniDB);
      } catch (error) {
        semaphoreMiniDB.leave();
        reject({
          data: 'miniDB.json',
          errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
          from: 'miniDBcontroller -> readMiniDB',
        });
      }
    });
  });
}

async function saveMiniDB(semaphoreMiniDB, miniDB) {
  return new Promise(async (resolve, reject) => {
    semaphoreMiniDB.take(async () => {
      try {
        await fs.writeFileSync('miniDB.json', JSON.stringify(miniDB, null, 4));

        semaphoreMiniDB.leave();
        resolve(true);
      } catch (error) {
        semaphoreMiniDB.leave();
        reject({
          data: miniDB,
          errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
          from: 'miniDBcontroller -> saveMiniDB',
        });
      }
    });
  });
}

module.exports = {
  readMiniDB,
  saveMiniDB,
};
