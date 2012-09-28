/***************************************************************************
                          bpskmodulator.cpp  -  description
                             -------------------
    begin                : Mon Feb 24 2003
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

#include "bpskmodulator.h"
#include "constants.h"

#define SYM_NOCHANGE 0 //Stay the same phase
#define SYM_P180 2  //Plus 180 deg
#define SYM_OFF 4  //No output
#define SYM_ON 5  //constant output

BPSKModulator::BPSKModulator ( int FS, double freq, CTxBuffer *TxBuffer ) : PSKModulator ( FS, freq, TxBuffer )
{
}
BPSKModulator::~BPSKModulator()
{
}
char BPSKModulator::GetNextSymbol ( void )
{
  char symb;
  int ch;
  symb = m_Lastsymb;
  if ( m_TxShiftReg == 0 )
  {
    if ( m_AddEndingZero ) // if is end of code
    {
      symb = SYM_P180;  // end with a zero
      m_AddEndingZero = false;
    }
    else
    {
      ch = GetChar();   //get next character to xmit
      if ( ch >= 0 ) //if is not a control code
      {
        //get next VARICODE codeword to send
        m_TxShiftReg = VARICODE_TABLE[ ch&0xFF ];
        symb = SYM_P180; //Start with a zero
      }
      else     // is a control code
      {
        switch ( ch )
        {
          case TXON_CODE:
            symb = SYM_ON;
            break;
          case TXTOG_CODE:
            symb = SYM_P180;
            break;
          case TXOFF_CODE:
            symb = SYM_OFF;
            break;
        }
      }
    }
  }
  else   // is not end of code word so send next bit
  {
    if ( m_TxShiftReg&0x8000 )
      symb = SYM_NOCHANGE;
    else
      symb = SYM_P180;
    m_TxShiftReg = m_TxShiftReg << 1; //point to next bit
    if ( m_TxShiftReg == 0 )  // if at end of codeword
      m_AddEndingZero = true;  // need to send a zero nextime
  }
  m_Lastsymb = symb;
  return symb;

}
