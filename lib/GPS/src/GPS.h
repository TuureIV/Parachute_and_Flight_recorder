#include <Arduino.h>
#include <Wire.h>



class GPS
{
private:
    
    long lastTime = 0;
    float heading;
    
public:
    GPS(/* args */);

    void initGPS();
    bool updateGPSdata();
    long getLat();
    long getLon();
    long getAlt();
    byte getSIV();
    String getGPStime();

    ~GPS();
};


