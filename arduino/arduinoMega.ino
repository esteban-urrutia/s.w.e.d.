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
    char receivedCharacter = Wire.read();
    if(aux < i2cMessageLength) {
      message[aux] = receivedCharacter;
      }
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
  if(message[0] == 'p'  &&
     message[1] == 'o'  &&
     message[2] == 'r'  &&
     message[3] == 'r'  &&
     message[4] == 'o' ) {
      
      responseMessage[0] = 'g';
      responseMessage[1] = 'o';
      responseMessage[2] = 'l';
      responseMessage[3] = 'l';
      responseMessage[4] = 'o';
  }
}