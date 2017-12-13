/***************************************************
  This is code for running multiple servos on teh Adafruit PWM 16 servo board. The code chnages
  servo positions with pulses of power.
 ****************************************************/

#include <Wire.h> //This library allows you to communicate with I2C / TWI devices.
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
int currentpositions[] ={0,0,0,0,0,0,0,0,0}; //{90,90,90,90,90,90,90,90,90};
int desiredpositions[] = {0,0,0,0,0,0,0,0,0};
// Example Input:
// 1,90,2,90,3,90,4,90,5,90,6,90,7,90,8,90,
int holderVec[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //the odd index's are the servos and the even index's are the angles that are going to be fed to the arduino
int numberOfServos = 8 + 1;//allows if statement to filter through the different servos
int timeRec = millis(); //returns the number of milliseconds the current sketch has been running since the last reset.
String input; //takes in the input from the kinect
boolean move = false;//senses any possible movement
boolean leftSholderPast90 = false;
boolean rightSholderPast90 = false;
boolean veryStart = true;

//serial moniter
void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  yield();
}

//Finds the angles; senses the angle and converts it to the relative arduino angle
//ex: if the angle is less than or equal to 10 than we know that the angle is around 120 because the range has a buffer around 20, we tested all the angles individually to ensure that these values match with the anglesint AngleFind(int ang){

int AngleFind(int ang){
  int a = ((2.7*ang)+83);
  if(a<140){
    a = 140;
  }
  return a;
}


void PWMmotion(){
  for(int i= 1; i <= numberOfServos; i ++){
    pwm.setPWM(i, 0, desiredpositions[i]);
  }
}

//sets the servos to the angles
int sumArray(int arrayToSum[]){
  int total = 0;

  for(int i = 0; i < sizeof(arrayToSum); i++){

    total = arrayToSum[i] + total;
    if(arrayToSum[i]<30 && arrayToSum[i]>-30){
      total = 0;
    }
  }

  return total;
}

void loop() {

 if(Serial.available() && timeRec - millis() > 7000) {
       timeRec = millis();
       input = Serial.readString();
       String holderString;
       for(int n = 0; n < 8; n++){
        if(n == 0){
          holderString = input;
        }

        byte index1 = holderString.indexOf(',');
        int ServoNum = holderString.substring(0, index1).toInt();
        holderString = holderString.substring(index1 + 1,holderString.length());
        byte index2 = holderString.indexOf(',');
        int AngleVal = holderString.substring(0, index2).toInt();
        holderString = holderString.substring(index2 + 1,holderString.length());
         
         // Setting the specific constraints on the joints.

        if(ServoNum == 8){ 
           // Bottom Right Leg
          AngleVal = 90 - AngleVal;
          if(AngleVal < 25){
            AngleVal = 25;
          }
          if(AngleVal > 70){
            AngleVal = 70;
          }
        }

        if(ServoNum == 7){
          // Bottom Left Leg
          AngleVal = 90 - AngleVal;
          if(AngleVal < 45){
            AngleVal = 45;
          }
          if(AngleVal > 90){
            AngleVal = 90;
          }
        }

        if(ServoNum == 5){ 
          // Top Right Leg
            AngleVal = 85;
        }
         
        if(ServoNum == 6){ 
          // Top Left Leg
            AngleVal = 10;
        }
         
        if(ServoNum == 4){ 
          // Right Shoulder
          if(AngleVal > 170){
            AngleVal = 170;
          }
           AngleVal = 180-AngleVal; 
        }

        if(ServoNum == 3){
          // Left Shoulder
          if(AngleVal > 150){
            AngleVal = 150;
          }
        }
         
        if(ServoNum == 2){ 
          // Right Arm
            AngleVal = 180-AngleVal; 
        }

        if(ServoNum == 1){ 
          // Left Arm
            AngleVal = 180-AngleVal; 
        }
        
        desiredpositions[ServoNum] = AngleFind(AngleVal);
        move= true;
       }
 }


  if(move){
         PWMmotion();
         veryStart = false;
         Serial.print(input);
         move = false;
         delay(500);
  }
}
