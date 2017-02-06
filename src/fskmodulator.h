/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef FSKMODULATOR_H
#define FSKMODULATOR_H

#include "cmodulator.h"


class FSKModulator : public CModulator  {
public: 
	FSKModulator(int,CTxBuffer *);
	~FSKModulator();
	int CalcSignal( double* pData , int n);
protected:
virtual int getToneNumber()=0;
unsigned int NumberofTones;

double ToneFrequencyr[16];
double ToneFrequencyi[16];
double Frequencyr,Frequencyi;

unsigned int SamplesperSymbol;
float Baudrate;
unsigned int NxSamples;
private:
int c;
};

#endif
