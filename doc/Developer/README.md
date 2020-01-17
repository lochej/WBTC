# WBTC Developer

If you want to develop software for the WBTC, you are going to need the following tools:

1. Building environment: machine with Xubuntu 18.04 native of virtual machine 
2. DE0-Nano-SOC **board**
3. DE0-Nano-SOC **power brick**

## Development Environment installation

You should follow the attached procedure to install that I have written in the following file:

[**Quartus and SocEDS Install Procedure**](INSTALL.md)

Additionnaly to this install procedure, you can install a distribution of **Python 3.7** or newer to develop additionnal software (use the ```sudo apt install python3 python3-pip```). But that should already be included your Xubuntu distribution.

Once your Xubuntu distribution is set up with your Quartus and SocEDS install ready, you'll be able to:

1. **Develop Verilog HDL** files to synthesize in the FPGA part of the DE0-Nano board.
2. **Build the system configuration** to use the **HPS** system in combination with the **FPGA**
3. Deploy IP cores interfaced on the **Intel FPGA Avalon Bus** and generate the device tree overlay for your Linux installation.
4. Develop, compile and test, and run you C compiled programs or Python 3 programs on the board.


## Developing and compiling tools for the FPGAs

You will have to use Quartus Prime on your developing environment to develop HDL IP Cores and create an fpga bitstream.

There are 2 ways of developing on the DE0-Nano-SOC target. You can either build a full FPGA image and use it on its own with the JTAG connection.

Or you can create an image that will be loaded by the Linux Kernel on your board.

### **FPGA Only development**

You don't need any OS running on the board to only use the FPGA part. This is very useful if you just want to get around with the board and tinker with the LEDs and push-buttons. Clearly, that was for me the way to test my pure HDL IP Cores and it works just fine !

### **HPS / FPGA with Linux development**

I won't cover how to run bare-metal code on the HPS part of the FPGA but you have to know that it is clearly feasible.

On the other hand, it is very useful to run a Linux distribution on the HPS part of the processor and just use the FPGA as an extension for custom peripherals or additionnal hardware acceleration.

If you want to be running the same distribution as I did when I developed, I sugget you have a look at my **Debian 10** image creation procedure in order to create your very own booting image for the **DE0-Nano-SOC board**.

Refer to the sections only talking about deploying the system and not the specific apps for the WBTC.

[**Debian 10 Linux system creation for DE0-Nano-SOC**](../Administrator/README.md)

For this, I made a **very nice toturial** and **template project** that you can use to start developing for the DE0-Nano-SOC target in combination with the HPS/FPGA communication.

[**DE0-Nano-SOC HPS Example project**](https://github.com/lochej/DE0_HPS_Example)

Carefully read the PDF documentation and you'll be able to run your first HPS / FPGA program in just a few hours (beginners).

You now have all the base information needed to start a new Quartus Prime project and develop you own design architecture for your HPS/FPGA implementation.