/***************************************************************************
                          ctxbuffer.cpp  -  description
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
 ***************************************************************************/

#include "ctxbuffer.h"
#include <QString>
#include <QApplication>

CTxBuffer::CTxBuffer()
{
filled=0;
inpos=0;
outpos=0;
}
CTxBuffer::~CTxBuffer()
{
}
bool CTxBuffer::Filled()
{
if ( filled < TXBUFFER_LENGTH )
  return false;
else
  return true;
}
int CTxBuffer::getTxChar()
{
int ch;
if (filled > 0)
	{
		ch=txbuffer[outpos++];
		filled--;
		if (filled == 0)
			{
				inpos=0;
				outpos=0;
			}
		 else
			outpos=outpos % TXBUFFER_LENGTH;
		return ch;
	}
else
	 return TXTOG_CODE;

}
void CTxBuffer::insert(int c)
{
if ( c != '\b' )
{
 if (filled < TXBUFFER_LENGTH)
 {
  filled++;
  txbuffer[inpos++]=c;
  inpos = inpos % TXBUFFER_LENGTH;
  }
}
else
{
 filled--;
 if ( filled <= 0)
 {
  inpos=0;
  outpos=0;
  if( filled < 0)
  {
    filled=1;
    txbuffer[inpos++]=c;
  }
 }
 else
 {
 inpos--;
 if (inpos < 0)
  inpos +=TXBUFFER_LENGTH;
 }
}   
}
void CTxBuffer::clear()
{
  if(filled > 0)
  {
   if(txbuffer[(outpos+filled-1) %  TXBUFFER_LENGTH] == TXOFF_CODE) // We have to keep switching to rx
   {
     outpos = (outpos+filled-1) %  TXBUFFER_LENGTH;
     inpos = (inpos+filled-1) %  TXBUFFER_LENGTH;
     filled=1;
     return;
   }
  }
filled=0;
inpos=0;
outpos=0;
}
void CTxBuffer::insert(QString Text,int length)
{
for (int i=0;i <length; i++)
 {
  if ( filled < TXBUFFER_LENGTH )
    {
    filled++;
    txbuffer[inpos++]=(unsigned char) Text.at(i).toLatin1();
    inpos = inpos % TXBUFFER_LENGTH;
    }
   else
    {
    while(this->Filled()) // Wait until Buffer is not filled
      qApp->processEvents(QEventLoop::AllEvents,100);
    filled++;
    txbuffer[inpos++]=Text.at(i).toLatin1();
    inpos = inpos % TXBUFFER_LENGTH;
      
    }   
  } 
}
bool CTxBuffer::isEmpty()
{
  return (filled== 0);
}
