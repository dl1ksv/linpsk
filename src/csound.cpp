/***************************************************************************
                          csound.cpp  -  description
                             -------------------
    begin                : Wed Apr 5 2000
    copyright            : (C) 2000 by Volker Schroer
    email                : huv.schroer@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "csound.h"
#include "parameter.h"
#include <sstream>
#include <fstream>

using namespace std;
extern Parameter settings;
#ifdef SOUND_DEBUG
static snd_pcm_format_t format_types[] = {
  // SND_PCM_FORMAT_UNKNOWN,
  SND_PCM_FORMAT_S8,
  SND_PCM_FORMAT_U8,
  SND_PCM_FORMAT_S16_LE,
  SND_PCM_FORMAT_S16_BE,
  SND_PCM_FORMAT_U16_LE,
  SND_PCM_FORMAT_U16_BE,
  SND_PCM_FORMAT_S24_LE,
  SND_PCM_FORMAT_S24_BE,
  SND_PCM_FORMAT_U24_LE,
  SND_PCM_FORMAT_U24_BE,
  SND_PCM_FORMAT_S32_LE,
  SND_PCM_FORMAT_S32_BE,
  SND_PCM_FORMAT_U32_LE,
  SND_PCM_FORMAT_U32_BE,
  SND_PCM_FORMAT_FLOAT_LE,
  SND_PCM_FORMAT_FLOAT_BE,
  SND_PCM_FORMAT_FLOAT64_LE,
  SND_PCM_FORMAT_FLOAT64_BE,
  SND_PCM_FORMAT_IEC958_SUBFRAME_LE,
  SND_PCM_FORMAT_IEC958_SUBFRAME_BE,
  SND_PCM_FORMAT_MU_LAW,
  SND_PCM_FORMAT_A_LAW,
  SND_PCM_FORMAT_IMA_ADPCM,
  SND_PCM_FORMAT_MPEG,
  SND_PCM_FORMAT_GSM,
  SND_PCM_FORMAT_SPECIAL,
  SND_PCM_FORMAT_S24_3LE,
  SND_PCM_FORMAT_S24_3BE,
  SND_PCM_FORMAT_U24_3LE,
  SND_PCM_FORMAT_U24_3BE,
  SND_PCM_FORMAT_S20_3LE,
  SND_PCM_FORMAT_S20_3BE,
  SND_PCM_FORMAT_U20_3LE,
  SND_PCM_FORMAT_U20_3BE,
  SND_PCM_FORMAT_S18_3LE,
  SND_PCM_FORMAT_S18_3BE,
  SND_PCM_FORMAT_U18_3LE,
  SND_PCM_FORMAT_U18_3BE
};
static unsigned int test_rates[] = {
  8000, 11025, 16000, 22050, 32000, 44100, 48000, 96000, 192000
};

#define NELEMS(x) (sizeof(x)/sizeof(x[0]))
#endif
CSound::CSound ( int ptt = -1 ) : Input ( ptt )
{
  started = false;
  output = false;
  freePointer = 0;
  available = 0;
  readPointer = 0;
  free = 2 * BUF_SIZE;
  handle=0;

}
CSound::~CSound()
{
}

int CSound::getSamples ( double *sample, int anzahl )
{
  if ( !started )
    return 0;
  if ( anzahl > available )
    return 0;
  memcpy ( sample, &buffer[readPointer], anzahl*sizeof ( double ) ) ;
  LockPointers.lock();
  available -= anzahl;
  free += anzahl;
  WakeUp.wakeAll();
  LockPointers.unlock();
  readPointer += anzahl;
  readPointer %= ( 2 * BUF_SIZE );
  return anzahl;
}

bool CSound::open_Device_read ( QString *errorstring )
{
  int err;
  snd_pcm_hw_params_t *hwparams;
  snd_pcm_sw_params_t *swparams;
  snd_pcm_uframes_t period_size = 512;
  unsigned int SampleRate = 11025;
  output = false;
  freePointer = 0;
  available = 0;
  readPointer = 0;
  free = 2 * BUF_SIZE;
  QString device("");
  // Could InputDeviceName contain a card name ?
  if(!settings.InputDeviceName.contains(':')) // Could be card name, so check /proc/asound/cards
  {
      int card_number=getDeviceNumber(settings.InputDeviceName);
      if(card_number < 0)
        device=settings.InputDeviceName;
      else
        device = QString("plughw:%1").arg(card_number);
  }
  else
   device=settings.InputDeviceName;
  err = snd_pcm_open ( &handle, device.toLatin1(), SND_PCM_STREAM_CAPTURE, SND_PCM_ASYNC );

  if ( err < 0 )
  {
      *errorstring = QString ( "Unable to open Device: " )+ settings.InputDeviceName + QString(" ") + QString ( snd_strerror ( err ) ) + QString ( "\nPlease read the README for configuration hints\n" );
    return false;
  }
  snd_pcm_hw_params_malloc ( &hwparams );
  snd_pcm_sw_params_malloc ( &swparams );

  /* Choose all parameters */
  err = snd_pcm_hw_params_any ( handle, hwparams );
  if ( err < 0 )
  {
    *errorstring = QString ( "Broken configuration : no configurations available: " ) + QString ( snd_strerror ( err ) );
    return false;
  }
  #ifdef SOUND_DEBUG
  dump_hw_params(handle,hwparams);
  #endif
  /* Set the interleaved read/write format */
  err = snd_pcm_hw_params_set_access ( handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED );
  if ( err < 0 )
  {
    *errorstring = QString ( "Access type not available : " ) + QString ( snd_strerror ( err ) );
    return false;
  }
  /* Set the sample format */
  err = snd_pcm_hw_params_set_format ( handle, hwparams, SND_PCM_FORMAT_FLOAT_LE );
