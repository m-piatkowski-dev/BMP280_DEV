/*
  Device is an I2C/SPI compatible base class library.
	
	Copyright (C) Martin Lindupp 2019
	
	V1.0.0 -- Initial release 									

	The MIT License (MIT)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include <Device.h>

////////////////////////////////////////////////////////////////////////////////
// Device Class Constructors
////////////////////////////////////////////////////////////////////////////////

Device::Device() : comms(I2C_COMMS) {}															// Initialise constructor for I2C communications
Device::Device(uint8_t cs) : comms(SPI_COMMS), cs(cs), spiClockSpeed(1000000) {}		// Constructor for SPI communications

////////////////////////////////////////////////////////////////////////////////
// Device Public Member Function
////////////////////////////////////////////////////////////////////////////////

void Device::setClock(uint32_t clockSpeed)
{
	if (comms == I2C_COMMS)
	{
		Wire.setClock(clockSpeed);
	}
	else
	{
		spiClockSpeed = clockSpeed;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Device I2C & SPI Wrapper (Protected) Member Functions
////////////////////////////////////////////////////////////////////////////////

void Device::initialise()																						// Initialise device communications
{
  if (comms == I2C_COMMS)																						// Check with communications bus has been selected I2C or SPI
	{
		Wire.begin();																										// Initialise I2C communication
		Wire.setClock(400000);																					// Set the SCL clock to default of 400kHz
	}
	else
	{
		digitalWrite(cs, HIGH);																					// Pull the chip select (CS) pin high
		pinMode(cs, OUTPUT);																						// Set-up the SPI chip select pin
		SPI.begin();																										// Initialise SPI communication at 1MHz
	}
}

void Device::setI2CAddress(uint8_t addr)														// Set the Device's I2C address
{	
	address = addr;
}

void Device::writeByte(uint8_t subAddress, uint8_t data)
{
  if (comms == I2C_COMMS)
	{
		Wire.beginTransmission(address);  // Select the device I2C address
		Wire.write(subAddress);           // Specify the device sub (register) address
		Wire.write(data);                 // Specify the data to be sent
		Wire.endTransmission();           // Transmit
	}
	else // if (comms == SPI_COMMS)
	{
		SPI.beginTransaction(SPISettings(spiClockSpeed, MSBFIRST, SPI_MODE0));
		digitalWrite(cs, LOW);
		SPI.transfer(subAddress);
		SPI.transfer(data);
		digitalWrite(cs, HIGH);
		SPI.endTransaction();
	}
}

uint8_t Device::readByte(uint8_t subAddress)
{
  uint8_t data;
	if (comms == I2C_COMMS)
	{
		Wire.beginTransmission(address);         // Select the device I2C address
		Wire.write(subAddress);                  // Specify the device sub (register) address
		Wire.endTransmission(false);             // Transmit, but send a restart to keep connection alive
		Wire.requestFrom(address, (uint8_t)1);	 // Read one byte from slave register address 
		data = Wire.read();                      // Fill Rx buffer with result
	}
	else // if (comms == SPI_COMMS)
	{
		SPI.beginTransaction(SPISettings(spiClockSpeed, MSBFIRST, SPI_MODE0));
		digitalWrite(cs, LOW);
		SPI.transfer(subAddress);
		data = SPI.transfer(data);
		digitalWrite(cs, HIGH);
		SPI.endTransaction();	
	}
  return data;                             // Return data read from slave register
}

void Device::readBytes(uint8_t subAddress, uint8_t* dest, uint8_t count)
{  
  if (comms == I2C_COMMS)
	{
		Wire.beginTransmission(address);          // Initialize the Tx buffer
		Wire.write(subAddress);                   // Put slave register address in Tx buffer
		Wire.endTransmission(false);              // Send the Tx buffer, but send a restart to keep connection alive
		uint8_t i = 0;
		Wire.requestFrom(address, count);  // Read bytes from slave register address 
		while (Wire.available()) 
		{
			dest[i++] = Wire.read();          // Put read results in the Rx buffer
		}
	}
	else // if (comms == SPI_COMMS)
	{
		SPI.beginTransaction(SPISettings(spiClockSpeed, MSBFIRST, SPI_MODE0));
		digitalWrite(cs, LOW);
		SPI.transfer(subAddress);
		SPI.transfer(dest, count);
		digitalWrite(cs, HIGH);
		SPI.endTransaction();	
	}
}


