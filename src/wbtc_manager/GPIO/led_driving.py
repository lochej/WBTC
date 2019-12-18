#!/usr/bin/python3

from subprocess import run
from time import sleep

led_id=0

leds_state=[0]*8


def setLeds(leds_state):
	
	args=[]
	args.append("gpioset")
	args.append("gpiochip3")
	
	for i in range(len(leds_state)):
		
		args.append("%d=%d"%(i,leds_state[i]))
	run(args)


while(True):
	
	for i in range(len(leds_state)):
		leds_state[i]=1
		setLeds(leds_state)
		sleep(0.2)
	
	for i in reversed(range(len(leds_state))):
		leds_state[i]=0
		setLeds(leds_state)
		sleep(0.2)
		

