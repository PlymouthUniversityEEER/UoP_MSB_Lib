#include "mbed.h"
  #include "MSB_Config.h"

class Buttons{
    public:
        // Constructor
        Buttons(PinName b1 = BTN1_PIN, PinName b2 = BTN2_PIN, PinName b3 = BTN3_PIN, PinName b4 = BTN4_PIN, PinName buser = USER_BUTTON);
        // DigitalIn Objects
        DigitalIn Button1, Button2, Button3, Button4, BlueButton;
};



class DIPSwitches{
    private:
        // BusIn Object
        BusIn _bus;

    public:
        // Constructor
        DIPSwitches(PinName d0,PinName d1,PinName d2,PinName d3);
        // Read all swwitches and return binary
        uint8_t read();
        // Read single switch and return value
        uint8_t read(int pin);
        // [] Overload for reading single switch
        uint8_t operator[](const uint8_t pin);

};