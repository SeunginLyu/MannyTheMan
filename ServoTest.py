import RPi.GPIO as GPIO
import models

from time import sleep

# RaspberryPi Pin initialization
outPinA = 2
outPinB = 3
outPinC = 4
outPinD = 17

GPIO.setmode(GPIO.BCM)

# Initialies servo output pins
GPIO.setup(outPinA, GPIO.OUT)
GPIO.setup(outPinB, GPIO.OUT)
GPIO.setup(outPinC, GPIO.OUT)
GPIO.setup(outPinD, GPIO.OUT)

skeleton = models.Skeleton(models.sample_names,
                           models.sample_links,
                           models.sample_servos)

skeleton.actuateMotion(models.sample_motions[1])


# def setServoZero(pin):
#     pwm =

def setServoAngle(angle, pin):
    pwm = GPIO.PWM(pin, angle)
    pwm.start(0)
    duty = angle / 18 + 2
    GPIO.output(pin, True)
    pwm.ChangeDutyCycle(duty)
    sleep(0.3)
    pwm.ChangeDutyCycle(0)
    pwm.stop()
    GPIO.output(pin, False)


for joint in skeleton.joints:
    setServoAngle(170, joint.servo)
    print(170, joint.servo)

GPIO.cleanup()
