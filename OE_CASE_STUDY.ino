#include <Wire.h>

#include <LiquidCrystal_I2C.h>// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
LiquidCrystal_I2C lcd(0x27,20,4);
#include <Servo.h> //includes the servo library
int Contrast =130;
Servo myservo1;
int irPin1 = 3; // define IR sensor 1 pin
int irPin2 = 4; // define IR sensor 2 pin
int irPin3 = 6; // define IR sensor 3 pin
int led1Pin = 8; // define LED1 pin
int led2Pin = 9; // define LED2 pin
int led3Pin = 7; // define LED3 pin
int ir_s1 = 5;
int ir_s2 = 4;

int Total = 5;
int Space;

int flag1 = 0;
int flag2 = 0;

void setup() {

  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);
  pinMode(irPin3, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
analogWrite(5,Contrast);
pinMode(ir_s1, INPUT);
pinMode(ir_s2, INPUT);

lcd.init();
lcd.clear();
lcd.backlight(); 
myservo1.attach(2);
myservo1.write(90);

lcd.begin(20, 4);  
lcd.setCursor (0,0);
lcd.print("  Car  Parking  ");
lcd.setCursor (0,1);
lcd.print("     System     ");
Wire.setClock(2000);
delay (2000);
lcd.clear();  

Space = Total;
}

void loop(){ 
checkIR();
if(digitalRead (ir_s1) == LOW && flag1==0){
if(Space>0){flag1=1;
if(flag2==0){myservo1.write(0); Space = Space-1;}
}else{
lcd.setCursor (0,0);
lcd.print(" Sorry not Space ");  
lcd.setCursor (0,1);
lcd.print("    Available    "); 
delay (1000);
lcd.clear(); 
}
}

if(digitalRead (ir_s2) == LOW && flag2==0){flag2=1;
if(flag1==0){myservo1.write(0); 
if(Space <5)
Space = Space+1;}
}

if(flag1==1 && flag2==1){
delay (1000);
myservo1.write(100);
flag1=0, flag2=0;
}

lcd.setCursor (0,0);
lcd.print("Total Space: ");
lcd.print(Total);

lcd.setCursor (0,1);
lcd.print("Have  Space: ");
lcd.print(Space);
}


void checkIR() {
  int ir1Status = digitalRead(irPin1);
  int ir2Status = digitalRead(irPin2);
  int ir3Status = digitalRead(irPin3);

  if (ir1Status != HIGH) {
    digitalWrite(led1Pin, HIGH);
  } else {
    digitalWrite(led1Pin, LOW);
  }

  if (ir2Status != HIGH) {
    digitalWrite(led2Pin, LOW);
  } else {
    digitalWrite(led2Pin, LOW);
  }

  if (ir3Status != HIGH) {
    digitalWrite(led3Pin, HIGH);
  } else {
    digitalWrite(led3Pin, LOW);
  }
}
