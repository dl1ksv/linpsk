/***************************************************************************
                          input.h  -  description
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

#ifndef INPUT_H
#define INPUT_H

#include <QString>
#include <QThread>
#include <sys/ioctl.h>
#include <cstdio>
#include <fcntl.h>



/**Abstract base class for the different input devices like soundcard, textfile,  Wavfile
  *@author Volker Schroer
  */


class Input: public QThread
{
    Q_OBJECT
  public:
    Input ( int ptt );
    virtual ~Input();

    /** Opens the Device named Device for reading */
    virtual bool open_Device_read ( QString * ) = 0;

    /** Opens the Device named Device for writing */
    virtual bool open_Device_write ( QString * ) = 0;

    /** Closes the open Device */
    virtual bool close_Device() = 0 ;

    /** Tries to read anzahl samples and returns the nuber of read samples */
    virtual int getSamples ( double *sample, int anzahl ) = 0;

    /** Tries to write anzahl samples and returns the nuber of written samples */
    virtual int putSamples ( double *sample, int anzahl ) = 0;

    /** Switches PTT */
    virtual void PTT ( bool mode ) = 0;
    /** Stopping the Thread **/
    void stop();
  signals:
    void samplesAvailable();
  protected:

    /** Filedescriptor of PTT- Device */
    int serial;
    void run(); //Running the thread
    bool started;


};

#endif
