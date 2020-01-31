# WBTC Administrator

This document relates to all the tasks that need to be performed by the system administrator in order to build and deploy the WBTC application.

## Hardware pre-requisites
To install the WBTC application you'll need:

1. DE0-Nano-SOC **board**
2. DE0-Nano-SOC **power brick**
3. Micro SD card **8 GB or more**
4. Mini-USB cable to access the **USB Serial terminal**
5. A building PC environment with either a **native or virtual machine**:  **Linux Xubuntu 18.04 64 bits**
6. A Time Circuit Display board : if you want to also deploy the hardware
7. A Flux capacitor board : if you want to also deploy the hardware

## Operating System build & install
Refer to my other github repository to **build the system image and install it to the micro SD card**:

[**DE0-Nano-SOC Debian 10 Creation**](https://github.com/lochej/DE0_Nano_Soc_Debian10)

Once you have followed this procedure, you should be able to successfully boot the DE0-Nano-SOC board with a fully working and booting base Debian 10 system image.

## Connecting the board to the Internet

For the rest of this reference manual, you'll need internet connection using the **ethernet** port of the board.

## Deploy the program files and configuring the system

Follow this procedure to copy all the necessary files to the **DE0-Nano-SOC** board.

[**Project files package creation procedure**](../../README.md) -> Gathering all the programs section.


Follow the **install_services.sh** procedure to do the following steps:
1. Mount **/tmp** as **tmpfs**
2. Authorize access to the **date** and **timedatectl** commands with **sudo password and tty**
3. Enable **NodeRED, timecircuitd, fluxcapacitord, ws2812_fpga_daemon, fluxcapacitorFrameBuffer** on boot with systemd
4. Add the **fluxcapacitorctl** and **timecircuitctl** to the **PATH** with the symbolic links


## System wiring

Please refer to the [**Wiring Diagram**](../Specs_and_Design/Wiring_Diagram_WBTC.pdf) to hook up the Time Circuit Display and Flux Capacitor to the DE0-Nano-SOC board.