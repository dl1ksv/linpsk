/***************************************************************************
                          mfskmodulator.h  -  description
                             -------------------
    begin                : Fre Feb 28 2003
    copyright            : (C) 2003 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   The PSK part is based on WinPSK 1.0 by Moe Wheatley, AE4JY            *              
 ***************************************************************************/

#ifndef MFSKMODULATOR_H
#define MFSKMODULATOR_H

#include <fskmodulator.h>


class MFSKVaricode;
class CTxBuffer;
class Interleaver;
class FECCoder;
/**
  *@author Volker Schroer
  */
class CTxBuffer;
class MFSKModulator : public FSKModulator
 {
public: 
	MFSKModulator(int,double,CTxBuffer *);
	~MFSKModulator();
int getToneNumber();


private:
MFSKVaricode * Encoder;
Interleaver * leave;
FECCoder *Fec;
bool Allbitsgone;
unsigned int bitcounter;
unsigned int bitposition;
unsigned char bit[4];
unsigned int varicode;
int postamblePtr;
int preamblePtr;
inline unsigned char grayencode(unsigned char data)
{
	unsigned char bits = data;

	bits ^= data >> 1;
	bits ^= data >> 2;
	bits ^= data >> 3;
	bits ^= data >> 4;
	bits ^= data >> 5;
	bits ^= data >> 6;
	bits ^= data >> 7;

	return bits;
}
};

#endif
