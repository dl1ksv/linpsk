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

MinimumWindowWidth=640;
MinimumWindowHeight=555;


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
RxChannels=1;
ActChannel = 0;   // Pointer to the active Channel
ChannelChain = 0; // Pointer to the Start of the Rx - ChannelChain
ApplicationFont =0;
autoCrLf=true;

}
Parameter::~Parameter()
{
}

