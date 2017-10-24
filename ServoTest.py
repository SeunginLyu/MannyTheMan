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

skeleton.actuateMotion(models.sample_motions[2])


def setServoAngle(angle, pin):
    pwm = GPIO.PWM(pin, 50)
    pwm.start(0)
    duty = angle / 18 + 2
    GPIO.output(pin, True)
    pwm.ChangeDutyCycle(duty)
    time.sleep(1)
    GPIO.output(pin, False)
    pwm.ChangeDutyCycle(0)
    pwm.stop()


refresh_period = 0.02

for joint in skeleton.joints:
    setServoAngle(0, joint.servo)

for joint in skeleton.joints:
    setServoAngle(joint.angle, joint.servo)
    print(joint.angle, joint.servo)

GPIO.cleanup()