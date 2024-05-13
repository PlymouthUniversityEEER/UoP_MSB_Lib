#include "mbed.h"
#include "MSB_Config.h"


#include "MPU6050.hpp"

typedef struct {
    float x;
    float y;
    float z;
} Motion_t;


class MotionSensor : MPU6050_DRIVER::MPU6050 {
        public:
        // Constructor with initialisation list
        MotionSensor(PinName SDA=PB_11, PinName SDC=PB_10, int i2cFreq = 400000);

        // Read x,y and z values from the accelerometer
        Motion_t getAcceleration();

        // Read x,y and z values from the gyroscope
        Motion_t getGyro();

        // Read temperature using the MPU6050
        float getTemperatureC();

        // Get the device ID
        uint8_t whoAmI();

};