
import pigpio
import time

Min = 1000;
Max = 2000;

NUM_GPIO = 32;

step = [0]*NUM_GPIO
width = [0]*NUM_GPIO
used = [False]*NUM_GPIO

pi = pigpio.pi()

Allpins = [1,2,3,4]


for g in Allpins:
	used[g] = True;
	step[g] = 10;
	width[g] = 1500;

print("Sending to Servos")

for g in Allpins:
	pi.set_servo_pulsewidth(g, width[g])

	width[g] += step[g]

	time.sleep(0.1)


for g in Allpins:
	pi.set_servo_pulsewidth(g, 0)


pi.stop()