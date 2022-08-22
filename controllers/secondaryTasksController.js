const ping =                     require("ping");
const utils =                    require("../utils/utils");
const {exec} =                   require("child_process");
let   lastNegativePingToGoogle = 0;
let   lastPositivePingToGoogle = 0;
let   internetStatus =           {online:null, runTasks:null};
let   internetDisconnected =     false;
const env =                      require('dotenv').config().parsed;

async function internetCheck() {
    return new Promise(async (resolve) => {

        await ping.promise.probe('www.google.com', {timeout: 10})
            .then((response) => {
                let timeStamp = utils.getTimeStamp();

                switch (response.alive){
                    case true:
                        internetStatus.runTasks = false;
                        if(timeStamp - lastNegativePingToGoogle >= parseInt(env.hardwareAlerts_secondsOf_stableInternetTo_setAs_activeConecction)){
                            internetStatus.online =   true;

                            if(internetDisconnected){
                                internetStatus.runTasks =                true;
                                internetDisconnected =                   false;
                                internetStatus.timestampReconnectedAt = utils.getTimeStamp();
                            }
                        }
                        lastPositivePingToGoogle = timeStamp;
                        break;

                    case false:
                        internetStatus.runTasks = false;
                        if(timeStamp - lastPositivePingToGoogle >= parseInt(env.hardwareAlerts_secondsOf_stableInternetTo_setAs_activeConecction)){
                            internetStatus.online =                  false;
                            internetDisconnected =                   true;
                            internetStatus.timestampDisconnectedAt = utils.getTimeStamp();
                        }
                        lastNegativePingToGoogle = timeStamp;
                        break;
                }
                resolve(internetStatus);
            });
    });
}

async function hardwareCheck() {
    return new Promise(async (resolve) => {

        // get free ram
        await exec("free -m", async (error, stdout, stderr) => {
            let outMemory, freeRam, buffCachedRam;
            if (error || stderr) {
                outMemory =     null;
                freeRam =       null;
                buffCachedRam = null;
            }
            else {
                outMemory =     stdout.split("\n")[1].replace(/\s+/g, ' ').split(" ");
                freeRam =       {megaBytes:  Number(outMemory[3]),
                                 percentage: (parseInt(outMemory[3]) / parseInt(outMemory[1]) * 100).toFixed(2)};
                buffCachedRam = outMemory[5];
            }

            // get cache and buffers of ram memory
            await exec("egrep --color 'Mem|Cache|Swap|Buffer|buffer' /proc/meminfo", async (error, stdout, stderr) => {
                let out, buffersRam, cachedRam;
                if (error || stderr) {
                    out =        null;
                    buffersRam = null;
                    cachedRam =  null;
                }
                else {
                    out =        stdout.replaceAll(" ", "").split("\n");
                    buffersRam = (parseInt(out[3].substr(out[3].indexOf(":") + 1).substr(0,out[3].indexOf("k") - 1))/1000).toFixed(2);
                    cachedRam =  (parseInt(out[4].substr(out[4].indexOf(":") + 1).substr(0,out[4].indexOf("k") - 1))/1000).toFixed(2);
                }

                // get temperature of the cpu
                await exec("cat /sys/class/thermal/thermal_zone0/temp", async (error, stdout, stderr) => {
                    let temperature;
                    if (error || stderr) {
                        temperature = null;
                    }
                    else {
                        temperature = (Number(stdout)/1000).toFixed(2);
                    }

                    // get free disk
                    await exec("df -H /", async (error, stdout, stderr) => {
                        let dataDisk, freeDisk, freeDiskPercentage;
                        if (error || stderr) {
                            dataDisk =           null;
                            freeDisk =           null;
                            freeDiskPercentage = null;
                        }
                        else {
                            dataDisk = stdout.split("\n")[1].replace(/\s+/g, ' ').split(" ")
                            freeDisk = {gigaBytes:  Number(dataDisk[3].replace('G', ' ')),
                                        percentage: (100 - Number(dataDisk[4].replace('%', ' ')))};
                        }

                        // get free cpu
                        await exec('top -b -n 1 | sed -n "s/^%Cpu.*ni, \([0-9.]*\) .*$/\1/p"', async (error, stdout, stderr) => {
                            let freeCpuPercentage;
                            if (error || stderr) {
                                freeCpuPercentage = null;
                            }
                            else {
                                freeCpuPercentage = Number(stdout);
                            }

                            // if (freeRam || temperature || freeDisk || freeCpu) are beyond acceptable limits... send alert over telegram
                            let alert = "";
                            if(freeRam  &&  freeRam.percentage < parseInt(env.hardwareAlerts_minFreeRamPercentage)){
                                alert = "FreeRam available "+  freeRam.percentage+" % ("+freeRam.megaBytes+" mB)\n" +
                                        "--> buffCachedRam: "+ buffCachedRam+" Mb\n" +
                                        "--> buffersRam: "+    buffersRam+" Mb\n" +
                                        "--> cachedRam: "+     cachedRam+" mB\n\n";
                            }
                            if(temperature  &&  temperature > parseInt(env.hardwareAlerts_maxTemperature)){
                                alert = alert + "Temperature "+temperature+" °C\n\n";
                            }
                            if(freeDisk  &&  freeDisk.percentage < parseInt(env.hardwareAlerts_minFreeRamPercentage)){
                                alert = alert + "FreeDisk available "+freeDisk.percentage+" % ("+freeDisk.gigaBytes+" Gb)\n\n";
                            }
                            if(freeCpuPercentage  &&  freeCpuPercentage < parseInt(env.hardwareAlerts_minFreeCpuPercentage)){
                                alert = alert + "freeCpu available "+freeCpuPercentage+" %\n\n";
                            }

                            resolve(alert);
                        })
                    })
                })
            })
        })
    });
}

module.exports = {
    internetCheck,
    hardwareCheck
}