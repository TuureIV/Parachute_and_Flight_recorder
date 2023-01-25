#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include "gyroAccel.h"
#include "Device.h"
#include "GPS.h"


// Global variables to print to serial monitor at a steady rate
const unsigned int PRINT_RATE = 100;
unsigned long lastPrint = 0;


GPS gps;
GyroAccelMag gyroAccelMag;


void setup(){
    Serial.begin(115200);

    
}

void loop(){

}