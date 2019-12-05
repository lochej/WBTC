#!/usr/bin/python3
# -*- coding: utf-8 -*-

import socket
import sys
import os
import json
from subprocess import Popen
from datetime import datetime,timezone
import enum

#Program Class
class ProcessStatus(enum.IntEnum):
    RUNNING=0
    CRASHED=1
    NOT_STARTED=2
    STANDBY=3
    UNKNOWN=4

#Program constants

#Address for the IPC link, Unix Socket
server_address = '/tmp/fluxcapacitor_ipc_socket'

default_conf={
    "brightness": "50",
    "power":"on",
    "animation":"0",
}

conf_filepath="fluxcapacitor.conf"

#Program variables
process_conf= default_conf
print(process_conf)

process=None

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

def write_brightness(value=None):
    """
    Write brightness value to fc brightness conf
    """
    logd("write_brightness()")
    if(value==None):
        value=range_map_limit(int(process_conf["brightness"]),0,100,0,255)
    
    logd("write_brightness value:",value)
    with open("/tmp/fcbrightness","w") as f:
        f.write(str(value))
    

def save_config():
    """
    save the config json to file
    """
    global process_conf,conf_filepath

    logd("save_config()")

    with open(conf_filepath, 'w') as conf_file:
        json.dump(process_conf, conf_file)

    pass

def load_config():
    """
    load the config json from file
    """
    global process_conf,conf_filepath

    try:
        with open(conf_filepath, 'r') as conf_file:
            try:
                
                #Try to parse the conf data
                logd("load_config(): loading config")
                process_conf=json.load(conf_file)
                logd("load_config():config loaded")

                #end here
                return

            except json.JSONDecodeError as e:
                #In case of failure, load the default conf
                loge("load_config() : couldn't parse json conf : ",e)
                #Jump to loading default config

    except FileNotFoundError as e:
        loge("load_config(): couldn't read conf, doesn't exist :",e)
        #Conf file doesn't exist, jump to loading default config
        
    #Load default config
    process_conf=default_conf
    #Save the conf file
    loge("load_config(): Trying to create conf file with default conf")
    save_config()

def get_process_status():

    #Process is None so never started
    if(process == None):
        logd("process_status: not started")
        return ProcessStatus.NOT_STARTED  #"not started"
    
    #Check for process state
    rescode=process.poll()

    #Process still running
    if(rescode == None): #Process still running
        logd("process_status: running")
        return ProcessStatus.RUNNING #"running"
    
    elif(rescode == 0): #Process is not running but exited gracefully, just standby
        logd("process_status: standby")
        return ProcessStatus.STANDBY #"standby"
    else:
        loge("process_status: crashed")
        return ProcessStatus.CRASHED #"crashed" 
    
    return ProcessStatus.UNKNOWN #"unknown"