//  err = snd_pcm_hw_params_set_format ( handle, hwparams, SND_PCM_FORMAT_S32_LE );
  if ( err < 0 )
  {
    *errorstring = QString ( "Sample format Float not available : " ) + QString ( snd_strerror ( err ) );
    return false;
  }
  /* Set the count of channels, most soundcards only support stereo */
  err = snd_pcm_hw_params_set_channels ( handle, hwparams, 2 );
  if ( err < 0 )
  {
    *errorstring = QString ( "Channels count 2 not available, please modify your plugin: " ) + QString ( snd_strerror ( err ) );
    return false;
  }

  err = snd_pcm_hw_params_set_rate ( handle, hwparams, SampleRate, 0 );
  if ( err < 0 )
  {
    *errorstring = QString ( "Samplerate %1 not available for capture: " ).arg ( SampleRate ) + QString ( snd_strerror ( err ) );
    return false;
  }
  int kkk = 0;
  err = snd_pcm_hw_params_set_period_size_near ( handle, hwparams, &period_size, &kkk );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to set period size %d for play: " ).arg ( period_size ) + QString ( snd_strerror ( err ) );
    return false;
  }
  snd_pcm_uframes_t buffer_size = 2048;
  err = snd_pcm_hw_params_set_buffer_size_near ( handle, hwparams, &buffer_size );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to set buffer size %i for capture: " ).arg ( buffer_size ) + QString ( snd_strerror ( err ) );
    return false;
  }
  err = snd_pcm_hw_params ( handle, hwparams );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to set hw params for input: " ) + QString ( snd_strerror ( err ) );
    #ifdef SOUND_DEBUG
    dump_hw_params(handle,hwparams);
    #endif
    return false;
  }
  snd_pcm_hw_params_free ( hwparams );
  /* Get the current swparams */
  err = snd_pcm_sw_params_current ( handle, swparams );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to determine current swparams for capture: " ) + QString ( snd_strerror ( err ) );
    return false;
  }

  err = snd_pcm_sw_params_set_start_threshold ( handle, swparams, 1 );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to set start threshold for capture: " )  + QString ( snd_strerror ( err ) );
    return false;
  }

  /* Write the parameters to the record device */
  err = snd_pcm_sw_params ( handle, swparams );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to set sw params for input: " ) + QString ( snd_strerror ( err ) );
    return false;
  }
  snd_pcm_sw_params_free ( swparams );

  output = false;
  err = snd_pcm_start ( handle );
  if ( err <= 0 )
    started = true;
  else
  {
    *errorstring = QString ( "Unable to start soundcard for receiving: " ) + QString ( snd_strerror ( err ) );
    started = false;
    return false;
  }

  return true;
}


