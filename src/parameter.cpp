/***************************************************************************
                          parameter.cpp  -  description
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
 *    based on the work of  Moe Wheatly, AE4JY                             *  
 ***************************************************************************/

#include "parameter.h"

Parameter::Parameter()
{

Status = UNDEF;

// Defaultsettings

callsign="";
QslData=0;

serial=-1; // Serial none
SerialDevice="none";
QSOFileName="QSOData.adif";

timeoffset=-2;

DemoMode=true;
DemoModeFileType[0]="*.wav";
DemoModeFileType[1]="*.out";
DemoTypeNumber=0;

slashed0=false;
autoCrLf=true;
autoDate=true;

RxChannels=1;
ActChannel = 0;   // Pointer to the active Channel
ChannelChain = 0; // Pointer to the Start of the Rx - ChannelChain
ApplicationFont =0;
dateFormat="dd.MM.yyyy";

InputDeviceName="LinPSK_Record";
OutputDeviceName="LinPSK_Play";
sampleRate=11025;
complexFormat=false;
LangName[0]="B";
LangName[1]="E";
LangName[2]="G";
}
Parameter::~Parameter()
{
}

