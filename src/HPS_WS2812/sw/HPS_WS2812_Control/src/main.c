/*
 * mainc.c
 *
 *  Created on: Nov 7, 2019
 *      Author: LOCHE Jeremy
 */
#include <stdio.h> //std lib
#include <stdlib.h>
#include <stdint.h> //usefull uint32_t etc types
#include <unistd.h> //linux headers
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h> //usleep and sleep

//Specific Header for Cyclone 5 SOC
#define soc_cv_av

#include "hwlib.h"
#include "soc_cv_av/socal/socal.h" //alt read and write functions
#include "soc_cv_av/socal/hps.h"
#include "hps_soc_system.h"

#include "ws2812_driver.h"


/**
 * Memory map registers
 */
#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )


#define NUM_LEDS 24

void *ws2812_base;
uint8_t power=0x10;
uint8_t animation=0;

uint32_t raindow[7]={0x9400D3,0x4B0082,0x0000FF,0x00FF00,0xFFFF00,0xFF7F00,0xFF0000};

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
		//leds[i]=0;
		WS2812_SET_RGB(ws2812_base,i,0);
	}
}

void setBar(uint8_t bar,uint8_t mask,uint32_t color)
{

	for(uint8_t i=0;i<8;i++)
	{
		if(mask & (1<<i))
		{
			//leds[bar_id(bar,i)]=color;
			WS2812_SET_RGB(ws2812_base,bar_id(bar,i),color);
		}
		else
		{
			//leds[bar_id(bar,i)]=0;
			WS2812_SET_RGB(ws2812_base,bar_id(bar,i),0);
		}

	}
}

void Process_Flux_Capacitor()
{

	static uint8_t id=7;

	while(!WS2812_IDLE(ws2812_base));

	id--;
	for(uint8_t bar=0;bar<3;bar++)
	{
		setBar(bar,1<<id,WS2812_SCALE8(0xFFFFFF,power));
	}

	if(id==0)
		id=8;

	WS2812_SYNC(ws2812_base);

	usleep(45*1000);

}

void Process_Rainbow()
{
	static uint8_t hue=0;

	while(!WS2812_IDLE(ws2812_base));

	for(uint8_t i=0;i<3;i++)
	{
		for(uint8_t j=0;j<8;j++)
		{
			uint8_t id=(hue+j)%7;

			//IOWR(ws2812_addr,3+bar_id(i,j),WS2812_SCALE8(WS2812_RGB_TO_GRB(raindow[id]),power));

			WS2812_SET_RGB(ws2812_base,bar_id(i,j),WS2812_SCALE8(raindow[id],power));
			//leds[bar_id(i,j)]=WS2812_SCALE8(WS2812_RGB_TO_GRB(raindow[id]),0x08);
		}

	}

	hue=(hue+1)%7;

	//ws2812_sync();
	//IOWR(ws2812_addr,1,0x3); //send SYNC
	WS2812_SYNC(ws2812_base);

	//Systick_delayms(50);
	usleep(60*1000);

}

int main(int argc,char **argv)
{
	printf("WS2812 driving starting\n");

	void *virtual_base;
	int fd;

	if(argc !=3)
	{
		printf("Error argument : argv[1] = animation argv[2] = intensity");
		return -1;
	}

	power = strtol(argv[2],NULL,16);
	animation = strtol(argv[1],NULL,10);

	// map the address space for the LED registers into user space so we can interact with them.
	// we'll actually map in the entire CSR span of the HPS since we want to access various registers within that span

	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	ws2812_base= virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + WS2812_DRIVER_0_BASE ) & ( unsigned long)( HW_REGS_MASK ) );

	WS2812_RESET(ws2812_base);

	WS2812_SET_LEDNUMBER(ws2812_base,NUM_LEDS);
	printf("LED number = %d\n",WS2812_GET_LEDNUMBER(ws2812_base));

	WS2812_SET_RGB(ws2812_base,0,0xA0A0A0);

	printf("RGB read = %06X\n",WS2812_GET_RGB(ws2812_base,0));

	printf("Set led number OK\n");

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


