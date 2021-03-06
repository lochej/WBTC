#ifndef _ALTERA_HPS_SOC_SYSTEM_H_
#define _ALTERA_HPS_SOC_SYSTEM_H_

/*
 * This file was automatically generated by the swinfo2header utility.
 * 
 * Created from SOPC Builder system 'DE0_HPS_Example' in
 * file './DE0_HPS_Example.sopcinfo'.
 */

/*
 * This file contains macros for module 'hps_0' and devices
 * connected to the following masters:
 *   h2f_axi_master
 *   h2f_lw_axi_master
 * 
 * Do not include this header file and another header file created for a
 * different module or master group at the same time.
 * Doing so may result in duplicate macro names.
 * Instead, use the system header file which has macros with unique names.
 */

/*
 * Macros for device 'WS2812_Driver_0', class 'WS2812_Driver'
 * The macros are prefixed with 'WS2812_DRIVER_0_'.
 * The prefix is the slave descriptor.
 */
#define WS2812_DRIVER_0_COMPONENT_TYPE WS2812_Driver
#define WS2812_DRIVER_0_COMPONENT_NAME WS2812_Driver_0
#define WS2812_DRIVER_0_BASE 0x0
#define WS2812_DRIVER_0_SPAN 128
#define WS2812_DRIVER_0_END 0x7f

/*
 * Macros for device 'pb_pio', class 'altera_avalon_pio'
 * The macros are prefixed with 'PB_PIO_'.
 * The prefix is the slave descriptor.
 */
#define PB_PIO_COMPONENT_TYPE altera_avalon_pio
#define PB_PIO_COMPONENT_NAME pb_pio
#define PB_PIO_BASE 0x400
#define PB_PIO_SPAN 16
#define PB_PIO_END 0x40f
#define PB_PIO_IRQ 1
#define PB_PIO_BIT_CLEARING_EDGE_REGISTER 1
#define PB_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PB_PIO_CAPTURE 1
#define PB_PIO_DATA_WIDTH 2
#define PB_PIO_DO_TEST_BENCH_WIRING 0
#define PB_PIO_DRIVEN_SIM_VALUE 0
#define PB_PIO_EDGE_TYPE ANY
#define PB_PIO_FREQ 50000000
#define PB_PIO_HAS_IN 1
#define PB_PIO_HAS_OUT 0
#define PB_PIO_HAS_TRI 0
#define PB_PIO_IRQ_TYPE EDGE
#define PB_PIO_RESET_VALUE 0

/*
 * Macros for device 'sw_pio', class 'altera_avalon_pio'
 * The macros are prefixed with 'SW_PIO_'.
 * The prefix is the slave descriptor.
 */
#define SW_PIO_COMPONENT_TYPE altera_avalon_pio
#define SW_PIO_COMPONENT_NAME sw_pio
#define SW_PIO_BASE 0x410
#define SW_PIO_SPAN 16
#define SW_PIO_END 0x41f
#define SW_PIO_IRQ 0
#define SW_PIO_BIT_CLEARING_EDGE_REGISTER 1
#define SW_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SW_PIO_CAPTURE 1
#define SW_PIO_DATA_WIDTH 4
#define SW_PIO_DO_TEST_BENCH_WIRING 0
#define SW_PIO_DRIVEN_SIM_VALUE 0
#define SW_PIO_EDGE_TYPE ANY
#define SW_PIO_FREQ 50000000
#define SW_PIO_HAS_IN 1
#define SW_PIO_HAS_OUT 0
#define SW_PIO_HAS_TRI 0
#define SW_PIO_IRQ_TYPE EDGE
#define SW_PIO_RESET_VALUE 0

/*
 * Macros for device 'led_pio', class 'altera_avalon_pio'
 * The macros are prefixed with 'LED_PIO_'.
 * The prefix is the slave descriptor.
 */
#define LED_PIO_COMPONENT_TYPE altera_avalon_pio
#define LED_PIO_COMPONENT_NAME led_pio
#define LED_PIO_BASE 0x420
#define LED_PIO_SPAN 16
#define LED_PIO_END 0x42f
#define LED_PIO_BIT_CLEARING_EDGE_REGISTER 0
#define LED_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LED_PIO_CAPTURE 0
#define LED_PIO_DATA_WIDTH 8
#define LED_PIO_DO_TEST_BENCH_WIRING 0
#define LED_PIO_DRIVEN_SIM_VALUE 0
#define LED_PIO_EDGE_TYPE NONE
#define LED_PIO_FREQ 50000000
#define LED_PIO_HAS_IN 0
#define LED_PIO_HAS_OUT 1
#define LED_PIO_HAS_TRI 0
#define LED_PIO_IRQ_TYPE NONE
#define LED_PIO_RESET_VALUE 0


#endif /* _ALTERA_HPS_SOC_SYSTEM_H_ */
