/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *    based on the work of  Moe Wheatly, AE4JY                             *  
 ***************************************************************************/

//#include <assert.h>
//#include <math.h>
#include "cpskdemodulator.h"
#include "firfilter.h"
#include "parameter.h"
#include <cstdio>
extern Parameter settings;

CPskDemodulator::CPskDemodulator(): CDemodulator()
{
  int i,temp;
  m_LastFreq = 1000.0;
  m_BitPhasePos = 0.0;
  Prev_Sample=complex<double>(0.0,0.0) ;
  Phase_Vector = Prev_Sample;
  m_DevAve = 0.78;
// FastSquelch
  fastSquelch=false;
// IMD
  IMD=0.0; // Means Unknown
  f1=15.625;
  f2=46.875;
  x_loop_1=0;
  y_loop_1=0;

  m_SampCnt = 0;
  m_VaricodeDecTbl = NULL;
  downFilter = NULL;
  syncFilter = NULL;
  for(i=0; i < 20; i++ )
    syncBuf[i]=0;
  m_VaricodeDecTbl = new unsigned char[2048];
  for (i=0;i< PHASE_DISPLAY_BUFFER_LENGTH; i++)
   Phase[i]=0.0;
  phasePosition=0;
  for(i=0; i < 2048;i++)
    m_VaricodeDecTbl[i]=0;
  for(i=0; i<256;i++)
    {
      temp = VARICODE_TABLE[i];
      temp >>= 4;
      while( !(temp&1) )
         temp >>= 1;
      temp >>= 1;
      m_VaricodeDecTbl[temp] = (unsigned char)i;
     }
  m_BitAcc=0;
  m_LastBitZero = false;
  m_OnCount=0;
  m_OffCount=0;
}

CPskDemodulator::~CPskDemodulator()
{
  if(m_VaricodeDecTbl)
    delete m_VaricodeDecTbl;
}

void CPskDemodulator::ProcessInput( double* pIn, double *Spectrum)
{
double vcophz = OszFrequency;
int i;
complex<double> acc;

if ( UseAfc == Wide )
 { 
// Check if we are near the centerphasePosition frequency in Spectrum
  int index;
  index=(int) (RxFrequency/2.69165);
  double smooth[15],xx;
// First we smooth the spectrum
  for(int i=0; i<15; i++)
   {
    smooth[i]=0.0; 
    for (int k=0; k <15; k++)
     smooth[i] +=Spectrum[index-14+k+i]/7;
   }
   int maxspect=0;
   xx=0.0;
   for(int i=0;i <15; i++)
    {
     if (smooth[i] >xx )
      {
       xx=smooth[i];
       maxspect=i;
      }
    }
   if ( maxspect < 5 )  // We are far above the Frequency
    RxFrequency -=1.5;
   else if (maxspect > 9 )
          RxFrequency += 1.5;
    }

 if( RxFrequency != m_LastFreq )	//caller is changing center frequency
 {
  OszFreqinc = (PI2/SampleRate)*RxFrequency;	//new center freq inc
  m_LastFreq = RxFrequency;
 }
 for( i = 0; i<NxSamples; i++ )	// put new samples into Queue
 {
//Generate complex sample by mixing input sample with NCO's sin/cos
  vcophz = vcophz + OszFreqinc;
  if(vcophz > PI2)		//handle 2 Pi wrap around
   vcophz -= PI2;
  complex<double> xxx =pIn[i]*complex<double>(cos(vcophz),sin(vcophz));
//  assert(isnan(xxx.real()) == 0);
  downFilter->processFilter(&xxx,&acc,1);
//  assert(isnan(acc.real()) == 0);
  m_SampCnt++;
  if( (m_SampCnt%downRate) == 0)
    {
      syncFilter->processFilter(&acc,&xxx,1);
//      assert(isnan(xxx.real()) ==0);
      if( symbSync(xxx))
      {
    Phase_Vector=xxx *conj(Prev_Sample);
    Prev_Sample = xxx;
    Phase[phasePosition++]=Phase_Vector;
    if(phasePosition >=PHASE_DISPLAY_BUFFER_LENGTH)
      phasePosition=0;
    double angle =  atan2(Phase_Vector.imag(),Phase_Vector.real());
    DecodeSymbol( angle);
    if ( (unsigned int) m_DevAve  > Threshold)
    {
  //  if (m_OffCount > 10)
     calcIMD(Spectrum); // We got enough Idles to calc the IMD
    }
    else
     IMD =0.0;
    if( UseAfc != Off )
     OszFreqinc +=calcFreqError(Phase_Vector);
    }
   }
  }
 m_SampCnt = m_SampCnt%9;
 OszFrequency = vcophz;

 if (UseAfc != Off)			// Change RxFrequency, but slowly
 {
  RxFrequency=OszFreqinc*SampleRate/PI2;
  m_LastFreq=RxFrequency;
 }

}


complex<float> * CPskDemodulator::getPhasePointer()
{
return  Phase;
}

int CPskDemodulator::getSquelchValue()
{
return  (int) m_DevAve;
}
void CPskDemodulator::calcIMD(double *Spectrum)
{
int freq1,freq2;

freq1 = (int) ((RxFrequency+f1)/2.69165);
freq2 = (int) ((RxFrequency+f2)/2.69165);
IMD=(Spectrum[freq2-1]+Spectrum[freq2]+Spectrum[freq2+1])/(Spectrum[freq1-1]+Spectrum[freq1]+Spectrum[freq1+1]);
freq1=(int) ( (RxFrequency- f1)/2.69165);
freq2=(int) ( (RxFrequency- f2)/2.69165);
IMD = 10.*log10( (IMD + (Spectrum[freq2-1]+Spectrum[freq2]+Spectrum[freq2+1])/(Spectrum[freq1-1]+Spectrum[freq1]+Spectrum[freq1+1]))/2);

}
float CPskDemodulator::getIMD()
{
return IMD;
}  
AfcMode CPskDemodulator::AfcProperties()
{
return Wide;
}

bool CPskDemodulator::symbSync(complex<double> sample)
{
  bool trigger;
  double sum,ampsum;
  trigger =false;

  int index = (int) m_BitPhasePos;
//  assert((index >= 0) && (index <20));
  syncBuf[index] = 0.7 * syncBuf[index] + 0.3 * abs(sample);
//  syncBuf[index] = abs(sample);
  sum=0;
  ampsum=0;
  index = SYNCBUFFERLENGTH/2;
  for(int i=0; i <index; i++)
    {
      sum += (syncBuf[i] - syncBuf[index+i]);
      ampsum +=(syncBuf[i] + syncBuf[index+i]);
    }
  sum = (ampsum == 0 ? 0 : sum / ampsum);
  m_BitPhasePos -= sum/5.0;
  m_BitPhasePos +=1.01;
  if( m_BitPhasePos >=SYNCBUFFERLENGTH)
    {
      m_BitPhasePos -=SYNCBUFFERLENGTH;
      trigger =  true;
    }
  else if(m_BitPhasePos < 0.)
    m_BitPhasePos +=SYNCBUFFERLENGTH;

  return trigger;
}


