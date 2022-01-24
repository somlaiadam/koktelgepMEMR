#include <Wire.h>
#include <LCD_I2C.h>

LCD_I2C lcd(0x27, 16, 2);

#define clk 2
#define dt 3
#define sw 4
#define in1 7
#define in2 8
#define in3 9
#define in4 10
#define in5 11
#define in6 12
volatile boolean TurnDetected;
volatile boolean up;
bool doonce = 0;
char screen = 0;
boolean changestate = 0;
int pump1ml = 20;
int pump2ml = 20;
int pump3ml = 20;

void isr0 ()  {
  TurnDetected = true;
  up = (digitalRead(clk) == digitalRead(dt));
}

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  pinMode(sw, INPUT_PULLUP);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
  attachInterrupt (0, isr0, RISING);
  lcd.setCursor(6, 0);
  lcd.print("Boot");
  
}

void loop() {

  if (TurnDetected) {
    delay(200);
    Serial.print("0");
    doonce = 0;
    if (changestate == 0) {
      if (up) {
        screen++;
        if (screen > 3) {
          screen = 3;
        }
      }
      else {
        screen = screen - 1;
        if (screen < 0) {
          screen = 0;
        }
      }
    }
  else {
      if (up) {
        switch (screen) {
          Serial.print("1");
          case 0: pump1ml = pump1ml + 10;
          break;
          case 1: pump2ml = pump2ml + 10;
          break;
        }
      }
      else {
        switch (screen) {
          Serial.print("2");
          case 0: pump1ml = pump1ml - 10;
          break;
          case 1: pump2ml = pump2ml - 10;
          break;
        }
      }
    }
    TurnDetected = false;
  }

  if (digitalRead(sw) == LOW) {
    delay(200);
    Serial.print("3");
    changestate = !changestate;
    doonce = 0;
  }
}