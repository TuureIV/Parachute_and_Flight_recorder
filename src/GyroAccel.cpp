#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

LSM9DS1 imu;  // Create an LSM9DS1 object

// Mag address must be 0x1E, would be 0x1C if SDO_M is LOW
#define LSM9DS1_M   0x1E
// Accel/gyro address must be 0x6B, would be 0x6A if SDO_AG is LOW
#define LSM9DS1_AG  0x6B



class GyroAccelMag
{
private:
    // Global variables to keep track of update rates
    unsigned long startTime;
    unsigned int accelReadCounter = 0;
    unsigned int gyroReadCounter = 0;
    unsigned int magReadCounter = 0;
    unsigned int tempReadCounter = 0;
    void setupGyro(boolean flipX, boolean flipY, boolean flipZ)
    {
        // [enabled] turns the gyro on or off.
        imu.settings.gyro.enabled = true;  // Enable the gyro
        // [scale] sets the full-scale range of the gyroscope.
        // scale can be set to either 245, 500, or 2000
        imu.settings.gyro.scale = 245; // Set scale to +/-245dps
        // [sampleRate] sets the output data rate (ODR) of the gyro
        // sampleRate can be set between 1-6
        // 1 = 14.9    4 = 238
        // 2 = 59.5    5 = 476
        // 3 = 119     6 = 952
        imu.settings.gyro.sampleRate = 3; // 59.5Hz ODR
        // [bandwidth] can set the cutoff frequency of the gyro.
        // Allowed values: 0-3. Actual value of cutoff frequency
        // depends on the sample rate. (Datasheet section 7.12)
        imu.settings.gyro.bandwidth = 0;
        // [lowPowerEnable] turns low-power mode on or off.
        imu.settings.gyro.lowPowerEnable = false; // LP mode off
        // [HPFEnable] enables or disables the high-pass filter
        imu.settings.gyro.HPFEnable = true; // HPF disabled
        // [HPFCutoff] sets the HPF cutoff frequency (if enabled)
        // Allowable values are 0-9. Value depends on ODR.
        // (Datasheet section 7.14)
        imu.settings.gyro.HPFCutoff = 1; // HPF cutoff = 4Hz
        // [flipX], [flipY], and [flipZ] are booleans that can
        // automatically switch the positive/negative orientation
        // of the three gyro axes.
        imu.settings.gyro.flipX = flipX; // Vaihdettu muuttujiksi
        imu.settings.gyro.flipY = flipY; // Vaihdettu muuttujiksi
        imu.settings.gyro.flipZ = flipZ; // Vaihdettu muuttujiksi
    }       
    void setupAccel()
    {
        // [enabled] turns the acclerometer on or off.
        imu.settings.accel.enabled = true; // Enable accelerometer
        // [enableX], [enableY], and [enableZ] can turn on or off
        // select axes of the acclerometer.
        imu.settings.accel.enableX = true; // Enable X
        imu.settings.accel.enableY = true; // Enable Y
        imu.settings.accel.enableZ = true; // Enable Z
        // [scale] sets the full-scale range of the accelerometer.
        // accel scale can be 2, 4, 8, or 16
        imu.settings.accel.scale = 16; // Set accel scale to +/-8g.
        // [sampleRate] sets the output data rate (ODR) of the
        // accelerometer. ONLY APPLICABLE WHEN THE GYROSCOPE IS
        // DISABLED! Otherwise accel sample rate = gyro sample rate.
        // accel sample rate can be 1-6
        // 1 = 10 Hz    4 = 238 Hz
        // 2 = 50 Hz    5 = 476 Hz
        // 3 = 119 Hz   6 = 952 Hz
        imu.settings.accel.sampleRate = 6; // Set accel to 10Hz.
        // [bandwidth] sets the anti-aliasing filter bandwidth.
        // Accel cutoff freqeuncy can be any value between -1 - 3. 
        // -1 = bandwidth determined by sample rate
        // 0 = 408 Hz   2 = 105 Hz
        // 1 = 211 Hz   3 = 50 Hz
        imu.settings.accel.bandwidth = 0; // BW = 408Hz
        // [highResEnable] enables or disables high resolution 
        // mode for the acclerometer.
        imu.settings.accel.highResEnable = false; // Disable HR
        // [highResBandwidth] sets the LP cutoff frequency of
        // the accelerometer if it's in high-res mode.
        // can be any value between 0-3
        // LP cutoff is set to a factor of sample rate
        // 0 = ODR/50    2 = ODR/9
        // 1 = ODR/100   3 = ODR/400
        imu.settings.accel.highResBandwidth = 0;  
    }
    void setupMag()
    {
        // [enabled] turns the magnetometer on or off.
        imu.settings.mag.enabled = true; // Enable magnetometer
        // [scale] sets the full-scale range of the magnetometer
        // mag scale can be 4, 8, 12, or 16
        imu.settings.mag.scale = 12; // Set mag scale to +/-12 Gs
        // [sampleRate] sets the output data rate (ODR) of the
        // magnetometer.
        // mag data rate can be 0-7:
        // 0 = 0.625 Hz  4 = 10 Hz
        // 1 = 1.25 Hz   5 = 20 Hz
        // 2 = 2.5 Hz    6 = 40 Hz
        // 3 = 5 Hz      7 = 80 Hz
        imu.settings.mag.sampleRate = 5; // Set OD rate to 20Hz
        // [tempCompensationEnable] enables or disables 
        // temperature compensation of the magnetometer.
        imu.settings.mag.tempCompensationEnable = false;
        // [XYPerformance] sets the x and y-axis performance of the
        // magnetometer to either:
        // 0 = Low power mode      2 = high performance
        // 1 = medium performance  3 = ultra-high performance
        imu.settings.mag.XYPerformance = 3; // Ultra-high perform.
        // [ZPerformance] does the same thing, but only for the z
        imu.settings.mag.ZPerformance = 3; // Ultra-high perform.
        // [lowPowerEnable] enables or disables low power mode in
        // the magnetometer.
        imu.settings.mag.lowPowerEnable = false;
        // [operatingMode] sets the operating mode of the
        // magnetometer. operatingMode can be 0-2:
        // 0 = continuous conversion
        // 1 = single-conversion
        // 2 = power down
        imu.settings.mag.operatingMode = 0; // Continuous mode
}
    void setupTemperature()
{
    // [enabled] turns the temperature sensor on or off.
    imu.settings.temp.enabled = true;
}
public:
    GyroAccelMag(/* args */);
    uint16_t initSensors(boolean flipX, boolean flipY, boolean flipZ)
    {   
        boolean _flipX = flipX;
        boolean _flipY = flipY;
        boolean _flipZ = flipZ;
        //setupDevice(); // Setup general device parameters
        setupGyro(_flipX, _flipY, _flipZ); // Set up gyroscope parameters
        setupAccel(); // Set up accelerometer parameters
        setupMag(); // Set up magnetometer parameters
        setupTemperature(); // Set up temp sensor parameter

        return imu.begin(LSM9DS1_AG, LSM9DS1_M, Wire);
    }

