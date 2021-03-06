#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> //usefull uint32_t etc types
#include <unistd.h> //linux headers
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h> //usleep and sleep
#include "notifyEvent.h"

//Specific Header for Cyclone 5 SOC
#define soc_cv_av

#include "hwlib.h"
#include "soc_cv_av/socal/socal.h" //alt read and write functions
#include "soc_cv_av/socal/hps.h"
#include "hps_soc_system.h"

#include "ws2812_driver.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

#define NUM_LEDS 24

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb_t;

rgb_t FrameBuffer[NUM_LEDS];



void *ws2812_base;



/**
 * Mapping of the address maps to drive the WS2812 driver
 */
int initFluxCapacitor()
{
	int fd;
	void *virtual_base;
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
	return 0;
}

//read the frame from the framebuffer
int readFrameBuffer()
{

	int numberLedRead;

	Fcfb = fopen(FRAMEBUFFER_Path, "r");
	if(!Fcfb)
	{
		fprintf(stderr, "Could not open the framebuffer file for reading !\n");
		//fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return 1;
	}

	numberLedRead = fread(&FrameBuffer, sizeof(rgb_t), NUM_LEDS, Fcfb);
	if(numberLedRead != NUM_LEDS)
	{
		fprintf(stderr, "Did not read the right number of LEDs !\n%d LEDs were read instead of %d !\n", numberLedRead,NUM_LEDS);
		//fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return 2;
	}
	fclose(Fcfb);


	return 0;
}

int renderFrameBuffer()
{

	while(!WS2812_IDLE(ws2812_base));

	for(uint8_t i=0;i<NUM_LEDS;i++)
		WS2812_SET_DATA(ws2812_base,i,(FrameBuffer[i].r <<8) | (FrameBuffer[i].g <<16) | FrameBuffer[i].b);

	WS2812_SYNC(ws2812_base);

	return 0;
}

int main(int argc, char ** argv)
{
	int ret;

	if(initEvent())
		return 1;

	if(initFluxCapacitor())
		return 2;

	while(1)
	{

		//Wait for the file to be closed by the user
		if(waitForEvent()!=0)
			return 3;

		ret = readFrameBuffer();
		if(ret==2)
			continue;
		else if(ret==1)
			return 4;

/*
		for(int i=0;i<NUM_LEDS;i++)
        {
            printf("%d={%02X,%02X,%02X}\n",i, FrameBuffer[i].r, FrameBuffer[i].g, FrameBuffer[i].b);

        }
*/
		/* Render frame*/
		renderFrameBuffer();


	}
	return 0;
}
