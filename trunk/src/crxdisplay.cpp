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

#include "crxdisplay.h"



#include <QTimer>
#include <QMessageBox>
#include <QRadioButton>
#include <QLineEdit>

#include "crecording.h"
#include "crxchannel.h"
#include "crxwindow.h"
#include "csquelch.h"
#include "ctrigger.h"
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
    : QFrame ( parent )
{
  setFrameShape ( QFrame::WinPanel );
  setFrameShadow ( QFrame::Sunken );

  RxFreq = new FrequencySelect ( this, Wide );
  RxFreq->setTitle ( "Rx Freq / AFC" );
  RxFreq->setFunctionText ( "Narrow" );

  Recording = new CRecording ( this );

  Trigger = new CTrigger ( "Trigger", this );

  Squelch = new CSquelch ( "Squelch", this );

  RxHeader = new QTabBar ( this );
  RxHeader->setShape ( QTabBar::RoundedNorth );

  Clear = new QPushButton ( this );
  Clear->setText ( "Clear" );

//    QTab *Tab = new QTab("Rx 1");
  RxHeader->insertTab ( 0, "Rx 1" );
  Sound = 0;

  dec2fir = new double[DEC2_LPFIR_LENGTH];

  RxChannel = new CRxChannel ( 0, this );
  connect ( RxChannel->RxWindow, SIGNAL ( copyCallSign ( QString ) ), this, SIGNAL ( copyCallSign ( QString ) ) );
  connect ( RxChannel->RxWindow, SIGNAL ( copyQTH ( QString ) ), this, SIGNAL ( copyQTH ( QString ) ) );
  connect ( RxChannel->RxWindow, SIGNAL ( copyName ( QString ) ), this, SIGNAL ( copyName ( QString ) ) );
  connect ( RxChannel->RxWindow, SIGNAL ( copyLocator ( QString ) ), this, SIGNAL ( copyLocator ( QString ) ) );
  connect ( RxChannel->RxWindow, SIGNAL ( copyRST ( QString ) ), this, SIGNAL ( copyRST ( QString ) ) );

  settings.ChannelChain = RxChannel;
  settings.ActChannel = RxChannel;
  Squelch->setSquelchState ( RxChannel->getSquelchState() );
  languageChange();
// Connect Signals and Slots
//  RxTimer = new QTimer ( this );
//  connect ( RxTimer, SIGNAL ( timeout() ), this, SLOT ( process_rxdata() ) );
  connect ( RxFreq, SIGNAL ( FrequencyChanged ( double ) ), this, SLOT ( setRxFrequency ( double ) ) );
  connect ( RxHeader, SIGNAL ( currentChanged ( int ) ), this, SLOT ( changeActiveRxWindow ( int ) ) );
// connect(RxFreq,SIGNAL(toggleAFC(bool)),this,SLOT(setAFC(bool)));
  connect ( Trigger->Activate, SIGNAL ( clicked() ), this, SLOT ( trigger() ) );
  connect ( Trigger->TriggerText, SIGNAL ( returnPressed() ), this, SLOT ( trigger() ) );
  connect ( RxChannel, SIGNAL ( Triggered ( int ) ), RxHeader, SLOT ( setCurrentIndex ( int ) ) );

  connect ( Clear, SIGNAL ( clicked() ), this, SLOT ( clearRxWindow() ) );
  trigger(); // We should ensure that the triggertext is stored;

// Creating Variables for the fft

// plan=rfftw_create_plan(BUF_SIZE/2,FFTW_REAL_TO_COMPLEX,FFTW_ESTIMATE);
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

void CRxDisplay::calculateSizeofComponents()
{
  /** in percent of whole widget **/
  /** RX Part **/
#define RXPARTWIDTH 64
  /**  Squelch **/
#define SQUELCHWIDTH 10
  /** General Width **/
#define GENERALWIDTH 18
  /** Recording **/
#define RECORDHEIGHT 25
  /** Trigger **/
#define TRIGGERHEIGHT 33
  /** RxFrequency (Height) **/
#define RXFREQHEIGHT 70
  /** Left and Right Margin **/
#define LEFTANDRIGHTMARGIN 1
  /** Top and Bottom Margin **/
#define TOPANDBOTTOMMARGIN 5
  /** Inner distance **/
#define DISTANCE 1
  /** Tab height **/
#define TABHEIGHT 10
#define TABWIDTH 30
  int xpos, ypos, width, height, innerheight, innerwidth;
  width = this->width();
  height = this->height();
  xpos = width * LEFTANDRIGHTMARGIN / 100;

  /**Recording **/

  ypos = height * TOPANDBOTTOMMARGIN / 100;
  innerwidth = width * GENERALWIDTH / 100;
  innerheight = height * RECORDHEIGHT / 100;
  Recording->setGeometry ( xpos, ypos, innerwidth, innerheight );
  ypos = ypos + innerheight + height * DISTANCE / 100;

  /** Trigger **/

  innerheight = height * TRIGGERHEIGHT / 100;
  Trigger->setGeometry ( xpos, ypos, innerwidth, innerheight );
  /** RXFrequency **/
  ypos = ypos + innerheight + height * DISTANCE / 100;
  RxFreq->setGeometry ( xpos, ypos, innerwidth, innerheight );

  /**SQuelch **/
  xpos = xpos + innerwidth + width * DISTANCE / 100;
  ypos = height * TOPANDBOTTOMMARGIN / 100;;
  innerheight = height - 2 * ypos;
  innerwidth = width * SQUELCHWIDTH / 100;
  Squelch->setGeometry ( xpos, ypos, innerwidth, innerheight );

  /** RxWindowTabBar **/
  xpos = xpos + innerwidth + width * DISTANCE / 100;
  innerwidth = ( width - xpos - width * LEFTANDRIGHTMARGIN / 100 ) * TABWIDTH / 100;;
  innerheight = height * TABHEIGHT / 100;
  RxHeader->setGeometry ( xpos, ypos, innerwidth, innerheight );

  Clear->setGeometry ( width - width*LEFTANDRIGHTMARGIN / 100 - 100, ypos, 100, innerheight );

  ypos = ypos + innerheight;
  innerheight = height - innerheight - height * TOPANDBOTTOMMARGIN / 100;
  innerwidth = width - xpos - width * LEFTANDRIGHTMARGIN / 100;
  settings.ActChannel->setGeometry ( xpos, ypos, innerwidth, innerheight );
}

void CRxDisplay::resizeEvent ( QResizeEvent * )
{
  calculateSizeofComponents();
}

bool CRxDisplay::start_process_loop()
{
  QString errorstring;


  if ( Sound == 0 )
  {
    if ( settings.DemoMode )
    {
      if ( settings.DemoTypeNumber == 0 )
        Sound = new WaveInput ( -1 );
      else
        Sound = new TextInput ( -1 );
    }
    else
      Sound = new CSound ( settings.serial );
    if ( Sound <= 0 )
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
    if ( ( modtype != RTTY ) && ( modtype != MFSK16 ) /* RIP && (modtype != RTTY2) */ )
    {

      p->processInput ( outbuf, output );
    }
    else
      p->processInput ( inbuf, output );
  }
  /** Update RxFreq for the active Channel **/
  emit new_IMD ( settings.ActChannel->getIMD() );


// Calculate FFT and start Ploting

// First  look for overload
  int N = BUF_SIZE / 2;
  for ( int i = 0; i < N;i++ )
  {
    if ( outbuf[i] > 0.77 )
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
  RxHeader->insertTab ( ID, Heading );
  p = new CRxChannel ( ID, this, Modulation, Frequency );
  connect ( p, SIGNAL ( Triggered ( int ) ), this, SLOT ( changeActiveRxWindow ( int ) ) );
  connect ( p->RxWindow, SIGNAL ( copyCallSign ( QString ) ), this, SIGNAL ( copyCallSign ( QString ) ) );
  connect ( p->RxWindow, SIGNAL ( copyQTH ( QString ) ), this, SIGNAL ( copyQTH ( QString ) ) );
  connect ( p->RxWindow, SIGNAL ( copyName ( QString ) ), this, SIGNAL ( copyName ( QString ) ) );
  connect ( p->RxWindow, SIGNAL ( copyLocator ( QString ) ), this, SIGNAL ( copyLocator ( QString ) ) );
  connect ( p->RxWindow, SIGNAL ( copyRST ( QString ) ), this, SIGNAL ( copyRST ( QString ) ) );

  RxChannel->insertChannel ( p );
  RxHeader->setCurrentIndex ( ID );
//RxHeader->repaint();
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
  if ( settings.ActChannel != 0 )
  {
    settings.ActChannel->hide();
    settings.ActChannel->setQsoData ( settings.QslData ); // Save actual Data to  Channel
    settings.ActChannel->setAfcMode ( RxFreq->getAfcMode() );
    if ( settings.ActChannel->getChannel ( ID ) != 0 )
      settings.ActChannel = settings.ActChannel->getChannel ( ID );
    settings.QslData = settings.ActChannel->getQsoData();
    RxFreq->setAfcDisplayMode ( settings.ActChannel->AfcProperties() );
    RxFreq->setAfcMode ( settings.ActChannel->getAfcMode() );
    RxFreq->setFrequency ( ( unsigned int ) settings.ActChannel->getRxFrequency() );
    Squelch->setSquelchState ( settings.ActChannel->getSquelchState() );
    Squelch->setThreshold ( settings.ActChannel->getThreshold() );
    Trigger->Activate->setChecked ( settings.ActChannel->RxWindow->getTriggerStatus() );
    Trigger->TriggerText->setText ( settings.ActChannel->RxWindow->getTriggerText() );
    Recording->Record->setChecked ( settings.ActChannel->RxWindow->getRecordingState() );
    calculateSizeofComponents();
    settings.ActChannel->show();
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
    delete Sound;
  }
  Sound = 0 ;
}
void CRxDisplay::trigger()
{

  settings.ActChannel->RxWindow->activateTrigger ( Trigger->TriggerText->text() );
  if ( !Trigger->Activate->isChecked() )
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

