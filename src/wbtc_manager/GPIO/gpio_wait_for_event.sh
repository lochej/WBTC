#!/bin/bash

#usage : gpio_wait_for_event.sh <gpiochip/chip-number> <gpiopin/line-number> <edge-level>
#edge-level : 0 falling edge, 1 rising edge

gpiochip=$1
gpiopin=$2
gpiolevel=$3

#line buffer output and format 0 for falling edge and 1 for rising edge
while read line
do
	if [ $line = $gpiolevel ]
	then
		echo $line
		exit 0
	fi
done < <(gpiomon -bF "%e" $gpiochip $gpiopin)


