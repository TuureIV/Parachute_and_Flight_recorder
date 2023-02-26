#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_I2C_GPS_Arduino_Library.h>
#include <TinyGPS++.h>
#include "GPS.h"


I2CGPS i2cGPSconnection; 

TinyGPSPlus gpsModule;

GPS::GPS()
{
}
void GPS::initGPS(){
    
    
   if (i2cGPSconnection.begin() == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }
  else
  {

  }
  

}

bool GPS::updateGPSdata(){
    while (i2cGPSconnection.available() > 0)
    {
        if (gpsModule.encode(i2cGPSconnection.read()))
        {
            // if (gpsModule.time.isValid())
            // {
            //     return true;
            // }  
        }
        else
        {
            return false;
        }
        
    }  
}



long GPS::getLat(){
    return gpsModule.location.lat();
}

long GPS::getLon(){
    return gpsModule.location.lng();
}

long GPS::getAlt(){
    return gpsModule.altitude.meters();
}

byte GPS::getSIV(){
    return gpsModule.satellites.value();
}

String GPS::getGPStime(){
    return String(gpsModule.date.day()) + "." + String(gpsModule.date.month()) + "." + String(gpsModule.date.year()) + "_" +
            String(gpsModule.time.hour()) + "." + String(gpsModule.time.minute()) + "." + String(gpsModule.time.second());
}

GPS::~GPS()
{
}