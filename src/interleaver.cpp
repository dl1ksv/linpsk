/***************************************************************************
                          interleaver.cpp  -  description
                             -------------------
    begin                : Sam Mär 1 2003
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
 * The interleaver follows a proposal of Chen, W7AY                        *
 * Instead of dinterleaving in multiple steps the interleaving is done     *
 * in one single step                                                      *
 * Many thanks to Chen, for the allowance to use his idea                  *
 ***************************************************************************/

#include "interleaver.h"

Interleaver::Interleaver ( int grade )
{
	interleaverGrade = grade * 4;
delayLine= new unsigned char[interleaverGrade*4];
	for ( int i = 0; i < interleaverGrade*4 ; i++ )
		delayLine[i] = 0;
	index = 0;
offset=grade*3;
}
Interleaver::~Interleaver()
{}
int Interleaver::interleave ( unsigned char *inout )
{
	int  i,code;
	for ( i = 0;i < 4; i++ )
 delayLine[index+interleaverGrade*i]=inout[i];
code=0;
	for ( i = 0;i < 4; i++ )
{
		inout[i] = delayLine[(index+i*offset)%interleaverGrade+i*interleaverGrade  ];
if(inout[i] != 0 )
 code |=  (1 << (3-i) );
}
	index = ( index + 1) % interleaverGrade;
return code;
}
