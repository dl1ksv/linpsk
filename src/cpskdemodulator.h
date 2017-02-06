/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   based on the work of Moe Wheatley, AE4JY                              *
 ***************************************************************************/


#ifndef CPSKDEMODULATOR_H
#define CPSKDEMODULATOR_H

using namespace std;
#include "constants.h"
#include <math.h>
#include <complex>
#include <stdlib.h>

#include "cdemodulator.h"

#define SYNCBUFFERLENGTH 20 // Just small oversample

class FIRFilter;

class CPskDemodulator  : public CDemodulator
{
 Q_OBJECT
public:
 CPskDemodulator();
 virtual ~CPskDemodulator();

 int getSquelchValue();
 void ProcessInput( double *pIn, double *Spectrum);
 complex<float> * getPhasePointer();
 virtual float getIMD();
 AfcMode AfcProperties();
	
protected:
// Methods
/** Decodes the symbol depending on the PskModes */	
 virtual void DecodeSymbol( double) = 0;
/** Calculates the Cuality of the signal -- depends on the mode */	
	virtual	void CalcQuality( double angle ) = 0;	
	
// Variables
  double m_LastFreq;
  complex<double> Prev_Sample;
  complex<double> Phase_Vector;

  bool m_LastBitZero;
  int m_BitAcc;
  int m_OnCount;
  int m_OffCount;

  unsigned char* m_VaricodeDecTbl;

  double m_DevAve;

  bool fastSquelch;
  FIRFilter *downFilter;
  int downRate;
  FIRFilter *syncFilter;
  float f1,f2;
  virtual double calcFreqError(complex<double>) =0;

/** Some Variable for CalcQuality */
  double x_loop_1,y_loop_1;

private:

  bool symbSync( complex<double> sample);


  void calcIMD(double *Spectrum);  
//variables

  int m_SampCnt;

// Phase Values
  complex<float> Phase[PHASE_DISPLAY_BUFFER_LENGTH];
  int phasePosition; // Position in phase display buffer

  double syncBuf[SYNCBUFFERLENGTH]; //  samples per symbol
  float m_BitPhasePos;

  float IMD;

public slots:
signals: // Signals

};

#endif
