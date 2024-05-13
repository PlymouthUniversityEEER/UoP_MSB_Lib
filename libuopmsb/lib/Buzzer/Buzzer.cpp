#include "mbed.h"
#include "MSB_Config.h"
#include "Buzzer.h"

// Function: Buzzer class Constructor
// Initialises PWMOut object 
Buzzer::Buzzer(PinName p) : _buzzer(p) {
    volatile double T = periodForNote_us("C");
    _buzzer.period_us(T);
    rest();      
}

// Function: Buzzer class playTone
// Plays note in octave
void Buzzer::playTone(const char* note, OCTAVE_REGISTER octave)
{
    _buzzer.period_us(periodForNote_us(note, octave));
    _buzzer.write(0.5f);
}

// Function: Buzzer class rest
// Stops the tone
void Buzzer::rest() {
    _buzzer.write(0.0);
}

// Function: Buzzer class periodForNote_us
// Converts a note into the coresponding period
double Buzzer::periodForNote_us(const char* note, OCTAVE_REGISTER octave)
{
    volatile uint8_t idx = offsetForNote(note); 
    volatile double f = note_freq[idx];

    switch (octave) {
        case LOWER_OCTAVE:
            f *= 0.5;
            break;
        case MIDDLE_OCTAVE:
            break;
        case HIGHER_OCTAVE:
            f *= 2.0;
            break;
        default:
            break;
    }
    return 1000000.0 / f;
}

// Function: Buzzer class periodForNote_us
// Convert note to index in note_freq array
uint8_t Buzzer::offsetForNote(const char *noteStr)
{
    switch (noteStr[0])
    {
        case 'A':
            return (noteStr[1]=='#') ? 1 : 0;
        case 'B':
            return 2;
        case 'C':
            return (noteStr[1]=='#') ? 4 : 3; 
        case 'D':
            return (noteStr[1]=='#') ? 6 : 5;                    
        case 'E':
            return 7;
        case 'F':
            return (noteStr[1]=='#') ? 9 : 8;                                           
        case 'G':
            return (noteStr[1]=='#') ? 11 : 10;  
        default:
            return 0;                   
    }
}


