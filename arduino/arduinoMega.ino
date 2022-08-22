#include <Wire.h>

const int i2cMessageLength = 5;
const int i2cArduinoMegaAddress = 8;
String responseMessage = "";

void setup() {
  Serial.begin(9600);
  Wire.begin(i2cArduinoMegaAddress);
  
  Wire.onReceive(i2cReceive);
  Wire.onRequest(i2cSend);
}

void loop() {
  delay(200);
}

void i2cReceive(int howMany) {
  char receivedChar;
  String message;
  
  while(Wire.available()) {
    receivedChar = Wire.read();
    message = message + receivedChar;
  }

  messageManager(message);
}

void i2cSend() {
  Wire.write(responseMessage.c_str());
  responseMessage = "";
}

void messageManager(String message) {
  if(message == "chela") {
    responseMessage = "beker";
  }
  else if(message == "porro") {
    responseMessage = "gollo";
  }
}