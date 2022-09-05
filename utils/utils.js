/* eslint-disable no-unneeded-ternary */
/* eslint-disable no-prototype-builtins */
/* eslint-disable radix */
const { exec } = require('child_process');

/**
 * sleep: sleep a certain amount of seconds
 * @returns {promise<void>}
 */
async function sleep(seconds) {
  // eslint-disable-next-line no-promise-executor-return
  return new Promise((resolve) => setTimeout(resolve, seconds * 1000));
}

/**
 * createDateStamp: create dateStamp in format like 31-12-2021_23:59:59
 * @returns {String}
 */
function createDateStamp(dateObject) {
  return `${(`0${dateObject.getDate()}`).slice(-2)}-${
    (`0${dateObject.getMonth() + 1}`).slice(-2)}-${
    dateObject.getFullYear()}_${
    (`0${dateObject.getHours()}`).slice(-2)}:${
    (`0${dateObject.getMinutes()}`).slice(-2)}:${
    (`0${dateObject.getSeconds()}`).slice(-2)}`;
}

/**
 * getDateStamp: get dateStamp
 * @returns {String}
 */
function getDateStamp() {
  return createDateStamp(new Date());
}

/**
 * getTimeStamp: get timeStamp in 10 character format
 * @returns {number}
 */
function getTimeStamp() {
  return Math.floor(new Date() / 1000);
}

/**
 * getDateStamp: get dateStamp in format like 31-12-2021_23:59:59,
 * from timeStamp in 10 character format
 * @returns {String}
 */
function getDateStampFromTimeStamp(timeStamp) {
  return createDateStamp(
    (timeStamp.toString().length === 10) ? new Date(timeStamp * 1000) : new Date(timeStamp),
  );
}

/**
 * getPhotoperiod: get photoperiod based on start/finish time
 * @returns {String}
 */
function getPhotoperiod(miniDB) {
  const now = new Date();

  const startTime = new Date();
  const startHour = parseInt(
    miniDB
      .growMarancandinhuanaParams
      .training
      .photoperiod
      .start.substring(0, 2),
  );
  const startMinute = parseInt(
    miniDB
      .growMarancandinhuanaParams
      .training
      .photoperiod
      .start.substring(3, 5),
  );
  startTime.setHours(startHour);
  startTime.setMinutes(startMinute);
  startTime.setSeconds(0);
  startTime.setMilliseconds(0);

  const finishTime = new Date();
  const finishHour = parseInt(
    miniDB
      .growMarancandinhuanaParams
      .training
      .photoperiod
      .finish.substring(0, 2),
  );
  const finishMinute = parseInt(
    miniDB
      .growMarancandinhuanaParams
      .training
      .photoperiod
      .finish.substring(3, 5),
  );
  finishTime.setHours(finishHour);
  finishTime.setMinutes(finishMinute);
  finishTime.setSeconds(0);
  finishTime.setMilliseconds(0);

  if (startTime.getTime() <= now.getTime()
  && now.getTime() <= finishTime.getTime()) {
    return 'day';
  }
  return 'night';
}

/**
 * normalize: normalize text for csv log
 * @returns {String}
 */
function normalizeTextForCsv(param) {
  let response = (typeof param === 'object'
    ? JSON.stringify(param)
    : param
  );
  if (response === '{}') {
    if (param.hasOwnProperty('stack')) {
      response = param.stack.toString();
    } else {
      response = param.toString();
    }
  }
  return response.replace(/[\n]/g, '  ');
}

/**
 * normalize: normalize for telegram message
 * @returns {String}
 */
function normalizeTextForTelegramMessage(param) {
  let response = (typeof param === 'object'
    ? JSON.stringify(param)
    : param
  );
  if (response === '{}') {
    if (param.hasOwnProperty('stack')) {
      response = param.stack.toString();
    } else {
      response = param.toString();
    }
  }
  return response;
}

function execute(command) {
  return new Promise((resolve, reject) => {
    exec(command, (error, stdout, stderr) => {
      if (error || stderr) {
        reject(error ? error : stderr);
      }
      resolve(stdout);
    });
  });
}

module.exports = {
  sleep,
  getDateStamp,
  getTimeStamp,
  getDateStampFromTimeStamp,
  getPhotoperiod,
  normalizeTextForCsv,
  normalizeTextForTelegramMessage,
  execute,
};
