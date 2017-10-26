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

#### I2CWriteSingleByte(char I2CAddress, char Data)
This should be used when only 1 byte of data needs to be sent to an I2C Slave Device. I2CAddress is the 7 bit I2C Address of the desired Slave. Data is the one byte of data you wish to transmit.
```c
char SlaveAddress = 0x40;

I2CWriteSingleByte(SlaveAddress, 0xFF);
```

#### I2CWriteMultipleBytes(char I2CAddress, int NumOfBytes, char \*Data)
This function should be called when more than one byte needs to be transmitted to an I2C Slave device. I2CAddress is the 7-bit address of the Slave Device. NumOfBytes is the total number of bytes which need to be sent to the Slave. \*Data is the pointer to an array of char data meant to be sent to the slave.
```c
char SlaveAddress = 0x40;
char TXData[4] = {0x00, 0x22, 0x55, 0x88};

I2CWriteMultipleBytes(SlaveAddress, 4, &TXData);
```

### Read from Slave I2C Device

#### I2CReadSingleByte(char I2CAddress, char \*ReturnedData)
This function should be called if you want to read only one byte of data from a slave device. Instead of returning anything in the function, you need to instead pass in an address to save the data in.
```c
char SlaveAddress = 0x40;
char RXData = 0;

I2CReadSingleByte(SlaveAddress, &RXData);
```

#### I2CReadMultipleBytes(char I2CAddress, int NumOfBytes, char \*ReceivedData)
This function should be called if you want to read more than one byte of data from the Slave device. You will need to pass in the number of bytes you are expecting to receive as well as a pointer to where you want to store your received data.
```c
char SlaveAddress = 0x40;
char RXData[4];

I2CReadMultipleBytes(SlaveAddress, 4, &RXData);
```