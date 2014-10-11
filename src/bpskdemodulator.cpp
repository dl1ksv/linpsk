/***************************************************************************
                          bpskdemodulator.cpp  -  description
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
 *    based on the work of  Moe Wheatly, AE4JY                             *
 ***************************************************************************/

#include "bpskdemodulator.h"

BPskDemodulator::BPskDemodulator():CPskDemodulator()
{
  ave1=1.0;
  ave2=1.0;

}
BPskDemodulator::~BPskDemodulator(){
}
void BPskDemodulator::DecodeSymbol(double angle)

{

  bool bit;

  char ch =0;
  CalcQuality(angle);
  bit = GetBPSKSymb();

  if( (bit==0) && m_LastBitZero )	//if character delimiter
    {
      if(m_BitAcc != 0 )
        {
          m_BitAcc >>= 2;				//get rid of last zero and one
          m_BitAcc &= 0x07FF;
          ch = m_VaricodeDecTbl[m_BitAcc];
          m_BitAcc = 0;
          if( (ch!=0)  &&
              ( !Squelch || (Squelch && (fastSquelch || ( ( unsigned int ) m_DevAve  > Threshold ))))) // Squelch Part
            {
              emit newSymbol(ch);
              if (fastSquelch && (( unsigned int ) m_DevAve < Threshold) )
                fastSquelch = false;
            }
        }
    }
  else
    {
      m_BitAcc <<= 1;
      m_BitAcc |= bit;
      if(bit==0)
        m_LastBitZero = true;
      else
        m_LastBitZero = false;
    }

  if (bit)
    {
      m_OffCount=0;
      if (m_OnCount++ >20)
        fastSquelch=true;

    }
  else
    {
      m_OnCount=0;
      if (m_OffCount++ > 20)
        fastSquelch=false;
    }


}
bool BPskDemodulator::GetBPSKSymb()
{
  return (Phase_Vector.real()> 0.0);
}

//////////////////////////////////////////////////////////////////////
// Calculate signal quality based on the statistics of the phase
//	difference angle.  The more dispersion of the "0" and "180" degree
//  phase shifts, the worse the signal quality.  This information is used
//  to activate the squelch control.  If 20 consecutive "180" degree shifts
//  occur, the squelch is forced on, and if 20 consecutive "0" degree
//  shifts occur, the squelch is forced off quickly.
//////////////////////////////////////////////////////////////////////

void BPskDemodulator::CalcQuality(  double angle )
{

  double temp;


  if ( fabs(angle) < M_PI_2)
    temp= angle;
  else
    {
      if ( angle > 0.0)
        temp=angle-M_PI;
      else
        temp = M_PI+ angle;
    }
  m_QFreqError = -temp;
  temp = fabs(temp);
  m_DevAve =0.5 * ave1 + 0.45 * ave2 + 0.05 *temp;
  ave2=ave1;
  ave1=m_DevAve;
  // And now norm m_DevAve for use in Squelch
  m_DevAve = 100. -m_DevAve *63.67;

}
