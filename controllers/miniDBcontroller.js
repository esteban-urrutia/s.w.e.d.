/* eslint-disable no-param-reassign */
/* eslint-disable no-prototype-builtins */
/* eslint-disable prefer-promise-reject-errors */
/* eslint-disable no-async-promise-executor */
const fs = require('fs');

async function read(semaphoreMiniDB) {
  return new Promise(async (resolve, reject) => {
    semaphoreMiniDB.take(async () => {
      let miniDB;
      try {
        // if miniDB.json exist
        if (fs.existsSync('miniDB.json')) {
          // read and parse miniDB.json
          miniDB = await JSON.parse((await fs.readFileSync('miniDB.json')).toString());
        } else {
          miniDB = {
            tradeEnabled: false,
            selectedSymbol: null,
            activeSymbol: null,
            lastOrder: null,
            ipBanUntil: null,
            timestampStartRoundOfTrade: null,
            sellOrdersFilled: 0,
            buyOrdersFilled: 0,
            priceOfLastBuyOrder: -1,
          };
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

async function save(semaphoreMiniDB, miniDB) {
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

async function update(semaphoreMiniDB, param, data) {
  return new Promise(async (resolve, reject) => {
    await read(semaphoreMiniDB)
      .then(async (miniDB) => {
        miniDB[param] = data;

        await save(semaphoreMiniDB, miniDB)
          .then(async () => {
            resolve(true);
          })
          .catch(async (error) => {
            reject(error);
          });
      })
      .catch(async (error) => {
        reject(error);
      });
  });
}

module.exports = {
  read,
  save,
  update,
};
