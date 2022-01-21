#include <Wire.h>
#include <LCD_I2C.h>

#define clk 2
#define dt 3
#define sw 4
#define in1 7
#define in2 8
#define in3 9
#define in4 10
#define in5 11
#define in6 12
long weight;
int pump1ml = 20;
int pump2ml = 20;
int pump3ml = 20;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  pinMode(sw, INPUT_PULLUP);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);
}

void loop() {
}