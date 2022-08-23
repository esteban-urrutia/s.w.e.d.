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

  for (int i = 22; i <= 43; i++) {
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
    message[aux] = Wire.read();
    aux = aux + 1;
  }

  messageManager(message);
}

void i2cSend() {
  Wire.write(responseMessage);
  for(int i = 0; i < i2cMessageLength; i++) {
    responseMessage[i] = ' ';
  }
}

void messageManager(char message[]) {
  if(message[0] == 'i'  &&
     message[1] == 'o') {
    

    responseMessage = "beker";
  }
  else if(message == "porro") {
    responseMessage = "gollo";
  }
}