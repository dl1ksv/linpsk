/***************************************************************************
                          input.cpp  -  description
                             -------------------
    begin                : Sat May 5 2001
    copyright            : (C) 2001 by Volker Schroer
    email                : DL1KSV@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "input.h"
#include <errno.h>
Input::Input ( int ptt ) : QThread()
{
  serial = ptt;
}
Input::~Input()
{
}
void Input::run()
{
  started = true;
  while ( started )
  {
    msleep ( 300 );
    emit samplesAvailable();
  }
}
void Input::stop()
{
  started = false;
}









