#include "EnvSensor.h"


float EnvSensor::fRand() {
    return 0.02f*(float)(rand() % 100) - 1.0f;
} 


EnvSensor:: EnvSensor(PinName mosi, PinName miso, PinName sclk, PinName cs) : sensor(mosi, miso, sclk, cs)
{
    //Initialise the mocked humidity algorithm
    hum = hum0 = 50.0f + 30.0f*fRand(); //20.0% .. 80.0%
    delta = 0.1f*fRand();
    set_time(0);
    prevTime = currTime = time(NULL);
}

EnvSensor::~EnvSensor(){
    
}

float EnvSensor::getTemperature() {
    return sensor.getTemperature();
}
float EnvSensor::getPressure() {
    return sensor.getPressure();
}

// Mocked Humidity Reading
float EnvSensor::getHumidity() 
{
    prevTime = currTime;
    currTime = time(NULL);
    hum = fmax(fmin(hum+(float)(currTime-prevTime)*delta, 100.0f),0.0f);

    if (fabs(hum-hum0)>=1.0f) {
        //Reset initial values
        set_time(0);
        hum0 = hum;
        prevTime = currTime = time(NULL);
        //New profile
        delta = 0.1f*fRand();  
    }
    return hum;
}

ENV_SENSOR_TYPE EnvSensor::getSensorType()
{
    #if MSB_VER == 2
    return BMP280;
    #elif MSB_VER == 4
    return SPL06_001;
    #else
    return NONE;
    #endif
}

