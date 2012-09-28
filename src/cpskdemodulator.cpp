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

extern Parameter settings;


static const unsigned short int VARICODE_TABLE[] = {
	0xAAC0,	// ASCII =   0	1010101011
	0xB6C0,	// ASCII =   1	1011011011
	0xBB40,	// ASCII =   2	1011101101
	0xDDC0,	// ASCII =   3	1101110111
	0xBAC0,	// ASCII =   4	1011101011
	0xD7C0,	// ASCII =   5	1101011111
	0xBBC0,	// ASCII =   6	1011101111
	0xBF40,	// ASCII =   7	1011111101
	0xBFC0,	// ASCII =   8	1011111111
	0xEF00,	// ASCII =   9	11101111
	0xE800,	// ASCII =  10	11101
	0xDBC0,	// ASCII =  11	1101101111
	0xB740,	// ASCII =  12	1011011101
	0xF800,	// ASCII =  13	11111
	0xDD40,	// ASCII =  14	1101110101
	0xEAC0,	// ASCII =  15	1110101011
	0xBDC0,	// ASCII =  16	1011110111
	0xBD40,	// ASCII =  17	1011110101
	0xEB40,	// ASCII =  18	1110101101
	0xEBC0,	// ASCII =  19	1110101111
	0xD6C0,	// ASCII =  20	1101011011
	0xDAC0,	// ASCII =  21	1101101011
	0xDB40,	// ASCII =  22	1101101101
	0xD5C0,	// ASCII =  23	1101010111
	0xDEC0,	// ASCII =  24	1101111011
	0xDF40,	// ASCII =  25	1101111101
	0xEDC0,	// ASCII =  26	1110110111
	0xD540,	// ASCII =  27	1101010101
	0xD740,	// ASCII =  28	1101011101
	0xEEC0,	// ASCII =  29	1110111011
	0xBEC0,	// ASCII =  30	1011111011
	0xDFC0,	// ASCII =  31	1101111111
	0x8000,	// ASCII = ' '	1
	0xFF80,	// ASCII = '!'	111111111
	0xAF80,	// ASCII = '"'	101011111
	0xFA80,	// ASCII = '#'	111110101
	0xED80,	// ASCII = '$'	111011011
	0xB540,	// ASCII = '%'	1011010101
	0xAEC0,	// ASCII = '&'	1010111011
	0xBF80,	// ASCII = '''	101111111
	0xFB00,	// ASCII = '('	11111011
	0xF700,	// ASCII = ')'	11110111
	0xB780,	// ASCII = '*'	101101111
	0xEF80,	// ASCII = '+'	111011111
	0xEA00,	// ASCII = ','	1110101
	0xD400,	// ASCII = '-'	110101
	0xAE00,	// ASCII = '.'	1010111
	0xD780,	// ASCII = '/'	110101111
	0xB700,	// ASCII = '0'	10110111
	0xBD00,	// ASCII = '1'	10111101
	0xED00,	// ASCII = '2'	11101101
	0xFF00,	// ASCII = '3'	11111111
	0xBB80,	// ASCII = '4'	101110111
	0xAD80,	// ASCII = '5'	101011011
	0xB580,	// ASCII = '6'	101101011
	0xD680,	// ASCII = '7'	110101101
	0xD580,	// ASCII = '8'	110101011
	0xDB80,	// ASCII = '9'	110110111
	0xF500,	// ASCII = ':'	11110101
	0xDE80,	// ASCII = ';'	110111101
	0xF680,	// ASCII = '<'	111101101
	0xAA00,	// ASCII = '='	1010101
	0xEB80,	// ASCII = '>'	111010111
	0xABC0,	// ASCII = '?'	1010101111
	0xAF40,	// ASCII = '@'	1010111101
	0xFA00,	// ASCII = 'A'	1111101
	0xEB00,	// ASCII = 'B'	11101011
	0xAD00,	// ASCII = 'C'	10101101
	0xB500,	// ASCII = 'D'	10110101
	0xEE00,	// ASCII = 'E'	1110111
	0xDB00,	// ASCII = 'F'	11011011
	0xFD00,	// ASCII = 'G'	11111101
	0xAA80,	// ASCII = 'H'	101010101
	0xFE00,	// ASCII = 'I'	1111111
	0xFE80,	// ASCII = 'J'	111111101
	0xBE80,	// ASCII = 'K'	101111101
	0xD700,	// ASCII = 'L'	11010111
	0xBB00,	// ASCII = 'M'	10111011
	0xDD00,	// ASCII = 'N'	11011101
	0xAB00,	// ASCII = 'O'	10101011
	0xD500,	// ASCII = 'P'	11010101
	0xEE80,	// ASCII = 'Q'	111011101
	0xAF00,	// ASCII = 'R'	10101111
	0xDE00,	// ASCII = 'S'	1101111
	0xDA00,	// ASCII = 'T'	1101101
	0xAB80,	// ASCII = 'U'	101010111
	0xDA80,	// ASCII = 'V'	110110101
	0xAE80,	// ASCII = 'W'	101011101
	0xBA80,	// ASCII = 'X'	101110101
	0xBD80,	// ASCII = 'Y'	101111011
	0xAB40,	// ASCII = 'Z'	1010101101
	0xFB80,	// ASCII = '['	111110111
	0xF780,	// ASCII = '\'	111101111
	0xFD80,	// ASCII = ']'	111111011
	0xAFC0,	// ASCII = '^'	1010111111
	0xB680,	// ASCII = '_'	101101101
	0xB7C0,	// ASCII = '`'	1011011111
	0xB000,	// ASCII = 'a'	1011
	0xBE00,	// ASCII = 'b'	1011111
	0xBC00,	// ASCII = 'c'	101111
	0xB400,	// ASCII = 'd'	101101
	0xC000,	// ASCII = 'e'	11
	0xF400,	// ASCII = 'f'	111101
	0xB600,	// ASCII = 'g'	1011011
	0xAC00,	// ASCII = 'h'	101011
	0xD000,	// ASCII = 'i'	1101
	0xF580,	// ASCII = 'j'	111101011
	0xBF00,	// ASCII = 'k'	10111111
	0xD800,	// ASCII = 'l'	11011
	0xEC00,	// ASCII = 'm'	111011
	0xF000,	// ASCII = 'n'	1111
	0xE000,	// ASCII = 'o'	111
	0xFC00,	// ASCII = 'p'	111111
	0xDF80,	// ASCII = 'q'	110111111
	0xA800,	// ASCII = 'r'	10101
	0xB800,	// ASCII = 's'	10111
	0xA000,	// ASCII = 't'	101
	0xDC00,	// ASCII = 'u'	110111
	0xF600,	// ASCII = 'v'	1111011
	0xD600,	// ASCII = 'w'	1101011
	0xDF00,	// ASCII = 'x'	11011111
	0xBA00,	// ASCII = 'y'	1011101
	0xEA80,	// ASCII = 'z'	111010101
	0xADC0,	// ASCII = '{'	1010110111
	0xDD80,	// ASCII = '|'	110111011
	0xAD40,	// ASCII = '}'	1010110101
	0xB5C0,	// ASCII = '~'	1011010111
	0xED40,	// ASCII = 127	1110110101
	0xEF40,	// ASCII = 128	1110111101
	0xEFC0,	// ASCII = 129	1110111111
	0xF540,	// ASCII = 130	1111010101
	0xF5C0,	// ASCII = 131	1111010111
	0xF6C0,	// ASCII = 132	1111011011
	0xF740,	// ASCII = 133	1111011101
	0xF7C0,	// ASCII = 134	1111011111
	0xFAC0,	// ASCII = 135	1111101011
	0xFB40,	// ASCII = 136	1111101101
	0xFBC0,	// ASCII = 137	1111101111
	0xFD40,	// ASCII = 138	1111110101
	0xFDC0,	// ASCII = 139	1111110111
	0xFEC0,	// ASCII = 140	1111111011
	0xFF40,	// ASCII = 141	1111111101
	0xFFC0,	// ASCII = 142	1111111111
	0xAAA0,	// ASCII = 143	10101010101
	0xAAE0,	// ASCII = 144	10101010111
	0xAB60,	// ASCII = 145	10101011011
	0xABA0,	// ASCII = 146	10101011101
	0xABE0,	// ASCII = 147	10101011111
	0xAD60,	// ASCII = 148	10101101011
	0xADA0,	// ASCII = 149	10101101101
	0xADE0,	// ASCII = 150	10101101111
	0xAEA0,	// ASCII = 151	10101110101
	0xAEE0,	// ASCII = 152	10101110111
	0xAF60,	// ASCII = 153	10101111011
	0xAFA0,	// ASCII = 154	10101111101
	0xAFE0,	// ASCII = 155	10101111111
	0xB560,	// ASCII = 156	10110101011
	0xB5A0,	// ASCII = 157	10110101101
	0xB5E0,	// ASCII = 158	10110101111
	0xB6A0,	// ASCII = 159	10110110101
	0xB6E0,	// ASCII = 160	10110110111
	0xB760,	// ASCII = 161	10110111011
	0xB7A0,	// ASCII = 162	10110111101
	0xB7E0,	// ASCII = 163	10110111111
	0xBAA0,	// ASCII = 164	10111010101
	0xBAE0,	// ASCII = 165	10111010111
	0xBB60,	// ASCII = 166	10111011011
	0xBBA0,	// ASCII = 167	10111011101
	0xBBE0,	// ASCII = 168	10111011111
	0xBD60,	// ASCII = 169	10111101011
	0xBDA0,	// ASCII = 170	10111101101
	0xBDE0,	// ASCII = 171	10111101111
	0xBEA0,	// ASCII = 172	10111110101
	0xBEE0,	// ASCII = 173	10111110111
	0xBF60,	// ASCII = 174	10111111011
	0xBFA0,	// ASCII = 175	10111111101
	0xBFE0,	// ASCII = 176	10111111111
	0xD560,	// ASCII = 177	11010101011
	0xD5A0,	// ASCII = 178	11010101101
	0xD5E0,	// ASCII = 179	11010101111
	0xD6A0,	// ASCII = 180	11010110101
	0xD6E0,	// ASCII = 181	11010110111
	0xD760,	// ASCII = 182	11010111011
	0xD7A0,	// ASCII = 183	11010111101
	0xD7E0,	// ASCII = 184	11010111111
	0xDAA0,	// ASCII = 185	11011010101
	0xDAE0,	// ASCII = 186	11011010111
	0xDB60,	// ASCII = 187	11011011011
	0xDBA0,	// ASCII = 188	11011011101
	0xDBE0,	// ASCII = 189	11011011111
	0xDD60,	// ASCII = 190	11011101011
	0xDDA0,	// ASCII = 191	11011101101
	0xDDE0,	// ASCII = 192	11011101111
	0xDEA0,	// ASCII = 193	11011110101
	0xDEE0,	// ASCII = 194	11011110111
	0xDF60,	// ASCII = 195	11011111011
	0xDFA0,	// ASCII = 196	11011111101
	0xDFE0,	// ASCII = 197	11011111111
	0xEAA0,	// ASCII = 198	11101010101
	0xEAE0,	// ASCII = 199	11101010111
	0xEB60,	// ASCII = 200	11101011011
	0xEBA0,	// ASCII = 201	11101011101
	0xEBE0,	// ASCII = 202	11101011111
	0xED60,	// ASCII = 203	11101101011
	0xEDA0,	// ASCII = 204	11101101101
	0xEDE0,	// ASCII = 205	11101101111
	0xEEA0,	// ASCII = 206	11101110101
	0xEEE0,	// ASCII = 207	11101110111
	0xEF60,	// ASCII = 208	11101111011
	0xEFA0,	// ASCII = 209	11101111101
	0xEFE0,	// ASCII = 210	11101111111
	0xF560,	// ASCII = 211	11110101011
	0xF5A0,	// ASCII = 212	11110101101
	0xF5E0,	// ASCII = 213	11110101111
	0xF6A0,	// ASCII = 214	11110110101
	0xF6E0,	// ASCII = 215	11110110111
	0xF760,	// ASCII = 216	11110111011
	0xF7A0,	// ASCII = 217	11110111101
	0xF7E0,	// ASCII = 218	11110111111
	0xFAA0,	// ASCII = 219	11111010101
	0xFAE0,	// ASCII = 220	11111010111
	0xFB60,	// ASCII = 221	11111011011
	0xFBA0,	// ASCII = 222	11111011101
	0xFBE0,	// ASCII = 223	11111011111
	0xFD60,	// ASCII = 224	11111101011
	0xFDA0,	// ASCII = 225	11111101101
	0xFDE0,	// ASCII = 226	11111101111
	0xFEA0,	// ASCII = 227	11111110101
	0xFEE0,	// ASCII = 228	11111110111
	0xFF60,	// ASCII = 229	11111111011
	0xFFA0,	// ASCII = 230	11111111101
	0xFFE0,	// ASCII = 231	11111111111
	0xAAB0,	// ASCII = 232	101010101011
	0xAAD0,	// ASCII = 233	101010101101
	0xAAF0,	// ASCII = 234	101010101111
	0xAB50,	// ASCII = 235	101010110101
	0xAB70,	// ASCII = 236	101010110111
	0xABB0,	// ASCII = 237	101010111011
	0xABD0,	// ASCII = 238	101010111101
	0xABF0,	// ASCII = 239	101010111111
	0xAD50,	// ASCII = 240	101011010101
	0xAD70,	// ASCII = 241	101011010111
	0xADB0,	// ASCII = 242	101011011011
	0xADD0,	// ASCII = 243	101011011101
	0xADF0,	// ASCII = 244	101011011111
	0xAEB0,	// ASCII = 245	101011101011
	0xAED0,	// ASCII = 246	101011101101
	0xAEF0,	// ASCII = 247	101011101111
	0xAF50,	// ASCII = 248	101011110101
	0xAF70,	// ASCII = 249	101011110111
	0xAFB0,	// ASCII = 250	101011111011
	0xAFD0,	// ASCII = 251	101011111101
	0xAFF0,	// ASCII = 252	101011111111
	0xB550,	// ASCII = 253	101101010101
	0xB570,	// ASCII = 254	101101010111
	0xB5B0	// ASCII = 255	101101011011
};



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
IMD=0.0; // Means Unkonown
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
    if (m_OffCount > 10)
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
