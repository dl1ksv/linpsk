/***************************************************************************
                          waveinput.h  -  description
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

#ifndef WAVEINPUT_H
#define WAVEINPUT_H

#include "input.h"
#include <stdlib.h>

/**Implementation of the Input Class for wave - files
The implementation is simplified . But it can process the two sample files
for mfsk16 and psk31.
The class doesn't produce output at the moment
  *@author Volker Schroer
  */

class WaveInput : public Input
{
Q_OBJECT
  public:
    WaveInput ( int ptt );
    ~WaveInput();
    /** Opens the Device for writting **/
    bool open_Device_write ( QString * );
    /** Opens the Device for reading **/
    bool open_Device_read ( QString * );
    /** Close the Device **/
    virtual bool close_Device();
    /** gets the samples from the device */
    int getSamples ( double *sample, int anzahl );
    /** puts the Samples onto the Device, for a Textmode Device nothing happens */
    int putSamples ( double *sample, int anzahl );
    /** Dummy */
    void PTT ( bool mode );
    /** Dummy */
    bool setParams ( QString *errorstring );
  signals:
//    void samplesAvailable();

  private:

    int offset;
    unsigned long int Length, Bytesread;
    bool EightBits;
    int fd;
};

#endif
