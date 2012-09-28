/***************************************************************************
                          rttymodulator.cpp  -  description
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
 *    based on the work of  Moe Wheatly, AE4JY                             *  
 ***************************************************************************/

#include "rttymodulator.h"
//#include "parameter.h"
#include "ctxbuffer.h"

extern Parameter settings;

RTTYModulator::RTTYModulator(int FS,double frequency,CTxBuffer *TxBuffer) : CModulator(FS,TxBuffer)

{
Baudrate=45;
NumberofStopBits=Onepoint5;
SamplesPerBit=SampleRate/Baudrate;
f0=frequency;
f1=frequency+170.;
xr=1.0;
xi=0.0;
dr[0]=cos(f0*2.0*M_PI/SampleRate);
di[0]=sin(f0*2.0*M_PI/SampleRate);
dr[1]=cos(f1*2.0*M_PI/SampleRate);
di[1]=sin(f1*2.0*M_PI/SampleRate);



NxSamples=SamplesPerBit;
actBit=0;  // Startbit
BitsPerChar = 5;
BitinChar=0;
actChar=0;
status=TX_PREAMBLE_STATE;

actCharacterStatus=IGNORE;
secondchar =0;

//Initialize ExtraParameter
extraParameter.stopbits=Onepoint5;
extraParameter.parity=None;
extraParameter.reverse=true;
extraParameter.offset=170;
}
RTTYModulator::~RTTYModulator()
{

}

int RTTYModulator::CalcSignal(double *data,int BUFSIZE)
{
int i;
for(i=0;i<BUFSIZE;i++)
{
 if ( NxSamples >= SamplesPerBit) // get next bit , NxSamples will be reset in getNextBit
    actBit = getNextBit();
 if ( status == TX_END_STATE)
 {
  if (i >0) i--;
   return -i;
 }
 // Calculate next Sample
 data[i] = calcNextSample(actBit);
 NxSamples++;
}
return BUFSIZE;
}

int RTTYModulator::getNextBit()
{
int bit;

NxSamples =0;
switch(status)
	{
	case TX_PREAMBLE_STATE:       							
  	bit = 1;
//		if (BitinChar++ >= Baudrate)
		if (BitinChar++ >= 1)
				{
				status = TX_SENDING_STATE;
				TX_Status = Startbit;
				actChar = getChar();
				if (actChar < 0 )
					{
						if ( actChar == TXOFF_CODE )
							status = TX_END_STATE;
						else     // No character to transmit at the moment
							TX_Status = WaitingforChar;
					}
				}
		break;
	case TX_SENDING_STATE:
			switch(TX_Status)
				{
				case WaitingforChar:
					actChar = getChar();
					bit = 1;
					if ( actChar < 0 )
						{
							if ( actChar == TXOFF_CODE )
								status = TX_END_STATE;
							else
								TX_Status = Startbit;
								actChar = 0;
							break;
						}
					else
						TX_Status = Startbit; // We got a char to transmit, so start with startbit
																	// direct
				case Startbit:
					bit=0;
					BitinChar = 0;
					TX_Status = SendChar;
          break;
				case SendChar:
					if (BitinChar < BitsPerChar )
						{
							bit = actChar & 1;
							BitinChar++;
							actChar >>=1;
						}
					if (BitinChar == BitsPerChar)
							TX_Status = Stopbit;
				break;
				case Stopbit:
					switch (NumberofStopBits)
						{
						case One:
							NxSamples=0;
							break;
						case Onepoint5:
							NxSamples=-SamplesPerBit/2;
							break;
						case Two:
							NxSamples= - SamplesPerBit;
							break;
						}
///					actChar = getChar();
					bit = 1;
///					if ( actChar == TXOFF_CODE )
///						status = TX_END_STATE;
///					else
///					TX_Status = Startbit;
					TX_Status = WaitingforChar;

				break;
			  }
		break;
//	case TX_POSTAMBLE_STATE:
//		break;	
	case TX_TUNE_STATE:
		actChar = getChar();
		bit =0;
		if ( actChar == TXOFF_CODE)
				status = TX_END_STATE;
		break;
	case TX_END_STATE:
		bit=1;
		break;


	}


return bit;
}
double RTTYModulator::calcNextSample(unsigned int bit)
{
double temp;
temp = xr *dr[bit] - xi*di[bit];
xi = xr *di[bit] + xi*dr[bit];
xr = temp;
temp = 2.0 -(xr*xr+xi*xi);
xr *=temp;
xi *=temp;

return  xr;
}

