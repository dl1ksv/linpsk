//////////////////////////////////////////////////////////////////////
// PSKMod.h: interface for the CPSKMod class.
//
//////////////////////////////////////////////////////////////////////
//      PSK31/CW modulator
// Copyright 1999.    Moe Wheatley AE4JY  <ae4jy@mindspring.com>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//////////////////////////////////////////////////////////////////////
//
// Modified by Volker Schroer, DL1KSV, for use in LinPsk

#ifndef PSKMOD_H
#define PSKMOD_H

#include <qobject.h>
#include <ctype.h>

#include "ctxdisplay.h"
#include "cmodulator.h"



class CTxBuffer;

class PSKModulator  : public CModulator
{
   Q_OBJECT
public:
	PSKModulator(int,double,CTxBuffer *);
	virtual ~PSKModulator();
// PSK31 and CW modulator
	int CalcSignal( double* pData , int n);
/** length = CalcSignal (double *pData, int n)
		Reads tx  Buffer and calculates the signal values to be transmitted
		double *pData 	pointer to array for computed signal values
		n								length of array
		length 					number of calculated values , normally n
                    -length at the end of transmission   
*/

private:
	QString m_TestString;
	unsigned int m_AmblePtr;
	int m_Preamble[33];
	int m_Postamble[33];

// PSK31 and CW generator variables
	double m_t;
	int m_Ramp;
///	double m_RMSConstant;

	double m_PSKSecPerSamp;
	double m_PSKTime;
	double m_PSKPeriodUpdate;
	double m_PSKPhaseInc;


	double* m_pPSKtxI;
	double* m_pPSKtxQ;
	int m_PresentPhase;
	int m_CWState;
	int m_CWtimer;


// PSK31 and CW modulator private functions

//	char GetNextBPSKSymbol(void);
//	char GetNextQPSKSymbol(void);
virtual  char GetNextSymbol(void)=0;
	char GetNextCWSymbol(void);
protected:
	char m_Lastsymb;
	unsigned short int m_TxShiftReg;
	int GetChar();
	bool m_AddEndingZero;
static const unsigned short int VARICODE_TABLE[];
enum Status
{
TX_END_STATE,				//Xmitting should be stoped
TX_OFF_STATE,				//TX is off, so we are receiving
TX_SENDING_STATE,		//TX is sending text
TX_PAUSED_STATE ,		//TX is paused
TX_PREAMBLE_STATE,	//TX sending starting preamble
TX_POSTAMBLE_STATE,	//TX sending ending posteamble
TX_CWID_STATE,			//TX sending CW ID
TX_TUNE_STATE			//TX is tuning mode
};
Status status; 
public slots:
signals: // Signals
  /** Tx finished */
//  void finished();
//  void charSend(char);
};

#endif
