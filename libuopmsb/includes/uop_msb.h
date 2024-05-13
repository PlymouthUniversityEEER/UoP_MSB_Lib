// (c) 2020 University of Plymouth
// Nicholas Outram
// nicholas.outram@plymouth.ac.uk
//
//
// Added support for SPL06-001 environmental sensor
// updated 14 09 2021

#ifndef __UOP_MSB__
#define __UOP_MSB__

#include "Buttons.h"
#include "LatchedLED.h"
#include "Buzzer.h"
#include "LCD.h"
#include "EnvSensor.h"
#include "MotionSensor.h"
#include "MSB_Utils.h"
#include "matrix.h"
#include "TrafficLights.h"


// SD Card (RTOS ONLY)
#ifdef USE_SD_CARD  //RTOS Only
    #include "SDCard.h"
    SDCard sd(SD_MOSI,SD_MISO,SD_SCLK,SD_CS);
#endif

namespace uop_msb {
    // Precreated library objects for you to use
    AnalogIn pot(AN_POT_PIN);
    AnalogIn ldr (AN_LDR_PIN);
    AnalogIn signal_in (BNC_AN_PIN);
    AnalogIn mic (MIC_AN_PIN);
    AnalogOut dac_out_1 (DAC1_AN_PIN);
    AnalogOut dac_out_2 (DAC2_AN_PIN);
    LatchedLED latchedLEDs(LEDMODE::STRIP);     
    Buttons buttons;
    LCD_16X2_DISPLAY disp;
    EnvSensor env (ENV_MOSI,ENV_MISO,ENV_SCLK,ENV_CS);
    Buzzer buzz;
    DIPSwitches dipSwitches (DIP0_PIN,DIP1_PIN,DIP2_PIN,DIP3_PIN);
    MotionSensor motion (MEMS_SDA,MEMS_SCL,MEMS_FREQ);
    Matrix matrix (MATRIX_MOSI,MATRIX_MISO,MATRIX_SCLK,MATRIX_CS,MATRIX_OE);
    TrafficLights traffic(TRAF_RED1_PIN,TRAF_YEL1_PIN,TRAF_GRN1_PIN,TRAF_RED2_PIN,TRAF_YEL2_PIN,TRAF_GRN2_PIN,TRAF_WHITE_PIN);
}
#endif