int RTTYModulator::Char2Baudot(int character)
{
int value;
static const char symbols[128]=
	{
	0x00,				// NULL               0
	0x00,				// SOH   undefined    1
	0x00,				// STX       "        2
	0x00,				// ETX       "        3
	0x00,				// EOT	     "        4
	0x09,				// ENQ                5
	0x00,				// ACK	undefined     6
	0x0A,				// BEL                7
	0x08,				// BS			?						8
	0x00,				// HT		undefined			9
	0x02,				// LF                10
	0x00,				// VT		undefined		 11
	0x00,				// FF				"        12
	0x08,				// CR								 13
	0x00,				// SO		undefined    14
	0x00,				// SI				"        15
	0x00,				// DLE	undefined    16
	0x00,				// DC1			"				 17
	0x00,				// DC2			"				 18
	0x00,				// DC3			"        19
	0x00,				// DC4			"				 20
	0x00,				// NAK			"				 21		
	0x00,				// SYN			"				 22
	0x00,				// ETB			"				 23
	0x00,				// CAN			"        24
	0x00,				// EM 			"				 25
	0x00,				// SUB			"				 26		
	0x00,				// ESC			"				 27
	0x00,				// FS 			"				 28
	0x00,				// GS	 		  "        29
	0x00,				// RS 	 		"				 30
	0x00,				// US		  	"				 31		
	0x04,				// SPACE						 32
	0x00,				// !	   undefined	 33
	0x11,				// "								 34
	0x14,				// #								 35
	0x09,				// $								 36
	0x00,				// %		 undefinded	 37
	0x00,				// &				"        38
	0x05,				// '				"				 39
	0x0f,				// (								 40
	0x12,				// )								 41
	0x00,				// *			undefined  42
	0x11,				// +								 43
	0x0c,				// ,								 44
	0x03,				// -								 45
	0x1A,				// .								 46
	0x1d,				// /								 47
	0x16,				// 0								 48
	0x17,				// 1								 49
	0x13,				// 2								 50
	0x01,				// 3								 51
	0x0A,				// 4								 52
	0x10,				// 5								 53
	0x15,       // 6								 54			
 	0x07,				// 7								 55
	0x06,				// 8								 56
	0x18,				// 9								 57
	0x0E,				// :								 58
	0x1e,				// ;								 59
	0x00,				// <		undefined		 60
	0x1e,				// =			?					 61
	0x00,				// >		undefined 	 62
	0x19,				// ?								 63
	0x00,				// @		undefined		 64
	0x03,				// A								 65
	0x19,				// B								 66
	0x0E,				// C								 67
	0x09,				// D								 68
	0x01,				// E								 69
	0x0D,				// F								 70
	0x1A,				// G								 71
	0x14,				// H								 72
	0x06,				// I								 73
	0x0b,				// J								 74
	0x0F,				// K								 75
	0x12,				// L								 76
	0x1C,				// M								 77
	0x0C,				// N								 78
	0x18,				// O								 79
	0x16,				// P								 80
	0x17,				// Q								 81
	0x0A,				// R								 82
	0x05,				// S								 83
	0x10,				// T							   84
	0x07,				// U								 85
	0x1E,				// V								 86
	0x13,				// W								 87
	0x1D,				// X								 88
	0x15,				// Y								 89
	0x11,				// Z								 90
	0x00,				// [ 		undefined		 91
	0x00,				// \				"				 92
	0x00,				// ]				"				 93
	0x00,				// ^				"				 94
	0x00,				// _				"				 95
	0x00,				// '				"				 96
	0x03,				// A								 97
	0x19,				// B								 98
	0x0E,				// C								 99
	0x09,				// D								100
	0x01,				// E								101
	0x0D,				// F								102
	0x1A,				// G								103
	0x14,				// H								104
	0x06,				// I								105
	0x0b,				// J								106
	0x0F,				// K								107
	0x12,				// L								108
	0x1C,				// M								109
	0x0C,				// N								110
	0x18,				// O								111
	0x16,				// P								112
	0x17,				// Q								113
	0x0A,				// R								114
	0x05,				// S								115
	0x10,				// T							  116
	0x07,				// U								117
	0x1E,				// V								118
	0x13,				// W								119
	0x1D,				// X								120
	0x15,				// Y								121
	0x11,				// Z								122
	0x00,				// { 		undefined		123
	0x00,				// \				"				124
	0x00,				// }				"				125
	0x00,				// ~				"				126
	0x00				// DEL			"				127
	};
static const CharacterStatus shift[128] =
	{

	 	IGNORE,IGNORE,IGNORE,IGNORE,  // 4* IGNORE
		FIGS,
		IGNORE,
		FIGS,
		IGNORE,IGNORE,IGNORE,IGNORE,  // 26 * IGNORE
	 	IGNORE,IGNORE,IGNORE,IGNORE,
	 	IGNORE,IGNORE,IGNORE,IGNORE,
	 	IGNORE,IGNORE,IGNORE,IGNORE,
	 	IGNORE,IGNORE,IGNORE,IGNORE,
	 	IGNORE,IGNORE,IGNORE,IGNORE,
	 	IGNORE,IGNORE,
		FIGS,FIGS,FIGS,              // 3 * FIGS
		IGNORE,
		FIGS,FIGS,FIGS,FIGS,         // 4 * FIGS
		IGNORE,
		FIGS,FIGS,FIGS,FIGS,FIGS,    // 17 * FIGS
		FIGS,FIGS,FIGS,FIGS,FIGS,
		FIGS,FIGS,FIGS,FIGS,FIGS,
		FIGS,FIGS,
		IGNORE,
		FIGS,
		IGNORE,
		FIGS,
		IGNORE,
		LTRS,LTRS,LTRS,LTRS, 					//26 * LTRS
		LTRS,LTRS,LTRS,LTRS,
		LTRS,LTRS,LTRS,LTRS,
		LTRS,LTRS,LTRS,LTRS,
		LTRS,LTRS,LTRS,LTRS,
		LTRS,LTRS,LTRS,LTRS,
		LTRS,LTRS,
		IGNORE,IGNORE,IGNORE,IGNORE,  // 6* IGNORE
		IGNORE,IGNORE,
		LTRS,LTRS,LTRS,LTRS,					// 26 * LTRS
		LTRS,LTRS,LTRS,LTRS,
		LTRS,LTRS,LTRS,LTRS,
		LTRS,LTRS,LTRS,LTRS,
		LTRS,LTRS,LTRS,LTRS,
		LTRS,LTRS,LTRS,LTRS,
		LTRS,LTRS,
		IGNORE,IGNORE,IGNORE,IGNORE,   // 5 * IGNORE
		IGNORE

	};
switch (shift[character])
	{
	case IGNORE:
		value = symbols[character];
		break;
	case LTRS:
		if ( actCharacterStatus != LTRS)
			{
				value = 0x1f;
				secondchar = symbols[character];
				actCharacterStatus = LTRS;
			}
		else
			value = symbols[character];
		break;
	case FIGS:
		if ( actCharacterStatus != FIGS)
			{
				value = 0x1b;
				secondchar = symbols[character];
				actCharacterStatus = FIGS;
			}
		else
			value = symbols[character];
		break;
			
	}
return value;
}

