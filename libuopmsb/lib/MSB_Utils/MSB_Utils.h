#include "mbed.h"
#include "MSB_Config.h"
#include <chrono>

using namespace std::chrono;

// Mbed os 5 like Timer
class TimerCompat : public Timer {

    public:
    long long read_ms();
    long long read_us();
    
};   

