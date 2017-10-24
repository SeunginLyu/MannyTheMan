import RPi.GPIO as GPIO
import models
import time

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
    pwm.ChangeDutyCycle(5)
    time.sleep(1)
    GPIO.output(pin, False)
    pwm.ChangeDutyCycle(0)
    pwm.stop()


refresh_period = 0.02


# def setServoAngle(angle, pin):
#     for _ in range(1, 100):
#         GPIO.output(pin, False)
#         time.sleep(0.001)
#         GPIO.output(pin, True)
#         time.sleep(refresh_period)
#

for joint in skeleton.joints:
    setServoAngle(170, joint.servo)
    print(170, joint.servo)

GPIO.cleanup()
