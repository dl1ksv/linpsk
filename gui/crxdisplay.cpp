/***************************************************************************
                          |FILENAME|  -  description
                             -------------------
    begin                : |DATE|
    copyright            : (C) |YEAR| by |AUTHOR|
    email                : |EMAIL|
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QTimer>
#include <QMessageBox>
#include <QRadioButton>
#include <QLineEdit>
#include "crxdisplay.h"
#include "crxchannel.h"
#include "crxwindow.h"
#include "csquelch.h"
#include "frequencyselect.h"
#include "input.h"
#include "textinput.h"
#include "waveinput.h"
#include "csound.h"
#include "fircoeffs.h"
#include "parameter.h"

extern Parameter settings;
/*
 *  Constructs a CRxDisplay which is a child of 'parent', with the
 *  name 'name'.'
 */



CRxDisplay::CRxDisplay ( QWidget* parent )
  : QFrame ( parent ), Ui::CRxDisplay()
{
  setupUi(this);

  Sound = 0;

  dec2fir = new double[DEC2_LPFIR_LENGTH];

  RxChannel = new CRxChannel ( 0, this );
  RxHeader->insertTab ( 0, RxChannel->getWindow(),QString("Rx 1") );
  RxFreq->setFunctionText("Narrow");


  settings.ChannelChain = RxChannel;
  settings.ActChannel = RxChannel;

  languageChange();
// Connect Signals and Slots
  connect ( RxChannel->RxWindow, SIGNAL ( setQsoData(QsoData,QString)),this, SIGNAL (setQsoData(QsoData,QString)));
  connect ( RxChannel, SIGNAL ( Triggered ( int ) ), RxHeader, SLOT ( setCurrentIndex ( int ) ) );

  trigger(); // We should ensure that the triggertext is stored;
  Squelch->setSquelchState ( RxChannel->getSquelchState() );

// Creating Variables for the fft
  plan = fftw_plan_r2r_1d ( BUF_SIZE / 2, outbuf, output, FFTW_R2HC , FFTW_PATIENT );
}

/*
 *  Destroys the object and frees any allocated resources
 */
