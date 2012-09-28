/***************************************************************************
                          bpskdemodulator.h  -  description
                             -------------------
    begin                : Sat Jun 2 2001
    copyright            : (C) 2001 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   based on the work of Moe Wheatley, AE4JY                              *
 ***************************************************************************/

#ifndef BPSKDEMODULATOR_H
#define BPSKDEMODULATOR_H

#include <complex>
#include "cpskdemodulator.h"


/**Implementation of the BPSK Demodulator
  *@author Volker Schroer
  */

class BPskDemodulator : public CPskDemodulator  {
public: 
	BPskDemodulator();
	~BPskDemodulator();
protected:	
/** Decodes a BPSK Symbol */
void DecodeSymbol( double);

void CalcQuality(double);
private:
	bool GetBPSKSymb();
  double ave1;
	double ave2;


};

#endif
