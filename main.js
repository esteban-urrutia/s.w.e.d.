/* eslint-disable max-len */
/* eslint-disable import/newline-after-import */
const semaphoreMiniDB = require('semaphore')(1);
const semaphoreI2cController = require('semaphore')(1);
const env = require('dotenv').config().parsed;
const schedule = require('node-schedule');
const TelegramBot = require('node-telegram-bot-api');

const log = require('./controllers/logController').getInstance();
const secondaryTasksController = require('./controllers/secondaryTasksController');
const telegramController = require('./controllers/telegramController');
const {
  getDateStampFromTimeStamp,
  getDateStamp,
  sleep,
} = require('./utils/utils');
const { readMiniDB } = require('./controllers/miniDBcontroller');
const { growFrontAndBackGarden } = require('./loopingTasks/growFrontAndBackGarden/growFrontAndBackGarden');
const { growNFTsystem } = require('./loopingTasks/growNFTsystem/growNFTsystem');
const { growMarancandinhuana } = require('./loopingTasks/growMarancandinhuana/growMarancandinhuana');
let telegram;

sleep(10)
  .then(() => {
    telegram = (env.telegram_enabled === 'true') ? new TelegramBot(env.telegram_token, { polling: true }) : null;
    // eslint-disable-next-line no-unused-vars
    let internetStatus = null;

    /* - - - - - - - - - - STARTUP - - - - - - - - - - */
    // boot notification
    telegramController.sendMessage(telegram, 'SmokeWeedEveryDay.jpg', 'image')
      .catch(async (telegramError) => {
        await log.save(telegramError, 'error');
      });
    /* - - - - - - - - - - - - - - - - - - - - - - - - */

    /* - - - - - - - - - - LOOPING TASKS - - - - - - - */
    schedule.scheduleJob(env.loopingTasks_frequencyOfLoop, async () => {
      const miniDB = await readMiniDB(semaphoreMiniDB);

      // manage grow of Marancandinhuana
      await growMarancandinhuana(miniDB, semaphoreMiniDB, semaphoreI2cController)
        .catch(async (error) => {
          await log.save(error, 'error');
          await telegramController.sendMessage(telegram, error, 'text')
            .catch(async (telegramError) => {
              await log.save(telegramError, 'error');
            });
        });

      // manage grow of front and back garden
      await growFrontAndBackGarden(miniDB, semaphoreMiniDB, semaphoreI2cController)
        .catch(async (error) => {
          await log.save(error, 'error');
          await telegramController.sendMessage(telegram, error, 'text')
            .catch(async (telegramError) => {
              await log.save(telegramError, 'error');
            });
        });

      // manage grow of NFT System
      await growNFTsystem(miniDB, semaphoreMiniDB, semaphoreI2cController)
        .catch(async (error) => {
          await log.save(error, 'error');
          await telegramController.sendMessage(telegram, error, 'text')
            .catch(async (telegramError) => {
              await log.save(telegramError, 'error');
            });
        });
    });
    /* - - - - - - - - - - - - - - - - - - - - - - - - */

    /* - - - - - - - - - - SECONDARY TASKS - - - - - - */
    // scheduler that checks internet connection and run tasks after connection is back online
    schedule.scheduleJob(env.hardwareAlerts_frequencyOf_internetCheck, async () => {
      await secondaryTasksController.internetCheck()
        .then(async (responseInternetStatus) => {
          internetStatus = responseInternetStatus.online;

          // tasks to run when internet connection is back online
          if (responseInternetStatus.runTasks) {
            // restart telegram connection
            await telegramController.restartConnection(telegram, semaphoreMiniDB, semaphoreI2cController)
              .then(async (responseTelegram) => {
                telegram = responseTelegram;
                await log.save({
                  message: 'telegram connection restarted and back online',
                  stack: 'main -> CRON_internetCheck -> secondaryTasksController.internetCheck -> telegramController.restartConnection',
                }, 'error');

                await telegramController.sendMessage(telegram, 'Internet back online\n\n\n'
                                                          + `Disconnected at:\n${getDateStampFromTimeStamp(responseInternetStatus.timestampDisconnectedAt)}\n\n`
                                                          + `Reconnected at:\n${getDateStampFromTimeStamp(responseInternetStatus.timestampReconnectedAt)}\n\n`
                                                          + `Time Offline:\n${Math.floor((responseInternetStatus.timestampReconnectedAt - responseInternetStatus.timestampDisconnectedAt) / 60)}  minutes\n${
                                                            (responseInternetStatus.timestampReconnectedAt - responseInternetStatus.timestampDisconnectedAt) - (60 * Math.floor((responseInternetStatus.timestampReconnectedAt - responseInternetStatus.timestampDisconnectedAt) / 60))}  seconds`, 'text')
                  .catch(async (telegramError) => {
                    await log.save(telegramError, 'error');
                  });
              })
              .catch(async (error) => {
                await log.save(error, 'error');
              });
          }
        });
    });

    // scheduler that checks hardware (CPU, RAM, DISK, TEMPERATURE)
    schedule.scheduleJob(env.hardwareAlerts_frequencyOf_hardwareCheck, async () => {
      await secondaryTasksController.hardwareCheck()
        .then(async (hardwareCheckResult) => {
          if (hardwareCheckResult) {
            await telegramController.sendMessage(telegram, `HARDWARE ALERT !!!\n\nat  ${getDateStamp()}\n\n\n${hardwareCheckResult}`, 'text')
              .catch(async (error) => {
                await log.save(error, 'error');
              });
          }
        })
        .catch(async (error) => {
          await log.save(error, 'error');
        });
    });
    /* - - - - - - - - - - - - - - - - - - - - - - - - */

    /* - - - - - - - - - - MESSAGING - - - - - - - - - */
    telegramController.listenMessages(telegram, semaphoreMiniDB, semaphoreI2cController)
      .then((response) => {
        telegram = response;
      });
    /* - - - - - - - - - - - - - - - - - - - - - - - - */
  });

/* - - - - - - - - - - UNHANDLED-REJECTION AND WARNING HANDLER - - - - - - - */
const errorHandler = async (error) => {
  // eslint-disable-next-line no-param-reassign
  error = {
    data: 'uncaughtError',
    message: error.stack.replace(/\n/g, '\n\n'),
    stack: 'main -> process.on(unhandledRejection  ||  warning)',
  };

  await log.save(error, 'error');
  await telegramController.sendMessage(telegram, `UNCAUGHT ERROR:\n\n${error.message}`, 'text')
    .catch(async (telegramError) => {
      await log.save(telegramError, 'error');
    });
};
process.on('unhandledRejection', errorHandler);
process.on('warning', errorHandler);
/* - - - - - - - - - - - - - - - - - - - - - - - - */
