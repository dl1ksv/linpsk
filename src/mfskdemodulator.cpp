/***************************************************************************
                          mfskdemodulator.cpp  -  description
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
 *   The PSK part is based on WinPSK 1.0 by Moe Wheatly, AE4JY             *
 ***************************************************************************/

#include "mfskdemodulator.h"
#include "deinterleaver.h"
#include "viterbi.h"
#include "constants.h"
#include "mfskvaricode.h"

#define NumberofTones 16

double MFSKDistance ( double *xa, int b )
{
	double dist;
	int a1, a2;
	a1 = ( b & 2 ) >> 1;
	a2 = b & 1;
	dist = abs ( xa[0] - a1 ) + abs ( xa[1] - a2 );
	return dist;

}

MFSKDemodulator::MFSKDemodulator() : CDemodulator()
{
	OszFrequency = 0.;
	Decoder = new MFSKVaricode();
	viterbi = new Viterbi ( 7, 0x6D, 0x4F, MFSKDistance );
	datashreg = 0;
}
MFSKDemodulator::~MFSKDemodulator()
{
	if ( viterbi != 0 )
		delete viterbi;
	if ( Decoder != 0 )
		delete Decoder;
}

void MFSKDemodulator::ProcessInput ( double * input, double * )
{
// Mix and downsample
	mixdown ( input ); // Result is now in downBuffer
	bitEstimate();

}

int MFSKDemodulator::getSquelchValue()
{

	return 0;
}
bool MFSKDemodulator::Init ( double FS, int NumberofSamples )
{
	int i;
	NxSamples = NumberofSamples;
	SampleRate = FS;
// OszFreqinc = ( RxFrequency - 1000. ) * PI2 / SampleRate;
	OszFreqinc = 2.*M_PI * ( RxFrequency -  4*SampleRate / ( DOWNRATE * DELAY ) ) / SampleRate;
	OszFrequency = 0.;
	leave = new Deinterleaver ( 10 );
	processPtr = 0;
	bitEstimatePtr = 0;
	syncPtr = 0;
	oscPos = 6;
	for ( i = 0;i < DELAY;i++ )
		delayLine[i] = complex<double> ( 0.0, 0.0 );
	for ( i = 0; i < RESOLUTION;i++ )
		twiddles[i] = complex<double> ( cos ( i *  M_PI / RESOLUTION ), - sin ( i  * M_PI / RESOLUTION ) );

	return true;
}


void MFSKDemodulator::afc ( int freq,double energy )
{
	if ( UseAfc == Off )
		return;
	if ( (( freq - oscPos ) > 30) && (energy > 10) )
	{
		oscPos = freq - 30;
		RxFrequency +=( oscPos-6 ) *7.8125 ;
		return;
	}
	if ( (freq < oscPos) && (energy > 10) )
	{
		oscPos = freq;
		RxFrequency+= ( oscPos-6 ) *7.8125 ;
	}

}

void MFSKDemodulator::recvbit ( int bit )
{
	int c;
	datashreg = ( datashreg << 1 );
	c = !! bit;
	datashreg = datashreg | c;

	/* search for "001" */
	c = datashreg & 7;
	if ( ( datashreg & 7 ) == 1 )
	{
		/* the "1" belongs to the next symbol */
		c = Decoder->decode ( datashreg >> 1 );
		if ( c != -1 )
			emit newSymbol ( ( char ) c );
		/* we already received this */
		datashreg = 1;
	}
}
double MFSKDemodulator::get2RxFrequency()
{
	return RxFrequency + NumberofTones*15.625;
}
AfcMode MFSKDemodulator::AfcProperties()
{
	return Narrow;
}
void MFSKDemodulator::mixdown ( double * inputBuffer )
{
	complex<double> accu;
	int index;
	index = 0;
	for ( int i = 0; i <= ( NxSamples - DOWNRATE );i += DOWNRATE )
	{
		accu = complex<double> ( 0., 0. );
		for ( int k = i;k < i + DOWNRATE;k++ )
		{
			accu += inputBuffer[k] * exp ( complex<double> ( 0., -OszFrequency ) );;
			OszFrequency += OszFreqinc;
			OszFrequency = fmod ( OszFrequency, M_PI + M_PI );
		}
		downBuffer[index] = accu;
		index++;
	}
}
void MFSKDemodulator::sfft ( complex<double> input )
{
	complex<double>  z;
	int i, j, index;

	/* calculate the energy */
	delayLine[processPtr] = input;
	if ( bitEstimatePtr >= 0 )
	{
		for ( i = 0; i < 48; i++ )
		{
			z = complex<double> ( 0., 0. );
			for ( j = DELAY - 1; j >= 0; j-- )
			{
				index = ( processPtr + j + 1 + DELAY ) % DELAY;
				z += delayLine[index] * twiddles[ ( j* ( 2+i ) ) %RESOLUTION];
			}
			energy[i][bitEstimatePtr] = abs ( z );
		}
	}
	processPtr = ( processPtr + 1 ) % DELAY;
}
void MFSKDemodulator::bitEstimate()
{
	double max ;
	int j, k, freq, pos;
	double softBits[4];;
	freq=pos=0;
// Now we do some sliding fft on the downsampled values
	for ( int i = 0;i < DOWNSAMPLEDBUFFERLENGTH;i++ )
	{
		sfft ( downBuffer[i] );
		bitEstimatePtr++;
		if ( bitEstimatePtr == DELAY )
		{
			bitEstimatePtr = 0;
			max = -1;
			for ( j = 0; j < 48; j++ )
			{
				for ( k = 5; k < DELAY;k++ )
				{
					if ( energy[j][k] > max )
					{
						freq = j;
						pos = k;
						max = energy[j][k];
					}
				}
			}
			afc ( freq,max );

			calcSoftBits ( pos, softBits );
			leave->deinterleave ( softBits );
			viterbi->decode ( &softBits[0] );
			recvbit ( viterbi->getbit ( 48 ) );
			viterbi->decode ( &softBits[2] );
			recvbit ( viterbi->getbit ( 48 ) );

			if ( pos > 3*DELAY / 4 )
				bitEstimatePtr = -DELAY / 8;
			else if ( pos < DELAY / 4 )
			{
				bitEstimatePtr = DELAY / 8;
				for ( j = 0; j < 48;j++ )
					for ( k = 0; k < bitEstimatePtr;k++ )
						energy[j][k] = energy[j][DELAY-bitEstimatePtr+k];
			}
		}
	}
}
void MFSKDemodulator::calcSoftBits ( int pos, double *softBits )
{
	int i, j;
	double x, sum;
	for ( i = 0;i < 4; i++ )
		softBits[i] = 0.;
	sum = 0.;
	for ( i = 0; i < 16;i++ )
	{
		j = graydecode ( i );
		x = energy[2*i+oscPos][pos];
		sum += x;
		softBits[0] += ( j & 8 ) ? x : 0.;
		softBits[1] += ( j & 4 ) ? x : 0.;
		softBits[2] += ( j & 2 ) ? x : 0.;
		softBits[3] += ( j & 1 ) ? x : 0.;
	}
	if ( sum > 0 )
		for ( i = 0; i < 4; i++ )
			softBits[i] /= sum;
}
void MFSKDemodulator::setRxFrequency ( double freq )
{
	RxFrequency = freq;
	OszFreqinc = 2.*M_PI * ( RxFrequency -  4*SampleRate / ( DOWNRATE * DELAY ) ) / SampleRate;
	oscPos = 6;
}
