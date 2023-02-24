#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

class IMU
{
private:
    boolean _flipX;
    boolean _flipY;
    boolean _flipZ;
    float roll;
    float pitch;
   
public:
    IMU();
    ~IMU();
    uint16_t initSensors(boolean flipX, boolean flipY, boolean flipZ);
    bool readSensors();
    float getGyroX();
    float getGyroY();
    float getGyroZ();
    float getAccelX();
    float getAccelY();
    float getAccelZ();

    
};