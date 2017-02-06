/***************************************************************************
                          firfilter.cpp  -  description
                             -------------------
    begin                : Fr Nov 7 2003
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

#include <assert.h>
#include "firfilter.h"
#include <math.h>

FIRFilter::FIRFilter(double Fc, int Filterlength, FilterMode Mode, double gain)
{
  // Cutoff Fc in radians
  double normalize = 0;
  int fhalbe;
  assert(Filterlength%2 == 1);
  fhalbe=(Filterlength-1)/2;
  NxCoeffs=Filterlength;

  h=new double[Filterlength];
  if (Mode == RealData)
    {
     filterbuffer=new double[Filterlength];
     cfilterbuffer = 0;
    }
  else
    {
     cfilterbuffer=new complex<double>[Filterlength];
     filterbuffer =0;
    }
  /** Calculate coefficients of sinc lp filter */

// sin(x-tau)/(x-tau)
  for (int i = 0; i < Filterlength; i++)
      if (i == fhalbe)
          h[i] = Fc;
      else
          h[i] = sin(Fc*(i - fhalbe))/(i-fhalbe);
// blackman window
  for (int i = 1; i < Filterlength+1; i++)
      h[i-1] = h[i-1] * (0.42 - 0.5 * cos(2*M_PI*i/(Filterlength+1)) + 0.08 * cos(4*M_PI*i/(Filterlength+1)));
//  h[0]=0;
//  h[Filterlength-1]=0;
// normalization factor
  for (int i = 1; i < Filterlength-1; i++)
      normalize += h[i];
  if(gain !=0)
    normalize /=gain;
// normalize the filter
  for (int i = 1; i < Filterlength-1; i++)
     h[i] /= normalize;

for(int i=0;i <Filterlength;i++)
 {
   if (Mode == RealData)
    filterbuffer[i]=0.0;
   else
    cfilterbuffer[i]=complex<double>(0.,0.);
 }
if (Mode == RealData)
 fbBuffer=filterbuffer;
else
 cfbBuffer=cfilterbuffer; 
}

FIRFilter::~FIRFilter()
{
if (h != 0)
 delete h;
if (filterbuffer != 0)
 delete filterbuffer;
if (cfilterbuffer != 0)
 delete cfilterbuffer;
}
void FIRFilter::processFilter(double *input,double *output,int NxSamples)
{
double *oPtr,*fPtr,*fbPtr;
double acc;
oPtr=output;
fbPtr=fbBuffer;

for(int i=0; i <NxSamples;i++)
 {
   *fbPtr=input[i];
   acc=0.0;
   fPtr=h;   
   for(int j=0;j <  NxCoeffs; j++)
    {
     acc +=(*fbPtr--)*(*fPtr++); // x(n-j)*h(j)
     if (fbPtr < filterbuffer )
      fbPtr = filterbuffer+NxCoeffs-1;  
     }
   fbPtr++;  
   if (fbPtr >= (filterbuffer + NxCoeffs ) )
    fbPtr=filterbuffer;
   *oPtr++ =acc;    
 }  


  fbBuffer=fbPtr;
}
/**
void FIRFilter::setnewCoeffs(double *Filtercoeffs)
{
for(int i=0;i <NxCoeffs;i++)
   h[i] = *(Filtercoeffs+i);
 }
**/
void FIRFilter::processFilter(complex<double> *input,complex<double> *output,int NxSamples)
{
 complex<double> *oPtr,*fbPtr;
 complex<double> acc;
 double *fPtr;
 oPtr=output;
 fbPtr=cfbBuffer;
 
 for(int i=0; i <NxSamples;i++)
 {
  *fbPtr=input[i];
  acc=0.0;
  fPtr=h;   
  for(int j=0;j <  NxCoeffs; j++)
  {
   acc +=(*fbPtr--)*(*fPtr++); // x(n-j)*h(j)
   if (fbPtr < cfilterbuffer )
    fbPtr = cfilterbuffer+NxCoeffs-1;  
  }
  fbPtr++;  
  if (fbPtr >= (cfilterbuffer + NxCoeffs ) )
   fbPtr=cfilterbuffer;
  *oPtr++ =acc;    
 }  
 
 
 cfbBuffer=fbPtr;          
}
