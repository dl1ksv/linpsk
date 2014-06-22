/***************************************************************************
                          crxchannel.cpp  -  description
                             -------------------
    begin                : Sam Jan 4 2003
    copyright            : (C) 2003 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include <qwidget.h>
#include <qapplication.h>

#include "crxchannel.h"
#include "cdemodulator.h"
#include "crxwindow.h"
#include "qpskdemodulator.h"
#include "bpskdemodulator.h"
#include "rttydemodulator.h"
#include "mfskdemodulator.h"
#include "parameter.h"

extern Parameter settings;

CRxChannel::CRxChannel ( int ID, QWidget *parent, Mode DemodulatorType, unsigned int Freq )
{


  nextChannel = 0;
  prevChannel = 0;
  RxMode = DemodulatorType;
  WindowsID = ID;

  double SampleRate = 5512.5;
  int BufferLength;
  BufferLength = BUF_SIZE / 2;
  switch ( DemodulatorType )
  {
    case QPSK:
      Demodulator = new QPskDemodulator();
      break;
    case RTTY:
      Demodulator = new RTTYDemodulator();
      SampleRate = 11025;
      BufferLength = BUF_SIZE;
      break;
    case MFSK16:
      Demodulator = new MFSKDemodulator();
      SampleRate = 11025;
      BufferLength = BUF_SIZE;
      break;

    default:
      Demodulator = new BPskDemodulator();
      break;
  }
  Demodulator->Init ( SampleRate, BufferLength ); // Should changed later to
  // adjusted Samplerate
  Demodulator->setRxFrequency ( double ( Freq ) );
  Demodulator->setAfcMode ( Demodulator->AfcProperties() );
  RxWindow = new CRxWindow ( parent );
  connect ( Demodulator, SIGNAL ( newSymbol ( char ) ), RxWindow, SLOT ( updateRx ( char ) ) );
  connect ( RxWindow, SIGNAL ( Triggered() ), this, SLOT ( triggered() ) );
  QDateTime t1;
  QDateTime t;
  t = QDateTime::currentDateTime();

  t1 = t.addSecs ( settings.timeoffset * 3600 );


  QsoData.QsoDate= t.date() ;
  QsoData.QsoTime= t1.time();
  QsoData.QsoFrequency=5;
}
CRxChannel::~CRxChannel()
{
  if ( Demodulator != 0 )
    delete Demodulator;
  if ( RxWindow != 0 )
    delete RxWindow;
  if ( prevChannel != 0 )
    prevChannel->changeChain ( nextChannel );
}

void CRxChannel::insertChannel ( CRxChannel *nChannel )
{
  CRxChannel *p;
// Find last Channel in Chain
  p = this;
  while ( p->getNextChannel() != 0 )
    p = p->getNextChannel();
  if ( p == this )
  {
    nextChannel = nChannel;
    nChannel->setPrevChannel ( p );
  }
  else
    p->insertChannel ( nChannel );
}
void CRxChannel::changeChain ( CRxChannel *nChannel )
{
  nextChannel = nChannel;
}

void CRxChannel::setGeometry ( int x, int y, int width, int height )
{
  if ( RxWindow != 0 )
    RxWindow->setGeometry ( x, y, width, height );
}

void CRxChannel::processInput ( double *Input, double *Spectrum )
{
  Demodulator->ProcessInput ( Input, Spectrum );
}

CRxChannel * CRxChannel::getNextChannel()
{
  return nextChannel;
}

void CRxChannel::setRxFrequency ( double Freq )
{
  Demodulator->setRxFrequency ( Freq );
}

Mode CRxChannel::getModulationType()
{
  return RxMode;
}

void CRxChannel::hide()
{
  RxWindow->hide();
}

void CRxChannel::show()
{
  RxWindow->show();
}

CRxChannel* CRxChannel::getChannel ( int ID )
{
  CRxChannel *p;
  if ( ID > WindowsID )
    for ( p = this; p != 0; p = p->getNextChannel() )
    {
      if ( p->getID() == ID )
        return p;
    }
  if ( ID < WindowsID )
    for ( p = this; p != 0; p = p->prevChannel )
    {
      if ( p->getID() == ID )
        return p;
    }

  return this;
}

void CRxChannel::setPrevChannel ( CRxChannel *p )
{
  prevChannel = p;
}

complex<float>* CRxChannel::getPhasePointer()
{
  if ( ( RxMode == BPSK ) || ( RxMode == QPSK ) )
    return Demodulator->getPhasePointer();
  else
    return 0;
}

int CRxChannel::getID()
{
  return WindowsID;
}

void CRxChannel::setAfcMode ( AfcMode mode )
{
  Demodulator->setAfcMode ( mode );
}

AfcMode CRxChannel::getAfcMode()
{
  return Demodulator->getAfcMode();
}
int CRxChannel::getSquelchValue()
{
  return Demodulator->getSquelchValue();
}
void CRxChannel::setThreshold ( int value )
{
  Demodulator->setThreshold ( value );
}
void CRxChannel::setSquelch ( bool OnOff )
{
  Demodulator->setSquelch ( OnOff );
}
bool CRxChannel::getSquelchState()
{
  return Demodulator->getSquelchState();
}
double CRxChannel::getRxFrequency()
{
  if ( Demodulator != 0 )
    return Demodulator->getRxFrequency();
  else
    return 0;
}
int CRxChannel::getThreshold()
{
  return Demodulator->getThreshold();
}
void CRxChannel::updateRx ( char c )
{
  RxWindow->updateRx ( c );
}
void CRxChannel::setWindowColor ( QColor c )
{
  RxWindow->setColor ( c );
}
void CRxChannel::clearRxWindow()
{
  RxWindow->clearRxWindow();
}
void CRxChannel::setMode ( Mode DemodulatorType )
{
  if ( RxMode != DemodulatorType )
  {
    Demodulator->disconnect();
    double SampleRate = 5512.5;
    double Freq = Demodulator->getRxFrequency();
    bool Squelch = Demodulator->getSquelchState();
    int Threshold = Demodulator->getThreshold();
    delete Demodulator;

    int BufferLength = BUF_SIZE / 2;
    switch ( DemodulatorType )
    {
      case QPSK:
        Demodulator = new QPskDemodulator();
        break;
      case RTTY:
        Demodulator = new RTTYDemodulator();
        SampleRate = 11025;
        BufferLength = BUF_SIZE;
        break;

      case MFSK16:
        Demodulator = new MFSKDemodulator();
        SampleRate = 11025;
        BufferLength = BUF_SIZE;
        break;

      default:
        Demodulator = new BPskDemodulator();
        break;
    }
    RxMode = DemodulatorType;
    Demodulator->Init ( SampleRate, BufferLength ); // Should changed later to
    // adjusted Samplerate
    Demodulator->setRxFrequency ( Freq );
    Demodulator->setThreshold ( Threshold );
    Demodulator->setSquelch ( Squelch );
    connect ( Demodulator, SIGNAL ( newSymbol ( char ) ), RxWindow, SLOT ( updateRx ( char ) ) );

  }
}
void CRxChannel::setQsoData ( QsoInfo *Data )
{
  if ( Data )
    QsoData = *Data;
}
QsoInfo *CRxChannel::getQsoData()
{
  return &QsoData;
}
void CRxChannel::triggered()
{
  int i = WindowsID;
  emit Triggered ( i );
  QApplication::beep();
}
void CRxChannel::record ( bool recording )
{
  if ( recording )
  {
    QString s;
    s.setNum ( WindowsID );
    RxWindow->startRecording ( QString ( "Window_" ) + s + QString ( ".txt" ) );
  }
  else
    RxWindow->stopRecording();
}
int CRxChannel::get2RxFrequency()
{
  return ( int ) Demodulator->get2RxFrequency();
}
float CRxChannel::getIMD()
{
  return Demodulator->getIMD();
}
void CRxChannel::setParameter ( RxTxParameterType Type, void *Value )
{
  Demodulator->setParameter ( Type, Value );
}
void *CRxChannel::getParameter ( RxTxParameterType Type )
{
  return Demodulator->getParameter ( Type );
}
void * CRxChannel::getBuffer()
{
  return Demodulator->getBuffer();
}
AfcMode CRxChannel::AfcProperties()
{
  return Demodulator->AfcProperties();
}
CRxWindow *CRxChannel::getWindow()
{
 return RxWindow;
}
