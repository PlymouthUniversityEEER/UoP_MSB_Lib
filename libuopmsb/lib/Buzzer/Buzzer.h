#include "mbed.h"
#include "MSB_Config.h"

class Buzzer {
    public:

        typedef enum {LOWER_OCTAVE, MIDDLE_OCTAVE, HIGHER_OCTAVE} OCTAVE_REGISTER;
        // Constructor
        Buzzer(PinName p = BUZZER_PIN);
        // Plays note in octave
        void playTone(const char* note, OCTAVE_REGISTER octave=MIDDLE_OCTAVE);
        // Stops the tone
        void rest();

    protected:
        // PWMout object
        PwmOut _buzzer;
        // Converts a note into the coresponding period
        double periodForNote_us(const char* note, OCTAVE_REGISTER octave=MIDDLE_OCTAVE);
    private:
        // Convert note to index in note_freq array
        uint8_t offsetForNote(const char *noteStr);

        // Frequencies of notes
        double note_freq[12] = {
            220.0, 233.08, 246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3
        };

};