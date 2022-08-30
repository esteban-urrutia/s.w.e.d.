/* eslint-disable no-plusplus */
/* eslint-disable no-restricted-syntax */
/* eslint-disable camelcase */
const { exec } = require('child_process');
const ObjectsToCsv = require('objects-to-csv');
const config_json = require('./processCheckerConfig.json');

const process_config_list = {};

config_json.forEach(async (value) => {
  process_config_list[value.name] = value;
});

function check() {
  let result;
  let output;
  const process_running_list = [];

  const ls = exec('/usr/bin/pm2 list');

  ls.stdout.on('data', (data) => {
    output += data.toString();
  });

  ls.on('exit', async () => {
    try {
      const separator = 14;
      result = output.toString();
      result = result.replace(/[[0-9]+m/g, '');
      result = result.replace(/[\n]/g, '');
      result = result.split('│');
      const size = result.length - 1;
      const mod = Math.trunc(size / separator);

      for (let i = 1; i < mod - 1; i++) {
        const process_running_name = (result[2 + separator * i]).replace(/[ ]/g, '');
        const process_running_status = (result[9 + separator * i]).replace(/[ ]/g, '');
        process_running_list.push(process_running_name);

        if (process_running_status !== 'online' && process_running_name !== 'Module') {
          exec((process_config_list[process_running_name].restart), async (error, response) => {
            let dateStamp = new Date();
            dateStamp = `${(`0${dateStamp.getDate()}`).slice(-2)}-${(`0${dateStamp.getMonth() + 1}`).slice(-2)}-${dateStamp.getFullYear()} ${(`0${dateStamp.getHours()}`).slice(-2)}:${(`0${dateStamp.getMinutes()}`).slice(-2)}:${(`0${dateStamp.getSeconds()}`).slice(-2)}`;

            if (response) {
              await (new ObjectsToCsv([{
                dateStamp,
                data: `Restarting ${process_config_list[process_running_name].name}`,
              }]))
                .toDisk('pm2ProcessCheck.csv', { append: true });
            } else if (error) {
              await (new ObjectsToCsv([{
                dateStamp,
                data: `Error restarting ${process_config_list[process_running_name].name} ... ${error.stack}`,
              }]))
                .toDisk('pm2ProcessCheck.csv', { append: true });
            }
          });
        }
      }

      for (const key in process_config_list) {
        if (!process_running_list.includes(process_config_list[key].name)) {
          exec((process_config_list[key].start), async (error, response) => {
            let dateStamp = new Date();
            dateStamp = `${(`0${dateStamp.getDate()}`).slice(-2)}-${(`0${dateStamp.getMonth() + 1}`).slice(-2)}-${dateStamp.getFullYear()} ${(`0${dateStamp.getHours()}`).slice(-2)}:${(`0${dateStamp.getMinutes()}`).slice(-2)}:${(`0${dateStamp.getSeconds()}`).slice(-2)}`;

            if (response) {
              await (new ObjectsToCsv([{
                dateStamp,
                data: `Starting ${process_config_list[key].name}`,
              }]))
                .toDisk('pm2ProcessCheck.csv', { append: true });
            } else if (error && !error.toString().includes('Script already launched')) {
              await (new ObjectsToCsv([{
                dateStamp,
                data: `Error starting ${process_config_list[key].name} ... ${error.stack}`,
              }]))
                .toDisk('pm2ProcessCheck.csv', { append: true });
            }
          });
        }
      }
    } catch (error) {
      let dateStamp = new Date();
      dateStamp = `${(`0${dateStamp.getDate()}`).slice(-2)}-${(`0${dateStamp.getMonth() + 1}`).slice(-2)}-${dateStamp.getFullYear()} ${(`0${dateStamp.getHours()}`).slice(-2)}:${(`0${dateStamp.getMinutes()}`).slice(-2)}:${(`0${dateStamp.getSeconds()}`).slice(-2)}`;

      await (new ObjectsToCsv([{
        dateStamp,
        data: `Error after execution of '/usr/bin/pm2 list'${error.stack}`,
      }]))
        .toDisk('pm2ProcessCheck.csv', { append: true });
    }
  });

  ls.on('error', async (error) => {
    let dateStamp = new Date();
    dateStamp = `${(`0${dateStamp.getDate()}`).slice(-2)}-${(`0${dateStamp.getMonth() + 1}`).slice(-2)}-${dateStamp.getFullYear()} ${(`0${dateStamp.getHours()}`).slice(-2)}:${(`0${dateStamp.getMinutes()}`).slice(-2)}:${(`0${dateStamp.getSeconds()}`).slice(-2)}`;

    await (new ObjectsToCsv([{
      dateStamp,
      data: `Error on execution of '/usr/bin/pm2 list'${error.stack}`,
    }]))
      .toDisk('pm2ProcessCheck.csv', { append: true });
  });
}

check();
