/* eslint-disable prefer-promise-reject-errors */
/* eslint-disable no-param-reassign */
/* eslint-disable no-restricted-syntax */
/* eslint-disable no-shadow */
/* eslint-disable no-await-in-loop */
/* eslint-disable no-prototype-builtins */
/* eslint-disable no-async-promise-executor */
const env = require('dotenv').config().parsed;
const device = require('systeminformation');
const { exec } = require('child_process');
const compress = require('compressing');
const fs = require('fs');
const log = require('./logController').getInstance();
const {
  normalizeTextForTelegramMessage,
  execute,
} = require('../utils/utils');
const { readMiniDB, saveMiniDB } = require('./miniDBcontroller');
const { temperatureAndHumidityOfGrowSpace } = require('../sensors/temperature-humidity');
const { temperatureOfNutrientSolution } = require('../sensors/waterTemperature');
const { PHofNutrientSolution } = require('../sensors/ph');
const { ECofNutrientSolution } = require('../sensors/ec');

async function sendMessage(telegram, message, type, deleteFileAfter = false) {
  if (env.telegram_enabled === 'true') {
    return new Promise(async (resolve, reject) => {
      switch (type) {
        case 'text':
          message = normalizeTextForTelegramMessage(message);
          await telegram.sendMessage(
            env.telegram_chatId,
            message,
          )
            .then(() => {
              resolve(true);
            })
            .catch(async (error) => {
              reject({
                data: { message: message.replace(/[\n]/g, ' '), type },
                message: (error.hasOwnProperty('stack') ? error.stack : error),
                stack: 'telegramController -> sendMessage -> text',
              });
            });
          break;

        case 'document':
          await telegram.sendDocument(env.telegram_chatId, message)
            .then(async () => {
              if (deleteFileAfter) {
                try {
                  fs.unlinkSync(message);
                  resolve(true);
                } catch (error) {
                  reject({
                    data: { message, type },
                    message: (error.hasOwnProperty('stack') ? error.stack : error),
                    stack: 'telegramController -> sendMessage -> document -> deleting from disk',
                  });
                }
              }
              resolve(true);
            })
            .catch(async (error) => {
              await telegram.sendMessage(env.telegram_chatId, `Error executing (send document):  ${error}`)
                .catch(async (error) => {
                  await log.save({
                    data: { message, type },
                    message: (error.hasOwnProperty('stack') ? error.stack : error),
                    stack: 'telegramController -> sendErrorMessage -> send document',
                  }, 'error');
                })
                .finally(() => {
                  reject({
                    data: { message, type },
                    message: (error.hasOwnProperty('stack') ? error.stack : error),
                    stack: 'telegramController -> send document -> error',
                  });
                });
            });
          break;

        case 'image':
          await telegram.sendPhoto(env.telegram_chatId, message)
            .then(async () => {
              if (deleteFileAfter) {
                try {
                  fs.unlinkSync(message);
                  resolve(true);
                } catch (error) {
                  reject({
                    data: { message, type },
                    message: (error.hasOwnProperty('stack') ? error.stack : error),
                    stack: 'telegramController -> sendMessage -> image -> deleting from disk',
                  });
                }
              }
              resolve(true);
            })
            .catch(async (error) => {
              await telegram.sendMessage(env.telegram_chatId, `Error executing (send image):  ${error}`)
                .catch(async (error) => {
                  await log.save({
                    data: { message, type },
                    message: (error.hasOwnProperty('stack') ? error.stack : error),
                    stack: 'telegramController -> sendErrorMessage -> send image',
                  }, 'error');
                })
                .finally(() => {
                  reject({
                    data: { message, type },
                    message: (error.hasOwnProperty('stack') ? error.stack : error),
                    stack: 'telegramController -> send image -> error',
                  });
                });
            });
          break;

        default:
          resolve(null);
          break;
      }
    });
  }
  return null;
}

