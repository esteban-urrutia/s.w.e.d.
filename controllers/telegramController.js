/* eslint-disable brace-style */
/* eslint-disable radix */
/* eslint-disable max-len */
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
const log = require('./logController').getInstance();
const {
  normalizeTextForTelegramMessage,
  execute,
  getTimeStamp,
  getDateStampFromTimeStamp,
  sleep,
} = require('../utils/utils');
const {
  readMiniDB,
  saveMiniDB,
  deleteMiniDB: deleteMiniDb,
} = require('./miniDBcontroller');
const { temperatureAndHumidityOfGrowSpace } = require('../sensors/temperature-humidity');
const { temperatureOfNutrientSolution } = require('../sensors/waterTemperature');
const { PHofNutrientSolution } = require('../sensors/ph');
const { ECofNutrientSolution } = require('../sensors/ec');
const { lightingOfGrowSpace } = require('../peripherals/lighting');
const { waterPumpForSampleOfNutrientSolution } = require('../peripherals/waterPumps');
const {
  waterValveForIrrigationOfFrontGarden,
  waterValveForIrrigationOfBackGarden,
} = require('../peripherals/waterValves');

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
                  await execute(`sudo rm ${message}`);
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
                  await execute(`sudo rm ${message}`);
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

  const menu = '1)  /marancandinhuanaMenu\n\n'
             + '2)  /frontAndBackGardenMenu\n\n'
             + '3)  /NFTsystemMenu\n\n'
             + '4)  /configMenu';
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function marancandinhuanaMenu(telegram) {
  const menu = '1)  /overviewMarancandinhuana\n\n'
             + '2)  /getSampleOfNutrientSolution\n\n'
             + '3)  /getPHofSample\n\n'
             // + '4)  /getECofSample\n\n'
             + '4)  /postECofSample number\n\n'
             + '5)  /managePeripheralsMarancandinhuana';
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function overviewMarancandinhuana(telegram, semaphoreMiniDB) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  const {
    temperatureOfGrowSpace,
    humidityOfGrowSpace,
  } = await temperatureAndHumidityOfGrowSpace.get();

  const temperatureOfNutSol = await temperatureOfNutrientSolution.get();

  const overviewMessage = 'Grow Space: \n'
                        + `    temperature:  ${temperatureOfGrowSpace.toFixed(2)} °C\n`
                        + `    humidity:  ${humidityOfGrowSpace.toFixed(2)} %rel\n\n`
                        + 'Nutrient Solution:\n'
                        + `    temperature:  ${temperatureOfNutSol} °C\n\n`
                        + 'Peripherals Status:\n'
                        + '    Grow Space:\n'
                        + `        light:  ${miniDB.growMarancandinhuanaParams.growSpace.light.status}\n`
                        + `        temperature:  ${miniDB.growMarancandinhuanaParams.growSpace.temperature.status}\n`
                        + `        humidity:  ${miniDB.growMarancandinhuanaParams.growSpace.humidity.status}\n`
                        + `        air renew:  ${miniDB.growMarancandinhuanaParams.growSpace.airRenew.status}\n`
                        + '    Nutritive Solution:\n'
                        + `        temperature:  ${miniDB.growMarancandinhuanaParams.nutritiveSolution.temperature.status}\n`
                        + `        PH (last reading):  ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.lastValue}\n`
                        + `        EC (last reading):  ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ec.lastValue}\n`
                        + `        Recirculation:  ${miniDB.growMarancandinhuanaParams.nutritiveSolution.recirculation.status}\n`
                        + `        Oxygenation:  ${miniDB.growMarancandinhuanaParams.nutritiveSolution.oxygenation.status}`;
  await sendMessage(telegram, overviewMessage, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function getSampleOfNutrientSolution(telegram, semaphoreI2cController) {
  try {
    await waterPumpForSampleOfNutrientSolution.on(semaphoreI2cController);
    await sendMessage(telegram, 'waterPump for Sample Of NutrientSolution ON', 'text');

    await sleep(parseInt(env.secondOfActionWaterPumpForSampleOfNutrientSolution));

    await waterPumpForSampleOfNutrientSolution.off(semaphoreI2cController);
    await sendMessage(telegram, 'waterPump for Sample Of NutrientSolution OFF', 'text');
  } catch (error) {
    await log.save(error, 'error');
  }
}

async function getPHofSample(telegram, semaphoreMiniDB, semaphoreI2cController) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  const { PHofNutSol, analogPhVoltageReading } = await PHofNutrientSolution.get(miniDB, semaphoreI2cController);

  const PHmarancandinhuana = `PH of Nutrient Solution:  ${(Math.round(PHofNutSol * 10) / 10)}\n\n`
                           + `Min value: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.values.min}\n`
                           + `Max value: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.values.max}\n\n`
                           + `Analog PH Voltage reading: ${analogPhVoltageReading}`;

  miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.lastValue = (Math.round(PHofNutSol * 10) / 10);
  await saveMiniDB(semaphoreMiniDB, miniDB);

  await log.save({ PHofNutSol: (Math.round(PHofNutSol * 10) / 10) }, 'manual-stats-PH');

  await sendMessage(telegram, PHmarancandinhuana, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function getECofSample(telegram, semaphoreMiniDB) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  const ECofNutSol = await ECofNutrientSolution.get();

  const ECmarancandinhuana = `EC of Nutrient Solution:  ${ECofNutSol}\n\n`
                           + `Min value: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ec.values.min}\n`
                           + `Max value: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ec.values.max}`;

  miniDB.growMarancandinhuanaParams.nutritiveSolution.ec.lastValue = ECofNutSol;
  await saveMiniDB(semaphoreMiniDB, miniDB);

  await log.save({ ECofNutSol }, 'manual-stats-EC');

  await sendMessage(telegram, ECmarancandinhuana, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function postECofSample(telegram, semaphoreMiniDB, ECinput) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  const ECmarancandinhuana = `EC of Nutrient Solution:  ${ECinput}\n\n`
                           + `Min value: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ec.values.min}\n`
                           + `Max value: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ec.values.max}`;

  miniDB.growMarancandinhuanaParams.nutritiveSolution.ec.lastValue = ECinput;
  await saveMiniDB(semaphoreMiniDB, miniDB);

  await log.save({ ECofNutSol: ECinput }, 'manual-stats-EC');

  await sendMessage(telegram, ECmarancandinhuana, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function managePeripheralsMarancandinhuana(telegram) {
  const menu = '1)  /growSpaceLighting\n\n'
             + '2)  /calibratePHsensor xxx yyy\n'
             + '    (analog voltage reading\n'
             + '     on PH buffer at 4.0 & 7.0)\n';
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function growSpaceLighting(telegram) {
  const menu = '1)  /enableGrowSpaceLighting\n\n'
             + '2)  /disableGrowSpaceLighting';
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function enableGrowSpaceLighting(telegram, semaphoreMiniDB, semaphoreI2cController) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  await lightingOfGrowSpace.on(semaphoreI2cController);
  miniDB.growMarancandinhuanaParams.growSpace.light.status = 'lighting';

  await saveMiniDB(semaphoreMiniDB, miniDB);

  await sendMessage(telegram, 'Lighting Of GrowSpace Enabled', 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function disableGrowSpaceLighting(telegram, semaphoreMiniDB, semaphoreI2cController) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  await lightingOfGrowSpace.off(semaphoreI2cController);
  miniDB.growMarancandinhuanaParams.growSpace.light.status = 'null';

  await saveMiniDB(semaphoreMiniDB, miniDB);

  await sendMessage(telegram, 'Lighting Of GrowSpace Disabled', 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function calibratePHsensor(telegram, semaphoreMiniDB, analogVoltageAtBuffer4, analogVoltageAtBuffer7) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.m = (6 * 1023) / (analogVoltageAtBuffer7 - analogVoltageAtBuffer4);
  miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.y = 40 - ((analogVoltageAtBuffer4 * miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.m * 5) / 1023);

  await saveMiniDB(semaphoreMiniDB, miniDB);

  await sendMessage(telegram, `PH sensor calibrated\n\nm: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.m}\ny: ${miniDB.growMarancandinhuanaParams.nutritiveSolution.ph.y}`, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function frontAndBackGardenMenu(telegram) {
  const menu = '1)  /overviewFrontAndBackGarden\n\n'
             + '2)  /suspendManageOfFrontAndBackGarden\n'
             + '      (command + space + hours)';
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function overviewFrontAndBackGarden(telegram, semaphoreMiniDB) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  let overviewMessage = 'FrontAndBackGarden:\n'
                        + `    irrigation:  ${miniDB.growFrontAndBackGardenParams.growSpace.irrigation.status}`;

  if (getTimeStamp() < miniDB.growFrontAndBackGardenParams.growSpace.irrigation.suspendedUntil) {
    overviewMessage += `\n\nsuspended until\n${getDateStampFromTimeStamp(miniDB.growFrontAndBackGardenParams.growSpace.irrigation.suspendedUntil)}`;
  }
  await sendMessage(telegram, overviewMessage, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function suspendManageOfFrontAndBackGarden(telegram, semaphoreMiniDB, semaphoreI2cController, hours) {
  const miniDB = await readMiniDB(semaphoreMiniDB);

  await waterValveForIrrigationOfFrontGarden.off(semaphoreI2cController);
  await waterValveForIrrigationOfBackGarden.off(semaphoreI2cController);

  miniDB.growFrontAndBackGardenParams.growSpace.irrigation.suspendedUntil = getTimeStamp() + hours * 60 * 60;
  await saveMiniDB(semaphoreMiniDB, miniDB);

  await sendMessage(telegram, `Manage Of Front And Back Garden\nSuspended until\n${getDateStampFromTimeStamp(miniDB.growFrontAndBackGardenParams.growSpace.irrigation.suspendedUntil)}`, 'text')
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
             + '3)   /getMiniDB\n\n'
             + '4)   /deleteMiniDB\n\n'
             + '5)   /restartService\n\n'
             + '6)   /rebootRpi\n\n'
             + '7)   /shutdownRpi';
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}

async function aliveReport(telegram) {
  const responseArray = [];
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
    ips += `external IP: \n${externalIp}\n\n`;

    // get time since boot
    const lastBoot = await execute('uptime -s');
    responseArray.push(`I'm alive! \n\n${ips}Last Boot: \n${lastBoot}`);

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
        stack: 'telegramController -> getLogs -> compress.zip.compressDir',
      });
    }
  });
}

async function getMiniDB(telegram, semaphoreMiniDB) {
  return new Promise(async (resolve, reject) => {
    semaphoreMiniDB.take(async () => {
      try {
        await sendMessage(telegram, 'miniDB.json', 'document');

        semaphoreMiniDB.leave();
        resolve(true);
      } catch (error) {
        semaphoreMiniDB.leave();
        reject({
          message: (error.hasOwnProperty('stack') ? error.stack : error),
          stack: 'telegramDBcontroller -> getMiniDB',
        });
      }
    });
  });
}

async function deleteMiniDB(telegram, semaphoreMiniDB) {
  try {
    await deleteMiniDb(semaphoreMiniDB);
    await sendMessage(telegram, 'miniDB.json successfully deleted', 'text');
  } catch (error) {
    await log.save(error, 'error');
  }
}

async function restartService(telegram) {
  await sendMessage(telegram, 'Service restart', 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    })
    .finally(async () => {
      exec('pm2 restart main', async (error) => {
        if (error) {
          await sendMessage(telegram, 'Error executing (pm2 restart main)', 'text')
            .catch(async (error) => {
              await log.save(error, 'error');
            });
        }
      });
    });
}

async function rebootRpi(telegram) {
  await sendMessage(telegram, 'Rpi rebooting', 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    })
    .finally(async () => {
      exec('sudo reboot now', async (error) => {
        if (error) {
          await sendMessage(telegram, `Error executing (sudo reboot now):  ${error}`, 'text')
            .catch(async (error) => {
              await log.save(error, 'error');
            });
        }
      });
    });
}

async function shutdownRpi(telegram) {
  await sendMessage(telegram, 'Rpi shutting down', 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    })
    .finally(async () => {
      exec('sudo shutdown now', async (error) => {
        if (error) {
          await sendMessage(telegram, `Error executing (sudo shutdown now):  ${error}`, 'text')
            .catch(async (error) => {
              await log.save(error, 'error');
            });
        }
      });
    });
}

async function listenMessages(telegram, semaphoreMiniDB, semaphoreI2cController) {
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

            case `/marancandinhuanaMenu${botName}`:
              await marancandinhuanaMenu(telegram);
              break;

            case `/overviewMarancandinhuana${botName}`:
              await overviewMarancandinhuana(telegram, semaphoreMiniDB);
              break;

            case `/getSampleOfNutrientSolution${botName}`:
              await getSampleOfNutrientSolution(telegram, semaphoreI2cController);
              break;

            case `/getPHofSample${botName}`:
              await getPHofSample(telegram, semaphoreMiniDB, semaphoreI2cController);
              break;

            case `/getECofSample${botName}`:
              await getECofSample(telegram, semaphoreMiniDB);
              break;

            case `/managePeripheralsMarancandinhuana${botName}`:
              await managePeripheralsMarancandinhuana(telegram, semaphoreMiniDB);
              break;

            case `/growSpaceLighting${botName}`:
              await growSpaceLighting(telegram, semaphoreMiniDB);
              break;

            case `/enableGrowSpaceLighting${botName}`:
              await enableGrowSpaceLighting(telegram, semaphoreMiniDB, semaphoreI2cController);
              break;

            case `/disableGrowSpaceLighting${botName}`:
              await disableGrowSpaceLighting(telegram, semaphoreMiniDB, semaphoreI2cController);
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

            case `/getMiniDB${botName}`:
              await getMiniDB(telegram, semaphoreMiniDB);
              break;

            case `/deleteMiniDB ${env.telegram_password}${botName}`:
              await deleteMiniDB(telegram, semaphoreMiniDB);
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

          if (incomingMessage.command.substring(0, 34) === '/suspendManageOfFrontAndBackGarden') {
            const hours = parseInt(incomingMessage.command.substring(35));
            if (hours <= 24 && hours >= 1) {
              await suspendManageOfFrontAndBackGarden(telegram, semaphoreMiniDB, semaphoreI2cController, hours);
            }
          }

          else if (incomingMessage.command.substring(0, 15) === '/postECofSample') {
            const ECinput = parseInt(incomingMessage.command.substring(16));
            if (ECinput <= 100000 && ECinput >= 1) {
              await postECofSample(telegram, semaphoreMiniDB, ECinput);
            }
          }

          else if (incomingMessage.command.substring(0, 18) === '/calibratePHsensor') {
            const analogVoltageAtBuffer4 = parseInt(incomingMessage.command.substring(19, 22));
            const analogVoltageAtBuffer7 = parseInt(incomingMessage.command.substring(23, 26));

            if (analogVoltageAtBuffer4 >= 1 && analogVoltageAtBuffer4 <= 999
            && analogVoltageAtBuffer7 >= 1 && analogVoltageAtBuffer7 <= 999) {
              await calibratePHsensor(telegram, semaphoreMiniDB, analogVoltageAtBuffer4, analogVoltageAtBuffer7);
            }
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

async function restartConnection(telegram, semaphoreMiniDB, semaphoreI2cController) {
  if (env.telegram_enabled === 'true') {
    return new Promise(async (resolve, reject) => {
      await telegram.stopPolling()
        .then(async () => {
          await telegram.closeWebHook()
            .then(async () => {
              await telegram.startPolling({ restart: true })
                .then(async () => {
                  await listenMessages(telegram, semaphoreMiniDB, semaphoreI2cController)
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
