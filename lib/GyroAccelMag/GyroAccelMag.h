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
    float heading;
    void calcAttitude(float ax, float ay, float az, float mx, float my, float mz);
   
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