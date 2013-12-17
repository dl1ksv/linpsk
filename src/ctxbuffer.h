/***************************************************************************
                          ctxbuffer.h  -  description
                             -------------------
    begin                : Sam Feb 22 2003
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

#ifndef CTXBUFFER_H
#define CTXBUFFER_H

#include "constants.h"

/**Stores Characters that should be transmitted
  *@author Volker Schroer
  */
class QString;
class CTxBuffer {
public: 
	CTxBuffer();
	~CTxBuffer();
  bool Filled();
int getTxChar();
void insert(int);
void insert(QString,int);
void clear();
bool isEmpty();

private:
int txbuffer[TXBUFFER_LENGTH];
int filled;
int inpos;
int outpos;
};

#endif
