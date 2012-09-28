/***************************************************************************
                          deinterleaver.h  -  description
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
 *   The PSK part is based on WinPSK 1.0 by Moe Wheatley, AE4JY            *              
 ***************************************************************************/

#ifndef DEINTERLEAVER_H
#define DEINTERLEAVER_H


/**Diagonal Deinterleaver
  *@author Volker Schroer
  */

class Deinterleaver {
public: 
	Deinterleaver(int);
	~Deinterleaver();
void deinterleave(double *);
private:
double *delayLine;
int index;
int interleaverGrade;
};

#endif
