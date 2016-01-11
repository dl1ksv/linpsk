/***************************************************************************
                          waveinput.cpp  -  description
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

#include "waveinput.h"
#include "parameter.h"

extern Parameter settings;

WaveInput::WaveInput(int ptt = -1): Input(ptt)
{
 EightBits=false;
}
WaveInput::~WaveInput(){
}

/** Opens the Device for writting, for Wavefiles this means write nothing ! */
bool WaveInput::open_Device_write(QString *)
{
return true;
}
/** Open Device for reading **/
/** puts the Samples onto the Device, for a Wavemode Device nothing happens */
int WaveInput::putSamples(double *,int anzahl)
{
return anzahl;
}
	
/** Dummy */
void WaveInput::PTT(bool )
{
}

 /** Opens the Device named Device for reading */

bool WaveInput::open_Device_read(QString *errorstring)
{
 char header[5];
 unsigned char header1[4];
 char c;
 QString s;
 int i;
 *errorstring=QString("");
 if (settings.inputFilename == "" )
  {
   *errorstring= QString(QObject::tr("Error, no Demofile selected")); 
    return false;
  }
 fd = open(settings.inputFilename.toLatin1(),O_RDONLY);
 if ( fd < 0)
  {
    *errorstring= QString(QObject::tr("Error, Could not open Demofile "));
    return false;
  }


// Check for RIFF
i =read(fd,&header[0],4*sizeof(header[0]));
if ( i != 4)
 {
  close(fd);
  *errorstring= QString(QObject::tr("Header mismatch"));		
  return false;
 }
header[4]=0;	
s=QString(header);

if  ( s != "RIFF")
 {
  close(fd);
  *errorstring = settings.inputFilename + QString(QObject::tr(  " is not a RIFF File\n"));
  return false;
 }

//Check for Wave
lseek(fd,8,SEEK_SET);
i =read(fd,&header[0],4*sizeof(header[0]));
header[4]=0;
if ( i != 4)
 {
  close(fd);
  *errorstring= QString(QObject::tr("SubType mismatch\n"));
  return false;
 }
s=QString(header);

if  ( s != "WAVE")
 {
  close(fd);
  *errorstring =settings.inputFilename + QString(QObject::tr(" is not a WAVE File\n"));
  return false;
 }
//Check for fmt
i =read(fd,&header[0],4*sizeof(header[0]));
header[4]=0;
if ( i != 4)
{
 close(fd);
 *errorstring= QString(QObject::tr("File is too short\n"));
 return false;
}
s=QString(header);
if  ( s != "fmt ")
{
 close(fd);
 *errorstring =settings.inputFilename + QString(QObject::tr(" does not have a fmt chunk\n"));
 return false;
}
// Read length of fmt chunk
i=read(fd,&header[0],4*sizeof(header[0]));
if ( i != 4)
{
 close(fd);
 *errorstring= QString(QObject::tr("File is too short, while reading fmt chunk\n"));
 return false;
}
Length=header[0] | (header[1] << 8) |(header[2] << 16) | (header[3] << 24 );
// To distinguish between the two versions of wav ( canonical and new)
if ((Length -16) >0)
 offset = Length-16;
else 
 offset = 0;
//Check  for format
lseek(fd,20,SEEK_SET);
i=read(fd,&header[0],2*sizeof(header[0]));
if ( i != 2)
 {
  close(fd);
  *errorstring= QString(QObject::tr("File is too short, while reading format info in fmt chunk\n"));
  return false;
 }
 Length=header[0] | ( header[1] << 8 );
 if (Length != 1 )
  {
   close(fd);
   *errorstring= QString(QObject::tr("File is too short, while reading format info in fmt chunk\n"));
   return false;
  }
       
//check for mono 
lseek(fd,22,SEEK_SET);
i=read(fd,&header[0],2*sizeof(header[0]));
if ( i != 2)
 {
  close(fd);
  *errorstring= QString(QObject::tr("File is too short, while reading channel info in fmt chunk\n"));
  return false;
 }
Length=header[0] | ( header[1] << 8 );
if ( Length != 1)
{
 close(fd);
 *errorstring= QString(QObject::tr("Only mono is supported at the moment\n"));
 return false;
} 
// Check for sample rate
lseek(fd,24,SEEK_SET);
i=read(fd,&header[0],4*sizeof(header[0]));
if ( i != 4)
 {
  close(fd);
  *errorstring= QString(QObject::tr("File is too short, while reading sample rate in fmt chunk\n"));
  return false;
 }
Length=header[0] | ( header[1] << 8 ) | (header[2] << 16) | (header[3] << 24 );
       
if ( Length != 11025)
{
 close(fd);
 *errorstring= QString(QObject::tr("Only a sample rate of 11025Hz is supported at the moment\n"));
 return false;
}
//let's check for 8 / 16 bit samples
lseek(fd,34,SEEK_SET);
i=read(fd,&c,1);
if (c == 8)
 EightBits=true;
else
 EightBits=false;
offset +=36;
// Now find data
lseek(fd,offset,SEEK_SET);
i=read(fd,&header[0],4*sizeof(header[0]));

header[4]=0;
s=QString(header);
// Checking for fact chunk
if ( s == "fact")
{
 lseek(fd,8,SEEK_CUR);
 i=read(fd,&header[0],4*sizeof(header[0]));
 header[4]=0;
 s=QString(header);

} 
if ( (i != 4) || s != "data" )		
{
 close(fd);
 *errorstring= QString(QObject::tr("No data found\n"));
	return false;
}
// got it
// Read 4 byte Length info
i=read(fd,&header1[0],4*sizeof(header1[0]));
if ( i != 4)
{
 close(fd);
 *errorstring= QString(QObject::tr("File is too short, while reading length of data chunk\n"));
 return false;
}
Length=0;
Length=header1[0] | (header1[1] << 8) |(header1[2] << 16) | (header1[3] << 24 );
Bytesread=0;
offset = lseek(fd,0,SEEK_CUR);		
i=lseek(fd,offset,SEEK_SET); 
return true;
}

int WaveInput::getSamples(double *sample,int anzahl)
{
unsigned char c[2];
int i,j;
j=0;
double x;

while (j<anzahl)
 {
  if (!EightBits)
   {
    i= read(fd,&c[0],2*sizeof(c[0]));
    if (i == 2)
     {
      if ( c[1] < 128 )
       i =  c[0]+ c[1]* 256 ;
      else
       i = ( c[1]- 256) *256 +c[0];
      x= i;
      *(sample++)=x/32767.;
      j++;
      Bytesread +=2;
     }		
   else		
   {
    printf("Error reading data chunk\nRetry from the beginning");
    lseek(fd,offset,SEEK_SET);
    Length=Bytesread;
    }
   }
  else
  {
   i=read(fd,&c[0],sizeof(c[0]));
   if ( i==1 )
    {
    if ( c[0] < 128 )
     x = float(c[0])/128.;
    else
     x = float(c[0]-256)/128;
    *(sample++)=x;
    j++;
    Bytesread++;
    }
   else		
   {
    printf("Error reading data chunk\nRetry from the beginning");
    lseek(fd,offset,SEEK_SET);
    Length=Bytesread;
    }
  }
 if (Bytesread >= Length )
 {
  lseek(fd,offset,SEEK_SET);  
  Bytesread=0;
 }
  
 } // End while
return j;

}

bool WaveInput::close_Device()
{
return true;
}
/**
void WaveInput::samplesAvailable()
{
 emit Input::samplesAvailable();
}
**/
