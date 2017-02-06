/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *    based on the work of  Moe Wheatly, AE4JY                             *  
 ***************************************************************************/

#include "firfilter.h"
#include "qpskdemodulator.h"
#include "viterbi.h"

// phase wraparound correction table for viterbi decoder
static const double AngleTbl1[4] = {  0.0, -M_PI_2, -M_PI,-M_PI_3_2};
static const double AngleTbl2[4] = {  0.0, M_PI_3_2, M_PI,M_PI_2};


static double  qdistance(double *x,int i)
{

const double *pAngle;


if ( *x >0.0)
  pAngle = AngleTbl2;
else
  pAngle = AngleTbl1;
//i = (~i) & 0x03;

i= i & 0x03;

return fabs(*x - pAngle[i]);
}


QPskDemodulator::QPskDemodulator():CPskDemodulator()
{

ave1=1.0;
ave2=1.0;

v=new Viterbi(5,0x19,0x17,qdistance);
}
QPskDemodulator::~QPskDemodulator()
{
  if( downFilter )
    delete downFilter;
  if ( syncFilter )
    delete syncFilter;
}
void QPskDemodulator::DecodeSymbol(double angle)

{

int bit;

char ch =0;
	CalcQuality(angle);
	v->decode( &angle);
	bit = v->getbitinvers(32);
   
	if( (bit==0) && m_LastBitZero )	//if character delimiter
	{
		if(m_BitAcc != 0 )
		{
			m_BitAcc >>= 2;				//get rid of last zero and one
			m_BitAcc &= 0x07FF;
			ch = m_VaricodeDecTbl[m_BitAcc];
			m_BitAcc = 0;
	if( (ch!=0)  &&
   ( !Squelch || (Squelch && (fastSquelch || ( ( unsigned int ) m_DevAve  > Threshold )))) // Squelch Part
    )
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

void QPskDemodulator::CalcQuality(  double angle )
{

double temp;
double absangle;

absangle= fabs(angle);          
if ( absangle < M_PI_4)
  temp= angle;
else
 {
 if ( absangle < M_PI_3_4 )
     temp =  M_PI_2 - absangle;
    else
     temp = M_PI - absangle; 
 }
if ( angle < 0.0 )
 temp = -temp;
temp = fabs(temp);

 
//		m_DevAve =0.47 * ave1 + 0.46 * ave2 + 0.07 *temp;
m_DevAve =0.5 * ave1 + 0.45 * ave2 + 0.05*temp;

ave2=ave1;
ave1=m_DevAve;

// And now norm m_DevAve for use in Squelch
m_DevAve = 100. -m_DevAve *110.;

}

void QPskDemodulator::Init(double Fs ,int BlockSize)
{
  SampleRate = Fs;        //sample rate
  NxSamples = BlockSize;  //size data input buffer
  downFilter = new FIRFilter(PI2*31.25/Fs,79,ComplexData,10.);
  syncFilter = new FIRFilter(PI2*31.25*18./Fs,79, ComplexData,5.);
  downRate = 18;

}
double QPskDemodulator::calcFreqError(complex<double> s)
{
 double x,y;
 complex<double> z;
 if (abs(s) >1 )
  z=s/abs(s);
 else z=s;
 x= z.real()*z.imag();
 x /=5000.; // Adopt deviation to samplerate
// x /=2.8016548; //Gain
 y=x_loop_1+x +0.2861361823*y_loop_1;
 x_loop_1=x;
 y_loop_1=y;
 return -y;

}