int RTTYModulator::getChar()
{
int value;
char ch;
	
ch = 0;

if (secondchar != 0)
	{
		value = secondchar;
		secondchar = 0;
	}
else
	do
		{
			value =   Buffer->getTxChar();
				if ( value > 0 )
					{
						ch = value;
						if ( value > 127 )
						value = value -128;
					value = Char2Baudot(value);
	       }
		}   while ( value == 0 );
if (ch > 0)
	emit charSend(ch);
return value;
}

void RTTYModulator::setParameter(RxTxParameterType Type,void *Value)
{
 switch (Type)
 {
  case Reverse:
   extraParameter.reverse =  * (bool *) Value;
   break;
  case Offset:
   extraParameter.offset = * (int *) Value;
   break;
  case Parity:
   extraParameter.parity = * (Paritaet *) Value;
   break;
  case Extra:
   extraParameter = * (ExtraParameter *) Value;
   break; 
  default:
   break;
 }
 init(); 
}
void RTTYModulator::init()
{
 f1=f0+extraParameter.offset;
 if (!extraParameter.reverse)
 {
  float x; //Change Mark and Space Frequency
  x=f0;
  f0=f1;
  f1=x;
 }
 xr=1.0;
 xi=0.0;
 dr[0]=cos(f0*2.0*M_PI/SampleRate);
 di[0]=sin(f0*2.0*M_PI/SampleRate);
 dr[1]=cos(f1*2.0*M_PI/SampleRate);
 di[1]=sin(f1*2.0*M_PI/SampleRate); 
}
