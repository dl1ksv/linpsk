//////////////////////////////////////////////////////////////////////
// PSKMod.cpp: implementation of the PSKModulator class.
//
//////////////////////////////////////////////////////////////////////
//      PSK31/CW modulator
// Copyright 1999.    Moe Wheatley AE4JY  <ae4jy@mindspring.com>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//////////////////////////////////////////////////////////////////////
//
// modified by Volker Schroer , DL1KSV for use in LinPsk

#include <math.h>
#include "psktable.h"
#include "pskmodulator.h"
#include "ctxbuffer.h"


// local defines.................

#define PHZ_0 0			//specify various signal phase states
#define PHZ_90 1
#define PHZ_180 2
#define PHZ_270 3
#define PHZ_OFF 4

#define SYM_NOCHANGE 0	//Stay the same phase
#define SYM_P90 1		//Plus 90  deg
#define SYM_P180 2		//Plus 180 deg
#define SYM_M90 3		//Minus 90 deg
#define SYM_OFF 4		//No output
#define SYM_ON 5		//constant output


#define SYMBOL_RATE 31.25		// 31.25 Symbols per Second

#define MAXRAMP_SIZE (((100*11100)/3125)+1) // max number of envelope ramp steps per symbol
/** Not needed any longer 
#define TX_CONSTANT 23000.0		// TX Amplitude Factor
 **/
#define CW_SPEED 1		//bigger is slower. 1 is fastest.

// The use of static's here preclude having multiple instantiations
// of this class but should not be an issue since only one soundcard.


