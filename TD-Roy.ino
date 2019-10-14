#include <Adafruit_MAX31855.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include "PID_v1.h"

#define MAXDO   10
#define MAXCS   13
#define MAXCLK  9
#define HEATER 3
#define FAN 7


// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, 100, 10, 5, DIRECT);

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 20, 4);

unsigned short setTemp = 35;//the start temperature of the TD oven
int v = 0;//this is where the temp is stored (voltage autput of themocaouple aplifier)
short stendby = 0;//this is used for chase is the switcher regulating stendby is on or off

//array of temperature set of the program e.g. 2 min on 35C then ramp... etc
//unsigned short tempSets [] = {25,26,27,28,29,30,31,31,31,31,33,34,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,36,36,37,38,38,39,40,40,41,42,42,43,44,44,45,46,46,47,48,48,49,50,50,51,52,52,53,54,54,55,56,56,57,58,58,59,60,60,61,62,62,63,64,64,65,66,66,67,68,68,69,70,70,71,72,72,73,74,74,75,76,76,77,78,78,79,80,80,81,82,82,83,84,84,85,86,86,87,88,88,89,90,90,91,92,92,93,94,94,95,96,96,97,98,98,99,100,100,101,102,102,103,104,104,105,106,106,107,108,108,109,110,110,111,112,112,113,114,114,115,116,116,117,118,118,119,120,120,121,122,122,123,124,124,125,126,126,127,128,128,129,130,130,131,132,132,133,134,134,135,136,136,137,138,138,139,140,140,141,142,142,143,144,144,145,146,146,147,148,148,149,150,150,151,152,152,153,154,154,155,156,156,157,158,158,159,160,160,161,162,162,163,164,164,165,166,166,167,168,168,169,170,170,171,172,172,173,174,174,175,176,176,177,178,178,179,180,180,181,182,182,183,184,184,185,186,186,187,188,188,189,190,190,191,192,192,193,194,194,195,196,196,197,198,198,199,200,200,201,202,202,203,204,204,205,206,206,207,208,208,209,210,210,211,212,212,213,214,214,215,216,216,217,218,218,219,220,220,221,222,222,223,224,224,225,226,226,227,228,228,229,230,230,231,232,232,233,234,234,235,235,236,236,237,237,238,238,238,238,239,239,239,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35};
unsigned short tempSets [] = {35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 36, 36, 37, 38, 38, 39, 40, 40, 41, 42, 42, 43, 44, 44, 45, 46, 46, 47, 48, 48, 49, 50, 50, 51, 52, 52, 53, 54, 54, 55, 56, 56, 57, 58, 58, 59, 60, 60, 61, 62, 62, 63, 64, 64, 65, 66, 66, 67, 68, 68, 69, 70, 70, 71, 72, 72, 73, 74, 74, 75, 76, 76, 77, 78, 78, 79, 80, 80, 81, 82, 82, 83, 84, 84, 85, 86, 86, 87, 88, 88, 89, 90, 90, 91, 92, 92, 93, 94, 94, 95, 96, 96, 97, 98, 98, 99, 100, 100, 101, 102, 102, 103, 104, 104, 105, 106, 106, 107, 108, 108, 109, 110, 110, 111, 112, 112, 113, 114, 114, 115, 116, 116, 117, 118, 118, 119, 120, 120, 121, 122, 122, 123, 124, 124, 125, 126, 126, 127, 128, 128, 129, 130, 130, 131, 132, 132, 133, 134, 134, 135, 136, 136, 137, 138, 138, 139, 140, 140, 141, 142, 142, 143, 144, 144, 145, 146, 146, 147, 148, 148, 149, 150, 150, 151, 152, 152, 153, 154, 154, 155, 156, 156, 157, 158, 158, 159, 160, 160, 161, 162, 162, 163, 164, 164, 165, 166, 166, 167, 168, 168, 169, 170, 170, 171, 172, 172, 173, 174, 174, 175, 176, 176, 177, 178, 178, 179, 180, 180, 181, 182, 182, 183, 184, 184, 185, 186, 186, 187, 188, 188, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195, 196, 196, 197, 198, 198, 199, 200, 200, 201, 202, 202, 203, 204, 204, 205, 206, 206, 207, 208, 208, 209, 210, 210, 211, 212, 212, 213, 214, 214, 215, 216, 216, 217, 218, 218, 219, 220, 220, 221, 222, 222, 223, 224, 224, 225, 226, 226, 227, 228, 228, 229, 230, 230, 231, 232, 232, 233, 234, 234, 235, 236, 236, 237, 238, 238, 239, 240, 241, 241, 242, 243, 243, 244, 245, 245, 246, 247, 247, 248, 249, 249, 250, 251, 251, 252, 253, 253, 254, 255, 255, 256, 257, 257, 258, 259, 259, 260, 261, 261, 262, 263, 263, 264, 265, 265, 266, 267, 267, 268, 269, 269, 270, 271, 271, 272, 273, 273, 274, 275, 275, 276, 277, 277, 278, 279, 279, 280, 281, 281, 282, 283, 283, 284, 285, 285, 286, 287, 287, 288, 289, 289, 290, 291, 291, 292, 293, 293, 294, 295, 295, 296, 297, 297, 298, 299, 299, 300, 301, 301, 302, 303, 303, 304, 305, 305, 306, 307, 307, 308, 309, 309, 310, 311, 311, 312, 313, 313, 314, 315, 315, 316, 317, 317, 318, 319, 319, 320, 321, 321, 322, 323, 323, 324, 325, 325, 326, 327, 327, 328, 329, 329, 330, 331, 331, 332, 333, 333, 334, 335, 335, 336, 337, 337, 338, 339, 339, 340, 341, 341, 342, 343, 343, 344, 345, 345, 346, 347, 347, 348, 349, 349, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35};

