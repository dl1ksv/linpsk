/***************************************************************************
                          mfskdemodulator.h  -  description
                             -------------------
    begin                : Mit Jan 29 2003
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

#ifndef MFSKDEMODULATOR_H
#define MFSKDEMODULATOR_H

#include "cdemodulator.h"
#include <complex>
#include <vector>

using namespace std;
class Deinterleaver;
class Viterbi;
class MFSKVaricode;
/**
  *@author Volker Schroer
  */

#define DOWNSAMPLEDBUFFERLENGTH 256 // 4096 /16
#define DOWNRATE 16
#define DELAY 44 // = Samplerate / DOWNRATE /BAUDRATE = 11025/16/15.625
#define RESOLUTION 88 // 88 = 2* DELAY

//class MFSKDemodulator : public FSKDemodulator  {
class MFSKDemodulator : public CDemodulator  {
public: 
  MFSKDemodulator();
  ~MFSKDemodulator();
	/** Prozess the input */
	void ProcessInput(double * input,double *);
  int getSquelchValue();
	bool Init(double,int);
  virtual double get2RxFrequency();
private:
AfcMode AfcProperties();
void mixdown(double *);
void calcSoftBits(int pos, double *softBits);

	unsigned int datashreg;

MFSKVaricode *Decoder;

Deinterleaver * leave;
complex<double> *history;
Viterbi * viterbi;
void sfft(complex<double>);
void bitEstimate();
void afc(int,double);
void recvbit(int);
int processPtr,bitEstimatePtr,syncPtr,oscPos;
/******************************/
/** coeficients for slidding fft */
complex<double> twiddles[RESOLUTION];;
complex<double> downBuffer[DOWNSAMPLEDBUFFERLENGTH];
complex<double> delayLine[DELAY];
double energy[48][DELAY];

inline unsigned char graydecode(unsigned char data)
{
	return data ^ (data >> 1);
}
inline unsigned char clamp(double x)
{
	return (x < 0) ? 0 : ((x > 255) ? 255 :(unsigned char) x);
}
public slots:
void setRxFrequency(double);
};

#endif
