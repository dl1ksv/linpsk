/***************************************************************************
                          rtty2demodulator.h  -  description
                             -------------------
    begin                : Mon Jun 4 2001
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

#ifndef RTTYDEMODULATOR_H
#define RTTYDEMODULATOR_H

#define DISTANCE 22 
#define NUMBEROFPROBES 11 //242  // (11025 / 45.45 ) 

#define SAMPLEBUFFERLENGTH  120  // Mindestens (1.5 Stop + 1Start + NumberOfBits Data +1.5 Stop) * NUMBEROFPROBES
#define MaxSamplestoProcess 4096 // >=4096 / Distance

#define FFTLENGTH 4096 //2048
#define FFTFILTERLENGTH 511 //255
#define RESULTLENGTH 3586 //1794 // 5382  FFTLENGTH-FFTFILTERLENGTH+1
#define RESULTLENGTHDOWN 163 // RESULTLENGTH/DISTANCE

#include "cdemodulator.h"
#include <complex>
#include "constants.h"
#include "fftw3.h"
using namespace std;
/**Decodes RTTY 
  *@author Volker Schroer
  */

class RTTYDemodulator : public CDemodulator  {
public: 
 RTTYDemodulator();
 ~RTTYDemodulator();

/** Prozess the input */
 void ProcessInput(double * input, double *);
 int getSquelchValue();
 bool Init(double,int);
 virtual double get2RxFrequency();
 virtual void setParameter(RxTxParameterType,void * );
 virtual void *getParameter(RxTxParameterType);
 virtual void *getBuffer();
 AfcMode AfcProperties();
 void setFilter(double freq, double bandwidth);
 void execFilter();
 void downmix(complex<double> *,complex<double> *, int,int *,double *,double *);

public slots: // Public slots
/** Set RxFrequencies for RTTY */
 void setRxFrequency(double);

private: 	//Private Variables
 enum StateOfReception { WaitingForMark,WaitingForSpace, 
                         CheckingStartBit,CollectingByte,
                         CheckingParity, CheckingStopBits};

int Demod[SAMPLEBUFFERLENGTH];
double SF0[SAMPLEBUFFERLENGTH],SF1[SAMPLEBUFFERLENGTH];

double F0,F1,F0inc,F1inc;
double F0max,F1max;

int BufferPointer;
int BufferCount;
/** Pointer for storing Data in the ellipse Display **/


/** Status of Shift */
bool ShiftOn;

/** Baudrate */
float Baudrate;

/** Samples per bit */

unsigned int NumberOfBits;
/** Length of first Stopbit can't be local, as it might cross 
    input buffer boundary **/
int StopBitLength,StopBit1Count,StartBitLead;
double StopBit1Value;
/** Status of reception*/
StateOfReception Status;


/** Look at FSKDemodulator **/
unsigned int SymbolLength;

/** has detected Frequency changed ? */
bool FrequencyChanged;

/** Variables for Squelch computing */
float ave1,ave2;
 // Some extra paramters 
 ExtraParameter extraParameter;

/** Variables for Fast FFT  Filtering ( BP ) **/
fftw_plan pforward,pbackwardF0,pbackwardF1,pfilter;
complex<double> *pIn,*pOutF0,*pOutF1,*pFilterF0,*pFilterF1,*pFilterIn,*pResultF0,*pResultF1,*pOverlapF0,*pOverlapF1;
int filteredSamples;
private: // Private methods
 /** returns the asci char coresponding to the baudot code */
 char baudot_code(char);
  
 /** Calc the quality of the signal for squelch */
 ///void CalcQuality(int);
 void CalcQuality(float);
 complex<double> delayZ;
  };

#endif
