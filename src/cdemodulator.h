/***************************************************************************
                          cdemodulator.h  -  description
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
 ***************************************************************************/

#ifndef CDEMODULATOR_H
#define CDEMODULATOR_H

#include <complex>
#include <qobject.h>
//#include <cstdio>
#include "constants.h"
/**Base class for all possible types of demodulators
  *@author Volker Schroer
  */

class CDemodulator : public QObject
 {
 Q_OBJECT
public:
 
 CDemodulator();

 virtual ~CDemodulator();
 /** processes the input */
 virtual void ProcessInput(double *input, double *spectrum) = 0;
 /** gets the selected frequency */
 virtual double getRxFrequency(void);
 virtual double get2RxFrequency(void);
 /** Initialises something */
 virtual bool Init(double,int) = 0;
 virtual std::complex<float> * getPhasePointer();
 virtual int getSquelchValue() = 0;
 virtual AfcMode getAfcMode();
 virtual AfcMode AfcProperties() = 0;
 void setAfcMode(AfcMode);
 virtual bool getSquelchState();
 virtual int getThreshold();
 virtual float getIMD();
 virtual void setParameter(RxTxParameterType,void *);
 virtual void *getParameter(RxTxParameterType);
 virtual void *getBuffer();
protected:
/** Status of AFC */
  AfcMode UseAfc;
  /** Status of Squelch **/
  bool Squelch;
  unsigned int Threshold;
	/** Frequency to be received */
  double RxFrequency;

  /** Number of Samples **/
  int NxSamples;
  /** Oszilatorfrequency **/
  double OszFrequency;
  /** Increment of Oszilator Frequency **/
  double OszFreqinc;
  /** Sample Rate **/
  double SampleRate;

public slots: //Slots
  /** enables/ disables the use of AFC */
  /** sets the frequency for the demodulator */
  virtual void setRxFrequency(double);
  void setThreshold(int);
  void setSquelch(bool);

	
signals: // Signals
  /** Signal will be emitted if a new character was detected */
  void newSymbol(char);

};

#endif
