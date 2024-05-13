#include "MSB_Utils.h"

//////////  TimerCompat  //////////
long long TimerCompat::read_ms() {
    return duration_cast<milliseconds>(elapsed_time()).count();
}

long long TimerCompat::read_us() {
    return duration_cast<microseconds>(elapsed_time()).count();
} 

