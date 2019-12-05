# Web Based Time Converter Manager software

This program aims to create an interface for the settings of the WBTC.

Here are the following settings you can set/get:

1. *Time Circuit Display*
    + Power : **set, get**
        + ON
        + OFF 
    + Brightness : **set, get**
        + **0** to **100** percents
    + Langages options : **list, set, get**
        + FR
        + EN
    + Hour mode : **12h + AM/PM** or **24h**
    + Status : **get**
        + OK
        + NOK
2. *Flux Capacitor*
    + Power : **set, get**
        + ON
        + OFF
    + Brightness : **set, get**
        + **0** to **100** percents
    + Status : **get**
        + OK
        + NOK

3. *Time synchronisation*
    + NTP enabled status : **set, get**
        + YES
        + NO
    + NTP synchronized status : **get**
        + YES
        + NO
    + Timezone setting : **list, set, get**
    + Time setting : **set, get**

## Node red

```
sudo apt install build-essential
sudo apt install curl
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)

```

## Time Circuit Control

The time circuit control consists of 3 software.

1. TimeCircuitDisplay program actually driving the I2C time circuit
2. timecircuitd a daemon program :
    1. Monitors the TimeCircuitDisplay process
    2. Is responsible for applying settings to the TimeCircuitDisplay process
    3. Hosts a server on Unix Socket to receive commands to reconfigure or control the TimeCircuitDisplay process
3. timecircuitctl, a command line client for timecircuitd :
    1. Passes command to timecircuitd 
    2. able to tell if timecircuitd is running
    3. checks if TimeCircuitDisplay process is running
    4. reads the timecircuitd configuration
    5. sets the timecircuitd configuration