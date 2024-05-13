#include "mbed.h"
  #include "MSB_Config.h"
  #include "Buttons.h"


/**********Buttons Class**********/

// Function: Buttons Class Constructor
// initialises DigitalIn objects
Buttons::Buttons(PinName b1,PinName b2, PinName b3, PinName b4, PinName buser):
            Button1(b1), Button2(b2), Button3(b3, PullDown), Button4(b4, PullDown), BlueButton(buser){
}

// Function: DIPSwitches Class Constructor
// Initialises BusIn object
DIPSwitches::DIPSwitches(PinName d0,PinName d1,PinName d2,PinName d3): _bus(d0,d1,d2,d3){
    _bus.mode(PullDown);
}

// Function: DIPSwitches Class read (overloaded)
// Read all swwitches and return binary
uint8_t DIPSwitches::read(){
    return _bus.read();
}

// Function: DIPSwitches Class read (overloaded)
// Read single switch and return value
uint8_t DIPSwitches::read(int pin){
    return (_bus.read() >> pin) & 1 ;
}

// Operator: DIPSwitches Class operator[]
// [] Overload for reading single switch
uint8_t DIPSwitches::operator[](const uint8_t pin) {
        return read(pin);
}

