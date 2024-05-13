#include "mbed.h"
#include "MSB_Config.h"
#include "LCD.h"



        // 8-bit constructor
        LCD_16X2_DISPLAY::LCD_16X2_DISPLAY(PinName rs, PinName rw, PinName e, PinName bkl, 
        PinName d0, PinName d1, PinName d2, PinName d3, 
        PinName d4, PinName d5, PinName d6, PinName d7,LCDType type): _rs(rs), _e(e), _rw(rw), _bkl(bkl), _data(d0, d1, d2, d3, d4, d5, d6, d7), _type(type)
        {
            //Power on wait
            wait_us(100000);

            //Function set
            functionSet(EIGHTBIT, TWOLINES, FONT_5X8);
            
            //Display ON
            displayOnOffControl(DISP_ON, CURSOR_VISIBLE_OFF, BLINK_OFF);

            //CLS
            cls();

            //Entry Mode
            setEntryMode(INCREMENT, CURSOR_MOVE_ON_ENTRY);

            //Cursor moves right
            //cursorOrDisplayShift(CURSOR_MOVE, SHIFT_RIGHT);        
        }


        uint8_t LCD_16X2_DISPLAY::address(int row, int column) {
            switch (_type) {
                case LCD20x4:
                    switch (row) {
                        case 0:
                            return 0x80 + column;
                        case 1:
                            return 0xc0 + column;
                        case 2:
                            return 0x94 + column;
                        case 3:
                            return 0xd4 + column;
                    }
                case LCD16x2B:
                    return 0x80 + (row * 40) + column;
                case LCD16x2:
                case LCD20x2:
                default:
                    return 0x80 + (row * 0x40) + column;
            }
        }

        void LCD_16X2_DISPLAY::character(int row, int column, uint8_t c) {
            volatile int a = address(row, column);
            set_DDRAM_Address(a);
            write(DATA, c);
        }

        void LCD_16X2_DISPLAY::write(REGISTER_SELECT rs, uint8_t b)
        {
            _rs = rs;
            _rw = WRITE;
            wait_ns(5);
            _e = ENABLE;
            wait_ns(25);    //Rising edge
            wait_ns(100);   //Setup
            _data = b;
            wait_ns(40);    //Data setup time
            _e = DISABLE;
            wait_ns(25);    //Falling edge
            wait_ns(10);    //Hold time

            //Prevent premature write
            wait_us(50);
        }

        uint8_t LCD_16X2_DISPLAY::read(REGISTER_SELECT rs)
        {
            _rs = rs;
            _rw = 1;
            wait_ns(5);
            _e = 1;
            wait_ns(25);    //Rise time
            wait_ns(100);   //T_DDR
            uint8_t data = _data;    //Read
            wait_ns(40);
            _e = 0;
            wait_ns(25);    //Fall time
            wait_ns(10);    //t_H
            _rw = 0;
            wait_us(1);     //Final wait to prevent early trans
            return data;
        }

        void LCD_16X2_DISPLAY::cls() {
            write(INSTRUCTION, 0b00000001);
            wait_us(1600); 
            home();
        }

        void LCD_16X2_DISPLAY::home() {
            write(INSTRUCTION, 0b00000010);
            wait_us(1600); 
            _column = 0;
            _row = 0;       
        }

        void LCD_16X2_DISPLAY::setEntryMode(ENTRY_MODE_DIR id, ENTRY_MODE_SHIFT sh)
        {
            uint8_t dat = 0b00000100;
            dat |= (id ? 2 : 0);
            dat |= (sh ? 1 : 0);
            write(INSTRUCTION, dat);          
        }

    void LCD_16X2_DISPLAY::displayOnOffControl(DISPLAY_STATUS d, CURSOR_VISIBLE c, BLINK_STATUS b)       {
            uint8_t dat = 0b00001000;
            dat |= (d ? 4 : 0);
            dat |= (c ? 2 : 0);
            dat |= (b ? 1 : 0);
            write(INSTRUCTION, dat);      
        }

        void LCD_16X2_DISPLAY::cursorOrDisplayShift(CURSOR_OR_DISPLAY_SHIFT sc, CURSOR_OR_DISPLAY_SHIFT_DIR rl)
        {
            uint8_t dat = 0b00010000;
            dat |= (sc ? 8 : 0);
            dat |= (rl ? 4 : 0);
            write(INSTRUCTION, dat); 
        }

        void LCD_16X2_DISPLAY::functionSet(INTERFACE_DATA_LENGTH dl, INTERFACE_NUM_LINES n, INTERFACE_FONT_SIZE f)
        {
            uint8_t dat = 0b00100000;
            dat |= (dl ? 16 : 0);   //Data lines (0: 4 bit; 1: 8 bit)
            dat |= (n ? 8 : 0);     //Number of display lines (0: 1 line; 1: 2 lines)
            dat |= (f ? 4 : 0);     //Font: (0: 5x8; 1: 5x10)
            write(INSTRUCTION, dat);     
        }    

        void LCD_16X2_DISPLAY::set_CGRAM_Address(uint8_t Addr)
        {
            uint8_t dat =  0b01000000;
            dat |= (Addr & 0b00111111);
            write(INSTRUCTION, dat);
        }

        void LCD_16X2_DISPLAY::set_DDRAM_Address(uint8_t Addr)
        {
            uint8_t dat =  Addr & 0b01111111;
            dat |= 0b10000000;
            write(INSTRUCTION, dat);
        }

        bool LCD_16X2_DISPLAY::isBusy()
        {
            return (read(INSTRUCTION) & 0b10000000) ? true : false;
        }

        void LCD_16X2_DISPLAY::locate(uint8_t row, uint8_t column) {
            _column = column;
            _row = row;
            volatile int a = address(row, column);
            set_DDRAM_Address(a);
        }

        uint8_t LCD_16X2_DISPLAY::columns() {
            switch (_type) {
                case LCD20x4:
                case LCD20x2:
                    return 20;
                case LCD16x2:
                case LCD16x2B:
                default:
                    return 16;
            }
        }

        uint8_t LCD_16X2_DISPLAY::rows() {
            switch (_type) {
                case LCD20x4:
                    return 4;
                case LCD16x2:
                case LCD16x2B:
                case LCD20x2:
                default:
                    return 2;
            }
        }

        void LCD_16X2_DISPLAY::backlight_brightness(float f){
            _bkl = f;
        }

        // STDIO
        int LCD_16X2_DISPLAY::_putc(int value) {
            if (value == '\n') {
                _column = 0;
                _row++;
                if (_row >= rows()) {
                    _row = 0;
                }
            } else {
                character(_row, _column, value);
                _column++;
                if (_column >= columns()) {
                    _column = 0;
                    _row++;
                    if (_row >= rows()) {
                        _row = 0;
                    }
                }
            }
            return value;
        }

        int LCD_16X2_DISPLAY::_getc() {
            return -1;
        }
    