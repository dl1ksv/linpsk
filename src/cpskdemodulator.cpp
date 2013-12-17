/***************************************************************************
                          cpskdemodulator.cpp  -  description
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

#include "cpskdemodulator.h"
#include "fircoeffs.h"
#include "parameter.h"
#include <cstdio>
extern Parameter settings;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPskDemodulator::CPskDemodulator(): CDemodulator()
{
	m_pQue1 = NULL;
	m_pQue2 = NULL;
	m_pQue3 = NULL;
	m_VaricodeDecTbl = NULL;
        Phase= NULL;
}

CPskDemodulator::~CPskDemodulator()
{
	if(m_pQue1)
		delete m_pQue1;
	if(m_pQue2)
		delete m_pQue2;
	if(m_pQue3)
		delete m_pQue3;
	if(m_VaricodeDecTbl)
		delete m_VaricodeDecTbl;
        if(Phase )
         delete Phase;

}

/////////////////////////////////////////////////////////////////
//       Initialize PskDet buffers and pointers
/////////////////////////////////////////////////////////////////
bool CPskDemodulator::Init( double Fs, int BlockSize )
{
unsigned short int wTemp;
int i;
//circular delay lines.(data stays put and the pointers move)
 
	if( (m_pQue1 = new complex<double>[DEC3_LPFIR_LENGTH] ) == NULL)
		return false;
	if( (m_pQue2 = new complex<double>[DEC3_LPFIR_LENGTH] ) == NULL)
		return false;
	if( (m_pQue3 = new complex<double>[BITFIR_LENGTH] ) == NULL)
		return false;
	if( (	m_VaricodeDecTbl = new unsigned char[2048] )== NULL)
		return false;
    
  if ( ( Phase = new complex<float>[228] ) == NULL )
    return false;
	for(int i=0; i<DEC3_LPFIR_LENGTH; i++)
	{
	// fill delay buffer with zero
		m_pQue1[i] = complex<double>(0.0,0.0);
	}
	for(i=0; i<DEC3_LPFIR_LENGTH; i++)
	{
	// fill delay buffer with zero
		m_pQue2[i] = complex<double>(0.0,0.0);

	}
	for( i=0; i<BITFIR_LENGTH; i++)
	{
	// fill delay buffer with zero
		m_pQue3[i] = complex<double>(0.0,0.0);
	}

	for( int j=0; j<2048; j++)		//init inverse varicode lookup decoder table
	{
		m_VaricodeDecTbl[j] = 0;
		for(int i=0; i<256;i++)

		{
			wTemp = VARICODE_TABLE[i];
			wTemp >>= 4;
			while( !(wTemp&1) )
				wTemp >>= 1;
			wTemp >>= 1;
			if( wTemp == j)
				m_VaricodeDecTbl[j] = (unsigned char)i;
		}
	}
	m_pInPtr1 = m_pQue1;		//initialize que ptrs
	m_pInPtr2 = m_pQue2;
	m_pInPtr3 = m_pQue3;
	SampleRate = Fs;					//sample rate
	NxSamples = BlockSize;	//size data input buffer
	OszFreqinc = 1000.0*PI2/SampleRate;
	m_BitPhaseInc = 9.0/SampleRate;		//bit oversampling period
	m_SignalLevel = 1.0;
	m_BitPhasePos = 0.0;
	m_BitAcc = 0;
	m_LastBitZero = false;
	m_SampCnt = 0;
	m_OnCount = 0;
	m_OffCount = 0;

 for (i=0;i< 228; i++)
  Phase[i]=0.0;
 for( i=0; i<40; i++)
  DelayLine[i] = 0.0;					// initialize the array


// Init a bunch of "static" variables used in various member fuctions
	m_AGCave = 0.0;
	m_FreqError = 0.0;
	OszFrequency = 0.0;
	m_LastFreq = 1000.0;
	m_PkPos = 0;
	m_BitPos = 0;
  Prev_Sample=complex<double>(0.0,0.0) ;
  Phase_Vector = Prev_Sample;  
	m_DevAve = 0.78;
	m_QFreqError = 0.0;
	m_LastPkPos = 0;
	m_ClkErrCounter = 0;
	m_ClkErrTimer = 0;
	m_ClkError = 0;
// Initializing Variables for AFC
fe0=0.0;
fe1=0.0;
fe2=0.0;
// FastSquelch
fastSquelch=false;
// IMD
IMD=0.0; // Means Unknown
 x_loop_1=0;
 y_loop_1=0;
return true;
}

//////////////////////////////////////////////////////////////////////
// Main routine called to process the next block of data 'pIn'.
//  The center frequency is specified by 'Freq'.
//  'UseAFC' specifies whether to automatically lock to the frequency.
//  30mSec for BPSK, 38mSec for QPSK( 133MHz Pentium )
//////////////////////////////////////////////////////////////////////

void CPskDemodulator::ProcessInput( double* pIn, double *Spectrum)
{
double vcophz = OszFrequency;
int i,j;
complex<double> acc;
const double* Kptr;
complex<double>* Firptr;
complex<double>* Inptr1;			//decimation FIR #1 variables
complex<double>* Qptr1;
complex<double>* Inptr2;			//decimation FIR #2 variables
complex<double>* Qptr2;

if ( UseAfc == Wide )
 { 
// Check if we are near the center frequency in Spectrum
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

 m_DispTrig = false;
 j=0;
 if( RxFrequency != m_LastFreq )	//caller is changing center frequency
 {
  OszFreqinc = (PI2/SampleRate)*RxFrequency;	//new center freq inc
  m_LastFreq = RxFrequency;
 }
 Inptr1 = m_pInPtr1;		//use local copies of member variables
 Qptr1 = m_pQue1;		// for better speed.
 Inptr2 = m_pInPtr2;
 Qptr2 = m_pQue2;
 for( i = 0; i<NxSamples; i++ )	// put new samples into Queue
 {
  if( --Inptr1 < Qptr1 )		//deal with FIR pointer wrap around
   Inptr1 = Qptr1+DEC3_LPFIR_LENGTH-1;
//Generate complex sample by mixing input sample with NCO's sin/cos
  vcophz = vcophz + OszFreqinc;
  if(vcophz > PI2)		//handle 2 Pi wrap around
   vcophz -= PI2;
  *Inptr1=pIn[i]*complex<double>(cos(vcophz),sin(vcophz));
//decimate by 3 filter
  if( ( (++m_SampCnt)%3 ) == 0 )	//calc first decimation filter every 3 samples
  {
   acc=complex<double>(0.0,0.0);
   Firptr = Inptr1;
   Kptr = Dec3LPCoef;
   while( Kptr < (Dec3LPCoef + DEC3_LPFIR_LENGTH) )	//do the MAC's
   {
    acc +=(*Firptr)*(*Kptr++);
    if( (++Firptr) >= Qptr1+DEC3_LPFIR_LENGTH ) //deal with wraparound
     Firptr = Qptr1;
   }
   if( --Inptr2 < Qptr2 )		//deal with FIR pointer wrap around
    Inptr2 = Qptr2+DEC3_LPFIR_LENGTH-1;
   *Inptr2=acc;
//decimate by 3 filter
  if( (m_SampCnt%9) == 0 )	//calc second decimation filter every 9 samples
  {
   acc=complex<double>(0.0,0.0);
   Firptr = Inptr2;
   Kptr = Dec3LPCoef;
   while( Kptr < (Dec3LPCoef + DEC3_LPFIR_LENGTH) )	//do the MAC's
   {
    acc +=(*Firptr)*(*Kptr);
    Kptr++;
    if( (++Firptr) >= Qptr2+DEC3_LPFIR_LENGTH ) //deal with wraparound
    Firptr = Qptr2;
   }
// here at Fs/9 = 612.5 Hz rate with latest sample in acc.
// Matched Filter the I and Q data and also a frequency error filter
// filter puts filtered signals in m_FreqSignal and m_BitSignal.
  CalcBitFilter( acc );
   if( SymbSync(m_BitSignal) )	
   {
    Phase_Vector=m_BitSignal*conj(Prev_Sample);
    Prev_Sample = m_BitSignal;
    Phase[j++]=Phase_Vector;
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
     OszFreqinc +=CalcFreqError(Phase_Vector);
    }
   }
  }
 }
 m_SampCnt = m_SampCnt%9;
 m_pInPtr1 = Inptr1;		// save position in circular delay line
 m_pInPtr2 = Inptr2;		// save position in circular delay line
 OszFrequency = vcophz;

 if (UseAfc != Off)			// Change RxFrequency, but slowly
 {
  RxFrequency=OszFreqinc*SampleRate/PI2;
  m_LastFreq=RxFrequency;
 }

}


//////////////////////////////////////////////////////////////////////
// Called at Fs/9 rate to calculate the symbol sync position
// Returns true if at center of symbol.
// Sums up the energy at each sample time, averages it, and picks the
//   sample time with the highest energy content.
//////////////////////////////////////////////////////////////////////

bool CPskDemodulator::SymbSync(complex<double> sample)

{
 bool Trigger;
 int BitPos = m_BitPos;
 Trigger=false;
 DelayLine[BitPos]=sample;
 m_BitPhasePos += m_BitPhaseInc;
 if( m_BitPhasePos >= Ts )
 {					// here every symbol time
  m_BitPhasePos -= Ts;
  float x,y,maxsample;
  int kk;
  x=0;
  maxsample=-1;
  for(int i=0; i <10;i++)
  {
   kk= BitPos-i-10;
   if (kk < 0)
    kk += 40;
   y= abs(DelayLine[kk]);
   maxsample=max(maxsample,y);
   x += y;
   kk = BitPos-i;
   if (kk < 0)
    kk += 40;
   y = abs(DelayLine[kk]);
   maxsample=max(maxsample,y);
   x -= y;
  }
   maxsample = 3.* maxsample;
   if ( x < -maxsample )
    m_BitPhasePos += m_BitPhaseInc;
   else if ( x > maxsample)
    m_BitPhasePos -= m_BitPhaseInc;
  
  Trigger=true;
 }
 BitPos++;
 if ( BitPos >= 40 )
  BitPos=0;
 m_BitPos=BitPos;
 return Trigger;
}


//////////////////////////////////////////////////////////////////////
//  Frequency error calculator
//////////////////////////////////////////////////////////////////////
double CPskDemodulator::CalcFreqError(complex<double> s)
{
 double x,y;
 complex<double> z;
 if (abs(s) >1 )
  z=s/abs(s);
 else z=s;
 x= z.real()*z.imag();
 x /=10000.; // Adopt deviation to samplerate
// x /=2.8016548; //Gain
 y=x_loop_1+x +0.2861361823*y_loop_1;
 x_loop_1=x;
 y_loop_1=y;
 return -y;

}

//////////////////////////////////////////////////////////////////////
// Automatic gain control calculator
//////////////////////////////////////////////////////////////////////
double CPskDemodulator::CalcAGC( complex<double> Samp)
{
double mag;

  mag = abs(Samp);
	if( mag > m_AGCave )

		m_AGCave = (1.0-1.0/250.0)*m_AGCave + (1.0/250.0)*mag;
	else
		m_AGCave = (1.0-1.0/1000.0)*m_AGCave + (1.0/1000.0)*mag;
	if( m_AGCave >= 1.0 )	// divide signal by ave if not almost zero
	{
		m_BitSignal /= m_AGCave;
//		m_BitSignal.y /= m_AGCave;
		m_FreqSignal /= m_AGCave;
//		m_FreqSignal.y /= m_AGCave;
	}
	return(m_AGCave);
}


//////////////////////////////////////////////////////////////////////
// BIT FIR filters. A narrow matched(?) data filter for data
//                  and wider filter for the AFC/AGC functions
//////////////////////////////////////////////////////////////////////
void CPskDemodulator::CalcBitFilter(complex<double> Samp)
{
complex<double> acc1;
complex<double> acc2;
const double* Kptr1;
const double* Kptr2;
complex<double>* Firptr;
complex<double>* Qptr;
complex<double>* Inptr;
	Inptr = m_pInPtr3;		//use local copies of member variables
	Qptr = m_pQue3;			// for better speed.
	if( --Inptr < Qptr )	//deal with LPFIR pointer wrap around
		Inptr = Qptr+BITFIR_LENGTH-1;
//	Inptr->x = Samp.x;	//place in circular Queue
	*Inptr = Samp;
//	acc1.x = 0.0;
	acc1 = complex<double>(0.0,0.0);
//	acc2.x = 0.0;
	acc2 = complex<double>(0.0,0.0);
	Firptr = Inptr;
	Kptr1 = FreqFirCoef;	//frequency error filter
	Kptr2 = BitFirCoef;	//bit data filter
	while( Kptr2 < (BitFirCoef + BITFIR_LENGTH) )	//do the MAC's
	{
//		acc1.x += ( (Firptr->x)*(*Kptr1) );
		acc1 += ( (*Firptr)*(*Kptr1++) );
						
//		acc2.x += ( (Firptr->x)*(*Kptr2) );
		acc2 += ( (*Firptr)*(*Kptr2++) );
						
		if( (++Firptr) >= (Qptr+BITFIR_LENGTH) ) //deal with wraparound
			Firptr = Qptr;
	}
	m_pInPtr3 = Inptr;		// save position in circular delay line
	m_FreqSignal = acc1;
	m_BitSignal = acc2;
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

freq1 = (int) ((RxFrequency+15.625)/2.69165);  
freq2 = (int) ((RxFrequency+46.875)/2.69165);
IMD=(Spectrum[freq2-1]+Spectrum[freq2]+Spectrum[freq2+1])/(Spectrum[freq1-1]+Spectrum[freq1]+Spectrum[freq1+1]);
freq1=(int) ( (RxFrequency- 15.625)/2.69165);
freq2=(int) ( (RxFrequency- 46.875)/2.69165);
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
