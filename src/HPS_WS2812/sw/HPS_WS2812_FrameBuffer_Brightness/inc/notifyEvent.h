#ifndef NOTIFYEVENT_H_INCLUDED
#define NOTIFYEVENT_H_INCLUDED

#include <unistd.h>
#include <stdio.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN (2*EVENT_SIZE)
#define FRAMEBUFFER_Path "/tmp/fcfb"
#define BRIGHTNESS_Path "/tmp/fcbrightness"



#define EVENT_NEW_FRAME 1
#define EVENT_NEW_BRIGHTNESS 2
#define EVENT_ERROR -1


extern FILE *Fcfb;
extern FILE *Fcbrightness;

int initEvent();
int waitForEvent();


#endif // NOTIFYEVENT_H_INCLUD
