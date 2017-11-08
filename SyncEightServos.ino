/*************************************************** 
  This is code for running multiple servos on teh Adafruit PWM 16 servo board. The code chnages 
  servo positions with pulses of power.
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
int currentpositions[8];
int desiredpositions[8];

int numberOfServos = 8;

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  yield();

  // setting the intial positions to 0;
  for(int p =0; p< sizeof(currentpositions); p++){
    currentpositions[p] = 0;
    desiredpositions[p] = 0;
  }

  desiredpositions[1] = 400;
  desiredpositions[3] = 600;

}


int AngleFind( int angle){

  // This function takes an servo angle and returns the PWM value
  return angle*2.6 + 135;
  
}

void incrementMotion(){
  int change[] = {0,0,0,0,0,0,0,0}; // Right spaces are hard coded right now
  for(int m = 0; m < sizeof(currentpositions); m ++){
    change[m] = desiredpositions[m] - currentpositions[m];
  }  
  boolean done = false;
  while(!done){
    int count = 0;
    
    for(int i= 1; i <= numberOfServos; i ++){
      if(sumArray(change) == 0){
        done = true;
      }
      if(change[i] != 0){
        if(change[i] < 0){
          pwm.setPWM(i, 0, currentpositions[i] - 8 );
        }else{
          pwm.setPWM(i, 0, currentpositions[i] + 8 );
        }
      }
      count++;
    }
  }
}



int sumArray(int arrayToSum[]){
  int total = 0;
  for(int i = 0; i < sizeof(arrayToSum); i++){
    total = arrayToSum[i] + total;
  }
  return total;
}

  int ServosToMove[] = {2, 3, 6};
void loop() {


  // NEEDS TO BE CONVERTED FIRST for now it has to be between 150 and 600
  incrementMotion();
  delay(1000);
  Serial.println("SWITCH");
  desiredpositions[1] = 200;
  desiredpositions[3] = 200;
  // If this dosen't work try updating currentpositions
  delay(1000);
}
