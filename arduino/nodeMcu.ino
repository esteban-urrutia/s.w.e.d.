#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
WiFiUDP ntpUDP;
WiFiClient client;
HTTPClient https;

#define pin_buzzer 13
#define pin_principalDoorSensor 14
#define pin_principalDoorButton 12

bool alarmTriggered = false;
bool deactivateAlarmUntilNextOpen = false;
bool alarmTemporizer = true;
int secondsWithoutPressingPrincipalDoorButton_alarmNotTriggered = 0;
int secondsPressingPrincipalDoorButton_alarmTriggered = 0;
int secondsBeforeTriggeringAlarm = 60;
int secondsPressingButtonToDeactivateAlarm = 5;

struct destinatary {
  String phone;
  String apikey;
};
const int destinatariesLength = 2;
destinatary destinataries[destinatariesLength];

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin("choripan-atenea-pumba", "super mascotas");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void sendWhatsappToAlldestinataries(String text) {
  for (int i = 0; i < destinatariesLength; i++) {
    https.begin(client, "http://api.callmebot.com/whatsapp.php?phone=" + destinataries[i].phone + "&text=" + text + "&apikey=" + destinataries[i].apikey);
    int httpCode = https.GET();
    https.end();
  }
}

String readPrincipalDoorSensor() {
  if (digitalRead(pin_principalDoorSensor)) {
    return "open";
  }
  return "close";
}

String readPrincipalDoorButton() {
  if (digitalRead(pin_principalDoorButton)) {
    return "unpressed";
  }
  return "pressed";
}

void notificationDeactivateAlarmUntilNextOpen() {
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
}

void notificationAlarmTriggered() {
  digitalWrite(pin_buzzer, HIGH);
}

void notificationDeactivatingAlarm() {
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
}

void notificationAlarmDeactivated() {
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
  delay(50);
  digitalWrite(pin_buzzer, HIGH);
  delay(50);
  digitalWrite(pin_buzzer, LOW);
}

void setup() {
  // Serial.begin(9600);
  delay(500);
  wifiConnect();

  pinMode(pin_principalDoorSensor, INPUT);
  pinMode(pin_principalDoorButton, INPUT);
  pinMode(pin_buzzer, OUTPUT);
  digitalWrite(pin_buzzer, LOW);

  EEPROM.begin(1);
  EEPROM.get(0, alarmTriggered);

  destinataries[0].phone = "56963469808"; destinataries[0].apikey = "4058999";
  destinataries[1].phone = "56976155131"; destinataries[1].apikey = "7534456";
}

void loop() {
  delay(2000);

  String principalDoorStatus = readPrincipalDoorSensor();
  String principalDoorButtonStatus = readPrincipalDoorButton();

  if (alarmTriggered) {
    sendWhatsappToAlldestinataries("ALARMA+ROBO+en:+Avenida+del+Canal+19784,+Pudahuel,+.+.+.+vehiculos+del+hogar:+Suzuky+Swift+Rojo,+Mazda+mx5(deportivo)+Blanco");
    notificationAlarmTriggered();

    if (principalDoorButtonStatus == "pressed") {
      notificationDeactivatingAlarm();

      secondsPressingPrincipalDoorButton_alarmTriggered = 2 + secondsPressingPrincipalDoorButton_alarmTriggered;

      if (secondsPressingPrincipalDoorButton_alarmTriggered > secondsPressingButtonToDeactivateAlarm) {

        alarmTriggered = false;
        EEPROM.put(0, alarmTriggered);
        EEPROM.commit();
        secondsPressingPrincipalDoorButton_alarmTriggered = 0;
        alarmTemporizer = false;
        deactivateAlarmUntilNextOpen = true;

        notificationAlarmDeactivated();
      }
    }

    if (principalDoorButtonStatus == "unpressed") {
      secondsPressingPrincipalDoorButton_alarmTriggered = 0;
    }
  }
  else {
    if (principalDoorStatus == "close") {
      deactivateAlarmUntilNextOpen = false;
    }
    if (principalDoorStatus == "open" && !deactivateAlarmUntilNextOpen) {
      alarmTemporizer = true;
    }
    if (alarmTemporizer == true) {
      secondsWithoutPressingPrincipalDoorButton_alarmNotTriggered = 2 + secondsWithoutPressingPrincipalDoorButton_alarmNotTriggered;

      if (principalDoorButtonStatus == "pressed") {
        notificationDeactivateAlarmUntilNextOpen();
        alarmTemporizer = false;
        deactivateAlarmUntilNextOpen = true;
        secondsWithoutPressingPrincipalDoorButton_alarmNotTriggered = 0;
      }

      if (secondsWithoutPressingPrincipalDoorButton_alarmNotTriggered > secondsBeforeTriggeringAlarm) {
        alarmTriggered = true;
        EEPROM.put(0, alarmTriggered);
        EEPROM.commit();
        secondsWithoutPressingPrincipalDoorButton_alarmNotTriggered = 0;
      }
    }
  }
}