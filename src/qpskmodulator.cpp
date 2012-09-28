/***************************************************************************
                          qpskmodulator.cpp  -  description
                             -------------------
    begin                : Don Feb 27 2003
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

#include "qpskmodulator.h"
#include "constants.h"
#define SYM_OFF 4		//No output
#define SYM_ON 5		//constant output
// For the QPSK modulator/demodulator, rate 1/2 constraint length 5
//   convolutional FEC coding is used.
// The generator polynomials used are:
//  g1(x) = x^4 + x^3           + 1		= 0x19
//  g0(x) = x^4	+       x^2 + x + 1		= 0x17
//
//                                g1(x)
//              /----+--------+--------------------------+
//            /      |        |                          |
// symbol msb       ---      ---      ---      ---      ---
//                 | b4|<---| b3|<---| b2|<---| b1|<---| b0| <-- inverted data in
// symbol lsb       ---      ---      ---      ---      ---
//            \      |                 |        |        |
//              \----+-----------------+--------+--------+
//                                g0(x)
//
// Lookup table to get symbol from non-inverted data stream
static const unsigned char ConvolutionCodeTable[32] =
{
	2, 1, 3, 0, 3, 0, 2, 1,
	0, 3, 1, 2, 1, 2, 0, 3,
	1, 2, 0, 3, 0, 3, 1, 2,
	3, 0, 2, 1, 2, 1, 3, 0
};

QPskModulator::QPskModulator(int FS,double freq,CTxBuffer *TxBuffer):PSKModulator(FS,freq,TxBuffer)
{
m_TxCodeWord = 0;
}
QPskModulator::~QPskModulator()
{
}
char QPskModulator::GetNextSymbol(void)
{
char symb;
int ch;
	symb = ConvolutionCodeTable[m_TxShiftReg&0x1F];	//get next convolution code
	m_TxShiftReg = m_TxShiftReg<<1;
	if( m_TxCodeWord == 0 )			//need to get next codeword
	{
		if( m_AddEndingZero )		//if need to add a zero
		{
			m_AddEndingZero = false;	//end with a zero
		}
		else
		{
			ch = GetChar();			//get next character to xmit
			if( ch >=0 )			//if not a control code
			{						//get next VARICODE codeword to send
				m_TxCodeWord = VARICODE_TABLE[ ch&0xFF ];
			}
			else					//is a control code
			{
				switch( ch )
				{
				case TXON_CODE:
					symb = SYM_ON;
					break;
				case TXTOG_CODE:
					m_TxCodeWord = 0;
					break;
				case TXOFF_CODE:
					symb = SYM_OFF;
					break;
				}
			}
		}
	}
	else
	{
		if(m_TxCodeWord&0x8000 )
		{
			m_TxShiftReg |= 1;
		}
		m_TxCodeWord = m_TxCodeWord<<1;
		if(m_TxCodeWord == 0)
			m_AddEndingZero = true;	//need to add another zero
	}
	return symb;
}


