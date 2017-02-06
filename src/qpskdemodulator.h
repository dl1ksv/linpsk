
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   based on the work of Moe Wheatley, AE4JY                              *
 ***************************************************************************/

#ifndef QPSKDEMODULATOR_H
#define QPSKDEMODULATOR_H

#include "cpskdemodulator.h"

/**Implementation of the QPsk demodulator
  *@author Volker Schroer
  */

class Viterbi;

class QPskDemodulator : public CPskDemodulator  {
public: 
  QPskDemodulator();
  ~QPskDemodulator();
  void Init(double Fs ,int BlockSize);

protected:	
  /** Decodes a QPSK Symbol */
  //void DecodeSymbol( complex<double> newsamp);
  void DecodeSymbol( double);
  double calcFreqError(complex<double> s);
  void CalcQuality(double);
private:
  double ave1,ave2;
  Viterbi *v;


};

#endif