bool CSound::open_Device_write ( QString *errorstring )
{
  int err;
  snd_pcm_hw_params_t *hwparams;
  snd_pcm_sw_params_t *swparams;
  snd_pcm_uframes_t period_size = 1024;
  unsigned int SampleRate = 11025;
  output = true;
  QString device("");
  // Could OutputDeviceName contain a card name ?
  if(!settings.OutputDeviceName.contains(':')) // Could be card name, so check /proc/asound/cards
  {
      int card_number=getDeviceNumber(settings.OutputDeviceName);
      if(card_number < 0)
        device=settings.OutputDeviceName;
      else
        device = QString("plughw:%1").arg(card_number);
  }
  else
   device=settings.OutputDeviceName;
  err = snd_pcm_open ( &handle, device.toLatin1(), SND_PCM_STREAM_PLAYBACK, 0 );
  if ( err < 0 )
  {
      *errorstring = QString ( "Unable to open Device: " )+settings.OutputDeviceName+ QString(" ") + QString ( snd_strerror ( err ) );
    return false;
  }
// Set blocking mode
  err = snd_pcm_nonblock ( handle, 0 );

  snd_pcm_hw_params_malloc ( &hwparams );
  snd_pcm_sw_params_malloc ( &swparams );

  /* Choose all parameters */
  err = snd_pcm_hw_params_any ( handle, hwparams );
  if ( err < 0 )
  {
    *errorstring = QString ( "Broken configuration : no configurations available: " ) + QString ( snd_strerror ( err ) );
    return false;
  }
  /* Set the interleaved read/write format */
  err = snd_pcm_hw_params_set_access ( handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED );

  if ( err < 0 )
  {
    *errorstring = QString ( "Access type not available : " ) + QString ( snd_strerror ( err ) );
    return false;
  }
  /* Set the sample format */
  err = snd_pcm_hw_params_set_format ( handle, hwparams, SND_PCM_FORMAT_FLOAT_LE );
  if ( err < 0 )
  {
    *errorstring = QString ( "Sample format Float not available : " ) + QString ( snd_strerror ( err ) );
    return false;
  }
  /* Set the count of channels */
  err = snd_pcm_hw_params_set_channels ( handle, hwparams, 2 );
  if ( err < 0 )
  {
    *errorstring = QString ( "Channels count 2 not available, please modify your plugin: " ) + QString ( snd_strerror ( err ) );
    return false;
  }

  err = snd_pcm_hw_params_set_rate ( handle, hwparams, SampleRate, 0 );
  if ( err < 0 )
  {
    *errorstring = QString ( "Samplerate %1 not available for playing: " ).arg ( SampleRate ) + QString ( snd_strerror ( err ) );
    return false;
  }
  int kkk = 0;
  err = snd_pcm_hw_params_set_period_size_near ( handle, hwparams, &period_size, &kkk );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to set period size %d for play: " ).arg ( period_size ) + QString ( snd_strerror ( err ) );
    return false;
  }
  snd_pcm_uframes_t buffer_size = 9192;
  err = snd_pcm_hw_params_set_buffer_size_near ( handle, hwparams, &buffer_size );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to set buffer size %i for play: " ).arg ( buffer_size ) + QString ( snd_strerror ( err ) );
    return false;
  }
