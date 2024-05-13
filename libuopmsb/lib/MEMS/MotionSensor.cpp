#include "MotionSensor.h"
#include "mbed.h"
#include "MSB_Config.h"



MotionSensor::MotionSensor(PinName SDA, PinName SDC, int i2cFreq): MPU6050_DRIVER::MPU6050(SDA, SDC, i2cFreq){
    if (_whoami != 0x68) {
        DEBUG_PRINT("ERROR: MotionSensor Constructor: Wrong whoami value. Is the board populated?\n");
    }
}


// Read x,y and z values from the accelerometer
Motion_t MotionSensor::getAcceleration()
{
    if (_whoami != 0x68) {
        return {0.0f, 0.0f, 0.0f};
    }

    Motion_t acc;
    // Wait for data ready bit set, all data registers have new data
    while ( (readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01) == 0 );

    int16_t accelCount[3];
    readAccelData(accelCount);  // Read the x/y/z adc values
    getAres();

    // Now we'll calculate the accleration value into actual g's
    acc.x = (float)accelCount[0]*aRes - accelBias[0];  // get actual g value, this depends on scale being set
    acc.y = (float)accelCount[1]*aRes - accelBias[1];   
    acc.z = (float)accelCount[2]*aRes - accelBias[2];  

    return acc;
}

// Read x,y and z values from the gyroscope
Motion_t MotionSensor::getGyro()
{
    if (_whoami != 0x68) {
        return {0.0f, 0.0f, 0.0f};
    }

    Motion_t gyro;
    // Wait for data ready bit set, all data registers have new data
    while ( (readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01) == 0 );

    int16_t gyroCount[3];  
    readGyroData(gyroCount);  // Read the x/y/z adc values
    getGres();

    // Calculate the gyro value into actual degrees per second
    gyro.x = (float)gyroCount[0]*gRes; // - gyroBias[0];  // get actual gyro value, this depends on scale being set
    gyro.y = (float)gyroCount[1]*gRes; // - gyroBias[1];  
    gyro.z = (float)gyroCount[2]*gRes; // - gyroBias[2];  

    return gyro;
}

// Read temperature using the MPU6050
float MotionSensor::getTemperatureC()
{
    if (_whoami != 0x68) {
        return 0.0f;
    }
    
    // Wait for data ready bit set, all data registers have new data
    while ( (readByte(MPU6050_ADDRESS, INT_STATUS) & 0x01) == 0 );

    float tempC = (float)readTempData();    // 
    temperature = tempC / 340. + 36.53;     // Temperature in degrees Centigrade             
    return temperature;
}

// Get the device ID
uint8_t MotionSensor::whoAmI() {
    return _whoami;
}

