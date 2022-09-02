#include <Servo.h>;
#include <Wire.h>
#include <SoftwareSerial.h>
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

#define lineSensor1 47
#define lineSensor2 51
#define lineSensor3 52

#define blue 44
#define green 46
#define red 45


const int TxPin  = 6;

SoftwareSerial mySerial = SoftwareSerial(255, TxPin);

Servo servo1;
Servo servo2;

int challengeCount = 0;

long initialTime = millis();

int signalCount = 0;

bool x1 = false;
bool x2 = true;
bool x3 = false;

bool group73 = true;
bool group75 = true;
bool group76 = true;
bool group77 = true;
bool group74 = true;

bool now = false;

bool boolin = true;

char nextChallenge[7][40] = {
  "0 Nytrogen",
  "1 Health",
  "2 Medicine",
  "3 VR",
  "4 Discovery",
  "5 Brain",
  "6 Learning"
};

void setup() {

  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
  
  servo1.attach(11);
  servo2.attach(12);

  pinMode(5, INPUT);
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(blue, HIGH);
  
  Serial.begin(9600); // Set to No line ending;
  Serial2.begin(9600);

}

void lightOff() {
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(blue, HIGH);
}

void colorSense() {
  for (int i = 0; i < 4; i++) {
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  if (lux > 40000) {
    Serial.println("red");
    digitalWrite(red, LOW);
    delay(1000);
    digitalWrite(red, HIGH);
  }
  if (lux < 50) {
    Serial.println("purple");
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
    delay(1000);
    digitalWrite(red, HIGH);
    digitalWrite(blue, HIGH);
  }
  if ((lux > 1100) && (lux < 1400)) {
    Serial.println("yellow");
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    delay(1000);
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
  }

  if ((lux > 60) && (lux < 100)) {
    Serial.println("blue");
    digitalWrite(blue, LOW);
    delay(1000);
    digitalWrite(blue, HIGH);
  }

  if ((lux > 800) && (lux < 1000)) {
    Serial.println("green");
    digitalWrite(green, LOW);
    delay(1000);
    digitalWrite(green, HIGH);
  }
  if ((lux > 400) && (lux < 700)) {
    Serial.println("gray");
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
    delay(1000);
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    digitalWrite(blue, HIGH);
  }
  }
}

long RCTime(int sensorIn)
{
  
  long duration = 0;
  pinMode(sensorIn, OUTPUT); // Sets pin as OUTPUT
  digitalWrite(sensorIn, HIGH); // Pin HIGH
  delay(1); // Waits for 1 millisecond
  pinMode(sensorIn, INPUT); // Sets pin as INPUT
  digitalWrite(sensorIn, LOW); // Pin LOW

  while(digitalRead(sensorIn)) { // Waits for the pin to go LOW
    duration++;

  }
return duration; // Returns the duration of the pulse
}

void checker() {
   if (RCTime(lineSensor1) > 100) {
  x1 = true;
 } else {
  x1 = false;
 }

  if (RCTime(lineSensor2) > 100) {
  x2 = true;
 } else {
  x2 = false;
 }

  if (RCTime(lineSensor3) > 100) {
  x3 = true;
 } else {
  x3 = false;
 }
}


void forward() {
   servo1.writeMicroseconds(1300);
   servo2.writeMicroseconds(1700);
   delay(50);
   servo1.writeMicroseconds(1500);
   servo2.writeMicroseconds(1500);
}

void communicate() {
      while (signalCount < 6 || millis()-initialTime < 30000) {
      if (now) {
      Serial2.print('8');
      delay(50);
      }
      else {
        Serial2.print('*');
        delay(5);
      }
      delay(20);

      
      if (Serial2.available()) { // If signal is received
      char incoming = Serial2.read(); // Assign character received by signal to variable

      if (incoming == '3' && group73 == true) {
        challengeCount++;
        signalCount++;
        Serial.println(challengeCount);
        group73 = false;
      }

      if (incoming == '#' && group73 == true) { 
        signalCount++;
        group73 = false;
      }

      if (incoming == '5' && group75 == true) {
        challengeCount++;
        signalCount++;
        Serial.println(challengeCount);
        group75 = false;
      }
      if (incoming == '%' && group75 == true) { 
         signalCount++;
         group75 = false;
      }

  
      if (incoming == '6' && group76 == true) {
        challengeCount++;
        signalCount++;
        Serial.println(challengeCount);
        group76 = false;
      }
      if (incoming == '^' && group76 == true) {
         signalCount++;
         group76 = false;
      }


      if (incoming == '7' && group77 == true) {
        challengeCount++;
        signalCount++;
        Serial.println(challengeCount);
        group77 = false;
      }
       if (incoming == '&' && group77 == true) {
              signalCount++;
              group77 = false;
            }
  
      if (incoming == '4' && group74 == true) {
        challengeCount++;
        signalCount++;
        Serial.println(challengeCount);
        group74 = false;
      }
       if (incoming == '$' && group74 == true) {
         challengeCount++;
         signalCount++;
         group74 = false;
       }
      delay(8);
    }
  }
}

void displayInfo() {
  mySerial.begin(9600);
  mySerial.write(12);
  mySerial.write(17);
  delay(5);
  String secondLine = String(nextChallenge[challengeCount]);
  mySerial.print(secondLine); 
  delay(10);
}


void dashmark() {
        servo1.writeMicroseconds(1300); //left
        servo2.writeMicroseconds(1300);
        delay(450);
        servo1.writeMicroseconds(1500);
        servo2.writeMicroseconds(1500);
        servo1.detach();
        servo2.detach();
        
        if (digitalRead(5)) {
          challengeCount++;
          digitalWrite(green, LOW);
          now = true;
          
        }else{
          digitalWrite(red, LOW);
        }
        communicate();
        displayInfo();
        delay(500);
}

void loop() {
   if (boolin == true) {
    colorSense();
    boolin = false;
   }
   checker();
   if (x2 == true && x3 == true && x1 == true) {
      delay(2000);
      dashmark();
    } 
  if (x1 == true) {
   while (x1 == true) { 
      if (x2 == true && x3 == true && x1 == true) {
        delay(2000);
        dashmark();
      }else {
        servo1.writeMicroseconds(1300);
        servo2.writeMicroseconds(1300);
        delay(20);
        servo1.writeMicroseconds(1500);
        servo2.writeMicroseconds(1500);
      }
      checker();
   }
  }
   
  if (x3 == true) {
    while (x3 == true) {
      if (x2 == true && x3 == true && x1 == true) {
      delay(2000);
      dashmark();
      while (x2 == true && x3 == true && x1 == true) {
      forward();
      checker();
      }
      } else {
      servo1.writeMicroseconds(1700);
      servo2.writeMicroseconds(1700);
      delay(20);
      servo1.writeMicroseconds(1500);
      servo2.writeMicroseconds(1500);
      }
      checker();
    }
   }

   forward();
}


  
