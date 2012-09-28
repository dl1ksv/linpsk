/***************************************************************************
                          cmodulator.cpp  -  description
                             -------------------
    begin                : Mon Aug 20 2001
    copyright            : (C) 2001 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ****************************************************************************/

#include "cmodulator.h"

CModulator::CModulator(int FS,CTxBuffer *TxBuffer)
{

SampleRate=FS;
Buffer=TxBuffer;

}
CModulator::~CModulator()
{
}

void CModulator::setParameter(RxTxParameterType,void *)
{
 /**
 * At the moment only RTTY uses some extra Parameters
  * So there is mostly nothing to do
  * but that may change in the future
  **/ 
 return;
}