    void printSensorReadings();

    ~GyroAccelMag();
};

GyroAccelMag::GyroAccelMag(/* args */)
{
}

GyroAccelMag::~GyroAccelMag()
{
}





void setup() 
{
Serial.begin(115200);

Wire.begin();

Serial.println("Initializing the LSM9DS1");
uint16_t status = initLSM9DS1();
Serial.print("LSM9DS1 WHO_AM_I's returned: 0x");
Serial.println(status, HEX);
Serial.println("Should be 0x683D");
Serial.println();

startTime = millis();
}

void loop() 
{  
// imu.accelAvailable() returns 1 if new accelerometer
// data is ready to be read. 0 otherwise.
if (imu.accelAvailable())
{
    imu.readAccel();
    accelReadCounter++;
}

// imu.gyroAvailable() returns 1 if new gyroscope
// data is ready to be read. 0 otherwise.
if (imu.gyroAvailable())
{
    imu.readGyro();
    gyroReadCounter++;
}

// imu.magAvailable() returns 1 if new magnetometer
// data is ready to be read. 0 otherwise.
if (imu.magAvailable())
{
    imu.readMag();
    magReadCounter++;
}

// imu.tempAvailable() returns 1 if new temperature sensor
// data is ready to be read. 0 otherwise.
if (imu.tempAvailable())
{
    imu.readTemp();
    tempReadCounter++;
}

// Every PRINT_RATE milliseconds, print sensor data:
if ((lastPrint + PRINT_RATE) < millis())
{
    printSensorReadings();
    lastPrint = millis();
}
}

// printSensorReadings prints the latest IMU readings
// along with a calculated update rate.
void printSensorReadings()
{
float runTime = (float)(millis() - startTime) / 1000.0;
float accelRate = (float)accelReadCounter / runTime;
float gyroRate = (float)gyroReadCounter / runTime;
float magRate = (float)magReadCounter / runTime;
float tempRate = (float)tempReadCounter / runTime;
// Serial.print("A: ");
// Serial.print(imu.calcAccel(imu.ax));
// Serial.print(", ");
// Serial.print(imu.calcAccel(imu.ay));
// Serial.print(", ");
// Serial.print(imu.calcAccel(imu.az));
// Serial.print(" g \t| ");
// Serial.print(accelRate);
// Serial.println(" Hz");
// 
Serial.print("Gx: ");
Serial.println(imu.calcGyro(imu.gx));
Serial.print("Gy: ");
Serial.println(imu.calcGyro(imu.gy));
Serial.print("Gz: ");
Serial.print(imu.calcGyro(imu.gz));
// Serial.print(" dps \t| ");
// Serial.print(gyroRate);
// Serial.println(" Hz");
// Serial.print("M: ");
// Serial.print(imu.calcMag(imu.mx));
// Serial.print(", ");
// Serial.print(imu.calcMag(imu.my));
// Serial.print(", ");
// Serial.print(imu.calcMag(imu.mz));
// Serial.print(" Gs \t| ");
// Serial.print(magRate);
// Serial.println(" Hz");
// Serial.print("T: ");
// Serial.print(imu.temperature);
// Serial.print(" \t\t\t| ");
// Serial.print(tempRate);
// Serial.println(" Hz");  
// Serial.println();
}