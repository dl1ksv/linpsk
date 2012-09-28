/***************************************************************************
                        rttydemodulator.cpp  -  description
                             -------------------
    begin                : Mon Jun 4 2001
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

#include "rttydemodulator.h"
#include "firfilter.h"
#include "constants.h"



RTTYDemodulator::RTTYDemodulator() : CDemodulator()
{
  int i;
  ShiftOn = false;
  BufferPointer = 0;
  BufferCount = 0;
  StopBitLength = 0;
  RxFrequency = 0.0; // Will be set correctly in setFrequency !
//Initialize ExtraParameter
  extraParameter.stopbits = Onepoint5;
  extraParameter.parity = None;
  extraParameter.reverse = true;
  extraParameter.offset = 170;
  /** Have to use new with ggc 4, otherwise I get a lot of nan's in fftw_execute, at least on AMD64  **/
  pIn        = new complex<double>[FFTLENGTH];
  pOutF0     = new complex<double>[FFTLENGTH];
  pOutF1     = new complex<double>[FFTLENGTH];
  pFilterF0  = new complex<double>[FFTLENGTH];
  pFilterF1  = new complex<double>[FFTLENGTH];
  pFilterIn  = new complex<double>[FFTLENGTH];
  pResultF0  = new complex<double>[FFTLENGTH];
  pResultF1  = new complex<double>[FFTLENGTH];
  pOverlapF0 = new complex<double>[FFTFILTERLENGTH-1];
  pOverlapF1 =  new complex<double>[FFTFILTERLENGTH-1];

  pforward   = fftw_plan_dft_1d ( FFTLENGTH, ( fftw_complex * ) pIn, ( fftw_complex * ) pOutF0, FFTW_FORWARD, FFTW_ESTIMATE );
  pbackwardF0 = fftw_plan_dft_1d ( FFTLENGTH, ( fftw_complex * ) pOutF0, ( fftw_complex * ) pResultF0, FFTW_BACKWARD, FFTW_ESTIMATE );
  pbackwardF1 = fftw_plan_dft_1d ( FFTLENGTH, ( fftw_complex * ) pOutF1, ( fftw_complex * ) pResultF1, FFTW_BACKWARD, FFTW_ESTIMATE );
  pfilter  = fftw_plan_dft_1d ( FFTLENGTH, ( fftw_complex * ) pFilterIn, ( fftw_complex * ) pFilterF0, FFTW_FORWARD, FFTW_ESTIMATE );


  for ( i = 0; i < FFTFILTERLENGTH - 1;i++ )
  {
    pOverlapF0[i] = complex<double> ( 0., 0. );
    pOverlapF1[i] = complex<double> ( 0., 0. );
  }
  filteredSamples = 0;
  delayZ = complex<double> ( 0., 0. );
}

RTTYDemodulator::~RTTYDemodulator()
{
  fftw_destroy_plan ( pforward );
  fftw_destroy_plan ( pbackwardF0 );
  fftw_destroy_plan ( pbackwardF1 );
  fftw_destroy_plan ( pfilter );
  fftw_free ( pIn );
  fftw_free ( pOutF0 );
  fftw_free ( pOutF1 );
  fftw_free ( pFilterF0 );
  fftw_free ( pFilterF1 );
  fftw_free ( pFilterIn );
  fftw_free ( pResultF0 );
  fftw_free ( pResultF1 );
  fftw_free ( pOverlapF0 );
  fftw_free ( pOverlapF1 );
}
/** returns the asci char corresponding to the baudot code */
char RTTYDemodulator::baudot_code ( char data )
{
  /** Table of letters */

  static const char letters[32] =
  {
    0x00, 'E', '\r', 'A', ' ', 'S', 'I', 'U',
    '\n', 'D', 'R', 'J', 'N', 'F', 'C', 'K',
    'T', 'Z', 'L', 'W', 'H', 'Y', 'P', 'Q',
    'O', 'B', 'G', '^', 'M', 'X', 'V', '^'
  };


  /** Table of symbols */
  static const char symbols[32] =
  {
    0x00, '3', '\r', '-', ' ', '\'', '8', '7',
    '\n', '$', '4', '#', ',', '!', ':', '(',
    '5', '"', ')', '2', '#', '6', '0', '1',
    '9', '?', '&', '^', '.', '/', '=', '^'
  };

  char c;

  switch ( data )
  {
    case 0x1f :
      ShiftOn = false;  //LTRS
      c = 0;
      break;
    case 0x1b :
      ShiftOn = true;   //FIGS
      c = 0;
      break;
    default:
      if ( !ShiftOn )
        c = letters[ ( int ) data];
      else
        c = symbols[ ( int ) data];
      break;
  }


  if ( c == ' ' ) // Unshift on Space
    ShiftOn = false;

  return c;
}

