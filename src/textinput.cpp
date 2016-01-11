/***************************************************************************
                          textinput.cpp  -  description
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

#include "textinput.h"
#include <qobject.h>
#include <errno.h>
#include "parameter.h"

extern Parameter settings;
extern int errno;

TextInput::TextInput ( int ptt = -1 ) : Input ( ptt )
{
}
TextInput::~TextInput()
{
}

/** Opens the Device for writting, for Textfiles this means write nothing ! */
bool TextInput::open_Device_write ( QString *errorstring )
{

  const char name[] = "Demo.out";
  fd = open ( name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU );
  if ( fd >= 0 )
    return true;
  else
  {
    *errorstring = QString ( QObject::tr ( "Could not open Demo.out" ) );
    return false;
  }
}

/** gets the samples from the device */
int TextInput::getSamples ( double *sample, int anzahl )
{
  static char Buf[128];
  int pos;
  int i, j;
  double x;

  pos = 0;
  j = 0;
  while ( j < anzahl )
  {
    do
      i = read ( fd, &Buf[pos], sizeof ( Buf[0] ) );
    while ( ( i == 1 ) && ( Buf[pos++] != '\n' ) && ( pos < 128 ) );
    if ( i == 1 )
    {
      if ( pos >= 127 )
        qWarning ( "Input file has strange lines\n" );
      pos--;
      Buf[pos] = 0;
      x = atof ( Buf );
      * ( sample++ ) = x;
      j++;
      pos = 0;
    }
    else


      lseek ( fd, 0, SEEK_SET );

  } // End while
  return j;
}

/** puts the Samples onto the Device, for a Textmode Device nothing happens */
int TextInput::putSamples ( double *sample, int anzahl )
{
  int i;
  char c;
  QString s;
  for ( i = 0;i < anzahl;i++ )
  {

    s.setNum ( sample[i], 'f', 6 );
    write ( fd, s.toLatin1(), s.length() );
    c = '\n';
    write ( fd, &c, 1 );
  }
  return anzahl;
}

/** Dummy */
void TextInput::PTT ( bool )
{
}
/** Dummy */
bool TextInput::open_Device_read ( QString *errorstring )
{
  *errorstring = QString ( "" );
  if ( settings.inputFilename == "" )
  {
    *errorstring = QString ( QObject::tr ( "Error, no Demofile selected" ) );
    return false;
  }
  fd = open ( settings.inputFilename.toLatin1(), O_RDONLY );
  if ( fd > 0 )
    return true;
  else
  {
    *errorstring = QString ( QObject::tr ( "Error, Could not open Demofile " ) );
    return false;
  }

}
bool TextInput::close_Device()
{
  if ( fd >= 0 )
    close ( fd );
  fd = -1;
  return true;
}

