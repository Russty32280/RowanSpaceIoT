/*
 * RM2432DS.c
 *
 *  Created on: Nov 2, 2017
 *      Author: Russty
 */

#include "RM2432DS.h"
#include "BasicI2C.h"
#include <msp430.h>

void RM24ByteWrite(char I2CAddress, unsigned short MemoryAddress, char Data)
{
    char AddressMSB = (char)MemoryAddress>>8;   // Taking the MSB and casting into Char
    char AddressLSB = (char)MemoryAddress;      // Casting LSB into Char
    char Message[3] = {AddressMSB, AddressLSB, Data};   // Creating Payload for I2C Write
    I2CWriteMultipleBytes(I2CAddress, 3, Message);
}



void RM24PageWrite(char I2CAddress, unsigned short PageAddress, char *Data)
{
    char AddressMSB = (char)PageAddress>>8;     // Taking the MSB and casting into a Char
    char AddressLSB = (char)PageAddress;        // Taking LSB and casting into Char
    char Message[34] = {AddressMSB, AddressLSB};    // Generate a message array with the first two bytes being the address
    int i=0;
    for (i=0; i<32; i++)    // Populating the rest of the message array
    {
        Message[2+i] = *Data;   // Take the next point in the inputted array
        Data++;     // Move the pointer for the input array by 1
    }
    I2CWriteMultipleBytes(I2CAddress, 34, Message);    // We know that we are going to send 34 bytes since
                                                        // each page is 32 bytes and we need the memory address
}



void RM24CurrentAddressRead(char I2CAddress, char *Data)
{
    I2CReadSingleByte(I2CAddress, Data);   // Single I2C Read to receive a single Byte from the current address
}


void RM24RandomRead(char I2CAddress, unsigned short MemoryAddress, char *Data) // Should *Data be &Data?
{
    char AddressMSB = (char)MemoryAddress>>8;   // Taking the MSB and casting into a Char
    char AddressLSB = (char)MemoryAddress;      // Taking LSB and casting into Char
    char Message[2] = {AddressMSB, AddressLSB};      // Generate Message containing the two byte memory location
    I2CWriteMultipleBytes(I2CAddress, 2, Message); // Set the address pointer in the RM24 to the location desired
    I2CReadSingleByte(I2CAddress, Data);        // Read the data at that point. Address Pointer in RM24 will increment
}


void RM24SequentialRead(char I2CAddress, unsigned short StartingAddress, int NumOfBytes, char *Data)
{
    char AddressMSB = (char)StartingAddress>>8;   // Taking the MSB and casting into a Char
    char AddressLSB = (char)StartingAddress;      // Taking LSB and casting into Char
    char Message[2] = {AddressMSB, AddressLSB};      // Generate Message containing the two byte memory location
    I2CWriteMultipleBytes(I2CAddress, 2, Message); // Set the address pointer in the RM24 to the location desired
    I2CReadMultipleBytes(I2CAddress, NumOfBytes, Data); // Begin reading bytes from the starting address
}




