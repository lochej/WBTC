# ARM DS-5 Project
Compiled with:
**GCC ARM 4.x -> arm-linux-gnueabihf**
When started, the process creates a **framebuffer** file (i.e. /tmp/fcfb).

## Mount /tmp as tmpfs
Make sure to have /tmp mounted as tmpfs (in RAM) to prevent too much disk write to occur when writing to the framebuffer.
```
echo "tmpfs /tmp tmpfs defaults,size=32M 0 0" >> /etc/fstab
```

## How does it work ?
Each animation app that wants to control the LEDs will just write the LED colors bytes in this /tmp/fcfb file.

The sequence for setting the colors of the LEDs is the following:
1. **Open** the /tmp/fcfb file
2. **Write** the colors in /tmp/fcfb
3. **Close** the /tmp/fcfb

The format of writing is the following:
**r0g0b0r1g1b1r2g2b2....rigibi**

Where:
1. r0 is the red byte for led 0 in the chain (0 to 0xFF)
1. g0 is the green byte for led 0 in the chain (0 to 0xFF)
1. b0 is the blue byte for led 0 in the chain (0 to 0xFF)

And you follow with the other LED colors.

The framebuffer process will check for **close** events on the file.

On close, the framebuffer process will wake up, check if the files contains 3xNUM_LEDS bytes. 
You have to write this exact number of bytes for it to work.

This program is compiled for 24 LEDs only, so it expects 72 bytes.

Example:
```
#Set all the 24 LEDs OFF by writing all of them to 0
dd if=/dev/null of=/tmp/fcfb bs=1 count=72

#Apply random color the LEDs
dd if=/dev/random of=/tmp/fcfb bs=1 count=72
```

## Additionnal feature
This driver adds another file, **/tmp/fcbrightness**.

What this does is set the global brightness of the whole LED strip.

It simply scales the LED values with the given brightness value.

This takes 3 ASCII bytes as input. 

1. "0" is brightness OFF 
2. "255" is MAX brightness

Example:
```
#Set brightness to 100%
echo 255 > /tmp/fcbrightness

#Set brightness to 50%
echo 128 > /tmp/fcbrightness

#Set the LEDs Off by modifying brightness
echo 0 > /tmp/fcbrightness
```



