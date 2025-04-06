#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//ECE 4980 Capstone Group 7 Code//

LiquidCrystal_I2C lcd(0x27, 16, 2); //figure out how to use LCD

const float Vtrip = 0.06; //value for trip voltage can be changed based on client specification//

//int flag;
int tf = 1;

//timer variables
unsigned long trstartt;
unsigned long trcurrentt;
unsigned long trtotalt;
unsigned long rstartt;
unsigned long rcurrentt;
unsigned long rtotalt;

void setup() {
  lcd.begin();
  lcd.backlight();
  pinMode(2, OUTPUT); //digital pwm 2, relay switch
  pinMode(4, OUTPUT); //digital pwm 4, trip system
  pinMode(A1, INPUT); //sensor input
  pinMode(8, INPUT); //switch input
}

void loop() {

// current sensing module //

float adc = analogRead(A1); //value from the line
float voltage = adc*(5/1023.0); 
bool trip = voltage < Vtrip; //overcurrent tripping condition//

// state conditions //

if(trip && tf == 0) {
  tf = 1;
  trstartt = micros();
  digitalWrite(2, LOW); //trips the relay
  digitalWrite(4, HIGH); //turns on the trip system LED
  trcurrentt = micros();
  trtotalt = trcurrentt - trstartt;

  // lcd print functions on current state of the breaker //

  lcd.clear();
  lcd.setCursor(1, 0); //overcurrent fault trip statement
  lcd.print(trtotalt);
  lcd.setCursor(3,0);
  //lcd.print(vline);
  lcd.print("us");
  lcd.setCursor(0,1);
  lcd.print("Relay Tripped");
  delay(3000);

  // for testing comment out lines 52, 53, 55, 56, 57, uncommment 50 //

}

else if(digitalRead(8) == HIGH && tf == 1) {
  tf = 0;
  rstartt = micros();
  digitalWrite(4, LOW); //turns off the trip system LED
  digitalWrite(2, HIGH); //resets the digital relay, flips switch
  rcurrentt = micros();
  rtotalt = rcurrentt - rstartt;
  
  // lcd print functions on current state of the breaker //

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(rtotalt);
  lcd.setCursor(3,0);
  lcd.print("us");
  lcd.setCursor(0,1);
  lcd.print("Relay Reset");
  //lcd.print(vline);
  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Reset Achieved");
  delay(3000);
  digitalWrite(2, HIGH); //digital relay  
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Normal Operation");

// for testing comment out lines 76, 77, 78, 79, 80, 84, 85, 86, 87, 90, 91, 92, uncomment line 81 //

}
delay(1000);
return;
}
