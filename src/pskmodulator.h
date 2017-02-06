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

#ifndef PSKMODULATOR_H
#define PSKMODULATOR_H

#include "cmodulator.h"

class PskModulator : public CModulator
{
public:
  PskModulator(int FS, double frequency, CTxBuffer *TxBuffer);
  int CalcSignal(double *data,int BufferSize);

protected:
  virtual char getNextSymbolBit()=0;
  int getChar();

  double aktFrequency;
  double txFrequencyInc;
  unsigned short txShiftRegister;

  int symbolSize;
  int inSymbolPtr;
  int amblePtr;
  short int aktBit;
  enum TxStatus
  {
    TX_END_STATE,	//Xmitting should be stoped
    TX_OFF_STATE,	//TX is off, so we are receiving
    TX_SENDING_STATE,	//TX is sending text
    TX_PREAMBLE_STATE,	//TX sending starting preamble
    TX_POSTAMBLE_STATE	//TX sending ending posteamble
//  TX_TUNE_STATE	//TX is tuning mode
  };
  TxStatus status;
  enum PhaseState
  { //    old   , new
    ZZ,  // 0    , 0
    ZN,  // 0    , 90
    ZP,  // 0    , 180
    ZZ7, // 0    , 270
    NN , // 90   , 90
    NP , // 90   , 180
    NZ7, // 90   , 270
    NZ , // 90   , 0
    PZ,  // 180  , 0
    PN,  // 180  , 90
    PP,  // 180  , 180
    PZ7, // 180  , 270
    Z7Z, // 270  , 0
    Z7N, // 270  , 90
    Z7P, // 270  , 180
    Z7Z7,// 270  , 270
  };
  PhaseState pState;
  PhaseState transition[4][4];
  double periodTime;
  double periodDelta;
  double period;

  bool addEndingZero;
};

#endif // BPSKMODULATOR_H
