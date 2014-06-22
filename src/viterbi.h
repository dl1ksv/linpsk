/***************************************************************************
                          viterbi.h  -  description
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

#ifndef VITERBI_H
#define VITERBI_H


/**
  *@author Volker Schroer
  */

class Viterbi {
public: 
//	Viterbi(int,unsigned int,unsigned int,const double (*)(double *,int));
	Viterbi(int,unsigned int,unsigned int,double (*)(double *,int));
	~Viterbi();
  int encode(int);
  void decode(double *);

  int getbit(unsigned int i);
  int getbitinvers(unsigned int i);

private:
unsigned *StateTable;
long long int *PathMem;
double *Metric;
double * distance;
long long int * bitestimate;
int NumberofStates;
int MinimumPath;
unsigned int CoderState;
//const double (*dist)(double *,int);
double (*dist)(double *,int);


};

#endif
