#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "SD.h"
#include "Device.h"



MicroSD::MicroSD(/* args */)
{

#define SD_CS_PIN 33
#define SD_CD_PIN 38

}

bool MicroSD::initMicroSD(){
    pinMode(SD_CD_PIN, INPUT);
        if(digitalRead(SD_CD_PIN) == 1)
        {
            Serial.println("Card detected");
            if(!SD.begin(SD_CS_PIN, SPI, 1000000, "/sd"))
            {
                Serial.println("Card Mount Failed");
                return false;
            }

            uint8_t cardType = SD.cardType();

            if(cardType == CARD_NONE){
                Serial.println("No SD card attached");
                return false;
            }

            Serial.print("SD Card Type: ");
            if(cardType == CARD_MMC){
                Serial.println("MMC");
            } else if(cardType == CARD_SD){
                Serial.println("SDSC");
            } else if(cardType == CARD_SDHC){
                Serial.println("SDHC");
            } else {
                Serial.println("UNKNOWN");
            }
            return true;
        } else {
            Serial.println("Card not present");
            return false;
        }   
}

bool MicroSD::initLogFile(FileSerial logFile, const char * file_location, const char * file_name){

    if(!logFile.begin(&SD, 33, SPI, 10000000, "/sd")){
        Serial.println("SD begin did not succeed, halting.");
        // Stops the device
        while(1);

    }

    logFile.setMaxFileSize(10000);
    logFile.setWriteBufferSize(80);
	logFile.startLog(file_location,  file_name);





}

void logData(FileSerial logFile){

}

MicroSD::~MicroSD()
{
}


Led::Led(int pin)
{
    ledPin = pin;
}

void Led::initLed(){
    pinMode(ledPin, OUTPUT);
}

void Led::ledON(){
    digitalWrite(ledPin, HIGH);
    ledState = true;
}

void Led::ledOFF(){
    digitalWrite(ledPin, LOW);
    ledState = false;
}

void Led::toggleLed(){
    if (ledState)
    {
        digitalWrite(ledPin, LOW);
        ledState = !ledState;
    }
    else
    {
        digitalWrite(ledPin, HIGH);
        ledState = !ledState;
    }
}

Led::~Led()
{
}

Button::Button(int pin)
{
    buttonPin = pin;
}

void Button::initButton(){
    pinMode(buttonPin, INPUT);
}

void Button::toggleButtonState(){

    buttonState = !buttonState;
    Serial.println("Button state: " + String(buttonState));
}

void Button::checkButtonPress(){

    if(digitalRead(buttonPin) == 0){
        toggleButtonState();
        Serial.println("Button pressed");
        delay(1500);
    }

}

bool Button::getButtonState(){
    
    return buttonState;
}

Button::~Button()
{
}
