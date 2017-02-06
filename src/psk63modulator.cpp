/***************************************************************************
 *   Copyright (C) 2012 -2017 by Volker Schroer, DL1KSV                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "psk63modulator.h"
#include "constants.h"
#include "parameter.h"
#include "ctxbuffer.h"

extern Parameter settings;

#define SYM_NOCHANGE 0 //Stay the same phase
#define SYM_P180 2  //Plus 180 deg


Psk63Modulator::Psk63Modulator(int FS, double frequency, CTxBuffer *TxBuffer):PskModulator(FS,frequency,TxBuffer)
{
  symbolSize=(((100 * FS) / 6250) + 1);
  periodTime=1./62.5;
  period=periodTime;
}
char Psk63Modulator::getNextSymbolBit()
{
  int bit;
  if(txShiftRegister != 0)
    {
      if(txShiftRegister &0x8000)
        bit=SYM_NOCHANGE;
      else
        bit=SYM_P180;
      txShiftRegister <<=1;
      if(txShiftRegister == 0)
        addEndingZero=true;
    }
  else
    {
      if(addEndingZero)
      {
        bit=SYM_P180;
        addEndingZero=false;
       }
      else
        {
          int ch;
          if((ch = getChar())>= 0)
            {       // No controlcode
              txShiftRegister = VARICODE_TABLE[ ch&0xFF ];
              bit=SYM_P180; //Start with a zero
             }
          else
            switch ( ch )
            {
              case TXON_CODE:
              case TXTOG_CODE:    //Idle
                bit = SYM_P180;
                break;
              case TXOFF_CODE:
                bit = SYM_NOCHANGE;
                break;
            }
        }
    }
  return bit;
}
