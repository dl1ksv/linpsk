/***************************************************************************
                          deinterleaver.cpp  -  description
                             -------------------
    begin                : Sam Feb 1 2003
    copyright            : (C) 2003 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 * The deinterleaver follows a proposal of Chen, W7AY                      *
 * Instead of deinterleaving in multiple steps the deinterleaving is done  *
 * in one single step                                                      *
 * Many thanks to Chen, for the allowance to use his idea                  *
 ***************************************************************************/


#include "deinterleaver.h"

Deinterleaver::Deinterleaver ( int grade )
{
	interleaverGrade=grade*16;
	delayLine= new double[interleaverGrade];

	for ( int i=0; i <interleaverGrade ; i++ )
		delayLine[i]=0;
	index=0;
}
Deinterleaver::~Deinterleaver()
{
	if ( delayLine !=0 )
	{
		delete delayLine;
//		delayLine=0;
	}
}
void Deinterleaver::deinterleave ( double *p )
{
	double result[4];
	int offset,i;
	offset=interleaverGrade/4+1;
	for ( i=0;i < 4; i++ )
		result[i]=delayLine[ ( index+i*offset ) % interleaverGrade];
	for ( i=0;i < 4; i++ )
	{
		delayLine[index+i]=p[i];
		p[i]=result[i];
	}
	index = ( index+4 ) % interleaverGrade;
}
