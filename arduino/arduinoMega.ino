#include <Wire.h>

// I2C
#define i2cMessageLength 5
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
  // manages SolidStateRelay
  if(message[0] == 'S'
  && message[1] == 'R'
  && isDigit(message[2])
  && isDigit(message[3])
  && (message[4] == 'e' || message[4] == 'd')) {
    int pin = (String(message[2]) + String(message[3])).toInt();

    // validate pin received
    if(pin >= startPin_solidStateRelay
    && pin <= finishPin_solidStateRelay) {
      
      // pin enable/disable
      if(message[4] == 'e') { digitalWrite(pin, LOW); }
      else if(message[4] == 'd') { digitalWrite(pin, HIGH); }
  
      // response with same message
      for(int i = 0; i < i2cMessageLength; i++) { responseMessage[i] = message[i]; }
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





#define pin_sensor_PH A0
#define pin_5_sensor shit ( se prende solo para leer )

																		 for(int i=0;i<10;i++) 
																							 { 
																							 buf[i]=analogRead(pin_sensor_PH);
																							 delay(100);
																							 }
																		 for( int i = 0 ; i < 9 ; i++ )
																									 {
																									 for( int j = i + 1 ; j < 10 ; j++ )
																																		{
																																		 if( buf[i] > buf[j] )
																																							 {
																																							 temporal=buf[i];
																																							 buf[i]=buf[j];
																																							 buf[j]=temporal;
																																							 }
																																		}
																									 }
																							 
																		 promedio_PH_sol_nut = 0;
																		 promedio_PH_sol_nut = buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7]; 
																		 promedio_PH_sol_nut = promedio_PH_sol_nut/6;

																		 PH_sol_nut = (float)((calibracion_ph_M * promedio_PH_sol_nut)*(5.0/1023.0) + calibracion_ph_Y);
																		 