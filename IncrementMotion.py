
import RPi.GPIO as GPIO
import models
import time

# RaspberryPi Pin initialization
outPinA = 2
outPinB = 3
outPinC = 4
outPinD = 17

GPIO.setmode(GPIO.BCM)

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


def findDiff(current, desired):
	c = []
	for i in len(current):
		c.append(desired[i]-current[i])
	return c

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

def incrementMotion(currentPositions, desiredPositions):
	change = [0, 0, 0, 0]
	done = false;

	while(!done):
		count = 0
		for joint in skeleton.joints:
			count = count + 1
			change[count] = findDiff( currentPositions, desiredPositions)
			if (sum(change) == 0): 
				done = true
			if (change[count] != 0):
				if (change[count] < 0):
					setServoAngle(currentPositions[i]-1, joint.servo)
				else:
					setServoAngle(currentPositions[i]+1, joint.servo)

incrementMotion([0,0,0,0], [10,10,10,10])