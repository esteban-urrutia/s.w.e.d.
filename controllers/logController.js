const env = require('dotenv').config().parsed;
const ObjectsToCsv = require('objects-to-csv');
const getFolderSize = require('fast-folder-size/sync');
const semaphoreLog = require('semaphore')(1);
const fs = require('fs');
const utils = require('../utils/utils');

class Log {
  constructor() {
    this.objectsToCsvInstance = new ObjectsToCsv([{}]);
    Log.instance = null;
  }

  async save(dataToLog, logType) {
    semaphoreLog.take(async () => {
      // create log according to log type
      let logFile;
      switch (logType) {
        case 'info':
          logFile = './logs/info/log_info.csv';
          this.objectsToCsvInstance.data[0].dateStamp = utils.getDateStamp();
          this.objectsToCsvInstance.data[0].data = utils.normalizeTextForCsv(dataToLog);
          break;

        case 'error':
          logFile = './logs/error/log_error.csv';
          this.objectsToCsvInstance.data[0].dateStamp = utils.getDateStamp();
          this.objectsToCsvInstance.data[0].errorMessage = utils.normalizeTextForCsv(dataToLog.message);
          this.objectsToCsvInstance.data[0].from = utils.normalizeTextForCsv(dataToLog.stack);
          break;

        case 'incomingTelegramMessages':
          logFile = './logs/incomingTelegramMessages/log_incomingTelegramMessages.csv';
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
      if (!fs.existsSync('logs/error')) { fs.mkdirSync('logs/error'); }
      if (!fs.existsSync('logs/incomingTelegramMessages')) { fs.mkdirSync('logs/incomingTelegramMessages'); }

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
          })
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
