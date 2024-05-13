#include "mbed.h"
#include "MSB_Config.h"
#include "Matrix.h"

// Function: Matrix class constructor
// initialised the underlying SPI object
Matrix::Matrix(PinName mosi,PinName miso,PinName sclk, PinName cs, PinName oe) : matrix_spi(MATRIX_MOSI,MATRIX_MISO,MATRIX_SCLK), matrix_spi_cs(cs),matrix_spi_oe(oe){
    matrix_spi.format(8,0);            // 8bits, Rising edge, +VE Logic Data
    matrix_spi.frequency(1000000);     // 1MHz Data Rate
    enable(1);
}

// Function: Matrix class clear
// Clear the LED Matrix
void Matrix::clear(void){
    matrix_spi_cs=0;            //CLEAR Matrix
    matrix_spi.write(0x00);     //COL RHS
    matrix_spi.write(0x00);     //COL LHS
    matrix_spi.write(0x00);     //ROW
    matrix_spi_cs=1;
}

// Function: Matrix class dot
// Display a dot at coordinates row,col
void Matrix::dot(uint8_t row, uint8_t col){
    matrix_spi_cs=0;                    //Send Data to Matrix
    if(col<8){
        matrix_spi.write(0x00);         //COL RHS
        matrix_spi.write(1<<(col%8));   //COL LHS
    }
    else{
        matrix_spi.write(1<<(col%8));   //COL RHS
        matrix_spi.write(0x00);         //COL LHS
    }

    matrix_spi.write(row);              //Row
    matrix_spi_cs=1;                    //Send Data to Matrix
}

// Function: Matrix class line
// Display a horizontal line on row
void Matrix::line(uint8_t row){
    matrix_spi_cs=0;            //Send Data to Matrix
    matrix_spi.write(255);      //COL RHS
    matrix_spi.write(255);      //COL LHS
    matrix_spi.write(row);      //Row
    matrix_spi_cs=1;            //Send Data to Matrix
}

// Function: Matrix class write
// Display col_data binary value on row
void Matrix::write(uint8_t row, uint16_t col_data ){
    matrix_spi_cs=0;                        //Send Data to Matrix
    matrix_spi.write((col_data>>8)&0xff);   //COL RHS
    matrix_spi.write(col_data&0xff);        //COL LHS
    matrix_spi.write(row);                  //Row
    matrix_spi_cs=1;                        //Send Data to Matrix
}

// Function: Matrix class enable
// Set the output enable value
void Matrix::enable(bool en){
    matrix_spi_oe = !en;    // logic is inverted
}