//Ramp shape tables that are loaded with cosine shaped functions at init
static double PSKShapeTbl_Z[MAXRAMP_SIZE]; // 0
static double PSKShapeTbl_P[MAXRAMP_SIZE]; // +1
static double PSKShapeTbl_M[MAXRAMP_SIZE]; // -1
static double PSKShapeTbl_ZP[MAXRAMP_SIZE]; // 0 to +1
static double PSKShapeTbl_PZ[MAXRAMP_SIZE]; // +1 to 0
static double PSKShapeTbl_MZ[MAXRAMP_SIZE]; // -1 to 0
static double PSKShapeTbl_PM[MAXRAMP_SIZE]; // +1 to -1
static double PSKShapeTbl_MP[MAXRAMP_SIZE]; // -1 to +1
// each character is separated by two zeros. The bits are sent msbit first.
const unsigned short int PSKModulator::VARICODE_TABLE[] = {
  0xAAC0, // ASCII =   0	1010101011
  0xB6C0, // ASCII =   1	1011011011
  0xBB40, // ASCII =   2	1011101101
  0xDDC0, // ASCII =   3	1101110111
  0xBAC0, // ASCII =   4	1011101011
  0xD7C0, // ASCII =   5	1101011111
  0xBBC0, // ASCII =   6	1011101111
  0xBF40, // ASCII =   7	1011111101
  0xBFC0, // ASCII =   8	1011111111
  0xEF00, // ASCII =   9	11101111
  0xE800, // ASCII =  10	11101
  0xDBC0, // ASCII =  11	1101101111
  0xB740, // ASCII =  12	1011011101
  0xF800, // ASCII =  13	11111
  0xDD40, // ASCII =  14	1101110101
  0xEAC0, // ASCII =  15	1110101011
  0xBDC0, // ASCII =  16	1011110111
  0xBD40, // ASCII =  17	1011110101
  0xEB40, // ASCII =  18	1110101101
  0xEBC0, // ASCII =  19	1110101111
  0xD6C0, // ASCII =  20	1101011011
  0xDAC0, // ASCII =  21	1101101011
  0xDB40, // ASCII =  22	1101101101
  0xD5C0, // ASCII =  23	1101010111
  0xDEC0, // ASCII =  24	1101111011
  0xDF40, // ASCII =  25	1101111101
  0xEDC0, // ASCII =  26	1110110111
  0xD540, // ASCII =  27	1101010101
  0xD740, // ASCII =  28	1101011101
  0xEEC0, // ASCII =  29	1110111011
  0xBEC0, // ASCII =  30	1011111011
  0xDFC0, // ASCII =  31	1101111111
  0x8000, // ASCII = ' '	1
  0xFF80, // ASCII = '!'	111111111
  0xAF80, // ASCII = '"'	101011111
  0xFA80, // ASCII = '#'	111110101
  0xED80, // ASCII = '$'	111011011
  0xB540, // ASCII = '%'	1011010101
  0xAEC0, // ASCII = '&'	1010111011
  0xBF80, // ASCII = '''	101111111
  0xFB00, // ASCII = '('	11111011
  0xF700, // ASCII = ')'	11110111
  0xB780, // ASCII = '*'	101101111
  0xEF80, // ASCII = '+'	111011111
  0xEA00, // ASCII = ','	1110101
  0xD400, // ASCII = '-'	110101
  0xAE00, // ASCII = '.'	1010111
  0xD780, // ASCII = '/'	110101111
  0xB700, // ASCII = '0'	10110111
  0xBD00, // ASCII = '1'	10111101
  0xED00, // ASCII = '2'	11101101
  0xFF00, // ASCII = '3'	11111111
  0xBB80, // ASCII = '4'	101110111
  0xAD80, // ASCII = '5'	101011011
  0xB580, // ASCII = '6'	101101011
  0xD680, // ASCII = '7'	110101101
  0xD580, // ASCII = '8'	110101011
  0xDB80, // ASCII = '9'	110110111
  0xF500, // ASCII = ':'	11110101
  0xDE80, // ASCII = ';'	110111101
  0xF680, // ASCII = '<'	111101101
  0xAA00, // ASCII = '='	1010101
  0xEB80, // ASCII = '>'	111010111
  0xABC0, // ASCII = '?'	1010101111
  0xAF40, // ASCII = '@'	1010111101
  0xFA00, // ASCII = 'A'	1111101
  0xEB00, // ASCII = 'B'	11101011
  0xAD00, // ASCII = 'C'	10101101
  0xB500, // ASCII = 'D'	10110101
  0xEE00, // ASCII = 'E'	1110111
  0xDB00, // ASCII = 'F'	11011011
  0xFD00, // ASCII = 'G'	11111101
  0xAA80, // ASCII = 'H'	101010101
  0xFE00, // ASCII = 'I'	1111111
  0xFE80, // ASCII = 'J'	111111101
  0xBE80, // ASCII = 'K'	101111101
  0xD700, // ASCII = 'L'	11010111
  0xBB00, // ASCII = 'M'	10111011
  0xDD00, // ASCII = 'N'	11011101
  0xAB00, // ASCII = 'O'	10101011
  0xD500, // ASCII = 'P'	11010101
  0xEE80, // ASCII = 'Q'	111011101
  0xAF00, // ASCII = 'R'	10101111
  0xDE00, // ASCII = 'S'	1101111
  0xDA00, // ASCII = 'T'	1101101
  0xAB80, // ASCII = 'U'	101010111
  0xDA80, // ASCII = 'V'	110110101
  0xAE80, // ASCII = 'W'	101011101
  0xBA80, // ASCII = 'X'	101110101
  0xBD80, // ASCII = 'Y'	101111011
  0xAB40, // ASCII = 'Z'	1010101101
  0xFB80, // ASCII = '['	111110111
  0xF780, // ASCII = '\'	111101111
  0xFD80, // ASCII = ']'	111111011
  0xAFC0, // ASCII = '^'	1010111111
  0xB680, // ASCII = '_'	101101101
  0xB7C0, // ASCII = '`'	1011011111
  0xB000, // ASCII = 'a'	1011
  0xBE00, // ASCII = 'b'	1011111
  0xBC00, // ASCII = 'c'	101111
  0xB400, // ASCII = 'd'	101101
  0xC000, // ASCII = 'e'	11
  0xF400, // ASCII = 'f'	111101
  0xB600, // ASCII = 'g'	1011011
  0xAC00, // ASCII = 'h'	101011
  0xD000, // ASCII = 'i'	1101
  0xF580, // ASCII = 'j'	111101011
  0xBF00, // ASCII = 'k'	10111111
  0xD800, // ASCII = 'l'	11011
  0xEC00, // ASCII = 'm'	111011
  0xF000, // ASCII = 'n'	1111
  0xE000, // ASCII = 'o'	111
  0xFC00, // ASCII = 'p'	111111
  0xDF80, // ASCII = 'q'	110111111
  0xA800, // ASCII = 'r'	10101
  0xB800, // ASCII = 's'	10111
  0xA000, // ASCII = 't'	101
  0xDC00, // ASCII = 'u'	110111
  0xF600, // ASCII = 'v'	1111011
  0xD600, // ASCII = 'w'	1101011
  0xDF00, // ASCII = 'x'	11011111
  0xBA00, // ASCII = 'y'	1011101
  0xEA80, // ASCII = 'z'	111010101
  0xADC0, // ASCII = '{'	1010110111
  0xDD80, // ASCII = '|'	110111011
  0xAD40, // ASCII = '}'	1010110101
  0xB5C0, // ASCII = '~'	1011010111
  0xED40, // ASCII = 127	1110110101
  0xEF40, // ASCII = 128	1110111101
  0xEFC0, // ASCII = 129	1110111111
  0xF540, // ASCII = 130	1111010101
  0xF5C0, // ASCII = 131	1111010111
  0xF6C0, // ASCII = 132	1111011011
  0xF740, // ASCII = 133	1111011101
  0xF7C0, // ASCII = 134	1111011111
  0xFAC0, // ASCII = 135	1111101011
  0xFB40, // ASCII = 136	1111101101
  0xFBC0, // ASCII = 137	1111101111
  0xFD40, // ASCII = 138	1111110101
  0xFDC0, // ASCII = 139	1111110111
  0xFEC0, // ASCII = 140	1111111011
  0xFF40, // ASCII = 141	1111111101
  0xFFC0, // ASCII = 142	1111111111
  0xAAA0, // ASCII = 143	10101010101
  0xAAE0, // ASCII = 144	10101010111
  0xAB60, // ASCII = 145	10101011011
  0xABA0, // ASCII = 146	10101011101
  0xABE0, // ASCII = 147	10101011111
  0xAD60, // ASCII = 148	10101101011
  0xADA0, // ASCII = 149	10101101101
  0xADE0, // ASCII = 150	10101101111
  0xAEA0, // ASCII = 151	10101110101
  0xAEE0, // ASCII = 152	10101110111
  0xAF60, // ASCII = 153	10101111011
  0xAFA0, // ASCII = 154	10101111101
  0xAFE0, // ASCII = 155	10101111111
  0xB560, // ASCII = 156	10110101011
  0xB5A0, // ASCII = 157	10110101101
  0xB5E0, // ASCII = 158	10110101111
  0xB6A0, // ASCII = 159	10110110101
  0xB6E0, // ASCII = 160	10110110111
  0xB760, // ASCII = 161	10110111011
  0xB7A0, // ASCII = 162	10110111101
  0xB7E0, // ASCII = 163	10110111111
  0xBAA0, // ASCII = 164	10111010101
  0xBAE0, // ASCII = 165	10111010111
  0xBB60, // ASCII = 166	10111011011
  0xBBA0, // ASCII = 167	10111011101
  0xBBE0, // ASCII = 168	10111011111
  0xBD60, // ASCII = 169	10111101011
  0xBDA0, // ASCII = 170	10111101101
  0xBDE0, // ASCII = 171	10111101111
  0xBEA0, // ASCII = 172	10111110101
  0xBEE0, // ASCII = 173	10111110111
  0xBF60, // ASCII = 174	10111111011
  0xBFA0, // ASCII = 175	10111111101
  0xBFE0, // ASCII = 176	10111111111
  0xD560, // ASCII = 177	11010101011
  0xD5A0, // ASCII = 178	11010101101
  0xD5E0, // ASCII = 179	11010101111
  0xD6A0, // ASCII = 180	11010110101
  0xD6E0, // ASCII = 181	11010110111
  0xD760, // ASCII = 182	11010111011
  0xD7A0, // ASCII = 183	11010111101
  0xD7E0, // ASCII = 184	11010111111
  0xDAA0, // ASCII = 185	11011010101
  0xDAE0, // ASCII = 186	11011010111
  0xDB60, // ASCII = 187	11011011011
  0xDBA0, // ASCII = 188	11011011101
  0xDBE0, // ASCII = 189	11011011111
  0xDD60, // ASCII = 190	11011101011
  0xDDA0, // ASCII = 191	11011101101
  0xDDE0, // ASCII = 192	11011101111
  0xDEA0, // ASCII = 193	11011110101
  0xDEE0, // ASCII = 194	11011110111
  0xDF60, // ASCII = 195	11011111011
  0xDFA0, // ASCII = 196	11011111101
  0xDFE0, // ASCII = 197	11011111111
  0xEAA0, // ASCII = 198	11101010101
  0xEAE0, // ASCII = 199	11101010111
  0xEB60, // ASCII = 200	11101011011
  0xEBA0, // ASCII = 201	11101011101
  0xEBE0, // ASCII = 202	11101011111
  0xED60, // ASCII = 203	11101101011
  0xEDA0, // ASCII = 204	11101101101
  0xEDE0, // ASCII = 205	11101101111
  0xEEA0, // ASCII = 206	11101110101
  0xEEE0, // ASCII = 207	11101110111
  0xEF60, // ASCII = 208	11101111011
  0xEFA0, // ASCII = 209	11101111101
  0xEFE0, // ASCII = 210	11101111111
  0xF560, // ASCII = 211	11110101011
  0xF5A0, // ASCII = 212	11110101101
  0xF5E0, // ASCII = 213	11110101111
  0xF6A0, // ASCII = 214	11110110101
  0xF6E0, // ASCII = 215	11110110111
  0xF760, // ASCII = 216	11110111011
  0xF7A0, // ASCII = 217	11110111101
  0xF7E0, // ASCII = 218	11110111111
  0xFAA0, // ASCII = 219	11111010101
  0xFAE0, // ASCII = 220	11111010111
  0xFB60, // ASCII = 221	11111011011
  0xFBA0, // ASCII = 222	11111011101
  0xFBE0, // ASCII = 223	11111011111
  0xFD60, // ASCII = 224	11111101011
  0xFDA0, // ASCII = 225	11111101101
  0xFDE0, // ASCII = 226	11111101111
  0xFEA0, // ASCII = 227	11111110101
  0xFEE0, // ASCII = 228	11111110111
  0xFF60, // ASCII = 229	11111111011
  0xFFA0, // ASCII = 230	11111111101
  0xFFE0, // ASCII = 231	11111111111
  0xAAB0, // ASCII = 232	101010101011
  0xAAD0, // ASCII = 233	101010101101
  0xAAF0, // ASCII = 234	101010101111
  0xAB50, // ASCII = 235	101010110101
  0xAB70, // ASCII = 236	101010110111
  0xABB0, // ASCII = 237	101010111011
  0xABD0, // ASCII = 238	101010111101
  0xABF0, // ASCII = 239	101010111111
  0xAD50, // ASCII = 240	101011010101
  0xAD70, // ASCII = 241	101011010111
  0xADB0, // ASCII = 242	101011011011
  0xADD0, // ASCII = 243	101011011101
  0xADF0, // ASCII = 244	101011011111
  0xAEB0, // ASCII = 245	101011101011
  0xAED0, // ASCII = 246	101011101101
  0xAEF0, // ASCII = 247	101011101111
  0xAF50, // ASCII = 248	101011110101
  0xAF70, // ASCII = 249	101011110111
  0xAFB0, // ASCII = 250	101011111011
  0xAFD0, // ASCII = 251	101011111101
  0xAFF0, // ASCII = 252	101011111111
  0xB550, // ASCII = 253	101101010101
  0xB570, // ASCII = 254	101101010111
  0xB5B0 // ASCII = 255	101101011011
};

