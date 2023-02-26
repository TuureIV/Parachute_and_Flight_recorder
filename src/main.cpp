#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <FileSerial.h>
#include "GyroAccelMag.h"
#include "Device.h"
#include "GPS.h"

#define DEBUG
#define PLOTTER_VIEW

FileSerial GPSLog(&Serial);
FileSerial IMULog(&Serial);
FileSerial MasterLog(&Serial);


// Global variables to print to serial monitor at a steady rate
const unsigned int PRINT_RATE = 100;
const unsigned int DEBUG_PRINT_RATE = 1000;
unsigned long lastPrint = 0;
unsigned long startTime = 0;

GPS gps;
IMU gyroAccelMag;
Led builtinLed = Led(BUILTIN_LED);
Button builtinButton = Button(KEY_BUILTIN);
MicroSD sdCard;

void setup(){
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Initializing the GPS");
  gps.initGPS();
  builtinLed.initLed();
  builtinButton.initButton();
  if (!sdCard.initMicroSD())
  {
    for (size_t i = 0; i < 20; i++)
    {
      builtinLed.ledON();
      delay(200);
      builtinLed.ledOFF();
      delay(50);
    }
  }
  
  Serial.println("Initializing the LSM9DS1");
  uint16_t status = gyroAccelMag.initSensors(true, true, true);
  Serial.print("LSM9DS1 WHO_AM_I's returned: 0x");
  Serial.println(status, HEX);
  Serial.println("Should be 0x683D");
  Serial.println();
  
 


  builtinLed.ledON();
  delay(2000);
  builtinLed.toggleLed();


  Serial.println("Press 0 to sart ");
  bool start = false;
  while (start == false){
    builtinButton.checkButtonPress();
    if (builtinButton.getButtonState() == true)
    {
      start = true;
    }
    
  }
  startTime = millis();
}

void loop(){
    
    #ifdef DEBUG
    
      if ((lastPrint + DEBUG_PRINT_RATE) < millis())
      {
        if (gyroAccelMag.readSensors()){
          Serial.println("sensors availuable");
          #ifdef PLOTTER_VIEW
            Serial.print("Gyro X ");
            Serial.println(gyroAccelMag.getGyroX());
            Serial.print("Gyro Y ");
            Serial.println(gyroAccelMag.getGyroY());
            Serial.print("Gyro Z ");
            Serial.println(gyroAccelMag.getGyroZ());
            gps.updateGPSdata();
            Serial.println("Lat: " + String(gps.getLat()) + " Lon: " + String(gps.getLon()) + " Alt: " + String(gps.getAlt()) +
            " GPS time: " + gps.getGPStime());
            
            // Check if user wants to stop logging
            

          #endif
      }
        else
        {
          Serial.println("sensors not availuable");
        }
        lastPrint = millis();
      }
      
    #endif

}