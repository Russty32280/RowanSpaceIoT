/*
 * BasicI2C.c
 *
 *  Created on: Oct 26, 2017
 *      Author: Russty
 */

/** This code is meant to act as a simple test of the I2C capabilities
 * of the G2553. This will hopefully be able to take us into making a
 * simple I2C driver library. This is not going to care about LPM, and all
 * functions will be implemented using polling.
 */


#include "BasicI2C.h"


void I2CInit()
{
    P1SEL |= BIT6 + BIT7;       // Assign our GPIO pins to be used
    P1SEL2 |= BIT6 + BIT7;      // for I2C
    UCB0CTL1 |= UCSWRST;        // Enable SW Reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;   // Set USCB0 to Master I2C Synchronous Mode

    UCB0BR0 = 12;               // Set our clock prescaler for 100kHz
    UCB0BR1 = 0;                // SMCLK/12 is roughly 100kHz
}


void I2CWriteSingleByte(char I2CAddress, char Data)
{
    UCB0I2CSA = I2CAddress;         // Set the target slave address
    UCB0CTL1 &= ~UCSWRST;           // Clear the SW Reset (may not be needed?)
    UCB0CTL1 |= UCTR + UCTXSTT;     // Set the USCB0 Peripheral to TX and send Start Byte
    while (UCB0CTL1 & UCTXSTT);     // Hold up until Start condition is generated and first data byte can be written into TXBUF
    UCB0TXBUF = Data;               // Set TX Buffer to data
    while (UCB0CTL1 & UCTXSTT);     // Wait until we are able to send another piece of data
    UCB0CTL1 |= UCTXSTP;            // Generate our stop condition after the next acknowledge
}

void I2CWriteMultipleBytes(char I2CAddress, int NumOfBytes, char *Data)
{
    int CurrentByte = 1;
    UCB0I2CSA = I2CAddress;         // Set the target slave address
    UCB0CTL1 &= ~UCSWRST;          // Clear the SW Reset (may not be needed)
    UCB0CTL1 |= UCTR + UCTXSTT;     // Set the USCB0 Peripheral to TX and send Start Byte
    //while (~(IFG2 & UCB0TXIFG));    // Hold up until Start condition is generated and first data byte can be written into TXBUF
    while (CurrentByte <= NumOfBytes)   // While there are bytes to send
    {
        while (~(IFG2 & UCB0TXIFG));    // Wait until we are ready to send a byte
        UCB0TXBUF = *Data;          // Set TXBUF to the current piece of data
        CurrentByte++;              // Increment our internal byte counter
        *Data++;                    // Move to the next point of data
    }
    while (UCB0CTL1 & UCTXSTT);    // Wait until we can send our stop bit
    UCB0CTL1 |= UCTXSTP;            // Generate our stop condition after the next acknowledge
}

void I2CReadSingleByte(char I2CAddress, char *ReceivedData)
{
    UCB0I2CSA = I2CAddress;         // Set the target slave address
    UCB0CTL1 &= ~UCSWRST;          // Clear the SW Reset (again, may not be necessary)
    UCB0CTL1 &= ~UCTR;              // Set our I2C Master to Receive Mode for Reading
    UCB0CTL1 |= UCTXSTT;            // Send out our start condition
    while (UCB0CTL1 & UCTXSTT);     // Did the start get sent?
    UCB0CTL1 |= UCTXSTP;            // Send stop sequence after the next recieved byte of data
    *ReceivedData = UCB0RXBUF;      // Save the Received Char to the provided location
}

void I2CReadMultipleBytes(char I2CAddress, int NumOfBytes, char *ReceivedData)
{
    int CurrentByte = 1;            // We need to track how many bytes we have received
    UCB0I2CSA = I2CAddress;         // Set the target slave address
    UCB0CTL1 &= ~UCSWRST;          // Clear the SW Reset (again, may not be necessary)
    UCB0CTL1 &= ~UCTR;              // Set our I2C Master to Receive Mode for Reading
    UCB0CTL1 |= UCTXSTT;            // Send out our start condition
    while (CurrentByte <= NumOfBytes)    // We can run this until the last byte needs to be read
    {
        while (~(IFG2 & UCB0RXIFG));    // Wait until we receive a character
        *ReceivedData = UCB0RXBUF;  // Load Received Byte into the current address
        CurrentByte++;
        *ReceivedData++;

        if (CurrentByte == NumOfBytes)  // If we are receiving the last byte, we need to send our stop condition
        {
            UCB0CTL1 |= UCTXSTP;    // Send a NACK and Stop after the next received byte
        }
    }
}
