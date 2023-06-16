/*
 * sci.init.c
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#include "sci_init.h"

void PinMux_init()
{
    //
         // SCIA -> mySCI0 Pinmux
         //
         GPIO_setPinConfig(mySCI0_SCIRX_PIN_CONFIG);
         GPIO_setPadConfig(mySCI0_SCIRX_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
         GPIO_setQualificationMode(mySCI0_SCIRX_GPIO, GPIO_QUAL_ASYNC);

         GPIO_setPinConfig(mySCI0_SCITX_PIN_CONFIG);
         GPIO_setPadConfig(mySCI0_SCITX_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
         GPIO_setQualificationMode(mySCI0_SCITX_GPIO, GPIO_QUAL_ASYNC);
}

void SCI_init(){
     mySCI0_init();
 }

 void mySCI0_init(){
     SCI_clearInterruptStatus(mySCI0_BASE, SCI_INT_RXFF | SCI_INT_TXFF | SCI_INT_FE | SCI_INT_OE | SCI_INT_PE | SCI_INT_RXERR | SCI_INT_RXRDY_BRKDT | SCI_INT_TXRDY);
     SCI_clearOverflowStatus(mySCI0_BASE);
     SCI_resetTxFIFO(mySCI0_BASE);
     SCI_resetRxFIFO(mySCI0_BASE);
     SCI_resetChannels(mySCI0_BASE);
     SCI_setConfig(mySCI0_BASE, DEVICE_LSPCLK_FREQ, mySCI0_BAUDRATE, (SCI_CONFIG_WLEN_8|SCI_CONFIG_STOP_ONE|SCI_CONFIG_PAR_NONE));
     SCI_enableLoopback(mySCI0_BASE);
     SCI_performSoftwareReset(mySCI0_BASE);
     SCI_setFIFOInterruptLevel(mySCI0_BASE, SCI_FIFO_TX0, SCI_FIFO_RX0);
     SCI_enableFIFO(mySCI0_BASE);
     SCI_enableModule(mySCI0_BASE);
 }

 void intToStrPositive(uint32_t value, char* str)
 {
     uint32_t i = 0;

     // Convert each digit to a character
     do {
         str[i++] = (char)(value % 10) + '0';
         value /= 10;
     } while (value > 0);

     // Reverse the string
     uint32_t j;
     for (j = 0; j < i / 2; j++) {
         char temp = str[j];
         str[j] = str[i - j - 1];
         str[i - j - 1] = temp;
     }

     // Null-terminate the string
     str[i] = '\0';
 }



