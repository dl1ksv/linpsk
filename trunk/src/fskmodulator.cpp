/***************************************************************************
                          fskmodulator.cpp  -  description
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
 ***************************************************************************/

#include "fskmodulator.h"
#include "constants.h"

FSKModulator::FSKModulator(int FS,CTxBuffer * TxBuffer ):CModulator(FS,TxBuffer)
{
NxSamples=0;
NumberofTones=0;
Frequencyr=1.;
Frequencyi=0.;

}
FSKModulator::~FSKModulator()
{
}
int FSKModulator::CalcSignal( double* pData , int Bufsize)
{
 double temp;

for(int i=0;i<Bufsize;i++)
 {
  if (NxSamples >=SamplesperSymbol)
   {
    c=getToneNumber();
    NxSamples=0;
   }
  if ( c < 0)
   {
    if (i > 0) 
    {
     i--; // We should stop transmission
     return -i;
    }  
    else
     return -1; 
   }
 // Calculate next Sample
temp = Frequencyr *ToneFrequencyr[c] - Frequencyi*ToneFrequencyi[c];
Frequencyi = Frequencyr *ToneFrequencyi[c] + Frequencyi*ToneFrequencyr[c];
Frequencyr = temp;
temp = 0.95*(2.0 -(Frequencyr*Frequencyr+Frequencyi*Frequencyi));
Frequencyr *=temp;
Frequencyi *=temp;

  pData[i] = Frequencyr;
  NxSamples++;
 }
return Bufsize;

}

