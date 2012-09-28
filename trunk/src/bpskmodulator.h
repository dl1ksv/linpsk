/***************************************************************************
                          bpskmodulator.h  -  description
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
 *   The PSK part is based on WinPSK 1.0 by Moe Wheatley, AE4JY            *              
 ***************************************************************************/

#ifndef BPSKMODULATOR_H
#define BPSKMODULATOR_H

#include "pskmodulator.h"

class CTxBuffer;
/**
  *@author Volker Schroer
  */

class BPSKModulator : public PSKModulator
  {
public: 
	BPSKModulator(int, double, CTxBuffer *);
	~BPSKModulator();
char GetNextSymbol(void);
};

#endif
