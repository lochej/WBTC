#include "notifyEvent.h"

static int InFd; // inotify file descruptor
static int Wd; //watch descriptor
FILE *Fcfb = NULL;

int setRight()
{

	//Make sure framebuffer is writable to everyone
    if(chmod(FRAMEBUFFER_Path, 0666)==-1)
    {
        perror("chmod failed ! \n");
        return -1;
    }

    return 0;
}

int initEvent()
{
	//Create frambuffer
    Fcfb = fopen(FRAMEBUFFER_Path, "w");
    fclose(Fcfb);

    if(setRight()==-1)
        return -1;

    InFd = inotify_init();
    if(InFd == -1)
        return -1;

    Wd = inotify_add_watch(InFd, FRAMEBUFFER_Path, IN_CLOSE_WRITE);
    if(Wd == -1)
    {
        perror("Could not add watch on /tmp/lwfb !\n");
        return -1;
    }
    return 0;
}

int waitForEvent()
{
    char buffer[EVENT_SIZE];
    int numRead = read(InFd, buffer, EVENT_SIZE);
    if(numRead != EVENT_SIZE)
    {
        perror("Did not read EVENT_SIZE of bytes !\n");
        return 1;
    }

    return 0;
}
