#include "LedControlMS.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define NBR_MTX 1

//Dot Matrix
LedControl lc = LedControl(6, 5, 4, NBR_MTX);

byte help[4][8] = {
  {
  B10000001,
  B10000001,
  B10000001,
  B11111111,
  B10000001,
  B10000001,
  B10000001,
  B10000001
},{
  B11111111,
  B10000000,
  B10000000,
  B11111111,
  B10000000,
  B10000000,
  B10000000,
  B11111111
},{
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B10000000,
  B11111111
},{
  B01111110,
  B10000001,
  B10000001,
  B11111110,
  B10000000,
  B10000000,
  B10000000,
  B10000000
}
};

byte safe[4][8] = {
  {
  B11111111,
  B10000000,
  B10000000,
  B11111111,
  B00000001,
  B00000001,
  B00000001,
  B11111111
},{
  B01111110,
  B10000001,
  B10000001,
  B11111111,
  B10000001,
  B10000001,
  B10000001,
  B10000001
},{
  B11111111,
  B10000000,
  B10000000,
  B11111111,
  B10000000,
  B10000000,
  B10000000,
  B10000000
},{
  B01111111,
  B10000000,
  B10000000,
  B11111111,
  B10000000,
  B10000000,
  B10000000,
  B01111111
}
};
//

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
//

//potentiometer
const int pin_potentio = A2;

//key pad
const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  {'1', '2', '3', '4'},
  {'5', '6', '7', '8'},
  {'9', 'a', 'b', 'c'},
  {'d', 'e', 'f', 'g'}
};

byte pin_keypad_rows[rows] = {6, 7, 8, 9};
byte pin_keypad_cols[cols] = {2, 3, 4, 5};

Keypad keypad = Keypad(makeKeymap(keys), pin_keypad_rows, pin_keypad_cols, rows, cols);
//


//joystick
const int pin_xAxis = A0;
const int pin_yAxis = A1;
const int pin_zAxis = 3;
//

void setup() {
  Serial.begin(9600);
  
  //Dot matrix
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);
  //

  //LCD
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Arduino Project");
  lcd.setCursor(0, 1);
  lcd.print("set READY");
  //

  //potentiometer
  //

  //keypad
  //

  //joystick
  pinMode(pin_zAxis, INPUT_PULLUP);
  //

  delay(1000);
}

int temperature = 0;
int humidity = 0;
int state = 0;

int CurrentSpeed = 0;
int xValue=0;
int yValue=0;
bool zValue=false;

char buf[80];

void loop() {
  potentiometer();
  joystick();
  community();
  print_lcd();
  dot_matrix();  
}

void community(){
  String data = "";
  while(Serial.available()){
    char temp = Serial.read();
    data.concat(temp);
  }
  
    if(data[0] == 'a'){
      temperature = (data.substring(2, 5)).toInt();
      humidity = (data.substring(5, 8)).toInt();
      state = (data[8]);
    }

    String TX;
    sprintf(buf, "b)%3d%4d%4d", CurrentSpeed, xValue, yValue);
    TX = buf;
    
    TX += zValue == HIGH ? "1" : "0";

    Serial.println(TX);
}

void dot_matrix(){
  if(state == '1') print_safe();
  else if(state == '2') print_help();
  else;

}

void print_help(){
  for(int a=0;a<=2;a++) {
  for(int i = 0; i < 4; i++) {
    for(int j=0;j<8;j++)
      lc.setRow(0, j, help[i][j]);
    delay(500);
  }
  }
}

void print_safe(){
  for(int a=0;a<=2;a++) {
  for(int i = 0; i < 4; i++) {
    for(int j=0;j<8;j++)
      lc.setRow(0, j, safe[i][j]);
   delay(500); 
  }
  }
}
//
void print_lcd(){
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Temp=");
  lcd.setCursor(5, 0);
  lcd.print(temperature);

  lcd.setCursor(8, 0);
  lcd.print("humid=");
  lcd.setCursor(14,0);
  lcd.print(humidity);

  lcd.setCursor(0, 1);
  lcd.print("Speed = ");
  lcd.setCursor(8,1);
  lcd.print(CurrentSpeed);

  delay(400);
}

void potentiometer(){
  int adcValue;
  adcValue = analogRead(pin_potentio);
  CurrentSpeed = map(adcValue, 0, 1023, 0, 255);
}

void macro_keypad(){
  
}

void joystick(){
  xValue = analogRead(pin_xAxis);
  yValue = analogRead(pin_yAxis);
  zValue = digitalRead(pin_zAxis);
}
