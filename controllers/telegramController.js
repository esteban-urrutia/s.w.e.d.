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
const miniDBController = require('./miniDBcontroller');
const log = require('./logController').getInstance();

async function sendMessage(telegram, message, type) {
  if (env.telegram_enabled === 'true') {
    return new Promise(async (resolve, reject) => {
      // send message according to specified type
      switch (type) {
        case 'text':
          await telegram.sendMessage(
            env.telegram_chatId,
            (typeof message === 'object' ? JSON.stringify(message) : message),
          )
            .then(() => {
              resolve(true);
            })
            .catch(async (error) => {
              reject({
                data: { message: message.replace(/[\n]/g, ' '), type },
                errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
                from: 'telegramController -> sendMessage -> text',
              });
            });
          break;

        case 'document':
          await telegram.sendDocument(env.telegram_chatId, message)
            .then(async () => {
              try {
                await fs.unlinkSync(message);
                resolve(true);
              } catch (error) {
                reject({
                  data: { message, type },
                  errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
                  from: 'telegramController -> sendMessage -> document -> deleting from disk',
                });
              }
            })
            .catch(async (error) => {
              await telegram.sendMessage(env.telegram_chatId, `Error executing (send document):  ${error}`)
                .catch(async (error) => {
                  await log.save({
                    data: { message, type },
                    errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
                    from: 'telegramController -> sendErrorMessage -> send document',
                  }, 'error');
                })
                .finally(() => {
                  reject({
                    data: { message, type },
                    errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
                    from: 'telegramController -> send document -> error',
                  });
                });
            });
          break;

        case 'image':
          await telegram.sendPhoto(env.telegram_chatId, message)
            .then(async () => {
              try {
                await fs.unlinkSync(message);
                resolve(true);
              } catch (error) {
                reject({
                  data: { message, type },
                  errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
                  from: 'telegramController -> sendMessage -> image -> deleting from disk',
                });
              }
            })
            .catch(async (error) => {
              await telegram.sendMessage(env.telegram_chatId, `Error executing (send image):  ${error}`)
                .catch(async (error) => {
                  await log.save({
                    data: { message, type },
                    errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
                    from: 'telegramController -> sendErrorMessage -> send image',
                  }, 'error');
                })
                .finally(() => {
                  reject({
                    data: { message, type },
                    errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
                    from: 'telegramController -> send image -> error',
                  });
                });
            });
          break;

        default:
          break;
      }
    });
  }
  return null;
}

async function helpMenu(telegram) {
  const menu = `Commands for  ${env.telegram_botOf_thisRpi}\n\n\n`
                + '1)   /profitStatus\n\n'
                + '2)   /lastOrderStatus\n\n'
                + '3)   /getLogs\n\n'
                + '4)   /aliveReport\n\n'
                + '5)   /configMenu';

  // send helpMenu for this device
  await sendMessage(telegram, menu, 'text')
    .catch(async (error) => {
      await log.save(error, 'error');
    });
}
async function aliveReport(telegram) {
  const responseArray = [];
  let lastBoot = '';
  let ips = '';

  // Get network interfaces of the device
  await device.networkInterfaces()
    .then(async (network) => {
      network.forEach((net) => {
        if (net.ifaceName !== 'lo') {
          ips += `${net.ifaceName}: \n${net.ip4}\n\n`;
        }
      });

      // get time since boot
      await exec('uptime -s', async (error, data) => {
        lastBoot = data;
        if (error) {
          lastBoot = `Error: ${error.message}`;
        }
        responseArray.push(`${env.telegram_botOf_thisRpi}: I'm alive! \n\n\n${ips}Last Boot: \n${lastBoot}`);

        // send aliveReport
        for (const res of responseArray) {
          await sendMessage(telegram, res, 'text')
            .catch(async (error) => {
              await log.save(error, 'error');
            });
        }
      });
    })
    .catch(async (error) => {
      error = {
        data: 'aliveReport',
        errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
        from: 'telegramController -> aliveReport -> networkInterfaces',
      };
      await log.save(error, 'error');

      await sendMessage(telegram, error.errorMessage, 'text')
        .catch(async (telegramError) => {
          await log.save(telegramError, 'error');
        });
    });
}

async function rebootService(telegram) {
  await telegram.sendMessage(env.telegram_chatId, 'Service Rebooting')
    .catch(async (error) => {
      await log.save({
        data: null,
        errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
        from: 'rebootRpi -> rebooting',
      }, 'error');
    })
    .finally(async () => {
      await exec('pm2 restart main', async (error) => {
        if (error) {
          await telegram.sendMessage(env.telegram_chatId, `Error executing (pm2 restart main):  ${error}`)
            .catch(async (error) => {
              await log.save({
                data: `Error executing (pm2 restart main):  ${error}`,
                errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
                from: 'telegramController -> sendErrorMessage -> pm2 restart main',
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
        data: null,
        errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
        from: 'rebootRpi -> rebooting',
      }, 'error');
    })
    .finally(async () => {
      await exec('sudo reboot now', async (error) => {
        if (error) {
          await telegram.sendMessage(env.telegram_chatId, `Error executing (sudo reboot now):  ${error}`)
            .catch(async (error) => {
              await log.save({
                data: `Error executing (pm2 restart main):  ${error}`,
                errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
                from: 'telegramController -> sendErrorMessage -> sudo reboot now',
              }, 'error');
            });
        }
      });
    });
}

async function getLogs(telegram) {
  return new Promise(async (resolve, reject) => {
    const compressedLogs = 'logs.zip';

    await compress.zip.compressDir('logs', compressedLogs)
      .then(async () => {
        await sendMessage(telegram, compressedLogs, 'document')
          .catch(async (error) => {
            await log.save(error, 'error');
          });
      })
      .catch((error) => {
        reject({
          data: null,
          errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
          from: 'telegramController -> all_logsInZip -> compress.zip.compressDir',
        });
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
            case `/help${botName}`:
              await helpMenu(telegram);
              break;

            case `/getLogs${botName}`:
              await getLogs(telegram);
              break;

            case `/aliveReport${botName}`:
              await aliveReport(telegram);
              break;

            case `/config_rebootService ${env.telegram_password}${botName}`:
              await rebootService(telegram);
              break;

            case `/config_rebootRpi ${env.telegram_password}${botName}`:
              await rebootRpi(telegram);
              break;

            default:
              break;
          }
        }
      };

      // error handler
      const errorHandler = async (error) => {
        await log.save({
          data: null,
          errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
          from: 'telegramController -> errorHandler',
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
                    data: null,
                    errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
                    from: 'telegramController -> restartConnection -> telegram.stopPolling -> telegram.closeWebHook -> telegram.startPolling',
                  });
                });
            })
            .catch(async (error) => {
              reject({
                data: null,
                errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
                from: 'telegramController -> restartConnection -> telegram.stopPolling -> telegram.closeWebHook',
              });
            });
        })
        .catch(async (error) => {
          reject({
            data: null,
            errorMessage: (error.hasOwnProperty('stack') ? error.stack : error),
            from: 'telegramController -> restartConnection -> telegram.stopPolling()',
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
