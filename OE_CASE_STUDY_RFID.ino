#include <Wire.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

int irPin2 = 4; // define IR sensor 2 pin
#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 2
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522 rfid(SS_PIN, RST_PIN);
Servo servo;

byte authorizedUID[4] = {0xB9, 0x0C, 0x15, 0xCC};
int angle = 0; // the current angle of servo motor
Servo myservo;
#include <LiquidCrystal_I2C.h>// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
LiquidCrystal_I2C lcd(0x27,20,4);
#include <Servo.h> //includes the servo library
int Contrast =130;
Servo myservo1;
int irPin1 = 3; // define IR sensor 1 pin
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
  Serial.begin(9600); 
  SPI.begin(); 
  mfrc522.PCD_Init(); 
  myservo.attach(2); 
  
  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);
  pinMode(irPin3, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
analogWrite(5,Contrast);
pinMode(ir_s1, INPUT);
pinMode(ir_s2, INPUT);
Serial.begin(9600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  servo.attach(SERVO_PIN);
 // servo.write(angle); // rotate servo motor to 0°

  Serial.println("Tap RFID/NFC Tag on reader");
lcd.init();
lcd.clear();
lcd.backlight(); 
myservo1.attach(2);
//myservo1.write(100);

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
lock();
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

//
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
void lock()
{
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

      if (rfid.uid.uidByte[0] == authorizedUID[0] &&
          rfid.uid.uidByte[1] == authorizedUID[1] &&
          rfid.uid.uidByte[2] == authorizedUID[2] &&
          rfid.uid.uidByte[3] == authorizedUID[3] ) {
        Serial.println("Authorized Tag");

        // change angle of servo motor
        if (angle == 0)
          angle = 90;
        else //if(angle == 90)
          angle = 0;

        // control servo motor arccoding to the angle
        servo.write(angle);
        Serial.print("Rotate Servo Motor to ");
        Serial.print(angle);
        Serial.println("°");
      } else {
        Serial.print("Unauthorized Tag with UID:");
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();
      }

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
}