//  else
//    qDebug("Buffersize set to %i",(int) buffer_size);
  err = snd_pcm_hw_params ( handle, hwparams );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to set hw params for Output: " ) + QString ( snd_strerror ( err ) );
    return false;
  }
  snd_pcm_hw_params_free ( hwparams );
  /* Get the current swparams */
  err = snd_pcm_sw_params_current ( handle, swparams );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to determine current swparams for play: " ) + QString ( snd_strerror ( err ) );
    return false;
  }
  snd_pcm_uframes_t threshold=buffer_size/2;
  if ( threshold > 2048)
    threshold=2048;
  err = snd_pcm_sw_params_set_start_threshold ( handle, swparams, threshold );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to set start threshold mode : " ) + QString ( snd_strerror ( err ) );
    return false;
  }
 // else
 //   qDebug("Setting start threshold to %i", (int) threshold);
  /* Write the parameters to the record/playback device */
  err = snd_pcm_sw_params ( handle, swparams );
  if ( err < 0 )
  {
    *errorstring = QString ( "Unable to set sw params for output: " ) + QString ( snd_strerror ( err ) );
    return false;
  }
  output = true;
  return true;
}

int CSound::putSamples ( double *sample, int anzahl )
{
  if ( anzahl <= free )
  {
    memcpy ( &buffer[freePointer], sample, anzahl*sizeof ( double ) );
    LockPointers.lock();
    toBePlayed += anzahl;
    free -= anzahl;
 //   if ( toBePlayed > 2*BUF_SIZE )
 //     qDebug ( "putSamples: Bufferoverrun" );
    WakeUp.wakeAll();
    LockPointers.unlock();

    freePointer += anzahl;
    freePointer %= ( 2 * BUF_SIZE );

  }
  else
  {
    anzahl=0;
//    qDebug ( "++++++free too low %d", free );
  }
  return anzahl;
}

void CSound::PTT ( bool mode )
{
  int flags;
  int ii;
  flags = TIOCM_RTS | TIOCM_DTR;
  if ( serial < 0 ) // No serial Device selected
  {
// No PTT, only start stream
    if ( mode )
    {
      if ( !started )
      {
        started = true;
      }
    }
    else
    {
      if ( started )
      {
        started = false;
      }
    }
    return;
  }
  if ( mode )  // PTT on
  {
    ii = ioctl ( serial, TIOCMBIS, &flags );
    if ( !started )
    {
      started = true;
    }
  }
  else
  {
    if ( started )
    {
      started = false;
    }
    ii = ioctl ( serial, TIOCMBIC, &flags );
  }
  if (ii < 0)
    qDebug("Error using serial port");
  return;
}

bool CSound::close_Device()
{
  wait();
  if ( handle != 0 )
    snd_pcm_close ( handle );
  handle = 0;
  started = false;
  output = false;
  return true;
}

void CSound::run()
{
  started = true;
  if ( !output )
    record();
  else
    play();

}
void CSound::record()
{
  int toBeRead, err;
  float SampleBuffer[256];
  qDebug ( "Start recording" );
  while ( started )
  {
    LockPointers.lock();
    while ( free < 128 && started )
      WakeUp.wait ( &LockPointers );
    LockPointers.unlock();
    if ( !started )
      break;
    toBeRead = min ( free, 128 );
//    if ( toBeRead > BUF_SIZE )
//    {
//      qDebug ( "Error toBeRead zu gross" );
//      return;
//    }
//    if ( ( freePointer + toBeRead ) > 2*BUF_SIZE )
//      qDebug ( "Out of Bounds \n" );
    err = snd_pcm_readi ( handle,  SampleBuffer, toBeRead );
    if ( err == toBeRead )
    {
      for ( int i = 0; i < toBeRead;i++ )
        buffer[freePointer+i] = SampleBuffer[2*i];
      LockPointers.lock();
      available += toBeRead;
      free -= toBeRead;
      LockPointers.unlock();
      freePointer = ( freePointer + toBeRead ) % ( 2 * BUF_SIZE );
      if ( available >= BUF_SIZE )
        emit samplesAvailable();
    }
    else
    {
      if ( err == -EPIPE )
      {
        // Overrun
        snd_pcm_prepare ( handle );
        snd_pcm_start ( handle );
 //       qDebug ( "Record: Overrun" );
      }
      else
      {
        qDebug ( "Record: unhandled error %d\n %s\n", err, snd_strerror ( err ) );
        return ;
      }
    }
  }
  snd_pcm_drop ( handle );
  qDebug ( "record thread terminated" );
}