struct PSKStruct {
  double* iptr;
  double* qptr;

  int next;
};

typedef PSKStruct PSKSTATE;

//Lookup table for determining the next ramp shape depending on the
//  next symbol and the present output phase.
// indexing format is [symbol][presentPhase]
//returns the PSKSTRUCT containing the next phase and the I and Q
//   ramp table pointers.

static PSKSTATE PSKPhaseLookupTable[6][5] ={
  // SYMBOL = 0 = SYM_NOCHANGE
  //   I ramp shape     Q ramp shape     Next Phase
  {
    {PSKShapeTbl_P, PSKShapeTbl_P, PHZ_0}, //present PHZ_0
    {PSKShapeTbl_M, PSKShapeTbl_P, PHZ_90}, //present PHZ_90
    {PSKShapeTbl_M, PSKShapeTbl_M, PHZ_180}, //present PHZ_180
    {PSKShapeTbl_P, PSKShapeTbl_M, PHZ_270}, //present PHZ_270
    {PSKShapeTbl_Z, PSKShapeTbl_Z, PHZ_OFF}}, //present PHZ_OFF
  // SYMBOL = 1 = SYM_P90 = Advance 90 degrees
  //   I ramp shape     Q ramp shape     Next Phase
  {
    {PSKShapeTbl_PM, PSKShapeTbl_P, PHZ_90}, //present PHZ_0
    {PSKShapeTbl_M, PSKShapeTbl_PM, PHZ_180}, //present PHZ_90
    {PSKShapeTbl_MP, PSKShapeTbl_M, PHZ_270}, //present PHZ_180
    {PSKShapeTbl_P, PSKShapeTbl_MP, PHZ_0}, //present PHZ_270
    {PSKShapeTbl_ZP, PSKShapeTbl_ZP, PHZ_0}}, //present PHZ_OFF
  // SYMBOL = 2 = SYM_P180 = Advance 180 degrees
  //   I ramp shape     Q ramp shape     Next Phase
  {
    {PSKShapeTbl_PM, PSKShapeTbl_PM, PHZ_180}, //present PHZ_0
    {PSKShapeTbl_MP, PSKShapeTbl_PM, PHZ_270}, //present PHZ_90
    {PSKShapeTbl_MP, PSKShapeTbl_MP, PHZ_0}, //present PHZ_180
    {PSKShapeTbl_PM, PSKShapeTbl_MP, PHZ_90}, //present PHZ_270
    {PSKShapeTbl_ZP, PSKShapeTbl_ZP, PHZ_0}}, //present PHZ_OFF
  // SYMBOL = 3 = SYM_M90	= retard 90 degrees
  //   I ramp shape     Q ramp shape     Next Phase
  {
    {PSKShapeTbl_P, PSKShapeTbl_PM, PHZ_270}, //present PHZ_0
    {PSKShapeTbl_MP, PSKShapeTbl_P, PHZ_0}, //present PHZ_90
    {PSKShapeTbl_M, PSKShapeTbl_MP, PHZ_90}, //present PHZ_180
    {PSKShapeTbl_PM, PSKShapeTbl_M, PHZ_180}, //present PHZ_270
    {PSKShapeTbl_ZP, PSKShapeTbl_ZP, PHZ_0}}, //present PHZ_OFF
  // SYMBOL = 4 = SYM_OFF
  //   I ramp shape     Q ramp shape     Next Phase
  {
    {PSKShapeTbl_PZ, PSKShapeTbl_PZ, PHZ_OFF}, //present PHZ_0
    {PSKShapeTbl_MZ, PSKShapeTbl_PZ, PHZ_OFF}, //present PHZ_90
    {PSKShapeTbl_MZ, PSKShapeTbl_MZ, PHZ_OFF}, //present PHZ_180
    {PSKShapeTbl_PZ, PSKShapeTbl_MZ, PHZ_OFF}, //present PHZ_270
    {PSKShapeTbl_Z, PSKShapeTbl_Z, PHZ_OFF}}, //present PHZ_OFF
  // SYMBOL = 5 = SYM_ON
  //   I ramp shape     Q ramp shape     Next Phase
  {
    {PSKShapeTbl_P, PSKShapeTbl_P, PHZ_0}, //present PHZ_0
    {PSKShapeTbl_MP, PSKShapeTbl_P, PHZ_0}, //present PHZ_90
    {PSKShapeTbl_MP, PSKShapeTbl_MP, PHZ_0}, //present PHZ_180
    {PSKShapeTbl_P, PSKShapeTbl_MP, PHZ_0}, //present PHZ_270
    {PSKShapeTbl_ZP, PSKShapeTbl_ZP, PHZ_0}} //present PHZ_OFF
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSKModulator::PSKModulator(int FS, double freq, CTxBuffer *TxBuffer) : CModulator(FS, TxBuffer) {

  m_AmblePtr = 0;
  int RampSize;
  int i;

  m_PSKPhaseInc = PI2 * freq / (double) SampleRate; //carrier frequency

  m_PSKSecPerSamp = 1.0 / (double) SampleRate;
  m_PSKTime = 0.0;
  m_t = 0.0;
  m_Ramp = 0;
  m_PSKPeriodUpdate = 1.0 / SYMBOL_RATE; //symbol period
  m_Lastsymb = SYM_OFF;
  m_AddEndingZero = true;
  m_CWState = 0;
  m_CWtimer = 0;
  // Generate cosine ramp envelope lookup tables
  RampSize = (((100 * SampleRate) / 3125) + 1); //  number of envelope ramp steps per symbol
  for (i = 0; i < RampSize; i++) {
    PSKShapeTbl_Z[i] = 0.0;
    PSKShapeTbl_P[i] = 1.0;
    PSKShapeTbl_M[i] = -1.0;
    PSKShapeTbl_PM[i] = cos((double) i * PI2 / (RampSize * 2));
    PSKShapeTbl_MP[i] = -PSKShapeTbl_PM[i];

    if (i < RampSize / 2) {
      PSKShapeTbl_PZ[i] = cos((double) i * PI2 / (RampSize * 2));
      PSKShapeTbl_MZ[i] = -PSKShapeTbl_PZ[i];
      PSKShapeTbl_ZP[i] = 0.0;
    } else {
      PSKShapeTbl_ZP[i] = -cos((double) i * PI2 / (RampSize * 2));
      PSKShapeTbl_PZ[i] = 0.0;
      PSKShapeTbl_MZ[i] = 0.0;
    }

  }
  i = 0;
  while (i < 32) //create post/preamble tables
  {
    m_Preamble[i] = TXTOG_CODE;
    m_Postamble[i++] = TXON_CODE;
  }
  m_Preamble[i] = 0; // null terminate these tables
  m_Postamble[i] = 0;

  m_pPSKtxI = PSKShapeTbl_Z;
  m_pPSKtxQ = PSKShapeTbl_Z;
  m_PresentPhase = PHZ_OFF;
  m_TxShiftReg = 0;
  status = TX_PREAMBLE_STATE;

}

PSKModulator::~PSKModulator() {
}

///////////++++++++++++++++++++++++++++++++++////////////////
///////////   P S K 3 1   M O D U L A T O R  ////////////////
///////////++++++++++++++++++++++++++++++++++////////////////



/////////////////////////////////////////////////////////////
// generates n samples of psk31 waveform in data array pData
/////////////////////////////////////////////////////////////

int PSKModulator::CalcSignal(double *pData, int n)
 {
  int symbol;
  int i;

  //		m_RMSConstant = TX_CONSTANT;

  for (i = 0; i < n; i++) //calculate n samples of tx data stream
  {
    m_t += m_PSKPhaseInc; // increment radian phase count

    pData[i] = 0.3 * (m_pPSKtxI[m_Ramp] * sin(m_t) + m_pPSKtxQ[m_Ramp] * cos(m_t));
    m_Ramp++;
    m_PSKTime += m_PSKSecPerSamp;
    if (m_PSKTime >= m_PSKPeriodUpdate)//if time to update envelope ramp index
    {
      m_PSKTime -= m_PSKPeriodUpdate; //keep time bounded
      m_Ramp = 0; // time to update symbol
      m_t = fmod(m_t, PI2); //keep radian counter bounded
      if (status == TX_CWID_STATE)
        symbol = GetNextCWSymbol();
      else
        symbol = GetNextSymbol();

      //get new I/Q ramp tables and next phase
      m_pPSKtxI = PSKPhaseLookupTable[symbol][m_PresentPhase].iptr;
      m_pPSKtxQ = PSKPhaseLookupTable[symbol][m_PresentPhase].qptr;
      m_PresentPhase = PSKPhaseLookupTable[symbol][m_PresentPhase].next;
    }
    if (status == TX_END_STATE) // We have reached end of Transmission
      return -i;
  }
  return n;
}


/////////////////////////////////////////////////////////////
// called every symbol time to get next CW symbol and get the
// next character from the character Queue if no more symbols
// are left to send.
/////////////////////////////////////////////////////////////

char PSKModulator::GetNextCWSymbol(void) {
  char symb;
  int ch;
  symb = m_Lastsymb; //use last symbol unless it needs to change
  if ((m_TxShiftReg == 0) && (m_CWState == 0)) {
    ch = GetChar(); //get next character to xmit
    if (ch >= 0) //if is not a control code
    {
      ch &= 0xFF;
      ch = (int) toupper((char) ch); //make upper case
      if (ch >= ' ' && ch <= 'Z')
        m_TxShiftReg = CW_TABLE[ ch - ' ']; //look up pattern
    } else // is a control code
    {
      if (ch == TXON_CODE)
        symb = SYM_ON;
      else
        symb = SYM_OFF;
      return symb;
    }
    m_CWState = 0;
  }
  switch (m_CWState) // CW timing state machine
  {
    case 0: //get next cw symbol state from pattern
      switch (m_TxShiftReg & 0xC000) {
        case 0x4000: //dot
          m_CWState = 1;
          m_CWtimer = 1 * CW_SPEED;
          symb = SYM_ON;
          break;
        case 0x8000: //dash
          m_CWState = 1;
          m_CWtimer = 3 * CW_SPEED;
          symb = SYM_ON;
          break;
        case 0xC000: //inter char space
          m_CWState = 2;
          m_CWtimer = 3 * CW_SPEED;
          symb = SYM_OFF;
          break;
        default:
          symb = SYM_OFF;
          break;
      }
      m_TxShiftReg = m_TxShiftReg << 2; //
      break;
    case 1: //On time state
      if (--m_CWtimer <= 0) {
        m_CWState = 2;
        m_CWtimer = 1 * CW_SPEED; //inter symbol time
        symb = SYM_OFF;
      } else
        symb = SYM_NOCHANGE;
      break;
    case 2: //Off time state
      if (--m_CWtimer <= 0)
        m_CWState = 0;
      break;
  }
  m_Lastsymb = symb;
  return symb;
}
/////////////////////////////////////////////////////////////
//get next character/symbol depending on TX state.
/////////////////////////////////////////////////////////////

int PSKModulator::GetChar() {
  int ch=0;
  switch (status) {
    case TX_OFF_STATE: //is receiving
      ch = TXOFF_CODE; //else turn off
      break;
    case TX_TUNE_STATE:
      ch = TXON_CODE; // steady carrier
      break;
    case TX_PAUSED_STATE:
      ch = TXTOG_CODE; // steady idle symbol
      break;
    case TX_POSTAMBLE_STATE: // ending sequence
      ch = m_Postamble[m_AmblePtr++];
      if (ch == 0) //m_Postamble is 0 terminated
      {
        m_AmblePtr = 0;
        ch = TXOFF_CODE;
        status = TX_END_STATE;
      }
      break;
    case TX_PREAMBLE_STATE: //starting sequence
      if (!(ch = m_Preamble[m_AmblePtr++])) {
        status = TX_SENDING_STATE;
        m_AmblePtr = 0;
        ch = TXTOG_CODE;
      }
      break;
    case TX_CWID_STATE: // id sending CW ID
    /**
    if( m_AmblePtr >= settings.CWIdString.length() )
      {
       m_AmblePtr = 0;
       ch = TXOFF_CODE;
       status = TX_END_STATE;
      }
      else
      ch = settings.CWIdString.at(m_AmblePtr++).cell();
       **/
      ch = TXOFF_CODE;
      break;
    case TX_SENDING_STATE: //if sending text from TX window
      ch = Buffer->getTxChar();
      if (ch > 0) {
        emit charSend((char) ch);
        m_AmblePtr = 0;
      } else
        if (ch == TXOFF_CODE)
        status = TX_POSTAMBLE_STATE;
      m_AmblePtr = 0;
      break;
    case TX_END_STATE:
      ch=0;
      break;
  }
//  last = ch;
  return ( ch);
}

