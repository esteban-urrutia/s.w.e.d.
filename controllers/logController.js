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

  async rotateLogs() {
    return new Promise(async (resolve, reject) => {
      const logs = { filesInFolder: {}, fileSize: {} };

      try { logs.logsFolderSize = getFolderSize('./logs') / (1000 * 1000); } catch (error) { logs.logsFolderSize = undefined; }
      try { logs.filesInFolder.info = fs.readdirSync('./logs/info').length; } catch (error) { logs.filesInFolder.info = undefined; }
      try { logs.filesInFolder.error = fs.readdirSync('./logs/error').length; } catch (error) { logs.filesInFolder.error = undefined; }
      try { logs.filesInFolder.incomingTelegramMessages = fs.readdirSync('./logs/incomingTelegramMessages').length; } catch (error) { logs.filesInFolder.incomingTelegramMessages = undefined; }
      try { logs.fileSize.info = (fs.statSync('./logs/info/log_info.csv')).size / (1000 * 1000); } catch (error) { logs.fileSize.info = undefined; }
      try { logs.fileSize.error = (fs.statSync('./logs/error/log_error.csv')).size / (1000 * 1000); } catch (error) { logs.fileSize.error = undefined; }
      try { logs.fileSize.incomingTelegramMessages = (fs.statSync('./logs/incomingTelegramMessages/log_incomingTelegramMessages.csv')).size / (1000 * 1000); } catch (error) { logs.fileSize.incomingTelegramMessages = undefined; }

      // if a log file size in greater than 10 MB... rotate this log
      // then... if logs folder size is greater than 2 GB...
      // erase oldest logFile from folder with more logFiles
      await this.renameLogFilesGreaterThanSpecified(logs)
        .then(async (response) => {
          if (!response.hasOwnProperty('thrownOff')) {
            await this.eraseOldestLogFileFromFolderWithMoreLogFiles(logs)
              .then((innerResponse) => {
                if (!innerResponse.hasOwnProperty('thrownOff')) {
                  resolve(innerResponse);
                }
                reject(innerResponse);
              });
          }
          reject(response);
        });
    });
  }

  cleanObjectsToCsvInstance() {
    this.objectsToCsvInstance.data[0] = {};
  }

  renameLogFilesGreaterThanSpecified(logs) {
    // eslint-disable-next-line no-async-promise-executor
    return new Promise(async (resolve) => {
      const logFileSizeArray = Object.entries(logs.fileSize);
      // eslint-disable-next-line no-unused-expressions
      (logFileSizeArray.length === 0) ? resolve(true) : null;
      for (let i = 0; i < logFileSizeArray.length; i + 1) {
        try {
          if (logFileSizeArray[i][1]
            && logFileSizeArray[i][1] > parseInt(env.logs_maxSizeOfEachLogFileInMB, 10)) {
            fs.renameSync(`./logs/${logFileSizeArray[i][0]}/log_${logFileSizeArray[i][0]}.csv`, (`./logs/${logFileSizeArray[i][0]}/log_${logFileSizeArray[i][0]}__${utils.getDateStamp()}.csv`).replace(/[:]/g, '.'));
          }
          // if this is the last logFile, being analyzed
          if (i + 1 === logFileSizeArray.length) {
            resolve(true);
          }
        } catch (error) {
          error.thrownOff = true;
          resolve(error);
        }
      }
    });
  }

  eraseOldestLogFileFromFolderWithMoreLogFiles(logs) {
    // eslint-disable-next-line no-async-promise-executor
    return new Promise(async (resolve) => {
      const folderWithMoreFiles = { folder: '', files: 0 };
      let oldestFile;

      // checks if logs folder size is greater than 2000 MB
      if (logs.logsFolderSize
        && logs.logsFolderSize > parseInt(env.logs_maxSizeOfLogsFolderInMB, 10)) {
        // identify folder with more files
        // eslint-disable-next-line no-restricted-syntax
        for (const filesInFolder of Object.entries(logs.filesInFolder)) {
          if (filesInFolder[1] > folderWithMoreFiles.files) {
            folderWithMoreFiles.folder = filesInFolder[0];
            folderWithMoreFiles.files = filesInFolder[1];
          }
        }

        // erase oldest file from this folder
        try {
          oldestFile = fs.readdirSync(`./logs/${folderWithMoreFiles.folder}`);
          oldestFile = oldestFile[oldestFile.length - 1];
          fs.unlinkSync(`./logs/${folderWithMoreFiles.folder}/${oldestFile}`);

          resolve(true);
        } catch (error) {
          error.thrownOff = true;
          resolve(error);
        }
      } else {
        resolve(true);
      }
    });
  }

  static getInstance() {
    if (!Log.instance) {
      Log.instance = new Log();
    }
    return Log.instance;
  }
}

module.exports = Log;