def range_map(x,in_min,in_max,out_min,out_max):
    return round((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

def range_map_limit(x,in_min,in_max,out_min,out_max):
    mapped_value=range_map(x,in_min,in_max,out_min,out_max)
    
    if(mapped_value<out_min):
        return out_min
    elif(mapped_value>out_max):
        return out_max
    
    return mapped_value

def get_process_args():
    """
    create the launching command of the time circuit program
    """
    global process_conf
    #process_args=["./TimeCircuitDisplay","15"]
    #process_args=["python3","TimeCircuitDisplay.py","15"]

    process_args = ["./fpga_ws2812/WS2812_FrameBuffer_Animations","0","255"]
    arganim=1
    argbrightness=2

    if(process_conf["power"] == "on"):
        process_args[argbrightness]="255"
        process_args[arganim]=str(process_conf["animation"])
    else:
        process_args[argbrightness]="0"
        


    return process_args
  


def start_command(args=None):
    """
    Start the driver process
    """
    global process
    
    logd("start_command()")

    
    if(process==None):
        #start the process
        
        logd("start_command(): Process is None, starting")
        pass

    elif(process.poll() == None):
        #process still running

        logd("start_command(): Process still running")

        return 1
    
    try:
        args=get_process_args()

        process=Popen(args)
    
        logd("program started",args)
    except OSError as e:
        loge("start_command() OSError:",e)
        return 2
        
    return 0


def stop_command(args=None):
    """
    Stop the driver process of the TimeCircuitDisplay
    """
    logd("stop_command()")
    if(process == None):
        logd("stop_command(): Process is None, nothing to stop")
        return -1
    
    logd("stop_command(): Killing process")
    process.kill()
    
    logd("stop_command(): Sent SIGKILL signal")
    process.wait()

    logd("stop_comment(): Process killed")
    return 0

def status_command(args=None):
    """
    Get the driver process status of the TimeCircuitDisplay
    """
    logd("status_command()")

    status=get_process_status()
    
    return status

def reload_command(args=None):
    """
    """
    logd("reload_command()")

    
    stop_command()
    return start_command()
    

def set_command(args):
    """
    """
    field,value=args

    logd("set_command(): ",field,value)

    #Check if field is in configuration
    try:
        if(process_conf[field] is None):
            pass
        
    except KeyError as e:
        loge("set_command(): no ",field,"in config")
        loge("set_command() error:",e)
        return 1

    process_conf[field]=value

    logd("set_command(): updated conf = ",process_conf)
    
    #Save the set config
    save_config()

    #Need to restart the executable if anim or power changes
    #Off
    if(process_conf["power"] != "on"):

        write_brightness(0)
        return stop_command()
    #On
    else:
        
        write_brightness()
        #just applied the new brightness
        if(field=="brightness"):
            return 0

        #for every thing else, reload

    return reload_command()

def get_command(args=None):
    """
    returns a process_conf field if exists else None
    """

    field=args
    #Check if field is in configuration
    try:
        if(process_conf[field] is None):
            pass
        
    except KeyError as e:
        loge("get_command(): no ",field,"in config")
        loge("get_command() error:",e)
        return None #if field do
    
    return process_conf[field]

def parse_command(recv_data):
    """
    Normalise the data given by 
    """
    logd("parse_command(): data",recv_data)
    
    try:
        data = json.loads(recv_data.decode('utf-8'))
    except json.JSONDecodeError as e:
        loge("parse_command(): JSONDedodeError:",e)
        return None


    command=data.get("cmd")
    
    
    if(command=="start"):
        return start_command,None
    
    if(command=="stop"):
        return stop_command,None

    if(command=="status"):
        return status_command,None
    
    if(command=="reload"):
        return reload_command,None

    if(command=="set"):
        return set_command,(data.get("field"),data.get("value"))
    if(command=="get"):
        return get_command,(data.get("field"))

    loge("parse_command(): unknown command in JSON:", data)

    return None

def exec_command(command,arguments):
    logd("exec_command()")
    resvalue=command(arguments)
    '''
    try:
        
    except CommandError as e:
        loge("exec_command():",e)'''

    #handle set command result
    if(command==set_command):
        if(resvalue):
            return {"errorcode":1,"msg":"unknown field"}
        else:
            return {"errorcode":0,"msg":"ok"}

    #handle status_command result
    if(command==status_command):

        if(resvalue==ProcessStatus.RUNNING):
            return {"errorcode":0,"msg":"running"}
        elif(resvalue==ProcessStatus.STANDBY):
            return {"errorcode":0,"msg":"standby"}
        elif(resvalue==ProcessStatus.CRASHED):
            return {"errorcode": 1, "msg":"crashed"}
        elif(resvalue==ProcessStatus.NOT_STARTED):
            return {"errorcode":2,"msg":"not started"}
        else:
            return {"errorcode": 3, "msg":"unknown"}


    #Handled get_command result
    if(command==get_command):
        
        if(resvalue==None): #returned field value doesn't exist
            return {"errorcode":1,"msg":"unknown field"}    
                    
        return {"errorcode":0,"field":arguments,"value":resvalue}

    return {"errorcode":resvalue, "msg":"ok"}


def udp_mode():
    # Create a UDS socket
    #sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)

    # Bind the socket to the port
    logi('Daemon starting up on %s' % server_address)
    sock.bind(server_address)

    while True:
        # Wait for a message
        logi('Waiting for a message')

        try:
            data,client_address = sock.recvfrom(60)
            
            if data:
                
                logi("Message from client:",data)
                #parse the command
                cmd,args=parse_command(data)
    
                if(cmd==None):
                    
                    msg={"errorcode":"1", "error":"unknown command"}
                    loge("sending error to client",msg)

                    sock.sendto(json.dumps(msg).encode("utf-8"),client_address)
                else:
                    
                    #Donner la commande a un autre thread ?
                    msg=exec_command(cmd,args)

                    #msg={"errorcode":"0", "error":"ok"}
                    logi("Response sent to client:",msg)
                    sock.sendto(json.dumps(msg).encode("utf-8"),client_address)                        
            else:
                loge("Connection closed from", client_address)
                break
        except socket.error as error:
            loge("socket error:",error)

def tcp_mode():
    global server_address
    # Create a UDS socket
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    
    # Bind the socket to the port
    logi('Daemon starting up on %s' % server_address)
    sock.bind(server_address)

    #listen to 1 address client
    sock.listen(1)

    while True:
        # Wait for a connection
        logi('Waiting for a connection')
        connection, client_address = sock.accept()
        try:
            logi('Connection from', client_address)

            # Receive the data in small chunks and retransmit it
            while True:

                try:
                    data = connection.recv(60)
                    
                    if data:
                        
                        logi("Message from client:",data)
                        #parse the command
                        cmd,args=parse_command(data)
                        #cmd=None

                        #timecur=datetime.utcnow()
                        #time = datetime.fromtimestamp(float(data))
                        #print('send/recv time take')
                        #print(timecur-time)
                        if(cmd==None):
                            
                            msg={"errorcode":"1", "error":"unknown command"}
                            loge("sending error to client",msg)

                            connection.send(json.dumps(msg).encode("utf-8"))
                        else:
                            
                            #Donner la commande a un autre thread ?
                            msg=exec_command(cmd,args)

                            #msg={"errorcode":"0", "error":"ok"}
                            logi("Response sent to client:",msg)
                            connection.send(json.dumps(msg).encode("utf-8"))                        
                    else:
                        loge("Connection closed from", client_address)
                        break
                except socket.error as error:
                    loge("socket error:",error)
                                
        finally:
            # Clean up the connection
            connection.close()


if __name__=="__main__":

    # Make sure the socket does not already exist
    try:
        os.unlink(server_address)
    except OSError:
        if os.path.exists(server_address):
            raise

    #Load the config file
    load_config()

    write_brightness()

    #Start the display
    start_command()

    udp_mode()
