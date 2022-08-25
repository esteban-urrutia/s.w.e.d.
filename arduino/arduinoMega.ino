#include <Wire.h>

// I2C
#define i2cMessageLength 5
#define i2cArduinoMegaAddress 8
#define i2cArduinoMegaAddress 8
char responseMessage[i2cMessageLength];

// Solid State Relay
#define startPin_solidStateRelay 23
#define finishPin_solidStateRelay 38

void setup() {
  delay(2000);
  Serial.begin(9600);
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
}

void loop() {
  delay(200);
}

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

void deviceManager(char message[]) {

  // manages SolidStateRelay channels
  if(message[0] == 'S'
  && message[1] == 'R'
  && isDigit(message[2])
  && isDigit(message[3])
  && (message[4] == 'e' || message[4] == 'd')) {
    
    int pin = (String(message[2]) + String(message[3])).toInt();

    if(pin >= startPin_solidStateRelay
    && pin <= finishPin_solidStateRelay) {
      if(message[4] == 'e') { 
        digitalWrite(pin, LOW);
      }
      else if(message[4] == 'd') { 
        digitalWrite(pin, HIGH);
      }
  
      for(int i = 0; i < i2cMessageLength; i++) {
        responseMessage[i] = message[i];
      }
    }
    else {
      responseMessage[0] = 'u';
      responseMessage[1] = 'n';
      responseMessage[2] = 'd';
      responseMessage[3] = 'e';
      responseMessage[4] = 'f';
    }

  }

  // manages undefined message
  else {
      responseMessage[0] = 'u';
      responseMessage[1] = 'n';
      responseMessage[2] = 'd';
      responseMessage[3] = 'e';
      responseMessage[4] = 'f';
  }
}