async function helpMenu(telegram) {
  await sendMessage(telegram, 'SmokeWeedEveryDay.jpg', 'image')
    .catch(async (telegramError) => {
      await log.save(telegramError, 'error');
    });

  const menu = '1)  /marincandinhuanaMenu\n\n'
             + '2)  /frontAndBackGardenMenu\n\n'
             + '3)  /NFTsystemMenu\n\n'
             + '4)  /configMenu';
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function marincandinhuanaMenu(telegram) {
  const menu = '1)  /overviewMarincandinhuana\n\n'
             + '2)  /managePHmarincandinhuana\n\n'
             + '3)  /manageECmarincandinhuana';
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function overviewMarincandinhuana(telegram, semaphoreMiniDB) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  const {
    temperatureOfGrowSpace,
    humidityOfGrowSpace,
  } = await temperatureAndHumidityOfGrowSpace.get();

  const temperatureOfNutSol = await temperatureOfNutrientSolution.get();

  const overviewMessage = 'Grow Space: \n'
                        + `    temperature:  ${temperatureOfGrowSpace} °C\n`
                        + `    humidity:  ${humidityOfGrowSpace} %rel\n\n`
                        + 'Nutrient Solution:\n'
                        + `    temperature:  ${temperatureOfNutSol} °C\n\n`
                        + 'Peripherals Status:\n'
                        + '    Grow Space:\n'
                        + `        temperature:  ${miniDB.growMarancandinhuanaParams.growSpace.temperature.status}\n`
                        + `        humidity:  ${miniDB.growMarancandinhuanaParams.growSpace.humidity.status}\n`
                        + '    Nutritive Solution:\n'
                        + `        temperature:  ${temperatureOfNutSol} °C\n`
                        + `        PH (last reading):  ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.lastValue}\n`
                        + `        EC (last reading):  ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ec.lastValue}\n`
                        + `        Recirculation:  ${miniDB.growMarancandinhuanaParams.nutritiveSolution.recirculation.status}\n`
                        + `        Oxygenation:  ${miniDB.growMarancandinhuanaParams.nutritiveSolution.oxygenation.status}`;
  await sendMessage(telegram, overviewMessage, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function managePHmarincandinhuana(telegram, semaphoreMiniDB) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  const PHofSolNut = await PHofNutrientSolution.get();

  const PHmarincandinhuana = `PH of Nutrient Solution:  ${PHofSolNut}\n\n`
                           + `Min value: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.values.min}\n`
                           + `Max value: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.values.max}`;

  miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.lastValue = PHofSolNut;
  await saveMiniDB(semaphoreMiniDB, miniDB);

  await sendMessage(telegram, PHmarincandinhuana, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function manageECmarincandinhuana(telegram, semaphoreMiniDB) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  const ECofSolNut = await ECofNutrientSolution.get();

  const ECmarincandinhuana = `EC of Nutrient Solution:  ${ECofSolNut}\n\n`
                           + `Min value: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ec.values.min}\n`
                           + `Max value: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ec.values.max}`;

  miniDB.growMarancandinhuanaParams.nutritiveSolution.ec.lastValue = ECofSolNut;
  await saveMiniDB(semaphoreMiniDB, miniDB);

  await sendMessage(telegram, ECmarincandinhuana, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function frontAndBackGardenMenu(telegram) {
  const menu = '1)  /overviewFrontAndBackGarden';
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function overviewFrontAndBackGarden(telegram, semaphoreMiniDB) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  const overviewMessage = 'FrontAndBackGarden:\n'
                        + `    irrigation:  ${miniDB.growFrontAndBackGardenParams.growSpace.irrigation.status}`;
  await sendMessage(telegram, overviewMessage, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function NFTsystemMenu(telegram) {
  const menu = '1)  /overviewNFTsystem\n\n';
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function overviewNFTsystem(telegram, semaphoreMiniDB) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  const overviewMessage = 'NFTsystem:\n'
                        + `    irrigation:  ${miniDB.growNFTsystemParams.growSpace.irrigation.status}`;
  await sendMessage(telegram, overviewMessage, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function configMenu(telegram) {
  const menu = '1)   /aliveReport\n\n'
             + '2)   /getLogs\n\n'
             + '3)   /restartService\n\n'
             + '4)   /rebootRpi\n\n'
             + '5)   /shutdownRpi';
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function aliveReport(telegram) {
  const responseArray = [];
  const lastBoot = '';
  let ips = '';

  try {
    // Get network interfaces of the device
    const network = await device.networkInterfaces();
    network.forEach((net) => {
      if (net.ifaceName !== 'lo') {
        ips += `${net.ifaceName}: \n${net.ip4}\n\n`;
      }
    });
    const externalIp = await execute('dig +short myip.opendns.com @resolver1.opendns.com');
    ips += `${externalIp}: \n${externalIp}\n\n`;

    // get time since boot
    await execute('uptime -s');
    responseArray.push(`${env.telegram_botOf_thisRpi}: 
                      I'm alive! \n\n\n${ips}
                      Last Boot: \n${lastBoot}`);

    // send aliveReport
    for (const res of responseArray) {
      await sendMessage(telegram, res, 'text')
        .catch(async (error) => {
          await log.save(error, 'error');
        });
    }
  } catch (error) {
    await log.save({
      data: 'aliveReport',
      message: (error.hasOwnProperty('stack') ? error.stack : error),
      stack: 'telegramController -> aliveReport -> networkInterfaces',
    }, 'error');

    await sendMessage(telegram, (error.hasOwnProperty('stack') ? error.stack : error), 'text')
      .catch(async (telegramError) => {
        await log.save(telegramError, 'error');
      });
  }
}

async function getLogs(telegram) {
  return new Promise(async (resolve, reject) => {
    const compressedLogs = 'logs.zip';

    try {
      await compress.zip.compressDir('logs', compressedLogs);

      await sendMessage(telegram, compressedLogs, 'document', true)
        .catch(async (error) => {
          await log.save(error, 'error');
        });
      resolve(true);
    } catch (error) {
      reject({
        message: (error.hasOwnProperty('stack') ? error.stack : error),
        stack: 'telegramController -> all_logsInZip -> compress.zip.compressDir',
      });
    }
  });
}

async function restartService(telegram) {
  await telegram.sendMessage(env.telegram_chatId, 'Service restart')
    .catch(async (error) => {
      await log.save({
        message: (error.hasOwnProperty('stack') ? error.stack : error),
        stack: 'restartService -> restart',
      }, 'error');
    })
    .finally(async () => {
      exec('pm2 restart main', async (error) => {
        if (error) {
          await telegram.sendMessage(env.telegram_chatId, `Error executing (pm2 restart main):  ${error}`)
            .catch(async (error) => {
              await log.save({
                data: `Error executing (pm2 restart main):  ${error}`,
                message: (error.hasOwnProperty('stack') ? error.stack : error),
                stack: 'telegramController -> sendErrorMessage -> pm2 restart main',
              }, 'error');
            });
        }
      });
    });
}

async function rebootRpi(telegram) {
  await telegram.sendMessage(env.telegram_chatId, 'Rpi rebooting')
    .catch(async (error) => {
      await log.save({
        message: (error.hasOwnProperty('stack') ? error.stack : error),
        stack: 'rebootRpi -> rebooting',
      }, 'error');
    })
    .finally(async () => {
      exec('sudo reboot now', async (error) => {
        if (error) {
          await telegram.sendMessage(env.telegram_chatId, `Error executing (sudo reboot now):  ${error}`)
            .catch(async (error) => {
              await log.save({
                data: `Error executing (sudo reboot now):  ${error}`,
                message: (error.hasOwnProperty('stack') ? error.stack : error),
                stack: 'telegramController -> sendErrorMessage -> sudo reboot now',
              }, 'error');
            });
        }
      });
    });
}

async function shutdownRpi(telegram) {
  await telegram.sendMessage(env.telegram_chatId, 'Rpi shutting down')
    .catch(async (error) => {
      await log.save({
        message: (error.hasOwnProperty('stack') ? error.stack : error),
        stack: 'shutdownRpi -> shutting down',
      }, 'error');
    })
    .finally(async () => {
      exec('sudo shutdown now', async (error) => {
        if (error) {
          await telegram.sendMessage(env.telegram_chatId, `Error executing (sudo shutdown now):  ${error}`)
            .catch(async (error) => {
              await log.save({
                data: `Error executing (sudo shutdown now):  ${error}`,
                message: (error.hasOwnProperty('stack') ? error.stack : error),
                stack: 'telegramController -> sendErrorMessage -> sudo shutdown now',
              }, 'error');
            });
        }
      });
    });
}

async function listenMessages(telegram, semaphoreMiniDB) {
  if (env.telegram_enabled === 'true') {
    return new Promise(async (resolve) => {
      // message handler
      const messageHandler = async (msg) => {
        // if type of message received is text
        if (msg.hasOwnProperty('text')) {
          // log incoming message
          const incomingMessage = {
            from: {
              firstName: msg.from.first_name,
              lastName: msg.from.last_name,
              username: msg.from.username,
            },
            command: msg.text,
          };
          await log.save(incomingMessage, 'incomingTelegramMessages');

          // process incoming message
          const botName = incomingMessage.command.includes('@') ? `@${env.telegram_botOf_thisRpi}` : '';
          switch (incomingMessage.command) {
            case `/snoop${botName}`:
              await helpMenu(telegram);
              break;

            case `/marincandinhuanaMenu${botName}`:
              await marincandinhuanaMenu(telegram);
              break;

            case `/overviewMarincandinhuana${botName}`:
              await overviewMarincandinhuana(telegram, semaphoreMiniDB);
              break;

            case `/managePHmarincandinhuana${botName}`:
              await managePHmarincandinhuana(telegram, semaphoreMiniDB);
              break;

            case `/manageECmarincandinhuana${botName}`:
              await manageECmarincandinhuana(telegram, semaphoreMiniDB);
              break;

            case `/frontAndBackGardenMenu${botName}`:
              await frontAndBackGardenMenu(telegram);
              break;

            case `/overviewFrontAndBackGarden${botName}`:
              await overviewFrontAndBackGarden(telegram, semaphoreMiniDB);
              break;

            case `/NFTsystemMenu${botName}`:
              await NFTsystemMenu(telegram);
              break;

            case `/overviewNFTsystem${botName}`:
              await overviewNFTsystem(telegram, semaphoreMiniDB);
              break;

            case `/configMenu${botName}`:
              await configMenu(telegram);
              break;

            case `/aliveReport${botName}`:
              await aliveReport(telegram);
              break;

            case `/getLogs${botName}`:
              await getLogs(telegram);
              break;

            case `/restartService ${env.telegram_password}${botName}`:
              await restartService(telegram);
              break;

            case `/rebootRpi ${env.telegram_password}${botName}`:
              await rebootRpi(telegram);
              break;

            case `/shutdownRpi ${env.telegram_password}${botName}`:
              await shutdownRpi(telegram);
              break;

            default:
              break;
          }
        }
      };

      // error handler
      const errorHandler = async (error) => {
        await log.save({
          message: (error.hasOwnProperty('stack') ? error.stack : error),
          stack: 'telegramController -> errorHandler',
        }, 'error');
      };

      telegram.on('message', messageHandler);
      telegram.on('polling_error', errorHandler);
      telegram.on('webhook_error', errorHandler);
      telegram.on('error', errorHandler);

      resolve(telegram);
    });
  }
  return null;
}

async function restartConnection(telegram) {
  if (env.telegram_enabled === 'true') {
    return new Promise(async (resolve, reject) => {
      await telegram.stopPolling()
        .then(async () => {
          await telegram.closeWebHook()
            .then(async () => {
              await telegram.startPolling({ restart: true })
                .then(async () => {
                  await listenMessages(telegram)
                    .then((response) => {
                      resolve(response);
                    });
                })
                .catch((error) => {
                  reject({
                    message: (error.hasOwnProperty('stack') ? error.stack : error),
                    stack: 'telegramController -> restartConnection -> telegram.stopPolling -> telegram.closeWebHook -> telegram.startPolling',
                  });
                });
            })
            .catch(async (error) => {
              reject({
                message: (error.hasOwnProperty('stack') ? error.stack : error),
                stack: 'telegramController -> restartConnection -> telegram.stopPolling -> telegram.closeWebHook',
              });
            });
        })
        .catch(async (error) => {
          reject({
            message: (error.hasOwnProperty('stack') ? error.stack : error),
            stack: 'telegramController -> restartConnection -> telegram.stopPolling()',
          });
        });
    });
  }
  return null;
}

module.exports = {
  listenMessages,
  restartConnection,
  sendMessage,
};
