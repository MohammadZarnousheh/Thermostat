#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#define ONE_WIRE_BUS 7

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int Relay_on = 0;
int Relay_off = 0;
int Relay_status = 0;
int flag = 1;
int flag2 = 0;
unsigned long previousMillis = 0;
const int interval = 200;

const int PIN_SW_DOWN = 2;
const int PIN_SW_SET = 3;
const int PIN_SW_UP = 4;
boolean key = 0;
boolean key_SET , key_DOWN, key_UP;
////////////////////////////////////////////////////////////void setup
void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  sensors.begin();
  pinMode(9, OUTPUT);
  pinMode(PIN_SW_DOWN, INPUT_PULLUP);
  pinMode(PIN_SW_SET, INPUT_PULLUP);
  pinMode(PIN_SW_UP, INPUT_PULLUP);
  EEPROM.get(1, Relay_on);
  EEPROM.get(4, Relay_off);
  EEPROM.get(8, Relay_status);
}
////////////////////////////////////////////////////////////void loop
void loop() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  if (temp != DEVICE_DISCONNECTED_C) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      Serial.print("Temperature of Sensor 0 is: ");
      Serial.println(temp);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temperature");
      lcd.setCursor(0, 1);
      lcd.print(temp);
      if (Relay_status == 1) {
        if (temp > Relay_on) {
          digitalWrite(9, HIGH);
          flag = 0;
        }
        //  if (flag1 == 1) {
        if (temp < Relay_off) {
          digitalWrite(9, LOW);
          //flag = 0;
        } else {
          // flag = 1;
        }
      }
    }
  } else {
    Serial.println("Error to reading data");
    lcd.clear();
    lcd.print("Error");
    digitalWrite(9, LOW);
  }


  key_UP = digitalRead(PIN_SW_UP);
  key_SET = digitalRead(PIN_SW_SET);
  key_DOWN = digitalRead(PIN_SW_DOWN);

  if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
    key = 1;
    SET_Relay_on();
  }
  if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
    key = 0;
  }

}

////////////////////////////////////////////////////////////void SET_Relay_on
void SET_Relay_on() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> SET Relay on");
  lcd.setCursor (0, 1);
  lcd.print("Relay on = ");
  while (1) {
    lcd.setCursor (9, 1);
    lcd.print(Relay_on);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      Relay_on--;
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      Relay_on++;
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }
  SET_Relay_off();
}
////////////////////////////////////////////////////////////void SET_Relay_off
void SET_Relay_off() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> SET Relay off");
  lcd.setCursor (0, 1);
  lcd.print("Relay off = ");
  while (1) {
    lcd.setCursor (9, 1);
    lcd.print(Relay_off);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      Relay_off--;
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      Relay_off++;
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }
  Relay();
}

////////////////////////////////////////////////////////////Relay_status
void Relay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> Relay status");
  lcd.setCursor (0, 1);
  lcd.print(" Relay = ");
  while (1) {
    lcd.setCursor (7, 1);
    lcd.print(Relay_status);
    lcd.print("   ");
    key_UP = digitalRead(PIN_SW_UP);
    key_SET = digitalRead(PIN_SW_SET);
    key_DOWN = digitalRead(PIN_SW_DOWN);
    if (key == 0 && key_SET == 1 && key_DOWN == 0 && key_UP == 1) {
      key = 1;
      if (Relay_status == 0) {
        Relay_status = 1;
      } else {
        Relay_status--;
      }
    }
    if (key == 0 && key_SET == 1 && key_DOWN == 1 && key_UP == 0) {
      key = 1;
      if (Relay_status == 1) {
        Relay_status = 0;
      } else {
        Relay_status++;

      }
    }
    if (key == 0 && key_SET == 0 && key_DOWN == 1 && key_UP == 1) {
      key = 1;
      break;
    }
    if (key == 1 && key_SET == 1 && key_DOWN == 1 && key_UP == 1) {
      key = 0;
    }
  }
  Save();
}
////////////////////////////////////////////////////////////void Save
void Save() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">    Save...    ");
  EEPROM.put(1, Relay_on);
  EEPROM.put(4, Relay_off);
  EEPROM.put(8, Relay_status);
  delay(2000);
  lcd.clear();
}
