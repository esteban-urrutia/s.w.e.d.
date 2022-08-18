/* eslint-disable no-await-in-loop */
/* eslint-disable class-methods-use-this */
/* eslint-disable prefer-destructuring */
/* eslint-disable no-prototype-builtins */
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

  normalize(param) {
    return (typeof param === 'object'
      ? JSON.stringify(param)
      : param
    ).replace(/[\n]/g, '  ');
  }

  async save(dataToLog, logType) {
    semaphoreLog.take(async () => {
      // create log according to log type
      let logFile;
      switch (logType) {
        case 'info':
          logFile = './logs/info/log_info.csv';
          this.objectsToCsvInstance.data[0].dateStamp = utils.getDateStamp();
          this.objectsToCsvInstance.data[0].data = this.normalize(dataToLog);
          break;

        case 'error':
          logFile = './logs/error/log_error.csv';
          this.objectsToCsvInstance.data[0].dateStamp = utils.getDateStamp();
          this.objectsToCsvInstance.data[0].errorMessage = this.normalize(dataToLog.message);
          this.objectsToCsvInstance.data[0].from = this.normalize(dataToLog.stack);
          break;

        case 'incomingTelegramMessages':
          logFile = './logs/incomingTelegramMessages/log_incomingTelegramMessages.csv';
          this.objectsToCsvInstance.data[0].dateStamp = utils.getDateStamp();
          this.objectsToCsvInstance.data[0].from = dataToLog.from;
          this.objectsToCsvInstance.data[0].command = this.normalize(dataToLog.command);
          break;

        default:
          break;
      }

      // create logs folders if they don't exist
      if (!fs.existsSync('logs')) { await fs.mkdirSync('logs'); }
      if (!fs.existsSync('logs/info')) { await fs.mkdirSync('logs/info'); }
      if (!fs.existsSync('logs/error')) { await fs.mkdirSync('logs/error'); }
      if (!fs.existsSync('logs/incomingTelegramMessages')) { await fs.mkdirSync('logs/incomingTelegramMessages'); }

      // save to log's csv file
      await this.objectsToCsvInstance.toDisk(logFile, { append: true })
        .catch((error) => {
          console.log(`Error guardando log  :  \n${JSON.stringify(this.objectsToCsvInstance.data[0])}\nError details  :  ${error.hasOwnProperty('stack') ? error.stack : error}`);
        })
        .finally(() => {
          this.cleanObjectsToCsvInstance();
          semaphoreLog.leave();
        });
    });
  }

  async rotateLogs() {
    return new Promise((resolve, reject) => {
      semaphoreLog.take(async () => {
        const logs = { filesInFolder: {}, fileSize: {} };

        try { logs.logsFolderSize = getFolderSize('./logs') / (1000 * 1000); } catch (error) { logs.logsFolderSize = undefined; }
        try { logs.filesInFolder.info = fs.readdirSync('./logs/info').length; } catch (error) { logs.filesInFolder.info = undefined; }
        try { logs.filesInFolder.error = fs.readdirSync('./logs/error').length; } catch (error) { logs.filesInFolder.error = undefined; }
        try { logs.filesInFolder.incomingTelegramMessages = fs.readdirSync('./logs/incomingTelegramMessages').length; } catch (error) { logs.filesInFolder.incomingTelegramMessages = undefined; }
        try { logs.fileSize.info = (await fs.statSync('./logs/info/log_info.csv')).size / (1000 * 1000); } catch (error) { logs.fileSize.info = undefined; }
        try { logs.fileSize.error = (await fs.statSync('./logs/error/log_error.csv')).size / (1000 * 1000); } catch (error) { logs.fileSize.error = undefined; }
        try { logs.fileSize.incomingTelegramMessages = (await fs.statSync('./logs/incomingTelegramMessages/log_incomingTelegramMessages.csv')).size / (1000 * 1000); } catch (error) { logs.fileSize.incomingTelegramMessages = undefined; }

        // if a log file size in greater than 10 MB... rotate this log
        // then... if logs folder size is greater than 2 GB...
        // erase oldest logFile from folder with more logFiles
        await this.renameLogFilesGreaterThanSpecified(logs)
          .then(async (response) => {
            if (!response.hasOwnProperty('redirectedError')) {
              await this.eraseOldestLogFileFromFolderWithMoreLogFiles(logs)
                .then((innerResponse) => {
                  semaphoreLog.leave();
                  if (!innerResponse.hasOwnProperty('redirectedError')) {
                    resolve(innerResponse);
                  }
                  reject(innerResponse);
                });
            }
            semaphoreLog.leave();
            reject(response);
          });
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
            await fs.renameSync(`./logs/${logFileSizeArray[i][0]}/log_${logFileSizeArray[i][0]}.csv`, (`./logs/${logFileSizeArray[i][0]}/log_${logFileSizeArray[i][0]}__${utils.getDateStamp()}.csv`).replace(/[:]/g, '.'));
          }
          // if this is the last logFile, being analyzed
          if (i + 1 === logFileSizeArray.length) {
            resolve(true);
          }
        } catch (error) {
          resolve({
            data: null,
            redirectedError: true,
            errorMessage: `error rotating ${logFileSizeArray[0]}`,
            from: 'logController -> rotateLogs -> if a log file size in greater than 10 MB... rotate this log',
          });
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
          oldestFile = await fs.readdirSync(`./logs/${folderWithMoreFiles.folder}`);
          oldestFile = oldestFile[oldestFile.length - 1];
          await fs.unlinkSync(`./logs/${folderWithMoreFiles.folder}/${oldestFile}`);

          resolve(true);
        } catch (error) {
          resolve({
            data: null,
            redirectedError: true,
            errorMessage: `error erasing ${oldestFile} from folder ${folderWithMoreFiles.folder}`,
            from: 'logController -> rotateLogs -> eraseOldestLogFromFolderWithMoreLogs',
          });
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
