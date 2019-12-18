#!/bin/bash

gpiochip=$1
gpiopin=$2
gpiolevel=$3

gpiomon -bF "%e" $gpiochip $gpiopin | while read line
do
	if [ $line = $gpiolevel ]
	then
		echo $line
		exit 0
	fi
done