void CSound::play()
{
  float SampleBuffer[256], x;
  bool signaled;
  int err, laenge;
  laenge = 128;
  signaled = false;
  freePointer = 0;
  readPointer = 0;
  free = 2 * BUF_SIZE;
  toBePlayed = 0;
  qDebug ( "Play thread started" );

  while ( started )
  {
    LockPointers.lock();
    if ( toBePlayed <= BUF_SIZE && ( !signaled ) )
    {
      emit samplesAvailable();
      signaled = true;
    }
//    if ( free > ( 2* BUF_SIZE ) )
//      qDebug ( "Out of bounds" );
    while ( toBePlayed < laenge && started )
      WakeUp.wait ( &LockPointers );
    LockPointers.unlock();
    if ( !started )
    {
 //     qDebug ( "Stop received" );
      break;
    }
    if ( toBePlayed >= BUF_SIZE )
      signaled = false;
    for ( int i = 0;i < laenge;i++ )
    {
      x = buffer[readPointer+i];
      SampleBuffer[2*i] = x;
      SampleBuffer[2*i+1] = x;
    }
    err =  snd_pcm_writei ( handle, SampleBuffer , laenge );
    if ( err == laenge )
    {
      LockPointers.lock();
      toBePlayed -= err;
      free += err;
      readPointer = ( readPointer + err ) % ( 2 * BUF_SIZE );
      LockPointers.unlock();
    }
    else
    {
      if ( err == -EPIPE )
      {
        /* under-run */
   //     qDebug ( "Underun in Play position 2 laenge %d Pointer %d available %d\n", laenge, readPointer,  toBePlayed );
        err = snd_pcm_prepare ( handle );
        if ( err < 0 )
        {
          qDebug ( "Can't recover from underrun, prepare failed: %s\nStopping Play Thread", snd_strerror ( err ) );
          break;
        }
      }
      else if ( err ==  -EAGAIN )
        qDebug ( "Play ???? \n" );
      else
      {
        qDebug ( " %s\nStopping Play Thread", snd_strerror ( err ) );
        break;
      }

    }
  }
  snd_pcm_drop ( handle );
  qDebug ( "Play Thread terminated" );
}
int CSound::getDeviceNumber(QString device)
{
  std::string line;
  std::ifstream cards( "/proc/asound/cards" );
  int id;
  id=-1;
  if ( cards.is_open() )  {
      while ( cards.good() )  {
          getline (cards, line);
          if ( line.find( device.toStdString() ) != std::string::npos )  {
              std::istringstream( line ) >>id;
              return id;
          }
      }
  }
  return id;
}
#ifdef SOUND_DEBUG
void CSound::dump_hw_params(snd_pcm_t *h,snd_pcm_hw_params_t* hw_p)
{
 unsigned int i;
 qDebug("PCM name: %s\n", snd_pcm_name (h));
 qDebug("Formats:\n");

 for (i = 0; i < NELEMS (format_types); i++){
   snd_pcm_format_t    ft = format_types[i];
  if (snd_pcm_hw_params_test_format (h, hw_p, ft) == 0)
       qDebug( "    %-20s YES", snd_pcm_format_name (ft));
   }

 qDebug("Sample Rates:\n");
  for (i = 0; i < NELEMS (test_rates); i++){
    unsigned int rate = test_rates[i];
    qDebug( "    %6u  %s", rate,
             snd_pcm_hw_params_test_rate (h, hw_p, rate, 0) == 0 ? "YES" : "NO");
  }
}
#endif
