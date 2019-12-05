#!/usr/bin/python3
# -*- coding: utf-8 -*-

import socket
import sys
from datetime import datetime,timezone
import argparse
import json


#Utility logging functions
def loge(*args, **kwargs):
    """
    logger for errors
    """
    print(*args, file=sys.stderr, **kwargs)


def logd(*args,**kwargs):
    """
    logger for debug messages
    """
    print(*args,**kwargs)

def logi(*args,**kwargs):
    """
    logger for info messages
    """
    print(*args,**kwargs)


#Address for the IPC link, Unix Socket
server_address = '/tmp/fluxcapacitor_ipc_socket'

simpleactions=["start","stop","status"]
requestactions=["get"]
applyactions=["set"]

#Parse program arguments
argv=sys.argv
argc=len(argv)

message={}

if(argc == 1): #No argument passed, return if daemon running ?
    loge("No args")
    exit(1)
elif(argc==2): # One argument passed, must be one of the standard actions
    logd("simple action =",argv[1])
    if(argv[1] not in simpleactions):
        loge("action",argv[1],"not recognized")
        exit(1)

    message={"cmd":argv[1]}

elif(argc==3): # 2 argument, must be get command

    logd("request action =",argv[1])

    if(argv[1] not in requestactions):
        loge("request action",argv[1],"not recognized")
        exit(1)

    message={"cmd":argv[1],"field":argv[2]}

elif(argc==4): # 4 arguments, must be set command

    logd("apply action =",argv[1])
    if(argv[1] not in applyactions): 
        loge("apply action",argv[1],"not recognized")
        exit(1)

    message={"cmd":argv[1],"field":argv[2],"value":argv[3]}

logd("Ready to send")
logd(message)


# Create a UDS socket
#In TCP mode
def tcp_mode():
    global message,server_address

    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    try:
        sock.connect(server_address)
        logd("connected")
        #timestr=str(datetime.utcnow().timestamp()).encode("utf-8")
        #sock.send(timestr)
        
        
        sock.send(json.dumps(message).encode("utf-8"))
        
        recv_message = sock.recv(60)

        res = json.loads(recv_message.decode("utf-8"))
        logd("Received %s"%recv_message)
        logd(res)

        if(res["errorcode"] != 0):
            exit(2)
        elif((message["cmd"]=="status") and (res["msg"]=="running" or res["msg"]=="standby")):
            exit(0)

        
    except socket.error as msg:
        loge(msg)
        sys.exit(1)

def udp_mode():
    global message,server_address
    #Create UDS socket
    #In UDP mode
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)

    # Connect the socket to the port where the server is listening
    logd('connecting to %s' % server_address)

    sock.bind("")

    try:

        sock.sendto(json.dumps(message).encode("utf-8"),server_address)
        
        recv_message,address = sock.recvfrom(60)

        res = json.loads(recv_message.decode("utf-8"))
        logd("Received %s from %s"%(recv_message,address))
        logd(res)

        if(res["errorcode"] != 0):
            exit(2)
        elif((message["cmd"]=="status") and (res["msg"]=="running" or res["msg"]=="standby")):
            exit(0)
    except socket.error as msg:
        loge(msg)
        sys.exit(1)


if __name__=="__main__" :
    udp_mode()