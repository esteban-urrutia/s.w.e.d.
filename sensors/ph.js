const PHofNutrientSolution = {
  get: (async () => 6.0), // i2c obtener         int PH_sol_nut = (float)((ph_cal_M * PH_sol_nut_analogReading)*(5.0/1023.0) + ph_cal_Y);
};

module.exports = {
  PHofNutrientSolution,
};
