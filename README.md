# BMP280
An Arduino compatible, non-blocking, I2C/SPI library for the Bosch BMP280 barometer.

This fully featured BMP280 library offer the following features:

- Returns temperature, pressure and altitude information
- NORMAL or FORCED modes of operation
- I2C or SPI communications with configurable clock rates
- Non-blocking operation 
- In NORMAL mode returns results at the standby time interval
- Highly configurable, allows for configurable pressure and temperature oversampling, IIR filter and standby time

### __Version__

- Version 1.0.0 -- Intial version

### __Arduino Compatibility__

All Arduino boards

### __Installation__

After download simply un-zip the file and place the BMP280 directory in your ...Arduino/libraries folder. The Arduino folder is the one where your sketches are usually located.

### __Usage__

Simply include the BMP280.h file at the beginning of your sketch:

**_#include <BMP280.h>_**

For I2C communicaiton the BMP280 object is created (instantiated) without parameters:

**_BMP280 bmp280;			// Set up I2C communications_**

By default the library uses the BMP280's I2C address 0x77. (To change use the alternate I2C address: 0x76, see the begin() function below.

For SPI communication the chip select (CS) digital output pin is specified as an argument, for example digital pin 10:

**_BMP280 bmp280(10);			// Set up SPI communications_**

To initialise the bmp280 it is necessary to call the begin function with or without parameters. The parameters specify the starting mode, pressure and temperature oversampling, IIR filter and standby time options respectively:

**_bmp280.begin(SLEEP_MODE, OVERSAMPLING_X16, OVERSAMPLING_X2, IIR_FILTER_4, TIME_STANDBY_05MS);

Alternatively the begin function can be called with BMP280's alternate I2C address,

Another alternative is to simply call the begin function without any paremeters, this sets up the default configuration: SLEEP_MODE, pressure oversampling X16, temperature oversampling X2, IIR filter OFF and a standby time of 0.5ms:

**_bmp280.begin();		// Initialise the BMP280 with default configuration_**

After initialisation it is possible to change the configuration with the following functions:

**_bmp280.setPresOversamping(OVERSAMPING_X4);			// Options are OVERSAMPLING_SKIP, _X1, _X2, _X4, _X8, _X16_**

**_bmp280.setTempOversamping(OVERSAMPING_X4);			// Options are OVERSAMPLING_SKIP, _X1, _X2, _X4, _X8, _X16_**

**_bmp280.setIIRFilter(IIR_FILTER_16);			// Options are IIR_FILTER_OFF, _2, _4, _8, _16_**

**_bmp280.setTimeStandby(TIME_STANDBY_2000MS);			// Options are TIME_STANDBY_05MS, _62MS, _125MS, _250MS, _500MS, _1000MS, 2000MS, 4000MS_**

The BMP280 has 3 modes of operation: SLEEP_MODE, NORMAL_MODE and FORCED_MODE. 

SLEEP_MODE: puts the device into an inactive standby state. 

NORMAL_MODE: performs continuous conversions, separated by the standby time.

FORCED_MODE: performs a single conversion, returning to SLEEP_MODE upon completion.

To kick-off conversions in NORMAL_MODE:

**_bmp280.startNormalConversion();			// Start continous conversions, separated by the standby time_**

To perform a single oneshot conversion:

**_bmp280.startForcedConversion();			// Start a single oneshot conversion

To stop the conversion at anytime and return to sleep mode:

**_bmp280.stopConversion();			// Stop conversion and return to SLEEP_MODE

To acquire the results from the conversion, there are number of functions that return the either the temperature, pressure or altitude or all three measurements together. The float temperature, pressure and altitude parameters are passed by reference to the function and contain the results once the function has completed. The function's return value indicates if 


Configuration

Data Acquisition

### __Example Code__

I2C Normal Mode sampling at 2s standby time intervals with default settings

I2C Forced Mode with default settings

SPI Normal Mode sampling at 1s standby time intervals with custom settings

SPI Forced Mode with custom settings



