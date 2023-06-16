/*
 * sci_init.h
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#ifndef SCI_INIT_H_
#define SCI_INIT_H_

#include "driverlib.h"
#include "device.h"

#define GPIO_PIN_SCIRXDA 43
#define mySCI0_SCIRX_GPIO 43
#define mySCI0_SCIRX_PIN_CONFIG GPIO_43_SCIRXDA
//
// SCITXDA - GPIO Settings
//
#define GPIO_PIN_SCITXDA 42
#define mySCI0_SCITX_GPIO 42
#define mySCI0_SCITX_PIN_CONFIG GPIO_42_SCITXDA

//*****************************************************************************
//
// SCI Configurations
//
//*****************************************************************************
#define mySCI0_BASE SCIA_BASE
#define mySCI0_BAUDRATE 9600
#define mySCI0_CONFIG_WLEN SCI_CONFIG_WLEN_8
#define mySCI0_CONFIG_STOP SCI_CONFIG_STOP_ONE
#define mySCI0_CONFIG_PAR SCI_CONFIG_PAR_NONE
#define mySCI0_FIFO_TX_LVL SCI_FIFO_TX0
#define mySCI0_FIFO_RX_LVL SCI_FIFO_RX0


void mySCI0_init(void);
void    SCI_init(void);


void intToStrPositive(uint32_t value, char* str);
void PinMux_init(void);



#endif /* SCI_INIT_H_ */
