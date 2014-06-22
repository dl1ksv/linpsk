/***************************************************************************
                          mfskmodulator.cpp  -  description
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
 ***************************************************************************/

#include "mfskmodulator.h"
#include "mfskvaricode.h"
#include "ctxbuffer.h"
#include "interleaver.h"
#include "feccoder.h"

MFSKModulator::MFSKModulator ( int FS, double freq, CTxBuffer *TxBuffer ) : FSKModulator ( FS, TxBuffer )
{
	Encoder = new MFSKVaricode();
	leave = new Interleaver ( 10 );
	Fec = new FECCoder();
	Allbitsgone = true;
	bitcounter = 0;
	varicode = 0;
	NumberofTones = 16;
	postamblePtr = 0;
	preamblePtr = 5;
// ToneFrequencyr = new double[NumberofTones];
// ToneFrequencyi = new double[NumberofTones];
	Baudrate = 15.625;
	SamplesperSymbol = ( int ) ( SampleRate / Baudrate + 0.5 );
// Tonespacing has the same value like Baudrate so
	for ( unsigned int i = 0; i < NumberofTones;i++ )
	{
		ToneFrequencyr[i] = cos ( PI2 * ( freq + i * Baudrate ) / SampleRate );
		ToneFrequencyi[i] = sin ( PI2 * ( freq + i * Baudrate ) / SampleRate );
	}
	NxSamples = SamplesperSymbol;
}
MFSKModulator::~MFSKModulator()
{
	if ( Encoder != 0 )
		delete Encoder;
	if ( leave != 0 )
		delete leave;
	if ( Fec != 0 )
		delete Fec;
}
int MFSKModulator::getToneNumber()
{
	int ch;
	if ( postamblePtr == 1 )
		return  TXOFF_CODE;
	for ( ;; )
	{
		if ( Allbitsgone )
		{
			if ( postamblePtr > 1 )
			{
				postamblePtr--;
				ch = 0;
			}
			else if ( preamblePtr > 0 )
			{
				preamblePtr--;
				ch = 0;
			}
			else
			{
				ch = Buffer->getTxChar();
				if ( ch < 0 )
				{
					if ( ch == TXOFF_CODE )
//    {
						postamblePtr = 10;
//     ch=0;
//    }
//    else
					ch = 0; // <Nul>
				}
				else
					emit charSend ( ( char ) ch );
			}
			varicode = Encoder->encode ( ch );
			Allbitsgone = false ;
			bitposition = 0x0800;
			// find first bit from left
			while ( ! ( varicode & bitposition ) )
				bitposition = bitposition >> 1;

		}
		while ( bitposition  && ( bitcounter < 4 ) )
		{
			if ( varicode & bitposition )
				Fec->FECEncode ( 1 , &bit[bitcounter] );
			else
				Fec->FECEncode ( 0, &bit[bitcounter] );
			bitcounter += 2;
			bitposition = bitposition >> 1;
		}
		if ( bitposition == 0 )
			Allbitsgone = true;
		if ( bitcounter == 4 )
		{
			bitcounter = 0;
			return grayencode ( leave->interleave ( bit ) );
		}
	}
}


