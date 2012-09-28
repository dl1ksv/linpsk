/***************************************************************************
                          textinput.h  -  description
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
 *   based on the work of Moe Wheatley, AE4JY                              *
 ***************************************************************************/

#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include "input.h"
#include <stdlib.h>
#include <sys/stat.h>
/**Implementation of the Input Class for textfiles
  *@author Volker Schroer
  */

class TextInput : public Input
{
    Q_OBJECT
  public:
    TextInput ( int ptt );
    ~TextInput();

    /** Opens the Device for writting **/
    bool open_Device_write ( QString * );
    /** Opens the Device for Reading **/
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
  private:
    int fd;
};

#endif
