# Quartus Prime Lite 18.1 Project

This is the Quartus project for generating FPGA bitstream to control the WS2812 on the FPGA.

This is based on my template project for **DE0-Nano-Soc**: 

[Check out my repository and **tutorial** here for more info.](https://github.com/lochej/DE0_HPS_Example)

The goal is to generate the **fpga_firmware.rbf** needed to instanciate a WS2812 Avalon driver on the FPGA and access it from the HPS.

You then have 4 ARM-DS5 projects associated with the Quartus Hardware files.

[Check out the **src** folder](./sw)

You have 2 strategies to drive the WS2812 LEDs. 

Either drive them directly using the WS2812 driver by accessing the WS2812 Avalon driver through FPGA_brigde:

[Example code here](./sw/HPS_WS2812_Control)

Or use a Framebuffer and a daemon to do driver access for you. You will just write to the framebuffer and the daemon will do the rest.

1. [Version without brightness control](./sw/HPS_WS2812_FrameBuffer)

2. [Version with brightness control](/sw/HPS_WS2812_FrameBuffer_Brightness)