bool RTTYDemodulator::Init ( double FS, int  )
{
  SampleRate = FS;
  Baudrate = 45.45;
  NumberOfBits = 5;
  SymbolLength = int ( FS / Baudrate + 0.5 );
  Status = WaitingForMark;
  FrequencyChanged = false;
  ave1 = 0.5;
  ave2 = 0.0;
  setRxFrequency ( 1000. );
  F0inc = 0.;
  F1inc = 0.;
  F0max = 0.;
  F1max = 0.;
  return true;
}

void RTTYDemodulator::ProcessInput ( double *input, double * )
{
  char c1;
  int i, j, count;
  int StartBitCount, StartBitLength, StartBit2Lead;
  int StopBit2Count;
  int actSample;
// int Filtered[RESULTLENGTH];
// complex<double> omegaF0[RESULTLENGTH],omegaF1[RESULTLENGTH];
  int Filtered[RESULTLENGTHDOWN];
  double omegaF0[RESULTLENGTHDOWN], omegaF1[RESULTLENGTHDOWN];
  double f0sum, f1sum;
  double zf0sum, zf1sum, StartBitValue;

  count = 0;

// Calculating StopbitlengthStopBitLength
  switch ( extraParameter.stopbits )
  {
    case One:
      StopBitLength = NUMBEROFPROBES;
      break;
    case Onepoint5:
      StopBitLength = ( 3 * NUMBEROFPROBES ) / 2;
      break;
    case Two:
      StopBitLength = 2 * NUMBEROFPROBES;
      break;
  }

  while ( ( count + RESULTLENGTH ) < BUF_SIZE )
  {
    for ( i = filteredSamples;i < RESULTLENGTH;i++ )
      pIn[i] = complex<double> ( input[count++], 0. );
    filteredSamples = 0;
    fftw_execute ( pforward );
    execFilter();
    downmix ( pResultF0, pResultF1, RESULTLENGTH, Filtered, omegaF0, omegaF1 );
    actSample = 0;
    while ( actSample < RESULTLENGTHDOWN )
    {
      if ( ( BufferCount < SAMPLEBUFFERLENGTH ) )
      {
        while ( ( BufferCount < SAMPLEBUFFERLENGTH ) && ( actSample < RESULTLENGTHDOWN ) )
        {
          BufferCount++;
          Demod[BufferPointer] = Filtered[actSample];
          SF0[BufferPointer] = omegaF0[actSample];
          SF1[BufferPointer] = omegaF1[actSample];
          if ( !extraParameter.reverse )
          {
            Demod[BufferPointer] = -Demod[BufferPointer];
            SF0[BufferPointer] = omegaF1[actSample];
            SF1[BufferPointer] = omegaF0[actSample];
          }
          BufferPointer++;
          BufferPointer = BufferPointer % SAMPLEBUFFERLENGTH;
          actSample ++;
        } // End of filling CharacterData
      }
      while ( BufferCount == SAMPLEBUFFERLENGTH )
      {
        switch ( Status )  // Now let's analyze the data
        {
          case WaitingForMark:        // Waiting for Stopbit, previous state undefined
            // Check, if we are possibly at the beginning of a stop bit
            i = 0;
            while ( ( i < BufferCount ) && ( Demod[ ( BufferPointer + i ) % SAMPLEBUFFERLENGTH] < 0 ) )
              i++;
            if ( i == 0 ) // At the beginning
            {
              StopBit1Count = 0;
              f0sum = 0.;
              f1sum = 0.;
              StartBitLead = 0;
              while ( i < StopBitLength )
              {

                f0sum += SF0[ ( BufferPointer + i ) % SAMPLEBUFFERLENGTH];
                f1sum += SF1[ ( BufferPointer + i ) % SAMPLEBUFFERLENGTH];
                if ( Demod[ ( BufferPointer + i++ ) % SAMPLEBUFFERLENGTH] > 0 )
                {
                  StopBit1Count++;
                  StartBitLead = 0;
                }
                else
                  StartBitLead++;
              }
              zf0sum = abs ( f0sum );
              zf1sum = abs ( f1sum );
              StopBit1Value = zf1sum - zf0sum;
              if ( StartBitLead > 2 )
                StartBitLead = 2;
              if ( ( StopBit1Count  > StopBitLength / 2 ) || ( StopBit1Value > 0 ) )
                Status = WaitingForSpace;
              else
                BufferCount -= ( StopBitLength - StopBit1Count );
            }
            else         // Refill the buffer
              BufferCount -= i;
            break;

          case WaitingForSpace:      // Stopbit seems to be found, now waiting for transition
            //      i = StopBitLength  ;
            i = StopBitLength - StartBitLead;
            while ( ( i < SAMPLEBUFFERLENGTH ) && ( Demod[ ( BufferPointer + i ) % SAMPLEBUFFERLENGTH ] > 0 ) )
              i++;
            if ( i == SAMPLEBUFFERLENGTH )
            {
              BufferCount = StopBitLength;  // No Space found, keep only StopBitLength Samples
              CalcQuality((float)0.);
            }
            else
            {
              Status = CheckingStartBit;
              //         i = i - StopBitLength;
              //         i = i - StopBit1Count;
              i = i - StopBitLength + StartBitLead;
              if ( ( StartBitLead == 0 ) && ( i > 2 ) )
                i--;
              BufferCount -= i;             // Refill buffer
            }
            break;

          case CheckingStartBit:
            //      j = StopBitLength + BufferPointer;
            j = StopBitLength - StartBitLead + BufferPointer;
            StartBitCount = 0;
            StartBitLength = NUMBEROFPROBES;
            f0sum = 0.;
            f1sum = 0.;
            for ( i = 0; i < NUMBEROFPROBES; i++ )
            {
              f0sum += SF0[ ( i + j ) % SAMPLEBUFFERLENGTH];
              f1sum += SF1[ ( i + j ) % SAMPLEBUFFERLENGTH];
              if ( Demod[ ( i + j ) % SAMPLEBUFFERLENGTH]  < 0 )
                StartBitCount++;
            }
            zf0sum = abs ( f0sum );
            zf1sum = abs ( f1sum );
            StartBitValue = zf1sum - zf0sum;
            CalcQuality(abs(StartBitValue)/(zf0sum+zf1sum));
            Status = CheckingStopBits;
            /**
            if ( (StartBitCount > NUMBEROFPROBES/2) || ( zf0sum > zf1sum))
             Status = CheckingStopBits;
             else
             {
              Status = WaitingForMark;           // Was'nt the correct start bit
              BufferCount -= StopBitLength;
             }
             **/
            break;

          case CollectingByte:
            c1 = 0;
            for ( i = 0;i < NumberOfBits; i++ )
            {
              int j1, BitCount;
              j = ( BufferPointer + StopBitLength - StartBitLead + StartBitLength + i * NUMBEROFPROBES ) % SAMPLEBUFFERLENGTH;
              //       j=(BufferPointer + StopBit1Count + StartBitLength + i * NUMBEROFPROBES ) % SAMPLEBUFFERLENGTH;
              BitCount = 0;
              f0sum = 0.;
              f1sum = 0.;
              for ( j1 = j;j1 < j + NUMBEROFPROBES; j1++ )
              {

                if ( Demod[j1 % SAMPLEBUFFERLENGTH] > 0 )
                  BitCount++;
                f1sum += SF1[ j1 % SAMPLEBUFFERLENGTH];
                f0sum += SF0[ j1 % SAMPLEBUFFERLENGTH];

              }
              zf0sum = abs ( f0sum );
              zf1sum = abs ( f1sum );
              CalcQuality(abs(zf0sum-zf1sum)/(zf0sum+zf1sum));
              //       if ( BitCount > NUMBEROFPROBES/2 )
              //       if ( (zf1sum > zf0sum) || ( BitCount > NUMBEROFPROBES/2 ) )
              if ( zf1sum > zf0sum )
                c1 |= ( 1 << i );
            }
            if ( ( c1 > 0 ) && ( !Squelch || ( Squelch && ( ( unsigned int ) ( 100.*ave1 ) > CDemodulator::Threshold ) ) ) )
            {
              c1 = baudot_code ( c1 );
              if ( c1 > 0 )          // FIGS or LTRS result in c1 = 0 !
                emit newSymbol ( c1 );
            }


            if ( extraParameter.parity != None )
              Status = CheckingParity;
            else
            {
              StopBit1Value = zf1sum - zf0sum;
              /**
              if (  (StopBit2Count > (StopBitLength*2)/3)
                    ||(StopBit1Count > (StopBitLength*2)/3)
                    ||(StartBitCount > (StartBitLength*2)/3)   )
               **/
              if ( ( StopBit2Count > ( StopBitLength*2 ) / 3 ) || ( StopBit1Value > 0 ) )
                Status = WaitingForSpace;
              else
                //        Status=WaitingForMark;
                Status = CheckingStartBit;
              BufferCount -= ( StopBitLength + StartBitLength - StartBitLead  + 5 * NUMBEROFPROBES );
              StartBitLead = StartBit2Lead;
              StopBit1Count = StopBit2Count;

            }
            break;

          case CheckingParity: // Here we need BitsInData
            break;

          case CheckingStopBits:
            f0sum = 0.;
            f1sum = 0.;

            StopBit2Count = 0;
            StartBit2Lead = 0;
            j = BufferPointer + StopBitLength + StartBitLength - StartBitLead + NumberOfBits * NUMBEROFPROBES;
            //      j = BufferPointer+StopBit1Count + StartBitLength + NumberOfBits*NUMBEROFPROBES;
            for ( i = 0; i < StopBitLength;i++ )
            {
              f0sum += SF0[ ( j + i ) % SAMPLEBUFFERLENGTH];
              f1sum += SF1[ ( j + i ) % SAMPLEBUFFERLENGTH];

              if ( Demod[ ( j+i ) %SAMPLEBUFFERLENGTH] > 0 )
              {
                StopBit2Count++;
                StartBit2Lead = 0;
              }
              else
                StartBit2Lead++;
            }
            if ( StartBit2Lead > 2 )
              StartBit2Lead = 2;
            zf1sum = abs ( f1sum );
            zf0sum = abs ( f0sum );
            CalcQuality(abs(zf0sum-zf1sum)/(zf0sum+zf1sum));
            if ( ( ( ( StopBit1Count > ( 2* StopBitLength ) / 3 ) || ( StopBit1Value > 0 ) )
                   &&
                   ( ( StartBitCount > ( 2* NUMBEROFPROBES ) / 3 ) || ( StartBitValue < 0 ) ) )
                 ||
                 ( ( ( StartBitCount > ( 2* NUMBEROFPROBES ) / 3 ) || ( StartBitValue < 0 ) )
                   &&
                   ( ( StopBit2Count > ( 2* StopBitLength ) / 3 ) || ( zf1sum > zf0sum ) ) ) )
              Status = CollectingByte;
            else
            {
              Status = WaitingForMark;
              BufferCount -= ( StopBitLength - StopBit1Count + 1 );
            }
            break;
        }  // end of switch
      }
    }
  }
  if ( count < BUF_SIZE )
  {
    filteredSamples = 0;
    for ( i = count;i < BUF_SIZE;i++ )
      pIn[filteredSamples++] = complex<double> ( input[i], 0. );
  }
  else filteredSamples = 0;

}
void RTTYDemodulator::setRxFrequency ( double freq )
{
  if ( freq != RxFrequency )
  {
    RxFrequency = freq;
    F0 = PI2 * ( RxFrequency ) / SampleRate;
    F1 = PI2 * ( RxFrequency + extraParameter.offset ) / SampleRate;
    setFilter ( RxFrequency, RxFrequency + extraParameter.offset );
  }
}
void RTTYDemodulator::CalcQuality ( float x )
{
  ave2 = ave1;
  ave1 = 0.95 * ave1 + 0.05 * x;
// ave1 = 0.7*ave1 + 0.25 *ave2 + 0.05 *x;
}
///void RTTYDemodulator::CalcQuality ( int pointer )
///{
///  ave2 = ave1;
///  float sum, diff;
///  pointer = pointer % SAMPLEBUFFERLENGTH;
///
/////if ( sum > 0.1 )
///  ave1 = 0.7 * ave1 + 0.25 * ave2 + 0.05 * diff / sum;
/////else
///// ave1 = 0.5*ave1 + 0.3 *ave2;
///}

