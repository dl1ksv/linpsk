/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *    based on the work of  Moe Wheatly, AE4JY                             *
 ***************************************************************************/

#include "psk63demodulator.h"
#include "firfilter.h"

PSk63Demodulator::PSk63Demodulator():CPskDemodulator()
{
  ave1=1.0;
  ave2=1.0;
}
PSk63Demodulator::~PSk63Demodulator()
{
  if( downFilter )
    delete downFilter;
  if ( syncFilter )
    delete syncFilter;
}
void PSk63Demodulator::DecodeSymbol(double angle)

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
bool PSk63Demodulator::GetBPSKSymb()
{
  return (Phase_Vector.real()> 0.0);
}

void PSk63Demodulator::CalcQuality(  double angle )
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
  temp = fabs(temp);
  m_DevAve =0.5 * ave1 + 0.45 * ave2 + 0.05 *temp;
  ave2=ave1;
  ave1=m_DevAve;
  // And now norm m_DevAve for use in Squelch
  m_DevAve = 100. -m_DevAve *63.67;

}
void PSk63Demodulator::Init(double Fs ,int BlockSize)
{
  SampleRate = Fs;        //sample rate
  NxSamples = BlockSize;  //size data input buffer
  downFilter = new FIRFilter(PI2*125./Fs,79,ComplexData,10.);
  syncFilter = new FIRFilter(PI2*62.5*9./Fs,79, ComplexData);
  downRate = 9;
  f1=31.25;
  f2=93.75;
}
double PSk63Demodulator::calcFreqError(complex<double> s)
{
 double x,y;
 complex<double> z;
 if (abs(s) >1 )
  z=s/abs(s);
 else z=s;
 x= z.real()*z.imag();
 x /=2500.; // Adopt deviation to samplerate
// x /=2.8016548; //Gain
 y=x_loop_1+x +0.2861361823*y_loop_1;
 x_loop_1=x;
 y_loop_1=y;
 return -y;

}
