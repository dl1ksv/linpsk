/***************************************************************************
                          interleaver.h  -  description
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
 *   The PSK part is based on WinPSK 1.0 by Moe Wheatley, AE4JY            *              
 ***************************************************************************/

#ifndef INTERLEAVER_H
#define INTERLEAVER_H


/**
  *@author Volker Schroer
  */

class Interleaver {
public: 
	Interleaver(int);
	~Interleaver();
int interleave(unsigned char* inout);
private:
unsigned char * delayLine;
int interleaverGrade;
int index;
int offset;

};

#endif
