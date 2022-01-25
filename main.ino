#include <Wire.h>
#include <LCD_I2C.h>
#include "HX711.h"

LCD_I2C lcd(0x27, 16, 2);
HX711 scale;

#define clk 2
#define dt 3
#define sw 4
#define in1 7
#define in2 8
#define IN3 9
#define IN4 10
#define IN5 11
#define IN6 12
volatile boolean TurnDetected;
volatile boolean up;
bool doonce = 0;
char screen = 0;
boolean changestate = 0;
long weight;
int pump1ml = 20;
int pump2ml = 20;
int pump3ml = 20;

bool poharerzek = 1;
int helpcount = 5;

float calibration_factor = 429;
float units;
float pohar;


void isr0 ()  {
  TurnDetected = true;
  up = (digitalRead(clk) == digitalRead(dt));
}

void setup() {
  lcd.begin();
  lcd.backlight();
  scale.begin(5, 6);
  scale.set_scale();
  scale.tare();  //Reset the scale to 0
  long zero_factor = scale.read_average();
  pinMode(sw, INPUT_PULLUP);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  attachInterrupt (0, isr0, RISING);
  lcd.setCursor(5, 0);
  lcd.print("Hello!");
  delay(2000);
  
}

void loop() {
  if (TurnDetected) {
    delay(200);
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
          case 0: pump1ml = pump1ml + 10;
          break;
          case 1: pump2ml = pump2ml + 10;
          break;
        }
      }
      else {
        switch (screen) {
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
    changestate = !changestate;
    doonce = 0;
  }

  if (screen == 0 && doonce == 0) {
    lcd.clear();
    lcd.print("Narancsle");
    lcd.setCursor(0, 1);
    lcd.print(pump1ml);
    lcd.setCursor(3, 1);
    lcd.print("ml");
    if (changestate == 0) {
      lcd.setCursor(8, 1 );
      lcd.print("<-szerk?");
    }
    doonce = 1;
  }

  if (screen == 1 && doonce == 0) {
    lcd.clear();
    lcd.print("Vodka");
    lcd.setCursor(0, 1);
    lcd.print(pump2ml);
    lcd.setCursor(3, 1);
    lcd.print("ml");
    if (changestate == 0) {
      lcd.setCursor(8, 1 );
      lcd.print("<-szerk?");
    }
    doonce = 1;
  }

  if (screen == 2 && doonce == 0) {
    Serial.print("7");
    lcd.clear();
    if (changestate == 0) {
      lcd.setCursor(5, 0);
      lcd.print("Start?");
      lcd.setCursor(0, 1);
      lcd.print("Indul a keveres!");
      doonce = 1;
    }
    else {
      while (poharerzek == 1)
      {
        lcd.setCursor(0, 0);
        lcd.print("Tegye a poharat");
        lcd.setCursor(0, 1);
        lcd.print("a helyere (");
        lcd.setCursor(12, 1);
        lcd.print(helpcount);
        lcd.setCursor(13, 1);
        lcd.print("mp)");
        delay(1000); //1 mp
        helpcount = helpcount - 1;
        
        if (helpcount == 0)
        {
          poharerzek = 0;
        }
      }
      lcd.setCursor(3, 0);
      lcd.clear();
      lcd.print("Keszul...");
      delay(1000);
      lcd.clear();
      lcd.print("Motor 1 Be");
      scale.set_scale(calibration_factor);
      pohar = scale.get_units(), 10;
      
      digitalWrite(in1, HIGH);
      while (scale.get_units() - pohar < pump1ml) {
        delay(10);
      }
      lcd.clear();
      lcd.print(pump1ml);
      lcd.print("ml");
      digitalWrite(in1, LOW);
      
      lcd.clear();
      lcd.print("Motor 2 Be");
      delay(1000);
      while (scale.get_units() - pohar - pump1ml < pump2ml) {

      for (int step = 0 ; step < 9 ; step++) {
          switch (step) {
            case 0:
              digitalWrite(IN3, LOW);
              digitalWrite(IN4, LOW);
              digitalWrite(IN5, LOW);
              digitalWrite(IN6, HIGH);
              break;
            case 1:
              digitalWrite(IN3, LOW);
              digitalWrite(IN4, LOW);
              digitalWrite(IN5, HIGH);
              digitalWrite(IN6, HIGH);
              break;
            case 2:
              digitalWrite(IN3, LOW);
              digitalWrite(IN4, LOW);
              digitalWrite(IN5, HIGH);
              digitalWrite(IN6, LOW);
              break;
            case 3:
              digitalWrite(IN3, LOW);
              digitalWrite(IN4, HIGH);
              digitalWrite(IN5, HIGH);
              digitalWrite(IN6, LOW);
              break;
            case 4:
              digitalWrite(IN3, LOW);
              digitalWrite(IN4, HIGH);
              digitalWrite(IN5, LOW);
              digitalWrite(IN6, LOW);
              break;
            case 5:
              digitalWrite(IN3, HIGH);
              digitalWrite(IN4, HIGH);
              digitalWrite(IN5, LOW);
              digitalWrite(IN6, LOW);
              break;
            case 6:
              digitalWrite(IN3, HIGH);
              digitalWrite(IN4, LOW);
              digitalWrite(IN5, LOW);
              digitalWrite(IN6, LOW);
              break;
            case 7:
              digitalWrite(IN3, HIGH);
              digitalWrite(IN4, LOW);
              digitalWrite(IN5, LOW);
              digitalWrite(IN6, HIGH);
              break;
            default:
              digitalWrite(IN3, LOW);
              digitalWrite(IN4, LOW);
              digitalWrite(IN5, LOW);
              digitalWrite(IN6, LOW);
              break;
          }
        }

        delay(10);
      }
      lcd.clear();
      lcd.print(pump2ml);
      lcd.print("ml ");

      lcd.clear();
      lcd.print("Elkeszult!");
      delay(2000);
      bool poharerzek = 1;
      int helpcount = 5;
      changestate = 0;
    }
  }
}