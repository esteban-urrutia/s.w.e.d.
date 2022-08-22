const waterValveForIrrigationOfFrontGarden = {
  on: (async () => 'a'), // i2c encender
  off: (async () => 'b'), // i2c apagar
};

const waterValveForIrrigationOfBackGarden = {
  on: (async () => 'c'), // i2c encender
  off: (async () => 'd'), // i2c apagar
};

module.exports = {
  waterValveForIrrigationOfFrontGarden,
  waterValveForIrrigationOfBackGarden,
};
