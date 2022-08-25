#include <Wire.h>

#define i2cMessageLength 5
#define i2cArduinoMegaAddress 8
char responseMessage[i2cMessageLength];

void setup() {
  delay(3000);
  Serial.begin(9600);
  Wire.begin(i2cArduinoMegaAddress);
  
  Wire.onReceive(i2cReceive);
  Wire.onRequest(i2cSend);

  digitalWrite(22, LOW);
  for (int i = 23; i <= 43; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
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

  // manages SolidStateRelay
  if(message[0] == 'S'
  && message[1] == 'R'
  && isDigit(message[2])
  && isDigit(message[3])
  && (message[4] == 'e' || message[4] == 'd')) {
    
    int pin = (String(message[2]) + String(message[3])).toInt();

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
  // manages undefined message
  else {
      responseMessage[0] = 'u';
      responseMessage[1] = 'n';
      responseMessage[2] = 'd';
      responseMessage[3] = 'e';
      responseMessage[4] = 'f';
  }
}