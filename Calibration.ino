
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


   
//int ServosToMove[] = {2, 3, 6};
//int AnglesToMove [] = {170, 300, 300}; // NEEDS TO BE CONVERTED FIRST for now it has to be between 150 and 600
//incrementMotion(AnglesToMomve, ServosToMove);
 

void loop() {
  for(int i=150; i < 600 ; i = i + 45){
    pwm.setPWM(1, 0, i);
    delay(5000); 
  }
}
