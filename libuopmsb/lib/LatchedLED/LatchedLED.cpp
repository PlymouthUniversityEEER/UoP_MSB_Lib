#include "mbed.h"
#include "MSB_Config.h"
#include "LatchedLED.h"


// Function: LatchedLED class LE
// Returns the currently selected latch enable pin
DigitalOut& LatchedLED::LE(LEDGROUP grp)
{
    switch (grp) {
        case TENS:
        return LED_D1_LE;
        break;

        case UNITS:
        return LED_D2_LE;
        break;

        case RED:
        return LED_RED_LE;
        break;

        case GREEN:
        return LED_GRN_LE;
        break;

        case BLUE:
        return LED_BLUE_LE;
        break;
    }
}

// Function: LatchedLED class sevenSegclear
// Clear the seven segment display
void LatchedLED::sevenSegclear(void){
    write_seven_seg(0);
}

// Function: LatchedLED class dec_to_7seg
// Convert decimal to a 7-segment pattern
uint8_t LatchedLED::dec_to_7seg(uint8_t d)
{
    switch(d){
        case 0: return(A+B+C+D+E+F);    break;
        case 1: return(B+C);            break;
        case 2: return(A+B+D+E+G);      break;
        case 3: return(A+B+C+D+G);      break;
        case 4: return(B+C+F+G);        break;
        case 5: return(A+C+D+F+G);      break;
        case 6: return(C+D+E+F+G);      break;
        case 7: return(A+B+C);          break;
        case 8: return(A+B+C+D+E+F+G);  break;
        case 9: return(A+B+C+D+F+G);    break;
        default: return DP;              break;
    }            
}

// Function: LatchedLED class Constructor
// Initialised Digitalout and BusOut objects
LatchedLED::LatchedLED(LEDMODE mode, LEDGROUP grp) :  _mode(mode), 
                            _grp(grp),
                            LED_BAR_OE(LED_BAR_OE_PIN,1),
                            LED_DIGIT_OE(LED_DIGIT_OE_PIN,1),
                            LED_D1_LE(LED_D1_LE_PIN,0),
                            LED_D2_LE(LED_D2_LE_PIN,0),
                            LED_RED_LE(LED_RED_LE_PIN),
                            LED_GRN_LE(LED_GRN_LE_PIN),
                            LED_BLUE_LE(LED_BLUE_LE_PIN),
                            dataBus(LED_D0_PIN, LED_D1_PIN, LED_D2_PIN, LED_D3_PIN, LED_D4_PIN, LED_D5_PIN, LED_D6_PIN, LED_D7_PIN)                             
{                                

}


// Function: LatchedLED class destructor
// Disables all groups
LatchedLED::~LatchedLED() {
    LED_BAR_OE = 1;
    LED_DIGIT_OE = 1;
    LED_D1_LE = 0;
    LED_D2_LE = 0;
}

// Function: LatchedLED class enable
// Enable or disable a strip or seven seg
void LatchedLED::enable(bool en, LEDMODE mod)
{
    if(_lock.trylock_for(5ms)){
        switch (mod) {
            case STRIP:
                LED_BAR_OE = en ? 0 : 1;
            break;
            case SEVEN_SEG:
                LED_DIGIT_OE = en ? 0  : 1;
                break;
        }
        _lock.unlock();
    }
    //TODO: Handle error
}
// Function: LatchedLED class enable
// Enable or disable all groups
void LatchedLED::enable(bool en)
{
    if(_lock.trylock_for(5ms)){
        LED_BAR_OE = en ? 0 : 1;
        LED_DIGIT_OE = en ? 0 : 1;
        _lock.unlock();
    }
    //TODO: Handle error
}

// Function: LatchedLED class write_seven_seg
// Write decimal number to seven segment display
int LatchedLED::write_seven_seg(uint8_t dat) 
{
    uint8_t units = dec_to_7seg(dat % 10);
    uint8_t tens  = dec_to_7seg(dat / 10);
    if(_lock.trylock_for(100ms)){
        //Set tens data on bus
        dataBus = tens;
        //Strobe tens latch
        wait_us(1);
        LE(TENS)=1;
        wait_us(1);
        LE(TENS)=0;
        wait_us(1);

        //Set units data on bus
        dataBus = units;
        //Strobe units latch
        LE(UNITS)=1;
        wait_us(1);
        LE(UNITS)=0;
        wait_us(1);

        _lock.unlock();
        return 1;
    }
    return -2;
}

// Function: LatchedLED class write_strip
// Write binary value of dat to grp
int LatchedLED::write_strip(uint8_t dat, LEDGROUP grp) 
{
    if(grp==LEDGROUP::TENS || grp==LEDGROUP::UNITS){
        return -1;
    }
    if(_lock.trylock_for(5ms)){
        //Set data on bus
        dataBus = dat;
        //Latch strobe
        wait_us(1);
        LE(grp)=1;
        wait_us(1);
        LE(grp)=0;
        wait_us(1);
        _lock.unlock();
        return 1;
    }
    return -2;
}
