#include <Wire.h>

String receivedMessage;

void setup() {
  Serial.begin(9600);
  Wire.begin(8);
  
  Wire.onRequest(i2cSend);
  Wire.onReceive(i2cReceive);

  Serial.println("ENCENDIO EL ARDUINO");
}

void loop() {
  delay(100);
}

void i2cReceive(int howMany) {
  Serial.println("recibi mensaje desde rpi  : ");
  Serial.print( Wire.read() );
}

void i2cSend() {
  Wire.write("HOLA DESDE ARDUINO");
  Serial.println("respuesta HOLA DESDE ARDUINO enviada");
}