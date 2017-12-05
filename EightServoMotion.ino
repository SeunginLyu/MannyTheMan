#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

#if defined(ARDUINO_ARCH_SAMD)  
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

int numberOfServos = 8;

void setup() {
  // ESP8266 is a componet for getting commands over the internet
  // It is not important for our use
  // #ifdef and #endef mean if defined and end defined to make a concise if statement
#ifdef ESP8266
  Wire.pins(2, 14);   // ESP8266 can use any two pins, such as SDA to #2 and SCL to #14
#endif
  
  Serial.begin(9600);
  Serial.println("16 channel PWM test!");

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency
  // The larger this number is the more pwm "hits" the system gets

#ifdef TWBR    // TWBR stands for Two Wire Bit Rate Register, it contorls the frequency of the clock (scl)
// This is basically for having several devices talking together
  uint8_t twbrbackup = TWBR;
  TWBR = 12;
#endif
}


void AngleSet( Servos, Angles ){
  // This function takes an interger array of servos and an interger array of angles and moves them
  // Drive each PWM in a 'wave'
  // 4096 is the number of ticks in the run through
  for (uint16_t i=0; i<4096; i += 8) {
    // 4096 comes from the mathmatically relationship of a Analog to Digital Converter,
    // The number represetns 12 bits of resolution since 2 ^ 12 = 4096
    for (uint8_t pwmnum=0; pwmnum < sizeof(Servos); pwmnum++) {
      // for each servo run a loop
      pwm.setPWM(pwmnum, 0, (i + (4096/16)*pwmnum) % 4096 );
      // This last value is the pulse length in the other example code
      // You make this negative to go backwards
      // setPWM takes the following arguments: pinNumber, tick time to go high out of 4096, tick time to go low out of 4096
    }
  }
}

void incrementMotion(currentPositions, desiredPositions){
  int[] change = {0,0,0,0,0,0,0,0}; // Right spaces are hard coded right now
  //for(int m = 0; m < sizeof(currentPositions); m ++){
  //  change[m] = desiredPositions[m] - currentPositions[m];
  //}
  boolean done = false;
  while(!done){
    int count = 0;
    for(int i= 1; i <= numberOfServos; i ++){
      change[m] = desiredPositions[m] - currentPositions[m];
      if(sumArray(change) == 0){
        done = true;
      }
      if(change[count] != 0){
        if(change[count] < 0){
          pwm.setPWM(pwmnum, 0, currentPosition[i] - 8 );
        }else{
          pwm.setPWM(pwmnum, 0, currentPosition[i] + 8 );
        }
      }
      count++;
    }
  }
}


int[] sumArray(arrayToSum){
  int total = 0;
  for(int i = 0; i < sizeof(arrayToSum); i++){
    total = arrayToSum[i] + total;
  }
  return total;
}
  
   
int ServosToMove[] = {2, 3, 6};
int AnglesToMove [] = {170, 300, 300}; // NEEDS TO BE CONVERTED FIRST for now it has to be between 150 and 600
incrementMotion(AnglesToMomve, ServosToMove);
 

void loop() {

}