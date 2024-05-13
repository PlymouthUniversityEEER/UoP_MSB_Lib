#include "mbed.h"
#include "MSB_Config.h"

// Use the sensor for the appropriate board version
#if MSB_VER == 2
#include "BMP280_SPI.h"
#define SENSOR_T BMP280_SPI
#elif MSB_VER == 4
#include "SPL06-001.h"
#define SENSOR_T SPL06_001_SPI
#else
#error Valid Module Support Board Version is Needed
#endif


typedef enum {NONE, BMP280, SPL06_001} ENV_SENSOR_TYPE;

class EnvSensor {
    private:
        SENSOR_T sensor;
        float hum, hum0, delta; //Humidity and % persecond
        time_t prevTime, currTime;
        float fRand();

    public:
        
        EnvSensor(PinName mosi=PB_5, PinName miso=PB_4, PinName sclk=PB_3, PinName cs=PB_2);
        ~EnvSensor();
        float getTemperature();
        float getPressure();
        // Mocked Humidity Reading
        float getHumidity();
        ENV_SENSOR_TYPE getSensorType();


    };    //end class