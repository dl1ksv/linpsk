/***************************************************************************
                          csound.h  -  description
                             -------------------
    begin                : Wed Apr 5 2000
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

#ifndef CSOUND_H
#define CSOUND_H

#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <QMutex>
#include <QWaitCondition>
#include "input.h"
#include "constants.h"
#include <alsa/asoundlib.h>
#define ALSA_PCM_NEW_HW_PARAMS_API
#define ALSA_PCM_NEW_SW_PARAMS_API

/**Class for operating on SoundCard
  *@author Volker Schroer
  */
class CSound : public Input
{
  public:
    CSound ( int ptt );
    ~CSound();
    virtual bool open_Device_write ( QString * );
    virtual bool open_Device_read ( QString * );
    virtual bool close_Device();
    int getSamples ( double *, int );    // Reading Samples from Soundcard
    int putSamples ( double *, int );  // Writing Samples to Soundcard
    void PTT ( bool );

  private: // Private attributes
    snd_pcm_t *handle;
    bool output;
    double buffer[2*BUF_SIZE]; // 16 bit , stereo
    QMutex LockPointers;
    QWaitCondition WakeUp;
    void record();
    void play();
    int getDeviceNumber(QString device);
#ifdef SOUND_DEBUG
    void dump_hw_params(snd_pcm_t *h,snd_pcm_hw_params_t* hw_p);
#endif
  protected:
    void run();
    int free, freePointer, available, readPointer,toBePlayed;

};

#endif
