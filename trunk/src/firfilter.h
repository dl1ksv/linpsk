/***************************************************************************
                          firfilter.h  -  description
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

#ifndef FIRFILTER_H
#define FIRFILTER_H

#include <complex>
using namespace std;
/**Implements an FIR- Filter
  *@author Volker Schroer
  */
enum FilterMode{RealData,ComplexData};
class FIRFilter {
public: 
	FIRFilter(double *Filtercoeffs,int Filterlength,FilterMode);
	~FIRFilter();
  void processFilter(double *input,double *output,int NxSamples);
  void processFilter(complex<double> *input,complex<double> *output,int NxSamples);
  void setnewCoeffs(double *Filtercoeffs);
private:
int NxCoeffs;
double *h;
complex<double> *cfilterbuffer;
complex<double> *cfbBuffer;
double *filterbuffer;
double *fbBuffer;  
};

#endif
