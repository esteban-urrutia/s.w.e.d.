var ds18b20 = require('ds18x20');

var temp = ds18b20.get('28-0416c090eaff');
console.log(temp);