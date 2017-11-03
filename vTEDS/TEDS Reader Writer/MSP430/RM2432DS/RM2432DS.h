/*
 * RM2432DS.h
 *
 *  Created on: Nov 2, 2017
 *      Author: Russty
 */

#ifndef RM2432DS_H_
#define RM2432DS_H_

void RM24ByteWrite(char I2CAddress, unsigned short MemoryAddress, char Data);
void RM24PageWrite(char I2CAddress, unsigned short PageAddress, char *Data);
void RM24CurrentAddressRead(char I2CAddress, char *Data);
void RM24RandomRead(char I2CAddress, unsigned short MemoryAddress, char *Data);
void RM24SequentialRead(char I2CAddress, unsigned short StartingAddress, int NumOfBytes, char *Data);


#endif /* RM2432DS_H_ */
