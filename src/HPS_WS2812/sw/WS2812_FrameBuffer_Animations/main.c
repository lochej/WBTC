/*
 * main.c
 *
 *  Created on: Nov 12, 2019
 *      Author: fpga
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> //usefull uint32_t etc types
#include <unistd.h> //linux headers
#include <stdint.h>
#include <fcntl.h>

#define NUM_LEDS 24
#define WS2812_SCALE8(rgb,scale)  (((((rgb)>>16)&0xFF)*(scale)/0xFF)<<16) | (((((rgb)>>8)&0xFF)*(scale)/0xFF)<<8) | (((((rgb)>>0)&0xFF)*(scale)/0xFF)<<0)
#define FRAMEBUFFER_Path "/tmp/fcfb"

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb_t;

#define SET_RGB32(rgb,color)	do{(rgb).r=(0xFF0000&(color)) >> 16;(rgb).g=(0x00FF00 & (color))>>8;(rgb).b=(0x0000FF&(color));}while(0);

rgb_t leds[NUM_LEDS];

uint8_t power;
uint8_t animation;

FILE *Fcfb = NULL;
//int fd;

void WS2812_SYNC()
{
	//Using file API
	Fcfb = fopen(FRAMEBUFFER_Path, "w");
	fwrite(leds,sizeof(rgb_t),NUM_LEDS,Fcfb);
	fclose(Fcfb);
/*
	//or using OPEN system call instead of fopen
	fd=open(FRAMEBUFFER_Path,O_WRONLY);

	//maybe because write is not bufferised
	write(fd,(uint8_t*)leds,NUM_LEDS*3);

	close(fd);
*/
}

uint8_t barB_id(uint8_t id_bar)
{
	return 7-id_bar;
}

uint8_t barUL_id(uint8_t id_bar)
{
	return 16 + id_bar;
}

uint8_t barUR_id(uint8_t id_bar)
{
	return 8 + id_bar;
}

uint8_t bar_id(uint8_t bar, uint8_t pix)
{
	switch(bar)
	{
	case 0:
		return barB_id(pix);
	case 1:
		return barUL_id(pix);
	case 2:
		return barUR_id(pix);
	}
	return 0;
}
void clearLeds()
{
	for(uint8_t i=0;i<NUM_LEDS;i++)
	{
		SET_RGB32(leds[i],0);
		//leds[i]=0;
		//WS2812_SET_RGB(ws2812_base,i,0);

	}
}

void setBar(uint8_t bar,uint8_t mask,uint32_t color)
{

	for(uint8_t i=0;i<8;i++)
	{
		if(mask & (1<<i))
		{

			SET_RGB32(leds[bar_id(bar,i)],color);
			//leds[bar_id(bar,i)] = color;
			//WS2812_SET_RGB(ws2812_base,bar_id(bar,i),color);
		}
		else
		{
			//leds[bar_id(bar,i)] = 0;
			SET_RGB32(leds[bar_id(bar,i)],0);
			//WS2812_SET_RGB(ws2812_base,bar_id(bar,i),0);
		}

	}
}

void Process_Flux_Capacitor()
{

	static uint8_t id=7;


	id--;
	for(uint8_t bar=0;bar<3;bar++)
	{
		setBar(bar,1<<id,WS2812_SCALE8(0xFFFFFF,power));
	}

	if(id==0)
		id=8;

	WS2812_SYNC();

	usleep(35*1000);

}


void Process_Rainbow()
{
	static uint8_t hue=0;
	static uint32_t raindow[7]={0x9400D3,0x4B0082,0x0000FF,0x00FF00,0xFFFF00,0xFF7F00,0xFF0000};

	uint32_t color;
	for(uint8_t i=0;i<3;i++)
	{
		for(uint8_t j=0;j<8;j++)
		{
			uint8_t id=(hue+j)%7;

			color=WS2812_SCALE8(raindow[id],power);
			//leds[bar_id(i,j)]=color;
			SET_RGB32(leds[bar_id(i,j)],color);
		}

	}

	hue=(hue+1)%7;

	WS2812_SYNC();

	usleep(40*1000);

}

int main(int argc,char **argv)
{
	printf("WS2812 driving starting\n");

	if(argc !=3)
	{
		printf("Error argument : argv[1] = animation argv[2] = intensity");
		return -1;
	}

	power = strtol(argv[2],NULL,10);
	animation = strtol(argv[1],NULL,10);

	printf("Started driving anim:%d power:%d\n",animation,power);
	while(1)
	{
		switch(animation)
		{
		case 0:
			Process_Flux_Capacitor();
			break;
		case 1:
		default:
			Process_Rainbow();
			break;
		}

	}

	return 0;
}
