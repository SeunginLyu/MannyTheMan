import RPi.GPIO as GPIO

from time import sleep

# RaspberryPi Pin initialization
outPinA = 2
outPinB = 3
outPinC = 4
outPinD = 7

GPIO.setmode(GPIO.BCM)

GPIO.setup(outPinA, GPIO.OUT)
GPIO.setup(outPinB, GPIO.OUT)
GPIO.setup(outPinC, GPIO.OUT)
GPIO.setup(outPinD, GPIO.OUT)

pwm = GPIO.PWM(outPinD, 50)
pwm.start(0)


def setServoAngle(angle, pin):
    duty = angle / 18 + 2

    GPIO.output(pin, True)
    pwm.ChangeDutyCycle(duty)
    sleep(1)

    GPIO.output(pin, False)
    pwm.ChangeDutyCycle(0)


setServoAngle(90, outPinA)
setServoAngle(90, outPinB)
setServoAngle(90, outPinC)
setServoAngle(90, outPinD)

pwm.stop()
GPIO.cleanup()
