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

  if (screen == 0 && doonce == 0) {
    Serial.print("4");
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
    Serial.print("5");
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
      lcd.setCursor(3, 0);
      lcd.print("Keszul...");
      delay(1000);
      lcd.clear();
      lcd.print("Motor 1 BE");
      digitalWrite(in1, HIGH);
      delay(10000);
      lcd.clear();
      lcd.print(pump1ml);
      lcd.print("ml");
      digitalWrite(in1, LOW);
      
      lcd.clear();
      lcd.print("Motor 2 Be");
      digitalWrite(in3, HIGH);
      delay(10000);
      lcd.clear();
      lcd.print(pump2ml);
      lcd.print("ml ");
      digitalWrite(in3, LOW);
      lcd.clear();
      lcd.print("Elkeszult!");
      delay(2000);
      changestate = 0;
    }
  }
}