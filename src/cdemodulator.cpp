/***************************************************************************
                          cdemodulator.cpp  -  description
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

#include "cdemodulator.h"

CDemodulator::CDemodulator()
{
UseAfc = Off;
Squelch = true;
Threshold= 50;
}

CDemodulator::~CDemodulator()
{
}

void CDemodulator::setAfcMode(AfcMode afc) { UseAfc=afc; }

void CDemodulator::setRxFrequency(double freq)
{
 if ( (freq > 200 ) && (freq < 3000 ) ) 
  RxFrequency = freq;
}

double CDemodulator::getRxFrequency(void)
{
return RxFrequency;
}

void CDemodulator::setThreshold(int value)
{
Threshold= value;
}

void CDemodulator::setSquelch(bool OnOff)
{
Squelch=OnOff;
}

AfcMode CDemodulator::getAfcMode()
{
return UseAfc;
}
bool CDemodulator::getSquelchState()
{
return Squelch;
}
std::complex<float> * CDemodulator::getPhasePointer()
{
return 0;
}
int CDemodulator::getThreshold()
{
return Threshold;
}
double CDemodulator::get2RxFrequency(void)
{
return 0.0;
}  
float CDemodulator::getIMD()
{
return 0.0;
}

 void CDemodulator::setParameter(RxTxParameterType,void *)
 {
 /**
  * At the moment only RTTY uses some extra Parameters
  * So there is mostly nothing to do
  * but that may change in the future
 **/ 
 return;
 }
 void *CDemodulator::getParameter(RxTxParameterType)    
 {
 return 0;
 } 
 void *CDemodulator::getBuffer()    
 {
 return 0;
 }
 

