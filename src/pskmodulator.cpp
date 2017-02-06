/***************************************************************************
 *   Copyright (C) 2012 -2017 by Volker Schroer, DL1KSV                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <complex>
#include "pskmodulator.h"
#include "constants.h"
#include "parameter.h"
#include "ctxbuffer.h"



extern Parameter settings;
extern  unsigned short int VariCode_Table[];

#define SYM_NOCHANGE 0 //Stay the same phase
#define SYM_P180 2  //Plus 180 deg

PskModulator::PskModulator(int FS, double frequency, CTxBuffer *TxBuffer):CModulator(FS,TxBuffer)
{

  transition[0][0]=ZZ;  //old 0 new 0
  transition[0][1]=ZN;  //old 0 new 90
  transition[0][2]=ZP;  //old 0 new 180
  transition[0][3]=ZZ7; //old 0 new 270 = -90
  transition[1][0]=NN;  //old 90 new 90
  transition[1][1]=NP;  //old 90 new 180
  transition[1][2]=NZ7; //old 90 new 270 = -90
  transition[1][3]=NZ;  //old 90 new 0
  transition[2][0]=PP;  //old 180 new 180
  transition[2][1]=PZ7; //old 180 new 270 = -90
  transition[2][2]=PZ;  //old 180 new 0
  transition[2][3]=PN;  //old 180 new  = 90
  transition[3][0]=Z7Z7;//old 270 = -90 new 270 = -90
  transition[3][1]=Z7Z; //old 270 = -90 new 0
  transition[3][2]=Z7N; //old 270 = -90 new 90
  transition[3][3]=Z7P; //old 270 = -90 new 180

/**  transition[0][1]=ZZ7;
  transition[0][3]=ZN;
  transition[1][0]=Z7Z7;
  transition[3][0]=NN;**/
  status=TX_OFF_STATE;
  addEndingZero=false;
  amblePtr=0;
  pState=PZ;
  aktBit=2;

  aktFrequency=0.;
  txShiftRegister=0;
  txFrequencyInc=PI2 * frequency / SampleRate; //carrier frequency
  symbolSize=(((100 * FS) / 3125) + 1);
  periodTime=1./31.25;
  periodDelta=1./FS;
  period=periodTime;
  inSymbolPtr=0;
}

int PskModulator::CalcSignal(double *data, int BufferSize)
{
  short int c;
  double x;
  std::complex<double> temp;
  std::complex<double> I=std::complex<double>(0.,1.);
#define amp 0.3
  for(int i=0; i < BufferSize;i++)
    {
      if(period >=periodTime)
        {
          period -=periodTime;
          c=getNextSymbolBit();
          inSymbolPtr=0;
          if(status == TX_END_STATE)
            {
              for(int j=i;j < BufferSize;j++)
                {
                  temp=-amp*(cos(aktFrequency) + sin(aktFrequency)* I);
                  data[j]=temp.real()+temp.imag();
                  aktFrequency += txFrequencyInc;
                  aktFrequency=fmod(aktFrequency,PI2);
                }
            return -BufferSize;
            }
          pState=transition[aktBit][c];
          switch(pState)
           {
             case ZZ:
               aktBit=0;
              break;
             case ZN:
               aktBit=1;
              break;
             case ZP:
               aktBit=2;
              break;
             case ZZ7:
               aktBit=3;
              break;
             case NN:
               aktBit=1;
              break;
             case NP:
               aktBit=2;
              break;
             case NZ7:
               aktBit=3;
              break;
             case NZ:
               aktBit=0;
              break;
             case PP:
               aktBit=2;
              break;
             case PZ7:
               aktBit=3;
              break;
             case PZ:
               aktBit=0;
              break;
             case PN:
               aktBit=1;
              break;
            case Z7Z7:
               aktBit=3;
              break;
            case Z7Z:
               aktBit=0;
              break;
            case Z7N:
               aktBit=1;
              break;
            case Z7P:
               aktBit=2;
              break;
           }

        }
       switch(pState)
         {
           case ZZ:
             temp=amp*(cos(aktFrequency) + sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case ZN:
             x=cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(cos(aktFrequency)+x*sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case ZP:
             x=cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(x*cos(aktFrequency) + x*sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case ZZ7:
             x=cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(x*cos(aktFrequency) + sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case NN:
             temp=amp*(cos(aktFrequency) - sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case NZ:
             x=cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(cos(aktFrequency) -x * sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case NP:
             x=cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(x*cos(aktFrequency) - sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case NZ7:
             x=cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(x*cos(aktFrequency)-x*sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case PP:
             temp=-amp*(cos(aktFrequency) + sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case PZ7:
             x=cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(-cos(aktFrequency)-x*sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case PZ:
             x=-cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(x*cos(aktFrequency) + x*sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case PN:
             x=cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(-x*cos(aktFrequency) - sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case Z7Z7:
             temp=amp*(-cos(aktFrequency) + sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case Z7Z:
             x=cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(-x*cos(aktFrequency) + sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case Z7N:
             x=cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(-x*cos(aktFrequency) + x*sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
           case Z7P:
             x=cos((double) inSymbolPtr * M_PI / (double) symbolSize);
             temp=amp*(-cos(aktFrequency) + x*sin(aktFrequency)*I);
             data[i]=temp.real() + temp.imag();
            break;
          }
          aktFrequency += txFrequencyInc;
          aktFrequency=fmod(aktFrequency,PI2);
          inSymbolPtr++;
          period +=periodDelta;
    }
    return BufferSize;
}


char PskModulator::getNextSymbolBit()
{
  int bit;
  if(txShiftRegister != 0)
    {
      if(txShiftRegister &0x8000)
        bit=SYM_NOCHANGE;
      else
        bit=SYM_P180;
      txShiftRegister <<=1;
      if(txShiftRegister == 0)
        addEndingZero=true;
    }
  else
    {
      if(addEndingZero)
      {
        bit=SYM_P180;
        addEndingZero=false;
       }
      else
        {
          int ch;
          if((ch = getChar())>= 0)
            {       // No controlcode
              txShiftRegister = VARICODE_TABLE[ ch&0xFF ];
              bit=SYM_P180; //Start with a zero
             }
          else
            switch ( ch )
            {
              case TXON_CODE:
              case TXTOG_CODE:    //Idle
                bit = SYM_P180;
                break;
              case TXOFF_CODE:
                bit = SYM_NOCHANGE;
                break;
            }
        }
    }
  return bit;
}
int PskModulator::getChar()
{
  int ch=0;
  switch(status)
    {
     case TX_OFF_STATE:
      status=TX_PREAMBLE_STATE;
      amblePtr++;
      ch=TXON_CODE;
      break;
    case TX_PREAMBLE_STATE:
      if(amblePtr++ < 33)
          ch=TXON_CODE;
      else
        {
          amblePtr=0;
          status=TX_SENDING_STATE;
          ch = TXTOG_CODE;
        }
        break;
      case TX_SENDING_STATE:
        ch=transmitBuffer->getTxChar();
        if(ch > 0)
            emit charSend(ch);
        if(ch == TXOFF_CODE)
          {
            status=TX_POSTAMBLE_STATE;
            ch = TXTOG_CODE; //Necessary to print last character at receiver side
            amblePtr=0;
          }
        break;
      case TX_POSTAMBLE_STATE:
        ch=TXON_CODE;
        if(amblePtr++ >31)
          {
            status=TX_END_STATE;
            ch=TXOFF_CODE;
          }
       break;
    case TX_END_STATE:
      qDebug("TX_END_STATE should never be reached here!");
      break;
    }
  return ch;
}
