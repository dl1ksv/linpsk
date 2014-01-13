/***************************************************************************
                          parameter.h  -  description
                             -------------------
    begin                : Sat Apr 1 2000
    copyright            : (C) 2000 by Volker Schroer
    email                : DL1KSV@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef PARAMETER_H
#define PARAMETER_H

#include <qapplication.h>
#include <qfont.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <cledbutton.h>
#include "constants.h"

class CRxChannel;
class QString;
class QFont;

/**Contains parameters for transmitting and receiving

some of them are set by settup
  *@author Volker Schroer
  */

class Parameter
{

public: 
	Parameter();
	~Parameter();
void setupDevices();
// Variables


QString callsign;             // Callsign
QString myLocator;
QString inputFilename;        //Name of Demofile
QString DemoModeFileType[2];  // File Type of DemoFile
int DemoTypeNumber;           // Index of selected FileType
int serial;                   // Filedescriptor for serial Device for PTT
QString SerialDevice;         //Filename for PTT Device
//Logging
QString QSOFileName;          // Name of the file,where qsodata will be stored
QString Directory;
bool fileLog;
bool LinLog;
QString Host;
int Port;
bool DemoMode;                // DemoMode ?

QString LangName[3];	      //Contains the used language names for button labeling

int timeoffset;               // offset to UTC

/** DeviceSection */
QString InputDeviceName;
QString OutputDeviceName;

int sampleRate;
bool complexFormat;

bool slashed0;		      // True if to print 0 slashed
bool autoCrLf;		      // True if sending cr lf on enter
bool autoDate;		      // True if date/time will be set automatically when qso data are saved
			      // If false, fields are displayed in qso data and editable

int  RxChannels;
CRxChannel * ChannelChain;
CRxChannel * ActChannel;
QsoInfo *QslData;
BUTTONSTATUS Status;
QFont *ApplicationFont;
QString dateFormat;

};

#endif
