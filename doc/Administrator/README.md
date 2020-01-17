# WBTC Administrator

This document relates to all the tasks that need to be performed by the system administrator in order to build and deploy the WBTC application.

## Hardware pre-requisites
To install the WBTC application you'll need:

1. DE0-Nano-SOC **board**
2. DE0-Nano-SOC **power brick**
3. Micro SD card **8 GB or more**
4. Mini-USB cable to access the **USB Serial terminal**
5. A building PC environment with either a **native or virtual machine**:  **Linux Xubuntu 18.04 64 bits**

## Operating System build & install
Refer to my other github repository to **build the system image and install it to the micro SD card**:

[**DE0-Nano-SOC Debian 10 Creation**](https://github.com/lochej/DE0_Nano_Soc_Debian10)

Once you have followed this procedure, you should be able to successfully boot the DE0-Nano-SOC board with a fully working and booting base Debian 10 system image.

## Connecting the board to the Internet

For the rest of this reference manual, you'll need internet connection using the **ethernet** port of the board.

## Deploying the programs on the board

Now that you have access to the board using the USB Serial terminal we'll have to deploy all the binaries and programs:

1. **Build Essential:** This is necessary to install the **NodeRED** web services.
2. **Device tree compiler:** This is necessary to **compile the device tree overlay** for the FPGA programming procedure.
3. **NodeRED:** The **full-stack web application** that we use to control the system using the web browser.

```

#Update the repositories
sudo apt-get update

#Install build essential
sudo apt-get install -y build-essential

#Install DTC
sudo apt-get install -y device-tree-compiler

#Install libgpiod and utils
sudo apt-get install -y gpiod libgpiod-dev


#Install NodeRED
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)
```

The rest is already packaged in the Debian 10 distribution. 


## Deploy the program files

Follow this procedure to copy all the necessary files to the **DE0-Nano-SOC** board.

[Project files package creation procedure](../../README.md) -> Gathering all the programs section.

## Configure the system

We need to configure a few things in order to let NodeRED and the rest of the system access a few commands

1. Configure **/tmp** as in the RAM

To mount **/tmp** in the RAM we need to change it to **tmpfs** in the **/etc/fstab** file on the **Debian** system. Simply add the following line to your **/etc/fstab**: 

```
tmpfs /tmp tmpfs defaults,size=32M 0 0
```
or in a terminal run:
```
echo 'tmpfs /tmp tmpfs defaults,size=32M 0 0' >> /etc/fstab
```

Follow the **install_services.sh** procedure to do the following steps:

2. Authorize access to the **date** and **timedatectl** commands with **sudo password and tty**
3. Enable **NodeRED, timecircuitd, fluxcapacitord, ws2812_fpga_daemon, fluxcapacitorFrameBuffer** on boot with systemd
4. Add the **fluxcapacitorctl** and **timecircuitctl** to the **PATH** with the symbolic links