CRxDisplay::~CRxDisplay()
{
  // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CRxDisplay::languageChange()
{
//    setCaption( tr( "RxDisplay" ) );
}

bool CRxDisplay::start_process_loop()
{
  QString errorstring;
  if ( Sound == 0 )
  {
    if ( settings.DemoMode )
        Sound = new WaveInput ( -1 );
    else
      Sound = new CSound ( settings.serial );
    if ( Sound <= NULL )
      return false;

    connect ( Sound, SIGNAL ( samplesAvailable() ), this, SLOT ( process_rxdata() ) );
  }
  m_pDec2InPtr = dec2fir;

  for ( int i = 0; i < DEC2_LPFIR_LENGTH;i++ )
    dec2fir[i] = 0.0; // fill delay buffer with zero

  if ( ! Sound->open_Device_read ( &errorstring ) )  //Something went wrong in Opening Input File
  {
    if ( settings.DemoMode )
      QMessageBox::information ( 0, "LinPsk", errorstring );
    else
      QMessageBox::critical ( 0, "LinPsk", errorstring );
    if ( Sound != 0 )
      delete Sound;
    Sound = 0;
    return false;
  }
  Sound->start();
  return true;
}

void CRxDisplay::ProcDec2Fir ( double *pIn, double *pOut, int BlockSize )
{
  /**
  Decimate by 2 FIR filter on 'BlockSize' samples.
  pIn == pointer to input array of double's (can be same buffer as pOut )
  pOut == pointer to output array of double's
  Blocksize == number of samples to process
  This Procdeure is taken from WinPSK by Moe Wheatley
  **/
  int i, j;
  double acc;
  const double* Kptr;
  double* Firptr;
  double* Qptr;
  double* Inptr;
  Inptr = m_pDec2InPtr; //use automatic copies of member variables
  Qptr =  dec2fir;   // for better speed.
  j = 0;
  for ( i = 0; i < BlockSize; i++ ) // put new samples into Queue
  {
    if ( --Inptr < Qptr ) //deal with wraparound
      Inptr = Qptr + DEC2_LPFIR_LENGTH - 1;
    *Inptr = pIn[i];
    if ( i&1 ) //calculate MAC's every other time for decimation by 2
    {
      acc = 0.0;
      Firptr = Inptr;
      Kptr = Dec2LPCoef;
      while ( Kptr < ( Dec2LPCoef + DEC2_LPFIR_LENGTH ) ) //do the MAC's
      {
        acc += ( ( *Firptr++ ) * ( *Kptr++ ) );
        if ( Firptr >= Qptr + DEC2_LPFIR_LENGTH ) //deal with wraparound
          Firptr = Qptr;
      }
      pOut[j++] = acc;  //save output sample
    }
  }
  m_pDec2InPtr = Inptr;  // save position in circular delay line
}

void CRxDisplay::process_rxdata()

{
  bool overload;
  Mode modtype;
  if ( Sound->getSamples ( inbuf, BUF_SIZE ) == 0 )
    return; // No sample available, try later
  overload = false;
  ProcDec2Fir ( inbuf, outbuf , BUF_SIZE ); // 2uS per sample


  RxFreq->setFrequency ( settings.ActChannel->getRxFrequency() );
  Squelch->setSquelchLevel ( settings.ActChannel->getSquelchValue() );
  settings.ActChannel->setThreshold ( Squelch->getThreshold() );
  settings.ActChannel->setSquelch ( Squelch->getSquelchState() );
  settings.ActChannel->setAfcMode ( RxFreq->getAfcMode() );

  for ( CRxChannel * p = RxChannel;p != 0;p = p->getNextChannel() )
  {
    modtype=p->getModulationType();
/**    if ( ( modtype != RTTY ) && ( modtype != MFSK16 )  && (modtype != PSK63)  )
//    if ( ( modtype == BPSK ) || ( modtype == QPSK )  )
//      if ( ( modtype == BPSK ) )
    {

      p->processInput ( outbuf, output );
    }
    else */
      p->processInput ( inbuf, output );
  }
  /** Update RxFreq for the active Channel **/
  emit new_IMD ( settings.ActChannel->getIMD() );


// Calculate FFT and start Ploting

// First  look for overload
  int N = BUF_SIZE / 2;
  for ( int i = 0; i < N;i++ )
  {
    if ( inbuf[i] > 0.9 )
        overload = true;
// Apply Hamming to Data
    outbuf[i] *= ( 0.54 - 0.46 * cos ( ( i * PI2 ) / N ) );
  }
  fftw_execute ( plan );

//Calculate power spectrum
  for ( int i = 1;i < BUF_SIZE / 4;i++ )
    output[i] = output[i] * output[i] + output[BUF_SIZE/2-i] * output[BUF_SIZE/2-i];

  emit startPlotting ( output, overload );


}

void CRxDisplay::addRxWindow ( int Frequency, Mode Modulation, QString Heading )
{
  CRxChannel *p;
  int ID = RxHeader->count();
  p = new CRxChannel ( ID, this, Modulation, Frequency );
  RxHeader->insertTab ( ID, p->getWindow(),Heading );
  connect ( p, SIGNAL ( Triggered ( int ) ), this, SLOT ( changeActiveRxWindow ( int ) ) );
  connect ( p->RxWindow, SIGNAL ( setQsoData(QsoData,QString) ), this, SIGNAL ( setQsoData(QsoData,QString) ) );

  RxChannel->insertChannel ( p );
  RxHeader->setCurrentIndex ( ID );
  trigger(); // We should ensure that the triggertext is stored;
  RxHeader->setTabTextColor ( ID, Farbe->at ( ID ) );
}

void CRxDisplay::setRxFrequency ( double freq )
{
  settings.ActChannel->setRxFrequency ( freq );
}

void CRxDisplay::changeActiveRxWindow ( int ID )
{
setFocus();
CRxChannel *p;
  p=settings.ActChannel;
  if ( p != 0 )
  {
    p->hide();
    p->setQsoData ( settings.QslData ); // Save actual Data to  Channel
    p->setAfcMode ( RxFreq->getAfcMode() );
    if ( p->getChannel ( ID ) != 0 )
      settings.ActChannel = p->getChannel ( ID );
    p=settings.ActChannel;
    settings.QslData = p->getQsoData();
    RxFreq->setAfcDisplayMode ( p->AfcProperties() );
    RxFreq->setAfcMode ( p->getAfcMode() );
    RxFreq->setFrequency ( ( unsigned int ) p->getRxFrequency() );
    Squelch->setSquelchState ( p->getSquelchState() );
    Squelch->setThreshold ( p->getThreshold() );
    TriggerText->setText ( p->RxWindow->getTriggerText() );
    triggerActivate->setChecked(p->RxWindow->getTriggerStatus());
    recordActivate->setChecked ( p->RxWindow->getRecordingState() );
    p->show();
    emit newActiveChannel();
    RxHeader->setCurrentIndex ( ID );
  }
}


void CRxDisplay::stop_process_loop()
{
  if ( Sound != 0 )
  {
    Sound->stop();
    Sound->wait();
    Sound->close_Device();
  }
}
void CRxDisplay::trigger()
{
  settings.ActChannel->RxWindow->activateTrigger ( TriggerText->text() );
  if ( !triggerActivate->isChecked() )
    settings.ActChannel->RxWindow->deactivateTrigger();
}

void CRxDisplay::setColorList ( QList<QColor> *c )
{
  Farbe = c;

}

void CRxDisplay::clearRxWindow()
{
  settings.ActChannel->clearRxWindow();
}
void CRxDisplay::newColor()
{
  int anzahl = Farbe->size();
  if ( RxHeader->count() < anzahl )
    anzahl = RxHeader->count();
  for ( int i = 0; i < anzahl; i++ )
    RxHeader->setTabTextColor ( i, Farbe->at ( i ) );
}
void CRxDisplay::setAfc(AfcMode mode)
{
 RxFreq->setAfcMode ( mode );
}
void CRxDisplay::setAfcProperties(AfcMode mode)
{
  RxFreq->setAfcDisplayMode ( mode );
}

AfcMode CRxDisplay::getAfcMode()
{
  return RxFreq->getAfcMode();
}
void CRxDisplay::record(bool b)
{
  settings.ActChannel->record(b);
}
void CRxDisplay::stopRecording()
{
  //Stop recording for each channel if activated and close file.
  for ( CRxChannel * p = RxChannel;p != 0;p = p->getNextChannel() )
    p->record(false);
}
