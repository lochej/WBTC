#include "notifyEvent.h"

static int Inotifyd; // inotify file descriptors


static int WdFrameBuffer; //watch descriptor
static int WdBrightness;

FILE *Fcfb = NULL;
FILE *Fcbrightness=NULL;

int setRight()
{

	//Make sure framebuffer is writable to everyone
	if(chmod(FRAMEBUFFER_Path, 0666)==-1)
	{
		perror("chmod failed ! \n");
		return -1;
	}

	//Make sure framebuffer is writable to everyone
	if(chmod(BRIGHTNESS_Path, 0666)==-1)
	{
		perror("chmod failed ! \n");
		return -1;
	}

	return 0;
}

int initEvent()
{
	//Create framebuffer
	Fcfb = fopen(FRAMEBUFFER_Path, "w");
	fclose(Fcfb);

	//Create brightness File
	Fcbrightness = fopen(BRIGHTNESS_Path,"w");

	fclose(Fcbrightness);

	if(setRight()==-1)
		return -1;

	Inotifyd = inotify_init();
	if(Inotifyd == -1)
		return -1;

	WdFrameBuffer = inotify_add_watch(Inotifyd, FRAMEBUFFER_Path, IN_CLOSE_WRITE);
	if(WdFrameBuffer == -1)
	{
		perror("Could not add watch on /tmp/fcfb !\n");
		return -1;
	}

	WdBrightness = inotify_add_watch(Inotifyd, BRIGHTNESS_Path, IN_CLOSE_WRITE);
	if(WdBrightness == -1)
	{
		perror("Could not add watch on /tmp/fcbrightness !\n");
		return -1;
	}

	printf("WdFrameBuffer %d\n",WdFrameBuffer);
	printf("WdBrightness %d\n",WdBrightness);

	return 0;
}

int waitForEvent()
{

	//Select on Inotify Framebuffer and Inotify Brightness
	char buffer[EVENT_BUF_LEN];
	int res=0;
	int i=0;

	int length = read(Inotifyd, buffer, EVENT_BUF_LEN);

	if(length < 0)
	{
		perror("Error in read inotify !\n");
		return EVENT_ERROR;
	}

	/*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
	while ( i < length ) {

		struct inotify_event *event = ( struct inotify_event * ) (&buffer[ i ]);

		if(event->wd == WdBrightness)
		{
			//New event on file brightness
			res |=EVENT_NEW_BRIGHTNESS;
		}

		if(event->wd == WdFrameBuffer)
		{
			//New event on file framebuffer
			res |=EVENT_NEW_FRAME;
		}

		i += EVENT_SIZE + event->len;
	}


	return res;
}
