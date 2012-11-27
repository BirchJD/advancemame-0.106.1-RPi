/*******************************************/
/* Jason Birch   2012-11-26   V1.00        */
/* Joystick control for Raspberry Pi GPIO. */
/*******************************************/

#ifndef __JRPI_H
#define __JRPI_H

#include "joydrv.h"

#ifdef __cplusplus
extern "C" {
#endif

extern joystickb_driver joystickb_rpi_driver;


#define GPIO_PERI_BASE           0x20000000
#define GPIO_BASE                (GPIO_PERI_BASE + 0x200000)
#define BLOCK_SIZE               (4 * 1024)
#define PAGE_SIZE                (4 * 1024)
#define GPIO_ADDR_OFFSET         13
#define BUFF_SIZE                128
#define RPI_BUTTONS              17


#ifdef __cplusplus
}
#endif

#endif
