#include <Wire.h>

// I2C
#define i2cMessageLength 5
#define i2cArduinoMegaAddress 8
char responseMessage[i2cMessageLength];

// Solid State Relay
#define startPin_solidStateRelay 23
#define finishPin_solidStateRelay 38

// peripherals
#define pin_airHeaterOfGrowSpace 12

// PH Sensor
#define pin_sensor_PH A0

void setup() {
  delay(2000);
  //Serial.begin(9600);
  Wire.begin(i2cArduinoMegaAddress);
  
  Wire.onReceive(i2cReceive);
  Wire.onRequest(i2cSend);
  
  // Solid State Relay - all channels off
  for (int i = startPin_solidStateRelay; i <= finishPin_solidStateRelay; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  
  // 220v Power Supply NC fix
  delay(2000);
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);

  // airHeater of growSpace initial state
  pinMode(pin_airHeaterOfGrowSpace, OUTPUT);
  digitalWrite(pin_airHeaterOfGrowSpace, HIGH);
}

void loop() {}

void i2cReceive(int howMany) {
  char message[i2cMessageLength];
  int aux = 0;

  while(Wire.available()) {
    char receivedCharacter = Wire.read();
    if(aux < i2cMessageLength) {
      message[aux] = receivedCharacter;
      }
    aux = aux + 1;
  }

  deviceManager(message);
}

void i2cSend() {
  Wire.write(responseMessage);
  for(int i = 0; i < i2cMessageLength; i++) {
    responseMessage[i] = ' ';
  }
}

void i2cResponseUndefined() {
      responseMessage[0] = 'u';
      responseMessage[1] = 'n';
      responseMessage[2] = 'd';
      responseMessage[3] = 'e';
      responseMessage[4] = 'f';
}

void deviceManager(char message[]) {
  // manages Up State Relay
  if(message[0] == 'U'
  && message[1] == 'R'
  && isDigit(message[2])
  && isDigit(message[3])
  && (message[4] == 'e' || message[4] == 'd')) {
    int pin = (String(message[2]) + String(message[3])).toInt();

    // validate pin received
    if((pin >= startPin_solidStateRelay
    && pin <= finishPin_solidStateRelay)
    || pin == pin_airHeaterOfGrowSpace) {
      
      // pin enable/disable
      if(message[4] == 'e') { digitalWrite(pin, LOW); }
      else if(message[4] == 'd') { digitalWrite(pin, HIGH); }
  
      // response with same message
      for(int i = 0; i < i2cMessageLength; i++) {
        responseMessage[i] = message[i];
      }
    }    
    else {
      i2cResponseUndefined();
    }
  }

  // read PH of Nutrient Solution
  else if(message[0] == 'g'
  && message[1] == 'e'
  && message[2] == 't'
  && message[3] == 'P'
  && message[4] == 'H') {

    int buf[10];
    int temporal = 0;
    for (int k = 0; k < 10; k++)
    {
      buf[k] = analogRead(pin_sensor_PH);
      delay(100);
    }
    for (int i = 0; i < 9; i++)
    {
      for (int j = i + 1; j < 10; j++)
      {
        if (buf[i] > buf[j])
        {
          temporal = buf[i];
          buf[i] = buf[j];
          buf[j] = temporal;
        }
      }
    }

    int PH_sol_nut_analogReading = (buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7])/6;

    int PH_sol_nut_analogReading_1 = (PH_sol_nut_analogReading/100);
    int PH_sol_nut_analogReading_2 = ((PH_sol_nut_analogReading - ((PH_sol_nut_analogReading/100)*100))/10);
    int PH_sol_nut_analogReading_3 = (PH_sol_nut_analogReading - ((PH_sol_nut_analogReading/100)*100) - (((PH_sol_nut_analogReading - ((PH_sol_nut_analogReading/100)*100))/10)*10));

    responseMessage[0] = 'P';
    responseMessage[1] = 'H';
    responseMessage[2] = PH_sol_nut_analogReading_1;
    responseMessage[3] = PH_sol_nut_analogReading_2;
    responseMessage[4] = PH_sol_nut_analogReading_3;
  }

  // manages undefined message
  else {
    i2cResponseUndefined();
  }
}