/***************************************************************************
                          fskmodulator.h  -  description
                             -------------------
    begin                : Sam Mär 1 2003
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

#ifndef FSKMODULATOR_H
#define FSKMODULATOR_H

#include "cmodulator.h"

/**
  *@author Volker Schroer
  */

class FSKModulator : public CModulator  {
public: 
	FSKModulator(int,CTxBuffer *);
	~FSKModulator();
	int CalcSignal( double* pData , int n);
protected:
virtual int getToneNumber()=0;
unsigned int NumberofTones;
//double *ToneFrequencyr;
//double *ToneFrequencyi;
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
