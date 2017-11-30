/*************************************************** 
  This is code for running multiple servos on teh Adafruit PWM 16 servo board. The code chnages 
  servo positions with pulses of power.
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
int currentpositions[] = {0,0,0,0,0,0,0,0,0};
int desiredpositions[] = {0,0,0,0,0,0,0,0,0};
int holderVec[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int numberOfServos = 8 + 1;
int countInput = 0;
int timeRec = millis();
int count = 0;
String input;
boolean move = false;


void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  yield();
}


int AngleFind(int ang){
  if(ang <= 10){
    return 160;
  }
  if(ang <= 30){
    return 220;
  }
  if(ang <= 50){
    return 270;
  }
  if(ang <= 70){
    return 320;
  }
  if(ang <= 90){
    return 370;
  }
  if(ang <= 110){
    return 420;
  }
   if(ang <= 130){
    return 470;
  }
  if(ang <= 150){
    return 520;
  }
  if(ang <= 180){
    return 550;
  }
}

void incrementMotion(){
  int holdpos[] = {0,0,0,0,0,0,0,0,0};
  int change[] = {0,0,0,0,0,0,0,0,0}; // Right spaces are hard coded right now
  boolean done = false;
  for(int r = 0; r < numberOfServos; r ++){
      holdpos[r] = currentpositions[r];
  }  
  while(!done){
    for(int m = 0; m < numberOfServos; m ++){
      change[m] = desiredpositions[m] - holdpos[m];
    }  
    for(int i= 1; i <= numberOfServos; i ++){
      if(sumArray(change) == 0){
        done = true;
      }
      if(change[i] != 0){
        if(change[i] < 0){
          pwm.setPWM(i, 0, currentpositions[i] - 8 );
          holdpos[i] = holdpos[i] - 8;
        }else{
          pwm.setPWM(i, 0, currentpositions[i] + 8 );
          holdpos[i] = holdpos[i] + 8;
        }
      }
    }
  }
  
  for(int r = 0; r < numberOfServos; r ++){
      currentpositions[r] = holdpos[r];
  } 
}



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
        desiredpositions[ServoNum] = AngleFind(AngleVal);
        
       }

       move = true;

     
  }


  
   //String input = "2,90,3,40,4,50,1,20,6,30,5,80,7,100,8,100";

   


   
  //desiredpositions[0] = AngleFind(90);
  //desiredpositions[1] = AngleFind(90);
  /*desiredpositions[2] = AngleFind(90);
  desiredpositions[3] = AngleFind(90);
  desiredpositions[4] = AngleFind(90);
  desiredpositions[5] = AngleFind(90);
  desiredpositions[6] = AngleFind(90);
  desiredpositions[7] = AngleFind(90);
  countInput = 2;
  */
  if(move){
         incrementMotion(); 
         countInput = 0;
         Serial.print(input);
         move = false;
         delay(500);
  }
}
