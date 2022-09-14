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
 * createDateStamp: create dateStamp in format like 31-12-2021  23:59:59
 * @returns {String}
 */
function createDateStamp(dateObject) {
  return `${(`0${dateObject.getDate()}`).slice(-2)}-${
    (`0${dateObject.getMonth() + 1}`).slice(-2)}-${
    dateObject.getFullYear()}  ${
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
 * getDateStamp: get dateStamp in format like 31-12-2021  23:59:59,
 * from timeStamp in 10 character format
 * @returns {String}
 */
function getDateStampFromTimeStamp(timeStamp) {
  return createDateStamp(
    (timeStamp.toString().length === 10) ? new Date(timeStamp * 1000) : new Date(timeStamp),
  );
}

/**
 * validates if string is made only of digits
 * @returns {boolean}
 */
function isDigit(string) {
  return [...string].every((c) => '0123456789'.includes(c));
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
  isDigit,
  normalizeTextForCsv,
  normalizeTextForTelegramMessage,
  execute,
};
