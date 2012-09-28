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

#include "firfilter.h"

FIRFilter::FIRFilter(double *Filtercoeffs,int Filterlength,FilterMode Mode)
{
NxCoeffs=Filterlength;

h=new double[Filterlength];
if (Mode == RealData)
 filterbuffer=new double[Filterlength];
else
 cfilterbuffer=new complex<double>[Filterlength];
 
for(int i=0;i <Filterlength;i++)
 {
   if(Filtercoeffs != 0)
    h[i] = *(Filtercoeffs+i);
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
void FIRFilter::setnewCoeffs(double *Filtercoeffs)
{
for(int i=0;i <NxCoeffs;i++)
   h[i] = *(Filtercoeffs+i);
 }

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
