#!/usr/bin/python3

import colorsys
from time import sleep

FPS=20
pixels=[(0,0,0)]*24

hue=0
val=0
rev=False


def hsv2rgb(h,s,v):
    return tuple(int(round(i * 255)) for i in colorsys.hsv_to_rgb(h,s,v))

def scale(color,scale):
    return tuple(int(c*scale) for c in color)

def display():
    global pixels
    with open("/tmp/fcfb",'wb') as f:
        for color in pixels:
            f.write(bytes(color))


def brightness(light):

    light = light if 0<=light<=255 else 255

    with open("/tmp/fcbrightness","w") as f:
        f.write(str(light))


def solid(color):
    for i in range(len(pixels)):
        pixels[i]=color

def setup():
    print(pixels)
    pass

def setPix(bar,pix,color):
    global pixels

    if(bar==0):
        pixels[7-pix]=color
    elif(bar==1):
        pixels[8+pix]=color
    elif(bar==2):
        pixels[16+pix]=color
    else:
        pass

def getPix(bar,pix):
    global pixels

    if(bar==0):
        return pixels[7-pix]
    elif(bar==1):
        return pixels[8+pix]
    elif(bar==2):
        return pixels[16+pix]
    else:
        pass
    

def draw():
    global hue,val,rev
    
    newhue=(hue+0.03)%1.0

    if(val<=0):
        rev=False

    if(val>=7):
        rev=True
    
    if(rev):
        val-=1
    else:
        val+=1

    solid((0,0,0))
    color=hsv2rgb(newhue,1,1)

    for bar in range(3):        
        setPix(bar,val,color)         

        if(rev==False):
            #Light up LED before the current led
            for pix in range(max(0,val-2),val):
                setPix(bar,pix,scale(color,abs(pix-val)/7.0))
        else:
            for pix in range(min(7,val+1),min(8,val+3)):
                setPix(bar,pix,scale(color,abs(pix-val)/7.0))                
        
    display()
    hue=newhue%1
    

setup()
while(1):
    draw()
    sleep(1/FPS)
