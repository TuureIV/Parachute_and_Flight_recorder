#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_Ublox_Arduino_Library.h>


class GPS
{
private:
    
    long lastTime = 0;
    float heading;
    
public:
    GPS(/* args */);

    void initGPS();
    long getLat();
    long getLon();
    long getAlt();
    byte getSIV();
    String getGPStime();

    ~GPS();
};


