#include <DHT11.h>

//motor_driver
  const int motorA_front = 8;
  const int motorA_rear = 7;
  const int motorA_speed = 9;

  const int motorB_front = 5;
  const int motorB_rear = 4;
  const int motorB_speed = 6;
//

//ultraSonic
const int pin_echo = 12;
const int pin_trig = 13;
//

//CdS
const int pin_CdS = 0;
//

//Button
const int button1 = 2;
const int button2 = 3;
//

//LED
const int pin_led = 10;
//

//Temp_Humid
  const int pin_dht = 1;
  DHT11 dht11(pin_dht);
//

void setup() {
  Serial.begin(9600);
  
  //motor_driver
  pinMode(motorA_front, OUTPUT);
  pinMode(motorA_rear, OUTPUT);
  pinMode(motorB_front, OUTPUT);
  pinMode(motorB_rear, OUTPUT);
  //
  
  //ultraSonic
  pinMode(pin_trig, OUTPUT);
  pinMode(pin_echo, INPUT);
  //
  
  //CdS
  //
  
  //Button
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  //
  
  //LED
  pinMode(pin_led, OUTPUT);
  //
  
  //Temp_Humid
  //
}

int CurrentSpeed = 0;
int xValue = 512;
int yValue = 512;
bool zValue = false;

float temperature = 0;
float humidity = 0;
int bright = 0;
int state = 0;

char buf[80];

void loop() {
  temp_humid();
  CdS();
  ultrasonic();
  community();
  motor_driver();
}

//Coummunity
void community(){
  String data = "";
  while(Serial.available()){
    char temp = Serial.read();
    data.concat(temp);
  }

  if(data[0] == 'b'){
    CurrentSpeed = (data.substring(2, 5)).toInt();
    xValue = (data.substring(5, 9)).toInt();
    yValue = (data.substring(9, 13)).toInt();

    if(data[13] == '1') zValue = true;
    else zValue = false;
  }

  String TX;
  sprintf(buf, "a)'%2.1f''%2.1f''%1d'",temperature, humidity, state);
  TX = buf;

  Serial.println(TX);
}
//

//motor_driver
void motor_driver(){
  analogWrite(motorA_speed, CurrentSpeed);
  analogWrite(motorB_speed, CurrentSpeed);
  
  if(xValue < 100){
    digitalWrite(motorA_front, LOW);
    digitalWrite(motorA_rear, HIGH);

    digitalWrite(motorB_front, HIGH);
    digitalWrite(motorB_rear, LOW);
  }
  if(xValue > 900){
    digitalWrite(motorA_front, HIGH);
    digitalWrite(motorA_rear, LOW);

    digitalWrite(motorB_front, LOW);
    digitalWrite(motorB_rear, HIGH);
  }
  if(yValue < 100){
    digitalWrite(motorA_front, LOW);
    digitalWrite(motorA_rear, HIGH);

    digitalWrite(motorB_front, LOW);
    digitalWrite(motorB_rear, HIGH);
  }
  if(xValue > 900){
    digitalWrite(motorA_front, HIGH);
    digitalWrite(motorA_rear, LOW);

    digitalWrite(motorB_front, HIGH);
    digitalWrite(motorB_rear, LOW);
  }

  delay(100);
}
//
  
//ultraSonic
void ultrasonic(){
  float duration, distance;

  digitalWrite(pin_trig, HIGH);
  delay(10);
  digitalWrite(pin_trig, LOW);

  duration = pulseIn(pin_echo, HIGH);
  distance = ((float)(340 * duration) / 10000 ) / 2; //cm
}
//
  
//CdS
void CdS(){
  int adcValue;
  int illuminance;

  adcValue = analogRead(pin_CdS);
  illuminance = map(adcValue, 0, 1023, 100, 0);

  if(illuminance < 10)
    digitalWrite(pin_led, HIGH);
  else
    digitalWrite(pin_led, LOW);
}
//
  
//Button
void button(){
    if(button1 == LOW)  state = 1;
    if(button2 == LOW)  state = 2;

    if(button1 == HIGH && button2 == HIGH) state = 0;
}
//
  
//Temp_Humid
void temp_humid(){
    int err;
    err = dht11.read(humidity, temperature);

    if(err != 0){
      humidity = 0;
      temperature = 0;
    }
}
//
