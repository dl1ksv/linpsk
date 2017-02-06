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

#ifndef PSK63MODULATOR_H
#define PSK63MODULATOR_H

#include "pskmodulator.h"

class Psk63Modulator : public PskModulator
{
public:
  Psk63Modulator(int FS, double frequency, CTxBuffer *TxBuffer);

protected:
  char getNextSymbolBit();
};

#endif // BPSKMODULATOR_H
