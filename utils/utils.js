/**
 * sleep: sleep a certain amount of seconds
 * @returns {promise<void>}
 */
function sleep(seconds) {
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

module.exports = {
  sleep,
  getDateStamp,
  getTimeStamp,
  getDateStampFromTimeStamp,
};
