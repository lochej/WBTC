#!/bin/bash


#Wait for falling edge on gpiochip 5 button 0 KEY2
./gpio_wait_for_event.sh 1 25 0 && poweroff