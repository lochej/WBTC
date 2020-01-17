*This document was written by LOCHE Jérémy and aims at installing Quartus FPGA development suite on Ubuntu Linux*

# Install the ARM-Toolchain DS and Quartus Prime Lite 18.1 on Ubuntu


## Choosing the distro

You should start by installing **Xubuntu 18.04.2 x64** in a VM or host machine.
Make sure to install it with **en_US.UTF_8** locale as Quartus will need it.

Checkout this link to DL an image :
http://ftp.free.fr/mirrors/ftp.xubuntu.com/releases/18.04/release/

## Downloading the source files: Quartus 18.1 lite

Once you have installed Xubuntu, create root password, create the **fpga** user we will need them for later.

Quartus Lite Setup:
http://download.altera.com/akdlm/software/acdsinst/18.1std/625/ib_installers/QuartusLiteSetup-18.1.0.625-linux.run

FPGA Cyclone 5 files:
http://download.altera.com/akdlm/software/acdsinst/18.1std/625/ib_installers/cyclonev-18.1.0.625.qdz

ModelSim Ase:
http://download.altera.com/akdlm/software/acdsinst/18.1std/625/ib_installers/ModelSimSetup-18.1.0.625-linux.run

Soc EDS for ARM HPS cross compiling:
http://download.altera.com/akdlm/software/acdsinst/18.1std/625/ib_installers/SoCEDSSetup-18.1.0.625-linux.run

Put all the installation files into :
```
/home/fpga/intelFPGA/
```

Create a new folder for the actual installation :
```
/home/fpga/intelFPGA_lite
```

Now we will install all the dependencies before running quartus install.

## Installing the dependencies

With **apt install** command as **root**, install the following libraries.
```
sudo dpkg --add-architecture i386
sudo apt update
sudo apt install lib32z1 lib32stdc++6 lib32gcc1 gcc-multilib g++-multilib
sudo apt install libx11-6:i386 libxext6:i386 libxft2:i386 libncurses5:i386 libstdc++6:i386

libc6:i386
libncurses5:i386
libxtst6:i386
libxft2:i386
libstdc++6:i386
libc6-dev-i386
libxft2
lib32z1
lib32ncurses5
lib32bz2-1.0 
```

**Note:**

    For Ubuntu 18.04, additionally install libqt5xml5 and liblzma-dev libraries.
    For Ubuntu 18, libpng12 library is no longer available via the apt-get method. Download and install the libpng12 library from the following location:

Lib PNG 12 deb package for Ubuntu
https://packages.ubuntu.com/xenial/amd64/libpng12-0/download

Once downloaded, install the deb package using **dpkg** as **root**
```
dpkg -i libpng12.xx.deb
```

With all of this you should be fine, but beware of installing any other libraries asked by Quartus or else.

Now you need to add Freetype librarie:

```
# Retrieve Freetype
cd ~/intelFPGA_lite
wget http://download.savannah.gnu.org/releases/freetype/freetype-2.4.12.tar.bz2
tar xjvf freetype-2.4.12.tar.bz2
cd freetype-2.4.12

# Compilation
./configure --build=i686-pc-linux-gnu "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32"
make -j4
```

## Installing Quartus

Go to **/home/fpga/intelFPGA** directory.

First run the QuartusSetupLite **.run** file.
Make sure to install Quartus with ModelSim and Cyclone 5 support in the setup.

Make sure to select **/home/fpga/intelFPGA_lite/18.1/**.

At the end of the install you should have **/home/fpga/intelFPGA_lite/18.1/quartus**.

To make sure that Quartus and all the binaries are available in the PATH. Make sure the following lines are in you **/home/fpga/.profile**

```
export PATH=$PATH:/home/fpga/intelFPGA_lite/18.1/quartus/bin/
export QSYS_ROOTDIR="/home/fpga/intelFPGA_lite/18.1/quartus/sopc_builder/bin"
```

Now install SoCEDS by running the **SocEDS.run** file under **/home/fpga/intelFPGA_lite/18.1**.

To install ARM DS-5 eclipse environnment (optionnal) go to **/home/fpga/intelFPGA_lite/embedded** and run the **install.sh** file as **root** in a terminal.

Install it as default.


## Sources

1. https://www.intel.fr/content/www/fr/fr/programmable/documentation/esc1425946071433.html
2. https://github.com/Zigazou/DE0-nano-soc-francais
3. https://github.com/CTSRD-CHERI/quartus-install/blob/master/quartus-install.py

