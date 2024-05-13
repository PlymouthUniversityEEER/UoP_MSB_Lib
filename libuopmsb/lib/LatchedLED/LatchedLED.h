#include "mbed.h"
#include "MSB_Config.h"

typedef enum {STRIP, SEVEN_SEG} LEDMODE;   
typedef enum {TENS, UNITS, RED, GREEN, BLUE} LEDGROUP;     

class LatchedLED {

        // Convert decimal to a 7-segment pattern
        #define A  0x10
        #define B  0x20
        #define C  0x40
        #define D  0x08
        #define E  0x02
        #define F  0x01
        #define G  0x04
        #define DP 0x80

        private:
            // Variables for current group and mode
            LEDMODE _mode;
            LEDGROUP _grp;

            // DigitalOut and BusOUt objects for data and latches
            DigitalOut LED_BAR_OE;
            DigitalOut LED_DIGIT_OE;
            DigitalOut LED_D1_LE;
            DigitalOut LED_D2_LE;
            DigitalOut LED_RED_LE;
            DigitalOut LED_GRN_LE;
            DigitalOut LED_BLUE_LE;     
            BusOut dataBus;
            
            // Mutex to prevent corruption
            Mutex _lock;

            // Returns the currently selected latch enable pin
            DigitalOut& LE(LEDGROUP grp);

            // Convert decimal to a 7-segment pattern
            uint8_t dec_to_7seg(uint8_t d);

        public:
            // Constructor and Destructor
            LatchedLED(LEDMODE mode, LEDGROUP grp=RED);
            ~LatchedLED();
            
            // Clear the seven segment display
            void sevenSegclear(void);
            // Place number on the seven segment display
            int  sevenSegnum(int num);
            // Enable or disable all groups
            void enable(bool en);
            // Enable or disable a strip or seven seg
            void enable(bool en, LEDMODE mod);
            // Write binary value of dat to grp
            int write_strip(uint8_t dat, LEDGROUP grp);
            // Write decimal number to seven segment display
            int write_seven_seg(uint8_t dat);

    };