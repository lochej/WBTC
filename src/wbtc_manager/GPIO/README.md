# Using the GPIOs on the SocFPGA system

From Linux 4.x we have a new API for using GPIOs. 
We no longer use the legacy /sys/class/gpio sysfs.

Instead we use the new **libgpiod**.

## Install libgpiod


Install the gpiod utilities and libgpiod

```
sudo apt install -y gpiod libgpiod-dev
```

Library usage usefull websites:

[ST Micro Electronics MCU](https://wiki.st.com/stm32mpu/index.php/How_to_control_a_GPIO_in_userspace)

[Libgpiod examples](https://www.blaess.fr/christophe/2018/10/15/pilotage-de-gpio-avec-lapi-libgpiod-partie-1/)


List available GPIO devices
```
debian@arm:~$ gpiodetect
gpiochip0 [ff708000.gpio] (29 lines)
gpiochip1 [ff709000.gpio] (29 lines)
gpiochip2 [ff70a000.gpio] (27 lines)
gpiochip3 [/soc/base-fpga-region/gpio@420] (8 lines)
gpiochip4 [/soc/base-fpga-region/gpio@410] (4 lines)
gpiochip5 [/soc/base-fpga-region/gpio@400] (2 lines)
```

Reading GPIO lines
```
#Read the pushbutton hooked up on gpiochip5[1], pulled up when not pressed
$ gpioget gpiochip5 1
1

#Press the button gpiochip5[1] goes low, read the value
$ gpioget gpiochip5 1
0
```

Setting GPIO lines
```
#Set set 8 GPIOs on gpiochip3 to 1 'on'
$ for i in {0..7} ; do gpioset gpiochip3 $i=1; done
#Set set 8 GPIOs on gpiochip3 to 0 'off'
$ for i in {0..7} ; do gpioset gpiochip3 $i=0; done

#Or in one command
$ gpioset gpiochip3 0=0 1=0 2=0 3=0 4=0 5=0 6=0 7=0

#You can also just give the gpiochip number: set 'on' gpio 3[0]
$ gpioset 3 0=1
```

Monitor falling edge or rising edge on a GPIO
```
# Monitor gpiochip5 line 0 for any edge interrupt
$ gpiomon  -rf gpiochip5 0
# Appuis sur le bouton 
event: FALLING EDGE offset: 23 timestamp: [1538116251.893018202]
```

A usefull script is a GPIO non blocking wait for an event

[**gpio_wait_for_event.sh**](gpio_wait_for_event.sh)

**How to use it:**
```
#Non blocking wait for rising edge on gpiochip5[0]
./gpio_wait_for_event.sh 5 0 1 

#Non blocking wait for falling edge on gpiochip5[1]
./gpio_wait_for_event.sh 5 1 0 
```

Of course more details are available in the tutorials.