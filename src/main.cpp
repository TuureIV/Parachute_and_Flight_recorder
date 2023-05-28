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

char csvBuffer[300];

float gyroX;
float gyroY;
float gyroZ;

float accelX;
float accelY;
float accelZ;

float lat;
float lon;
float alt;
String gpsTime = "";

bool initLogFile(FileSerial logFile, const char * file_location, const char * file_name){

    if(!logFile.begin(&SD, 33, SPI, 10000000, "/sd")){
        Serial.println("SD begin did not succeed, halting.");
        // Stops the device
        while(1);

    }

  logFile.setMaxFileSize(10000);
  logFile.setWriteBufferSize(80);
	logFile.startLog(file_location,  file_name);

}

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
  

  initLogFile(MasterLog, "root", "master");

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

          gyroX = gyroAccelMag.getGyroX();
          gyroY = gyroAccelMag.getGyroY();
          gyroZ = gyroAccelMag.getGyroZ();
          accelX = gyroAccelMag.getAccelX();
          accelY = gyroAccelMag.getAccelY();
          accelZ = gyroAccelMag.getAccelZ();

          if (gps.updateGPSdata())
          {
            lat = gps.getLat();
          lon = gps.getLon();
          alt = gps.getAlt();
          gpsTime = gps.getGPStime();
          }
          
          sprintf(csvBuffer, "%4.3f,%4.3f,%4.3f,%4.3f,%4.3f,%4.3f", gyroX, gyroY, gyroZ, accelX, accelY, accelZ);
          MasterLog.println(csvBuffer);
          //sdCard.logData(MasterLog, csvBuffer);

          #ifdef PLOTTER_VIEW
            Serial.print("Gyro X ");
            Serial.println(gyroX);
            Serial.print("Gyro Y ");
            Serial.println(gyroY);
            Serial.print("Gyro Z ");
            Serial.println(gyroZ);

            Serial.print("Accel X ");
            Serial.println(accelX);
            Serial.print("Accel Y ");
            Serial.println(accelY);
            Serial.print("Accel Z ");
            Serial.println(accelZ);

            Serial.println("Lat: " + String(lat) + " Lon: " + String(lon) + " Alt: " + String(alt) +
            " GPS time: " + gpsTime);
          #endif

          

          builtinButton.checkButtonPress();
          if (!builtinButton.getButtonState())
          {
            while (1);
            
          }
            
          
            

          
      }
        else
        {
          Serial.println("sensors not availuable");
        }
        lastPrint = millis();
      }
      
    #endif

}