#include "mbed.h"
#include "MSB_Config.h"


class Matrix{
    private:
        SPI matrix_spi;
        DigitalOut matrix_spi_cs;     //Chip Select ACTIVE LOW
        DigitalOut matrix_spi_oe;     //Output Enable ACTIVE LOW
    public:
        // Constructor
        Matrix(PinName mosi,PinName miso,PinName sclk, PinName cs, PinName oe);
        // Clear the LED Matrix
        void clear(void);
        // Display a dot at coordinates row,col
        void dot(uint8_t row, uint8_t col);
        // Display a horizontal line on row
        void line(uint8_t row);
        // Display col_data binary value on row
        void write(uint8_t row, uint16_t col_data );
        // Set the output enable value
        void enable(bool en);

};

