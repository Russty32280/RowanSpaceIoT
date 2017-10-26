/*
 * BasicI2C.h
 *
 *  Created on: Oct 26, 2017
 *      Author: Russty
 */

#ifndef BASICI2C_H_
#define BASICI2C_H_

#include <msp430.h>

void I2CInit();     // This will be adapted in future revisions to allow for multiple I2C peripherals
void I2CWriteSingleByte(char I2CAddress, char Data);
void I2CWriteMultipleBytes(char I2CAddress, int NumOfBytes, char *Data);
void I2CReadSingleByte(char I2CAddress, char *ReturnedData);
void I2CReadMultipleBytes(char I2CAddress, int NumOfBytes, char *ReceivedData);


#endif /* BASICI2C_H_ */
