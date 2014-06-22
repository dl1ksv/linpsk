/***************************************************************************
                          viterbi.cpp  -  description
                             -------------------
    begin                : Sam Maerz 8 2003
    copyright            : (C) 2003 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "viterbi.h"
#include <iostream>
using namespace std;

Viterbi::Viterbi(int K, unsigned int PolyA, unsigned int PolyB,double (*d)(double *, int))
{
int i;
dist = d;
NumberofStates= 1 << K; // 2 ** K
Metric =  new double[NumberofStates/2];
PathMem= new long long int [NumberofStates/2];
StateTable = new unsigned int[NumberofStates];

distance = new double[NumberofStates];
bitestimate = new long long int [NumberofStates];

Metric[0]=0.0;

for(i=1; i < NumberofStates/2; i++)
  Metric[i]=10000.;
for(i=0; i < NumberofStates/2;i++)
 {
  PathMem[i]=0;
  bitestimate[i]=0;
 } 

/** Calculate State Table **/
for(i=0; i < NumberofStates; i++)
  {
    unsigned int k,count;
     k = PolyA & i;

     count = 0;
     while ( k > 0)
      {
       if ( k & 1) count ++;
       k = k >>1;
      }
      StateTable[i]=  (count &1) << 1;

     k = PolyB & i;
     count = 0;
     while ( k > 0)
      {
       if ( k & 1) count ++;
       k = k >>1;
      }
      StateTable[i]=  StateTable[i] | (count & 1 ) ;

    }
CoderState=0;
}
Viterbi::~Viterbi()
{
if ( Metric !=0 )
  delete Metric;
if ( PathMem !=0 )
  delete PathMem;
if ( StateTable != 0)
  delete StateTable;
if (distance !=0 )
  delete distance;
if (bitestimate !=0 )
  delete bitestimate;

}
int Viterbi::encode(int i)
{
CoderState= (( CoderState << 1 ) | ( i & 0x03)) & (NumberofStates - 1);
 
return StateTable[CoderState];
}

void Viterbi::decode(double *c)
{
double min;
int i;
min=1.0E99;
MinimumPath=0;


for(i=0; i <NumberofStates; i++)
  {

  distance[i]=Metric[i/2]+(*dist)(c,StateTable[i]);
  if ( distance[i] < min )
  {
    min=distance[i];
    MinimumPath=i;
   }
  bitestimate[i]= (PathMem[i/2] <<1) + ( i & 1);
  }
MinimumPath = MinimumPath % (NumberofStates/2);
for(i=0;i < NumberofStates/2;i++)
  {
  if (distance[i] < distance[i+NumberofStates/2])
    {
    Metric[i] = distance[i] - min;
    PathMem[i]= bitestimate[i];
   }
  else
    {
    Metric[i] = distance[NumberofStates/2+i] - min;
    PathMem[i]= bitestimate[NumberofStates/2+i];

    }
   }

}
int Viterbi::getbit(unsigned int i)
{
return ( PathMem[MinimumPath] >> i ) & 1;
}
int Viterbi::getbitinvers(unsigned int i)
{
return (~( PathMem[MinimumPath] >> i )) & 1;
}

