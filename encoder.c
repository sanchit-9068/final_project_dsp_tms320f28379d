/*
 * encoder.c
 *
 *  Created on: 13-Jun-2023
 *      Author: Sanchit Awasthi
 */

#include "encoder.h"

void setupEQEPModule()
{
    //
        EQEP_setDecoderConfig(EQEP1_BASE, (EQEP_CONFIG_2X_RESOLUTION |
                                           EQEP_CONFIG_QUADRATURE |
                                           EQEP_CONFIG_NO_SWAP));
        EQEP_setEmulationMode(EQEP1_BASE, EQEP_EMULATIONMODE_RUNFREE);

        //
        // Configure the position counter to reset on an index event
        //
        EQEP_setPositionCounterConfig(EQEP1_BASE, EQEP_POSITION_RESET_IDX,
                                      0xFFFFFFFF);

        //
        // Enable the unit timer, setting the frequency to 100 Hz
        //
        EQEP_enableUnitTimer(EQEP1_BASE, (DEVICE_SYSCLK_FREQ / 50));

        //
        // Configure the position counter to be latched on a unit time out
        //
        EQEP_setLatchMode(EQEP1_BASE, EQEP_LATCH_UNIT_TIME_OUT);

        //
        // Enable the eQEP1 module
        //
        EQEP_enableModule(EQEP1_BASE);

        //
        // Configure and enable the edge-capture unit. The capture clock divider is
        // SYSCLKOUT/64. The unit-position event divider is QCLK/32.
        //
        EQEP_setCaptureConfig(EQEP1_BASE, EQEP_CAPTURE_CLK_DIV_64,
                              EQEP_UNIT_POS_EVNT_DIV_32);
        EQEP_enableCapture(EQEP1_BASE);
}

void Pin_EQEP()
{
        GPIO_setPinConfig(GPIO_20_EQEP1A);
        GPIO_setPadConfig(20, GPIO_PIN_TYPE_STD);

        GPIO_setPinConfig(GPIO_21_EQEP1B);
        GPIO_setPadConfig(21, GPIO_PIN_TYPE_STD);

        GPIO_setPinConfig(GPIO_23_EQEP1I);
        GPIO_setPadConfig(23, GPIO_PIN_TYPE_STD);
        GPIO_setPinConfig(DEVICE_GPIO_CFG_LED1);
        GPIO_setMasterCore(DEVICE_GPIO_PIN_LED1, GPIO_CORE_CPU1);
        GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
        GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);
}
