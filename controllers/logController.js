/* eslint-disable no-console */
/* eslint-disable no-prototype-builtins */
/* eslint-disable max-len */
const ObjectsToCsv = require('objects-to-csv');
const semaphoreLog = require('semaphore')(1);
const fs = require('fs');
const env = require('dotenv').config().parsed;
const utils = require('../utils/utils');

let lastMinute = 60;

class Log {
  constructor() {
    this.objectsToCsvInstance = new ObjectsToCsv([{}]);
    Log.instance = null;
  }

  async saveOncePerMinute(dataToLog, logType) {
    const currentMinute = (new Date()).getMinutes();

    if (currentMinute !== lastMinute) {
      lastMinute = currentMinute;
      await this.save(dataToLog, logType);
    }
  }

  async save(dataToLog, logType) {
    semaphoreLog.take(async () => {
      // create log according to log type
      let logFile;
      switch (logType) {
        case 'info':
          logFile = './logs/info/log-info.csv';
          this.objectsToCsvInstance.data[0].dateStamp = utils.getDateStamp();
          this.objectsToCsvInstance.data[0].data = utils.normalizeTextForCsv(dataToLog);
          break;

        case 'scheduled-stats':
          logFile = './logs/stats/log-scheduled-stats.csv';
          this.objectsToCsvInstance.data[0].dateStamp = utils.getDateStamp();
          this.objectsToCsvInstance.data[0].temperatureOfGrowSpace1 = dataToLog.temperatureOfGrowSpace1;
          this.objectsToCsvInstance.data[0].humidityOfGrowSpace1 = dataToLog.humidityOfGrowSpace1;
          this.objectsToCsvInstance.data[0].temperatureOfGrowSpace2 = dataToLog.temperatureOfGrowSpace2;
          this.objectsToCsvInstance.data[0].humidityOfGrowSpace2 = dataToLog.humidityOfGrowSpace2;
          this.objectsToCsvInstance.data[0].temperatureOfGrowSpace3 = dataToLog.temperatureOfGrowSpace3;
          this.objectsToCsvInstance.data[0].humidityOfGrowSpace3 = dataToLog.humidityOfGrowSpace3;
          this.objectsToCsvInstance.data[0].temperatureOfNutSol = dataToLog.temperatureOfNutSol;
          break;

        case 'manual-stats-PH':
          logFile = './logs/stats/log-manual-stats-PH.csv';
          this.objectsToCsvInstance.data[0].dateStamp = utils.getDateStamp();
          this.objectsToCsvInstance.data[0].PHofNutSol = dataToLog.PHofNutSol;
          break;

        case 'manual-stats-EC':
          logFile = './logs/stats/log-manual-stats-EC.csv';
          this.objectsToCsvInstance.data[0].dateStamp = utils.getDateStamp();
          this.objectsToCsvInstance.data[0].ECofNutSol = dataToLog.ECofNutSol;
          break;

        case 'error':
          logFile = './logs/error/log-error.csv';
          this.objectsToCsvInstance.data[0].dateStamp = utils.getDateStamp();
          this.objectsToCsvInstance.data[0].errorMessage = utils.normalizeTextForCsv(dataToLog.message);
          this.objectsToCsvInstance.data[0].from = utils.normalizeTextForCsv(dataToLog.hasOwnProperty('stack') ? dataToLog.stack : dataToLog);
          break;

        case 'incomingTelegramMessages':
          logFile = './logs/incomingTelegramMessages/log-incomingTelegramMessages.csv';
          this.objectsToCsvInstance.data[0].dateStamp = utils.getDateStamp();
          this.objectsToCsvInstance.data[0].from = dataToLog.from;
          this.objectsToCsvInstance.data[0].command = utils.normalizeTextForCsv(dataToLog.command);
          break;

        default:
          break;
      }

      // create logs folders if they don't exist
      if (!fs.existsSync('logs')) { fs.mkdirSync('logs'); }
      if (!fs.existsSync('logs/info')) { fs.mkdirSync('logs/info'); }
      if (!fs.existsSync('logs/stats')) { fs.mkdirSync('logs/stats'); }
      if (!fs.existsSync('logs/error')) { fs.mkdirSync('logs/error'); }
      if (!fs.existsSync('logs/incomingTelegramMessages')) { fs.mkdirSync('logs/incomingTelegramMessages'); }

      // rotate log of any file greater than 10 mb
      const maxSizeOfEachLogFileInMB = parseInt(env.logs_maxSizeOfEachLogFileInMB, 10);
      if (fs.existsSync('./logs/info/log-info.csv')
      && (fs.statSync('./logs/info/log-info.csv').size / (1024 * 1024)) > maxSizeOfEachLogFileInMB) {
        await utils.execute(`sudo mv ./logs/info/log-info.csv ./logs/info/log-info-${utils.getDateStampForFileName()}.csv`);
      }
      if (fs.existsSync('./logs/stats/log-scheduled-stats.csv')
      && (fs.statSync('./logs/stats/log-scheduled-stats.csv').size / (1024 * 1024)) > maxSizeOfEachLogFileInMB) {
        await utils.execute(`sudo mv ./logs/stats/log-scheduled-stats.csv ./logs/stats/log-scheduled-stats-${utils.getDateStampForFileName()}.csv`);
      }
      if (fs.existsSync('./logs/stats/log-manual-stats-EC.csv')
      && (fs.statSync('./logs/stats/log-manual-stats-EC.csv').size / (1024 * 1024)) > maxSizeOfEachLogFileInMB) {
        await utils.execute(`sudo mv ./logs/stats/log-manual-stats-EC.csv ./logs/stats/log-manual-stats-EC-${utils.getDateStampForFileName()}.csv`);
      }
      if (fs.existsSync('./logs/stats/log-manual-stats-PH.csv')
      && (fs.statSync('./logs/stats/log-manual-stats-PH.csv').size / (1024 * 1024)) > maxSizeOfEachLogFileInMB) {
        await utils.execute(`sudo mv ./logs/stats/log-manual-stats-PH.csv ./logs/stats/log-manual-stats-PH-${utils.getDateStampForFileName()}.csv`);
      }
      if (fs.existsSync('./logs/error/log-error.csv')
      && (fs.statSync('./logs/error/log-error.csv').size / (1024 * 1024)) > maxSizeOfEachLogFileInMB) {
        await utils.execute(`sudo mv ./logs/error/log-error.csv ./logs/error/log-error-${utils.getDateStampForFileName()}.csv`);
      }
      if (fs.existsSync('./logs/incomingTelegramMessages/log-incomingTelegramMessages.csv')
      && (fs.statSync('./logs/incomingTelegramMessages/log-incomingTelegramMessages.csv').size / (1024 * 1024)) > maxSizeOfEachLogFileInMB) {
        await utils.execute(`sudo mv ./logs/incomingTelegramMessages/log-incomingTelegramMessages.csv ./logs/incomingTelegramMessages/log-incomingTelegramMessages-${utils.getDateStampForFileName()}.csv`);
      }

      // save to log's csv file
      await this.objectsToCsvInstance.toDisk(logFile, { append: true })
        .then(() => {
          this.cleanObjectsToCsvInstance();
          semaphoreLog.leave();
        })
        .catch((error) => {
          console.log(`Error guardando log  :
              \n${JSON.stringify(this.objectsToCsvInstance.data[0])}
              \nError details  :  
              ${error.hasOwnProperty('stack') ? error.stack : error}`);
          this.cleanObjectsToCsvInstance();
          semaphoreLog.leave();
        });
    });
  }

  cleanObjectsToCsvInstance() {
    this.objectsToCsvInstance.data[0] = {};
  }

  static getInstance() {
    if (!Log.instance) {
      Log.instance = new Log();
    }
    return Log.instance;
  }
}

module.exports = Log;
