

import RPi.GPIO as GPIO
from time import sleep

x = 3

GPIO.setmode(GPIO.BCM)
GPIO.setup(x, GPIO.OUT)

pwm=GPIO.PWM(x, 50)
pwm.start(0)


def SetAngle(angle):
	duty = angle / 18 + 2
	GPIO.output(x, True)
	pwm.ChangeDutyCycle(duty)
	sleep(1)
	GPIO.output(x, False)
	pwm.ChangeDutyCycle(0)

SetAngle(90) 


pwm.stop()
GPIO.cleanup()

