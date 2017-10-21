import RPi.GPIO as GPIO

from time import sleep

outPinA = 2
outPinB = 3
outPinC = 4
outPinD = 7

GPIO.setmode(GPIO.BCM)

GPIO.setup(outPinA, GPIO.OUT)
GPIO.setup(outPinB, GPIO.OUT)
GPIO.setup(outPinC, GPIO.OUT)
GPIO.setup(outPinD, GPIO.OUT)

pwm=GPIO.PWM(outPinD, 50)

pwm.start(0)


def SetAngle(angle, pin):
    duty = angle / 18 + 2

    GPIO.output(pin, True)
    pwm.ChangeDutyCycle(duty)
    sleep(1)

    GPIO.output(pin, False)
    pwm.ChangeDutyCycle(0)

SetAngle(90, outPinA)
SetAngle(90, outPinB)
SetAngle(90, outPinC)
SetAngle(90, outPinD)

pwm.stop()
GPIO.cleanup()
