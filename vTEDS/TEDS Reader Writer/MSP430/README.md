# Basic I2C Library for the MSP430G2553
This library is meant to provide functions for performing the basic I2C functions of Reading and Writing to a slave.

## Dependencies
This library requires you to have included the MSP430 header file (or your board equivalent) as well as initializing the UCB0 peripheral.
```c
#include <msp430.h>

void I2CInit()
{
    P1SEL |= BIT6 + BIT7;       // Assign our GPIO pins to be used
    P1SEL2 |= BIT6 + BIT7;      // for I2C
    UCB0CTL1 |= UCSWRST;        // Enable SW Reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;   // Set USCB0 to Master I2C Synchronous Mode
    
    UCB0BR0 = 12;               // Set our clock prescaler for 100kHz
    UCB0BR1 = 0;                // SMCLK/12 is roughly 100kHz
}
```

## Peripherals Utilized
Until this library has been modified, this library will utilize the UCB0 peripheral. This will cause conflicts in any existing system already using this peripheral.

## Functions
The functions exposed by this library can be broken into two main categories: Read and Write.

### Write to Slave I2C Device

#### void I2CWriteSingleByte(char I2CAddress, char Data)
This should be used when only 1 byte of data needs to be sent to an I2C Slave Device. I2CAddress is the 7 bit I2C Address of the desired Slave. Data is the one byte of data you wish to transmit.
```c
SlaveAddress = 0x40;

I2CWriteSingleByte(SlaveAddress, 0xFF);
```

#### void I2CWriteMultipleBytes(char I2CAddress, int NumOfBytes, char \*Data)
This function should be called when more than one byte needs to be transmitted to an I2C Slave device. I2CAddress is the 7-bit address of the Slave Device. NumOfBytes is the total number of bytes which need to be sent to the Slave. \*Data is the pointer to an array of char data meant to be sent to the slave.
```c
SlaveAddress = 0x40;
TXData[4] = {0x00, 0x22, 0x55, 0x88};

I2CWriteMultipleBytes(SlaveAddress, 4, &TXData);
```