int WindowSize = 1000;//this is number of miliseconds in which is PID loop executed
unsigned long windowStartTime;//absolute start of a prog
unsigned long freshStartTime;//start in miliseconds after the switcher is trigered on or off for the stendby

void setup() {
  // put your setup code here, to run once:
  windowStartTime = millis();
  freshStartTime =  windowStartTime;
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();


  pinMode(MAXCLK, OUTPUT);
  pinMode(MAXDO, INPUT);
  pinMode(MAXCS, OUTPUT);
  pinMode(FAN, OUTPUT);//to the fan
  pinMode(HEATER, OUTPUT); //to the Heater

  //switching pins
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH); //one end off swither
  pinMode(5, INPUT);

  digitalWrite(MAXCS, HIGH);
  digitalWrite(MAXCLK, LOW);
  digitalWrite(FAN, LOW); //fan off

  Input = thermocouple.readCelsius();

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  // tempSets = setRamp();

}

void loop() {

  v = thermocouple.readCelsius();

  lcd.setCursor(0, 0);


  if (digitalRead(5) == HIGH) { //swither on
    //time = int(millis());//time in seconds since started
    unsigned long now = millis();
    if (stendby == 1) {
      Serial.println("inside ON");
      freshStartTime = millis();
      stendby = 0;
    }



    digitalWrite(7, LOW); //fan off

    if (int((now - freshStartTime) / 1000) >= 742) { //800
      Setpoint = setTemp;
      if (v > (Setpoint)) {
        digitalWrite(FAN, HIGH); //fan on
      }
    }
    else {
      Setpoint = tempSets[(now - freshStartTime) / 1000];
      //Setpoint = setTemp;
    }

    if (v == -1)   {
      Serial.print("No sensor \n");
      lcd.init(); //roy
      lcd.backlight(); //roy
      lcd.setCursor(0, 0); //roy
      lcd.print("No sensor"); //roy
      //analogWrite(3,0);

    }
    else   {

      Input = v;
      myPID.Compute();

      if (now - windowStartTime > WindowSize) {
        //time to shift the Relay Window
        windowStartTime += WindowSize;
        Serial.print("Time: ");
        Serial.print((now - freshStartTime) / 1000);
        Serial.print("   GetTemp: ");
        Serial.print(Input);
        Serial.print("   Output: ");
        Serial.print(Output);
        Serial.print("   SetTemp: ");
        Serial.print(Setpoint);
        Serial.println("   Sequence ON !!!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("TD run:");
        lcd.setCursor(8, 0);
        lcd.print((now - freshStartTime) / 1000);
        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.setCursor(6, 1);
        lcd.print(Input,0);
      }

      if (Output > now - windowStartTime) digitalWrite(HEATER, HIGH);
      else digitalWrite(HEATER, LOW);

      //analogWrite(3,Output);
    }
  }//end digitl readline
  else { //stendby
    if (stendby == 0) { //it supeso to go automaticaly into stedby mode
      Serial.println("inside STENDBY");
      freshStartTime = millis();
      stendby = 1;
    }
    unsigned long now = millis();
    Setpoint = setTemp;
    if (v > (Setpoint)) {
      digitalWrite(FAN, HIGH); //fan on
    }
    else {
      digitalWrite(FAN, LOW); //fan on
    }
    if (v == -1)   {
      Serial.print("No sensor \n");
      lcd.init(); //roy
      lcd.backlight(); //roy
      lcd.setCursor(0, 0); //roy
      lcd.print("No sensor"); //roy
      //analogWrite(3,0);

    }
    else   {

      Input = v;
      myPID.Compute();

      if (now - windowStartTime > WindowSize) {
        //time to shift the Relay Window
        windowStartTime += WindowSize;
        Serial.print("Time: ");
        Serial.print((now - freshStartTime) / 1000);
        Serial.print("   GetTemp: ");
        Serial.print(Input);
        Serial.print("   Output: ");
        Serial.print(Output);
        Serial.print("   SetTemp: ");
        Serial.print(Setpoint);
        Serial.println("   STANDBY !!!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("STANDBY:");
        lcd.setCursor(8, 0);
        lcd.print((now - freshStartTime) / 1000);
        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.setCursor(6, 1);
        lcd.print(Input,0);
      }

      if (Output > now - windowStartTime) digitalWrite(HEATER, HIGH);
      else digitalWrite(HEATER, LOW);

      //analogWrite(3,Output);
    }

  }

}