int RTTYDemodulator::getSquelchValue()
{
  return ( int ) ( 100.*ave1 );
}

double RTTYDemodulator::get2RxFrequency()
{
  return RxFrequency + extraParameter.offset;

}
void RTTYDemodulator::setParameter ( RxTxParameterType Type, void *Value )
{
  switch ( Type )
  {
    case Reverse:
      extraParameter.reverse =  * ( bool * ) Value;
      break;
    case Offset:
      extraParameter.offset = * ( int * ) Value;
      break;
    case Parity:
      extraParameter.parity = * ( Paritaet * ) Value;
      break;
    case Extra:
      extraParameter = * ( ExtraParameter * ) Value;
      break;
    default:
      break;
  }
}
void *RTTYDemodulator::getParameter ( RxTxParameterType Type )
{
  switch ( Type )
  {
    case Reverse:
      return ( void * ) &extraParameter.reverse;
      break;
    case Offset:
      return ( void * ) &extraParameter.offset;
      break;
    case Parity:
      return ( void * ) &extraParameter.parity;
      break;
    case Extra:
      return ( void * ) &extraParameter;
      break;
    default:
      return 0;
      break;
  }
}
void *RTTYDemodulator::getBuffer()
{
  return ( void * ) 0;
}
AfcMode RTTYDemodulator::AfcProperties()
{
  return Wide;
}
void RTTYDemodulator::setFilter ( double freq0, double freq1 )
{
  double x0, norm;
  int FilterLength, i;
  FilterLength = FFTFILTERLENGTH;
  double coeffs[FFTLENGTH];

//  x0 = ( PI2 * 15. ) / SampleRate;
  x0 = ( PI2 * 30. ) / SampleRate;
  for ( i = FilterLength;i < FFTLENGTH;i++ )
    coeffs[i] = 0.;
  for ( i = 0; i < FilterLength; i++ )
  {
    if ( i != ( FilterLength - 1 ) / 2 )
      coeffs[i] = sin ( x0 * ( i - ( FilterLength - 1 ) / 2 ) ) / ( i - ( FilterLength - 1 ) / 2 );
    else
      coeffs[i] = x0;
    coeffs[i] *= ( 0.42 - 0.5 * cos ( ( PI2 * i ) / ( FilterLength - 1 ) )
                   + 0.08 * cos ( ( PI2 * ( i + i ) ) / ( FilterLength - 1 ) ) );
  }
  norm = 0.;
  for ( i = 0;i < FilterLength;i++ )
    norm += coeffs[i];
  norm = ( norm * FilterLength ) / 10.;
  for ( i = 0;i < FilterLength;i++ )
    coeffs[i] /= norm;
  for ( i = 0;i < FFTLENGTH;i++ )
    pFilterIn[i] = coeffs[i] * exp ( complex<double> ( 0., PI2 * freq1 / SampleRate * ( i - ( FilterLength - 1 ) / 2 ) ) );
  for ( i = FilterLength;i < FFTLENGTH;i++ )
    pFilterIn[i] = complex<double> ( 0., 0. );

  fftw_execute ( pfilter );
  memcpy ( pFilterF1, pFilterF0, FFTLENGTH*sizeof ( complex<double> ) );

  for ( i = 0;i < FFTLENGTH;i++ )
    pFilterIn[i] = coeffs[i] * exp ( complex<double> ( 0., PI2 * freq0 / SampleRate * ( i - ( FilterLength - 1 ) / 2 ) ) );
  fftw_execute ( pfilter );
}
void RTTYDemodulator::execFilter()
{
  int i, overlapLength;
  overlapLength = FFTFILTERLENGTH - 1;

  for ( i = 0; i < FFTLENGTH;i++ )
  {
    pOutF1[i] = pOutF0[i] * pFilterF1[i];
    pOutF0[i] *= pFilterF0[i];
  }
  fftw_execute ( pbackwardF0 );
  fftw_execute ( pbackwardF1 );
  for ( i = 0;i < overlapLength;i++ )
  {
    pResultF0[i] += pOverlapF0[i];
    pResultF1[i] += pOverlapF1[i];
  }
  memcpy ( pOverlapF0, &pResultF0[RESULTLENGTH], sizeof ( fftw_complex ) * overlapLength );
  memcpy ( pOverlapF1, &pResultF1[RESULTLENGTH], sizeof ( fftw_complex ) * overlapLength );
}
void RTTYDemodulator::downmix ( complex<double> * z0, complex<double> *z1, int anzahl, int *y,
                                double *omegaF0, double *omegaF1 )
{
  int i, j, k;
  complex<double> sumf0, sumf1, zz0, zz1;
#define C1 0.8   /// 1. 0.1 0.7 , 0.775 0.1 0.7 , 0.8 0.08 0.575
//#define C3 0.05
#define C3 0.01
  double norm;
  k = 0;
  for ( i = 0;i < anzahl; i += DISTANCE )
  {
    sumf0 = 0.;
    sumf1 = 0.;
    for ( j = i;j < i + DISTANCE;j++ )
    {
      norm = abs ( z0[j] + z1[j] );
//      zz0    = ( z0[j] * exp ( complex<double> ( 0., -F0inc ) ) / norm ) ;
//      zz1    = ( z1[j] * exp ( complex<double> ( 0., -F1inc ) ) / norm ) ;
      z0[j] *= exp ( complex<double> ( 0., -F0inc ));
      zz0    = z0[j]/ norm  ;
      sumf0 += zz0;
      z1[j] *= exp ( complex<double> ( 0., -F1inc ));
      zz1    = z1[j]/ norm  ;
      sumf1 += zz1;

      F0inc += F0;
      if ( F0inc > PI2 )
        F0inc -= PI2;
      F1inc += F1;
      if ( F1inc > PI2 )
        F1inc -= PI2;
    }
    if ( F0max < abs ( zz0 ) )
      F0max = ( 1. - C1 ) * F0max + C1 * abs ( zz0 );
    else
      F0max = ( 1. - C3 ) * F0max - C3 * abs ( zz0 );
    if ( F1max < abs ( zz1 ) )
      F1max = ( 1. - C1 ) * F1max + C1 * abs ( zz1 );
    else
      F1max = ( 1. - C3 ) * F1max - C3 * abs ( zz1 );
    if ( ( abs ( sumf0 ) - 0.5 * F0max )  > ( abs ( sumf1 ) - 0.5*F1max ) )
      y[k] = -1;
    else
      y[k] = 1;

    omegaF0[k] = ( abs ( sumf0 ) - 0.5 * F0max );
    omegaF1[k++] = ( abs ( sumf1 ) - 0.5 * F1max );
  }
}


