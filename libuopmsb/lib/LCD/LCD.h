#include "mbed.h"
#include "MSB_Config.h"


    class LCD_16X2_DISPLAY : public Stream {
    public:
        typedef enum {INSTRUCTION=0, DATA=1} REGISTER_SELECT;   // RS
        typedef enum {WRITE=0, READ=1} READWRITE;               // R/W
        typedef enum {DISABLE=0, ENABLE=1} ENABLE_STATE;        // E

        typedef enum {DECREMENT=0, INCREMENT=1} ENTRY_MODE_DIR;                             // I/D
        typedef enum {CURSOR_MOVE_ON_ENTRY=0, DISPLAY_SHIFT_ON_ENTRY=1} ENTRY_MODE_SHIFT;   // S
        typedef enum {DISP_OFF=0, DISP_ON=1} DISPLAY_STATUS;                        // D
        typedef enum {CURSOR_VISIBLE_OFF=0, CURSOR_VISIBLE_ON=1} CURSOR_VISIBLE;    // C
        typedef enum {BLINK_OFF=0, BLINK_ON=1} BLINK_STATUS;                        // B
        typedef enum {CURSOR_MOVE=0, DISPLAY_SHIFT=1} CURSOR_OR_DISPLAY_SHIFT;      // S/C
        typedef enum {SHIFT_LEFT=0, SHIFT_RIGHT} CURSOR_OR_DISPLAY_SHIFT_DIR;       // R/L
        typedef enum { FOURBIT=0, EIGHTBIT=1 } INTERFACE_DATA_LENGTH;               // DL
        typedef enum { ONELINE=0, TWOLINES=1 } INTERFACE_NUM_LINES;                 // N
        typedef enum { FONT_5X8=0, FONT_5X10=1} INTERFACE_FONT_SIZE;                // F

        enum LCDType {
            LCD16x2     /**< 16x2 LCD panel (default) */
            , LCD16x2B  /**< 16x2 LCD panel alternate addressing */
            , LCD20x2   /**< 20x2 LCD panel */
            , LCD20x4   /**< 20x4 LCD panel */
        };

        protected:  
        DigitalOut _rs;
        DigitalOut _e;
        DigitalOut _rw;
        PwmOut _bkl;
        BusOut _data;
        LCDType _type;
        uint8_t _row;
        uint8_t _column;

        public:

        // 8-bit constructor
        LCD_16X2_DISPLAY(PinName rs=LCD_RS_PIN, PinName rw=LCD_RW_PIN, PinName e=LCD_E_PIN, PinName bkl=LCD_BKL_PIN, 
        PinName d0=LCD_D0_PIN, PinName d1=LCD_D1_PIN, PinName d2=LCD_D2_PIN, PinName d3=LCD_D3_PIN, 
        PinName d4=LCD_D4_PIN, PinName d5=LCD_D5_PIN, PinName d6=LCD_D6_PIN, PinName d7=LCD_D7_PIN,
        LCDType type=LCD16x2);


        uint8_t address(int row, int column);


    public:
        void character(int row, int column, uint8_t c);

        void write(REGISTER_SELECT rs, uint8_t b);

        uint8_t read(REGISTER_SELECT rs=INSTRUCTION);

        void cls();

        void home();
        void setEntryMode(ENTRY_MODE_DIR id=INCREMENT, ENTRY_MODE_SHIFT sh=CURSOR_MOVE_ON_ENTRY);
        void displayOnOffControl(DISPLAY_STATUS d=DISP_ON, CURSOR_VISIBLE c=CURSOR_VISIBLE_OFF, BLINK_STATUS b=BLINK_OFF);

        void cursorOrDisplayShift(CURSOR_OR_DISPLAY_SHIFT sc=CURSOR_MOVE, CURSOR_OR_DISPLAY_SHIFT_DIR rl=SHIFT_RIGHT);

        void functionSet(INTERFACE_DATA_LENGTH dl=EIGHTBIT, INTERFACE_NUM_LINES n=TWOLINES, INTERFACE_FONT_SIZE f=FONT_5X8);
        void set_CGRAM_Address(uint8_t Addr);

        void set_DDRAM_Address(uint8_t Addr);
        bool isBusy();
        void locate(uint8_t row, uint8_t column) ;

        uint8_t columns();

        uint8_t rows();

        void backlight_brightness(float f);
        
        // STDIO
        virtual int _putc(int value);

        virtual int _getc();
    };