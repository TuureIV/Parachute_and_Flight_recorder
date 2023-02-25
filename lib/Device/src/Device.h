#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FileSerial.h>
#include <SD.h>

class MicroSD
{
private:
    char csvBuffer[300];
    int parsingState = 0;
    char parseBuffer[200];
    int parseBufferPtr = 0;

public:
    MicroSD(/* args */);
    ~MicroSD();
    bool initMicroSD();
    bool initLogFile(FileSerial logFile, const char * file_location, const char * file_name);

    void logData(FileSerial logFile);
};

// Built-in LED
class Led
{
private:
    int ledPin;
    bool ledState;

public:
    Led(int pin);
    void initLed();
    void toggleLed();
    void ledON();
    void ledOFF();

    ~Led();
    
};

class Button
{
private:
    int buttonPin;
    bool buttonState = false;
public:
    Button(int pin);
    void initButton();
    void checkButtonPress();
    void toggleButtonState();
    bool getButtonState();

    ~Button();
};


