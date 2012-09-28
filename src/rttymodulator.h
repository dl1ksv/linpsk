/***************************************************************************
                          rttymodulator.h  -  description
                             -------------------
    begin                : Tue Aug 21 2001
    copyright            : (C) 2001 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef RTTYMODULATOR_H
#define RTTYMODULATOR_H

#include "cmodulator.h"
#include "parameter.h"
#include "constants.h"

/**Implementation of the RTTY Modulator
  *@author Volker Schroer
  */
enum CharacterStatus {IGNORE,LTRS,FIGS};
enum RTTYStates {Startbit,SendChar,Stopbit,WaitingforChar};
class CTxBuffer;
class RTTYModulator : public CModulator
{
Q_OBJECT
public: 
	RTTYModulator(int FS,double freq,CTxBuffer *);
	~RTTYModulator();
int CalcSignal(double *data,int BufferSize);
virtual void setParameter(RxTxParameterType,void * );

private:
enum Status {TX_PREAMBLE_STATE,TX_SENDING_STATE,TX_END_STATE,TX_TUNE_STATE};

Status status;
unsigned int Baudrate;
StopBits NumberofStopBits;
int SamplesPerBit;
unsigned int BitsPerChar;
int NxSamples;
unsigned int actBit;
int actChar;
unsigned int BitinChar;
double xr,xi;
double dr[2],di[2];

double f0,f1; // Mark and Space frequencies
ExtraParameter extraParameter;
CharacterStatus actCharacterStatus;
RTTYStates TX_Status;
int secondchar;
int getNextBit();
/** int getNextBit() returns the next bit to be transmitted */
double calcNextSample(unsigned int bit);
/** calculates the next Sample value depending on bit */
int Char2Baudot(int);
int getChar();
void init();
};

#endif
