/***************************************************************************
                          qpskmodulator.h  -  description
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
 *   The PSK part is based on WinPSK 1.0 by Moe Wheatley, AE4JY            *              
 ***************************************************************************/

#ifndef QPSKMODULATOR_H
#define QPSKMODULATOR_H

#include "pskmodulator.h"
class CTxBuffer;
/**
  *@author Volker Schroer
  */

class QPskModulator : public PSKModulator  {
public: 
	QPskModulator(int, double, CTxBuffer *);
	~QPskModulator();
char GetNextSymbol(void);
private:

	unsigned short int m_TxCodeWord;
};

#endif
