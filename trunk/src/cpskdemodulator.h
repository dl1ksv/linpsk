/***************************************************************************
                          cpskdemodulator.h  -  description
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


#ifndef CPSKDEMODULATOR_H
#define CPSKDEMODULATOR_H

using namespace std;
#include "constants.h"
#include <math.h>
#include <complex>
#include <stdlib.h>

#include "cdemodulator.h"


class CPskDemodulator  : public CDemodulator
{
 Q_OBJECT
public:
 CPskDemodulator();
 virtual ~CPskDemodulator();
 bool Init(double Fs ,int BlockSize);
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
  complex<double> Prev_Sample;
  complex<double> Phase_Vector;
	int m_BitAcc;
	bool m_LastBitZero;
	unsigned char* m_VaricodeDecTbl;
	int m_OnCount;
	int m_OffCount;	
	double m_FreqError;
	double m_QFreqError;

  double m_DevAve;

  bool fastSquelch;
private:
	int m_ClkErrTimer;
	int m_ClkErrCounter;
	int m_ClkError;

//methods


	bool SymbSync(complex<double> );
	void CalcBitFilter(  complex<double>);
	double CalcAGC( complex<double> );
	double CalcFreqError(complex<double>);
  void calcIMD(double *Spectrum);  
//variables
	double m_QPSKprob[4];
	int m_LastPkPos;
	int m_SampCnt;
	bool m_DispTrig;
	float m_BitPhaseInc;
	float m_BitPhasePos;
//	float m_SyncAve[21];
        complex<float> DelayLine[40];
	float m_SignalLevel;

	complex<double>* m_pQue1;
	complex<double>* m_pQue2;
	complex<double>* m_pQue3;
	complex<double>* m_pInPtr1;
	complex<double>* m_pInPtr2;
	complex<double>* m_pInPtr3;

	complex<double> m_FreqSignal;
	complex<double> m_BitSignal;
// Phase Values
  complex<float> * Phase;
// Local variables for various functions that need to be saved between calls
	double m_AGCave;
	double m_LastFreq;
	int	m_PkPos;
	int m_BitPos;
float IMD;

/** Some Variable for CalcQuality */

double fe0,fe1,fe2;
double x_loop_1,y_loop_1;

public slots:
signals: // Signals

};

#endif
