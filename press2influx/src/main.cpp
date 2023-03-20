#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
float target = 24;
float current = 0;

#define encoderA 8
#define encoderB 9
#define relay 16

bool encoderAState = 0;
bool encoderBState = 0;

Adafruit_MCP9600 mcp;

int start = 0;

void encoder() {
  bool newEncoderAState = digitalRead(encoderA);
  bool newEncoderBState = digitalRead(encoderB);
  if (newEncoderAState != encoderAState && newEncoderAState == LOW) {
    if (newEncoderBState == LOW) {
      target++;
    } else {
      target--;
    }
  }
  encoderAState = newEncoderAState;
}

void setup() {
  // put your setup code here, to run once:
  lcd.init();                      // initialize the lcd 
  lcd.backlight();

  mcp.begin(0x60);
  mcp.setThermocoupleType(MCP9600_TYPE_K);

  Serial.begin(9600);

  //  encoder for target
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  pinMode(relay, OUTPUT);
  start = millis();
}

void loop() {
  // current = (20*current+target)/21;

  // serial
  // Serial.print(" ");
  // Serial.print(target);
  // Serial.print(" ");
  // Serial.print(current);
  // Serial.println(" ");
  encoder();
  if (millis() - start > 300){
    start = millis();
      current = mcp.readThermocouple();
    // lcd
    // lcd.clear();
    // lcd.blink();
    lcd.setCursor(0,0);
    lcd.print("Target: ");
    // lcd.write("Target: ");
    lcd.print(target);
    lcd.setCursor(0,1);
    lcd.print("Current: ");
    lcd.print(current);
    Serial.print(" ");
    Serial.print(target);
    Serial.print(" ");
    Serial.print(current);
    Serial.println(" ");

    if(current < target){
      digitalWrite(relay, HIGH);
      lcd.setCursor(14,0);
      lcd.print("ON");
    } else {
      digitalWrite(relay, LOW);
      lcd.setCursor(14,0);
      lcd.print("OFF");
    }
  }
}