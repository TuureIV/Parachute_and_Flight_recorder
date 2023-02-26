#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "GPS.h"
#include <SoftwareSerial.h>

SoftwareSerial GPSUART(16,17);
SFE_UBLOX_GNSS gpsModule;

GPS::GPS()
{
}
void GPS::initGPS(){
    
   do {
    Serial.println("GPS: trying 38400 baud");
    GPSUART.begin(38400);
    if (gpsModule.begin(GPSUART) == true) break;

    delay(100);
    Serial.println("GPS: trying 9600 baud");
    GPSUART.begin(9600);
    if (gpsModule.begin(GPSUART) == true) {
        Serial.println("GPS: connected at 9600 baud, switching to 38400");
        gpsModule.setSerialRate(38400);
        delay(100);
    } else {
        gpsModule.factoryReset();
        delay(2000); //Wait a bit before trying again to limit the Serial output
    }
  } while(1);
  Serial.println("GPS serial connected");

  gpsModule.setUART1Output(COM_TYPE_UBX);
  gpsModule.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  gpsModule.saveConfiguration();        //Save the current settings to flash and BBR
}


long GPS::getLat(){
    return gpsModule.getLatitude();
}

long GPS::getLon(){
    return gpsModule.getLongitude();
}

long GPS::getAlt(){
    return gpsModule.getAltitude();
}

byte GPS::getSIV(){
    return gpsModule.getSIV();
}

String GPS::getGPStime(){
    return String(gpsModule.getDay()) + "." + String(gpsModule.getMonth()) + "." + String(gpsModule.getYear()) + "_" +
            String(gpsModule.getHour()) + "." + String(gpsModule.getMinute()) + "." + String(gpsModule.getSecond());
}

GPS::~GPS()
{
}