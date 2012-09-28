/***************************************************************************
                          cmodulator.h  -  description
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
 ***************************************************************************/

#ifndef CMODULATOR_H
#define CMODULATOR_H

#include <qobject.h>
#include "parameter.h"
#include "constants.h"


/**Base Class for all possible transmit types
  *@author Volker Schroer
  */

class CTxBuffer;

class CModulator : public QObject
{
Q_OBJECT
public: 
	CModulator(int FS,CTxBuffer *);
	~CModulator();
        virtual void setParameter(RxTxParameterType,void *);

/** Calculate the Signal to be fed into the soundcard */
	virtual int CalcSignal(double *data,int BufferSize) = 0;
/** data Pointer to the computed signal values
    Size of Buffer for the computed signal
		returns Number of computed signal values  and -Number at the end of Transmission
  **/

protected: // Protected attributes
  /** Samplerate of Soundcard */
  unsigned int SampleRate;
	CTxBuffer *Buffer;


public slots:

signals:
void charSend(char);

};

#endif
