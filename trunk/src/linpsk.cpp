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
 *   The PSK part is based on WinPSK 1.0 by Moe Wheatly, AE4JY             *
 ***************************************************************************/

#include "linpsk.h"


#include "controlpanel.h"
#include "crxdisplay.h"
#include "ctxdisplay.h"

#include "parameter.h"
#include "addrxwindow.h"
#include "spectrumdisplay.h"
#include "frequencyselect.h"
#include "cledbutton.h"
#include "rttymodulator.h"
#include "pskmodulator.h"
#include "bpskmodulator.h"
#include "qpskmodulator.h"
#include "mfskmodulator.h"
#include "ctxbuffer.h"
#include "crxchannel.h"
#include "input.h"
#include "textinput.h"
#include "waveinput.h"
#include "csound.h"
#include "generalsettings.h"
#include "modemenu.h"
#include "macrowindow.h"
#include "addmacro.h"
#include "deletemacro.h"
#include "macros.h"
#include "qsodata.h"
#include "editmacro.h"
#include "deletemacro.h"
#include "renamemacro.h"
#include "color.h"
#include "crecording.h"


#include <QtGui>
#include <QEventLoop>

#ifdef HAVE_CONFIG
#include "config.h"
#else
#define VERSION "1.1"
#endif


#define ProgramName "LinPSK "

extern Parameter settings;
/*
 *  Constructs a LinPSK as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
LinPSK::LinPSK ( QWidget* parent, Qt::WFlags fl )
    : QMainWindow ( parent, fl )
{
  RxDisplay = 0;
  TxDisplay = 0;
  Control = 0;
  Sound = 0;
  Modulator = 0;
  inAction=false;
  SaveParameters = new Parameter();

  Macro = new Macros(); // Macros will be used in read_config
  read_config();

  if ( settings.ApplicationFont == 0 )
  {
    settings.ApplicationFont = new QFont ( qApp->font().family() );
    settings.ApplicationFont->setPixelSize ( 10 );
    qApp->setFont ( *settings.ApplicationFont );
  }
  else
    qApp->setFont ( *settings.ApplicationFont );

// Save Settings to be able to make local modifications
  *SaveParameters = settings;

  if ( WindowColors.size() == 0 )
    WindowColors << color[0] ;
  setCentralWidget ( new QWidget ( this ) );
  centralWidget()->setMinimumSize ( settings.MinimumWindowWidth, settings.MinimumWindowHeight );
  setWindowTitle ( QString ( ProgramName ) + QString ( VERSION ) );
  setWindowIcon ( QIcon ( ":/images/linpsk.png" ) );
// Create Statusbar
  QStatusBar *StatusBar = this->statusBar();
  StatusBar->setSizeGripEnabled ( true );


//Messages
  msg = new QLabel ( StatusBar );
  StatusBar->addPermanentWidget ( msg, 2 );
  msg->setText ( tr ( "Ready" ) );

// IMD
  IMD = new QLabel ( StatusBar );
  StatusBar -> addPermanentWidget ( IMD, 1 );


// Time
  zeit = new QLabel ( StatusBar );
  StatusBar->addPermanentWidget ( zeit, 1 );

// date
  datum = new QLabel ( StatusBar );
  StatusBar ->addPermanentWidget ( datum, 1 );
  setclock();

// Let the time pass
  QTimer *clock = new QTimer ( this );
  connect ( clock, SIGNAL ( timeout() ), SLOT ( setclock() ) );
  clock->start ( 60000 );

  RxDisplay = new CRxDisplay ( centralWidget() );
  TxDisplay = new CTxDisplay ( centralWidget() );

  Control = new ControlPanel ( Macro, centralWidget() );
  RxDisplay->RxFreq->setAfcDisplayMode ( settings.ActChannel->AfcProperties() );
  RxDisplay->RxFreq->setAfcMode ( settings.ActChannel->getAfcMode() );
  RxDisplay->setColorList ( &WindowColors );
  connect ( RxDisplay, SIGNAL ( copyCallSign ( QString ) ), Control->QSO, SLOT ( copyCallSign ( QString ) ) );
  connect ( RxDisplay, SIGNAL ( copyQTH ( QString ) ), Control->QSO, SLOT ( copyQTH ( QString ) ) );
  connect ( RxDisplay, SIGNAL ( copyName ( QString ) ), Control->QSO, SLOT ( copyName ( QString ) ) );
  connect ( RxDisplay, SIGNAL ( copyLocator ( QString ) ), Control->QSO, SLOT ( copyLocator ( QString ) ) );
  connect ( RxDisplay, SIGNAL ( copyRST ( QString ) ), Control->QSO, SLOT ( copyRST ( QString ) ) );

  settings.QslData = settings.ActChannel->getQsoData();
  Control->Display->setPhasePointer ( settings.ActChannel->getPhasePointer() );

  Control->Display->setColorList ( &WindowColors );
  //Control->MacroBox->updateMacroWindow ( Macro );
  // menubar
  menubar = menuBar();
// File Menu
  fileMenu =  menubar->addMenu ( tr ( "File" ) );
  // Settings Menu

  editMenu =  menubar->addMenu ( tr ( "Settings" ) );

  menubar->addSeparator();
//=======
  changeRxParams =  menubar->addMenu ( tr ( "RxParams" ) );
  helpMenu =  menubar->addMenu ( tr ( "Help" ) );
  QAction *A;
  // File Menu
  // ================= File Actions ========
  A = fileMenu->addAction ( tr ( "Open Demo File" ) );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( fileOpen() ) );
  A = fileMenu->addAction ( tr ( "add RxWindow" ) );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( addRxWindow() ) );

  fileMenu->addSeparator();
  fileMenu->addSeparator();

  A = fileMenu->addAction ( tr ( "Exit" ) );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( Exit() ) );

// Settings Menu
  // ================= Settings Actions=================
  A = editMenu->addAction ( tr ( "General_Settings" ) );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( generalSettings() ) );
  editMenu->addSeparator();
  A = editMenu->addAction ( tr ( "Add Macros" ) );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( addMacro() ) );
  A = editMenu->addAction ( tr ( "Edit Macros" ) );
  connect ( A , SIGNAL ( triggered() ), this, SLOT ( editMacro() ) );
  A = editMenu->addAction ( tr ( "Delete Macros" ) );
  connect ( A  , SIGNAL ( triggered() ), this, SLOT ( deleteMacro() ) );
  A = editMenu->addAction ( tr ( "Rename Macros" ) );
  connect ( A  , SIGNAL ( triggered() ), this, SLOT ( renameMacro() ) );
//===============================================================================
  A = editMenu->addSeparator();

  A = editMenu->addAction ( tr ( "Font Settings" ) );
  connect ( A   , SIGNAL ( triggered() ), this, SLOT ( FontSetup() ) );
  A = editMenu->addAction ( tr ( "ColorSettings" ) );
  connect ( A , SIGNAL ( triggered() ), this, SLOT ( chooseColor() ) );
  editMenu->addSeparator();

  A = editMenu->addAction ( tr ( "Save Settings" ) );
  connect ( A  , SIGNAL ( triggered() ), this, SLOT ( saveSettings() ) );
  // Menu Rx Settings
  //================Actions for RX Window ===========
  A = changeRxParams->addAction ( tr ( "Change Rx Mode" ) );
  connect ( A , SIGNAL ( triggered() ), this, SLOT ( setRxMode() ) );
//Help Menu
  // ================= Help Actions =========
///  A = helpMenu->addAction ( tr ( "helpContentsAction" ) );
///  connect ( A, SIGNAL ( triggered() ), this, SLOT ( helpIndex() ) );
///  A = helpMenu->addAction ( tr ( "helpIndexAction" ) );
///  connect ( A, SIGNAL ( triggered() ), this, SLOT ( helpContents() ) );
  A = helpMenu->addAction ( tr ( "About LinPsk" ) );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( helpAbout() ) );
  A = helpMenu->addAction (tr("About Qt"));
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( helpAboutQt() ) );

  languageChange();

  // signals and slots connections

//================================= Rx Parames ===================
//================================================================
  connect ( TxDisplay, SIGNAL ( startRx() ), this, SLOT ( startRx() ) );
  connect ( TxDisplay, SIGNAL ( startTx() ), this, SLOT ( startTx() ) );
  connect ( Macro, SIGNAL ( StartRx() ), this, SLOT ( startRx() ) );
  connect ( Macro, SIGNAL ( StartTx() ), this, SLOT ( startTx() ) );

  connect ( RxDisplay, SIGNAL ( startPlotting ( double *, bool ) ), Control->Display, SLOT ( startPlot ( double *, bool ) ) );
  connect ( Control->Display, SIGNAL ( FrequencyChanged ( double ) ), RxDisplay->RxFreq, SLOT ( setFrequency ( double ) ) );
  connect ( RxDisplay, SIGNAL ( new_IMD ( float ) ), this, SLOT ( setIMD ( float ) ) );
  connect ( Control->MacroBox, SIGNAL ( callMacro ( int ) ), this, SLOT ( executeMacro ( int ) ) );
  connect ( RxDisplay, SIGNAL ( newActiveChannel() ), this, SLOT ( setChannelParams() ) );
  connect ( RxDisplay->Recording->Record, SIGNAL ( toggled ( bool ) ), this, SLOT ( recording ( bool ) ) );
//===================================================================

  TxBuffer = new CTxBuffer();
  TxDisplay->TxWindow->setTxBuffer ( TxBuffer );
  apply_settings();

}

/*
 *  Destroys the object and frees any allocated resources
 */
LinPSK::~LinPSK()
{

  // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void LinPSK::languageChange()
{
  /**
  // File
      Open_Demo_File->setText( tr( "Open Demo File" ) );
      Open_Demo_File->setMenuText( tr( "&Open Demo File" ) );
      Open_Demo_File->setAccel( tr( "Ctrl+O" ) );
      fileExitAction->setText( tr( "Exit" ) );
      fileExitAction->setMenuText( tr( "E&xit" ) );
      fileExitAction->setAccel( QString::null );
  // Help
///      helpContentsAction->setText( tr( "Contents" ) );
///      helpContentsAction->setMenuText( tr( "&Contents..." ) );
///      helpContentsAction->setAccel( QString::null );
///      helpIndexAction->setText( tr( "Index" ) );
///      helpIndexAction->setMenuText( tr( "&Index..." ) );
///      helpIndexAction->setAccel( QString::null );
      helpAboutAction->setText( tr( "About" ) );
      helpAboutAction->setMenuText( tr( "&About" ) );
      helpAboutAction->setAccel( QString::null );
      add_Rx_Window->setText( tr( "add Rx Window" ) );
      add_Rx_Window->setMenuText( tr( "&Add another RxWindow" ) );
  // Settings
      General_Settings->setText( tr( "General Settings" ) );
      AddMacros->setText( tr( "Add Macro" ) );
      FontSettings->setText( tr( "Font Settings" ) );
      ColorSettings->setText( tr( "Color Settings" ) );
      EditMacros->setText( tr( "Edit Macro" ) );
      DeleteMacros->setText( tr("Delete Macro") );
      RenameMacros->setText( tr("Rename Macro") );
  //    EditFiles->setText( tr( "Edit Files" ) );
      SaveSettings->setText( tr( "Save Settings" ) );
  // Rx Params
      Clear_RxWindow->setMenuText( tr ("Clear active RX Channel") );
      ChangeRxMode->setMenuText( tr ("Change Mode of active Rx Channel") );

      menubar->findItem( 0 )->setText( tr( "&File" ) );
      menubar->findItem( 1 )->setText( tr( "&Settings" ) );
      menubar->findItem( 2 )->setText( tr( "&RxParams" ) );
      menubar->findItem( 4 )->setText( tr( "&Help" ) );
  **/
}

void LinPSK::fileOpen()
{
  QString fileName;

  fileName = QFileDialog::getOpenFileName ( 0, tr ( "Open Demofile" ), "", settings.DemoModeFileType[settings.DemoTypeNumber] );
  if ( !fileName.isEmpty() )
    settings.inputFilename = fileName;
}

void LinPSK::Exit()
{
  if ( settings.Status == ON ) // We arejust transmitting
  {
    QMessageBox::information ( 0, ProgramName,
                               "You should stop transmitting before closing this window!" );
    return;
  }
  else
  {
    if ( RxDisplay != 0 )
      RxDisplay->stop_process_loop();
    settings = *SaveParameters;
    save_config();
    qApp->quit();
    return;
  }
}

///void LinPSK::helpIndex()
///{}

///void LinPSK::helpContents()
///{}

void LinPSK::helpAbout()
{
QMessageBox::about(this, QString(tr("About LinPsk")), QString(tr("<h3>About LinPsk</h3><p><a href=\"http://linpsk.sf.net/\">LinPsk</a> is a PSK31 program for Linux by Volker Schroer, DL1KSV. It supports some other digital modes and is developed  under GPL <br/>( Read the file COPYING contained in the distribution for more information )</p>")));
}
void LinPSK::helpAboutQt()
{
  QMessageBox::aboutQt(this);
}

void LinPSK::addRxWindow()
{
  AddRxWindow *Channel = new AddRxWindow();
  if ( Channel->exec() != 0 )
  {
    AfcMode modus;
    modus = RxDisplay->RxFreq->getAfcMode();
    settings.ActChannel->setAfcMode ( modus );
    Mode rxmode = ( Mode ) Channel->selectedMode();
    if ( WindowColors.size() <= settings.RxChannels )
      WindowColors << color[ ( settings.RxChannels*51 ) % 256];

    RxDisplay->addRxWindow ( Channel->frequency(), rxmode, Channel->titleText() );

    settings.ActChannel->setWindowColor ( WindowColors.at ( settings.RxChannels ) );
    settings.RxChannels++;
    RxDisplay->RxFreq->setAfcDisplayMode ( settings.ActChannel->AfcProperties() );
  }

}



void LinPSK::calculateSizeofComponents()
{
  /** Anteile in percent of mainwindow **/
  /** RXDisplay **/
#define RXPART 36
  /** TXDisplay **/
#define TXPART 20
  /** Controlpanel **/
#ifndef LINPSK_FOR_MAC
#define CONTROLPART 37
#else
#define CONTROLPART 40
#endif
  
  int width, height;
  int xpos, ypos;
  int windowsheight;

  width = this->width();
  height = this->height();

  xpos = 0;
  ypos = 0;;
  windowsheight = height * RXPART / 100;
  if ( RxDisplay != 0 )
    RxDisplay->setGeometry ( xpos, ypos, width, windowsheight );

  ypos = ypos + windowsheight;
  windowsheight = height * TXPART / 100;
  if ( TxDisplay != 0 )
    TxDisplay->setGeometry ( xpos, ypos, width, windowsheight );
  ypos = ypos + windowsheight;

  windowsheight = height * CONTROLPART / 100;
  if ( Control != 0 )
    Control->setGeometry ( xpos, ypos, width, windowsheight );
}



void LinPSK::resizeEvent ( QResizeEvent * )
{
  calculateSizeofComponents();
}


void LinPSK::setclock()
{
  QString s;
  QDateTime t;
  t = QDateTime::currentDateTime();
  t = t.addSecs ( settings.timeoffset * 3600 );
  s.sprintf ( " %2d:%2d UTC", t.time().hour(), t.time().minute() );
  s.replace ( QRegExp ( ": " ), ":0" );
  zeit->setText ( s );
  zeit->update();
  s = t.toString ( "dd.MM.yyyy" );
  datum->setText ( s );
}

void LinPSK::setIMD ( float IMDvalue )
{
  QString s;
  if ( IMDvalue != 0.0 )
    s.sprintf ( " IMD = %6.2f dB", IMDvalue );
  else
    s.sprintf ( " IMD " );
  IMD->setText ( s );
}
void LinPSK::startRx()
{
  if(inAction)
    return;
  inAction=true;
  if ( Modulator != 0 )
  {
    TxBuffer->insert ( TXOFF_CODE );
    if ( Sound > 0 )  // Switch Trx to rx
      while ( Sound->isRunning() ) // Wait for buffer to be cleared
        qApp->processEvents ( QEventLoop::AllEvents, 100 );
  }
  if ( RxDisplay->start_process_loop() )
  {
    QString Info;
    TxDisplay->TxFunctions->setStatus ( OFF );
    if ( settings.ActChannel != 0 )
      switch ( settings.ActChannel->getModulationType() )
      {
        case QPSK:
          Info = "QPSK";
          break;

        case BPSK:
          Info = "BPSK";
          break;

        case RTTY:
          Info = "RTTY";
          break;

        case MFSK16:
          Info = "MFSK16";
          break;


        default:
          Info = "undefined";
      }

    msg->setText ( tr ( "Receiving " ) + Info );
  }
  else
    TxDisplay->TxFunctions->setStatus ( UNDEF );
  Control->Display->show();
  settings.Status = TxDisplay->TxFunctions->getstatus();
TxDisplay->TxWindow->setFocus();
inAction=false;
}

void LinPSK::startTx()
{
  Mode ModulationType;
  QString errorstring;
  QString Info;
  double Frequency;


  RxDisplay->stop_process_loop();
  if ( settings.ActChannel == 0 )
  {
    QMessageBox::critical ( 0, " Programm Error! LinPsk", "No active Channel available" );
    TxDisplay->TxFunctions->setStatus ( UNDEF );
    return;
  }
  ModulationType = settings.ActChannel->getModulationType();
  if ( TxDisplay->TxFreq->getAfcMode() != Off ) // net ?
    TxDisplay->TxFreq->setFrequency ( settings.ActChannel->getRxFrequency() );
  Frequency = TxDisplay->TxFreq->getFrequency();
  switch ( ModulationType )
  {
    case QPSK:
      Modulator = new QPskModulator ( 11025, Frequency, TxBuffer );
      Info = "QPSK";
      break;
    case BPSK:
      Modulator = new BPSKModulator ( 11025, Frequency, TxBuffer );
      Info = "BPSK";
      break;

    case RTTY:
      Modulator = new RTTYModulator ( 11025, Frequency, TxBuffer );
      if ( settings.ActChannel->getParameter ( Extra ) != 0 )
        Modulator->setParameter ( Extra, settings.ActChannel->getParameter ( Extra ) );
      Info = "RTTY";
      break;
    case MFSK16:
      Modulator = new MFSKModulator ( 11025, Frequency, TxBuffer );
      Info = "MFSK16";
      break;

    default:
      Modulator = new BPSKModulator ( 11025, Frequency, TxBuffer );
      Info = "BPSK";
      break;
  }
  if ( Sound <= 0 ) // Only create Sound Device once for output
  {
    if ( settings.DemoMode )
    {
      if ( settings.DemoTypeNumber == 0 )
        Sound = new WaveInput ( -1 );
      else
        Sound = new TextInput ( -1 );
      msg->setText ( tr ( "Transmitting (Demo)" ) );
    }
    else
      Sound = new CSound ( settings.serial );
    connect ( Sound, SIGNAL ( samplesAvailable() ), this, SLOT ( process_txdata() ) );
  }
  if ( Sound <= 0 )
  {
    QMessageBox::critical ( 0, " Programm Error! LinPsk", "Could not create Sound Device for Output" );
    TxDisplay->TxFunctions->setStatus ( ON );
    return;
  }

  if ( Sound->open_Device_write ( &errorstring ) < 0 )
  {
    QMessageBox::information ( 0, ProgramName, errorstring );
    stopTx();
    return;
  }

  connect ( Modulator, SIGNAL ( charSend ( char ) ), settings.ActChannel, SLOT ( updateRx ( char ) ) );
  TxDisplay->TxFunctions->setStatus ( ON );
  msg->setText ( tr ( "Transmitting " ) + Info );
  TxDisplay->TxWindow->setFocus();
  settings.Status = TxDisplay->TxFunctions->getstatus();
  Control->Display->hide();

  Txcount = BUF_SIZE;
//  process_txdata(); // Generate first Sample
  Sound->PTT ( true );
  Sound->start();
}

void LinPSK::process_txdata()
{
  int length;
// To avoid pending calls to process_txdata() after stopping tx
//  when Modulator is alredy deleted
  if (Modulator == 0)
    return;
  if ( Txcount > 0 )
  {
    length = Modulator->CalcSignal ( Output, BUF_SIZE );
    if ( length <= 0 )
    {

      length = -length;
      while ( length < BUF_SIZE )
        Output[length++] = 0.0;
      length = BUF_SIZE;
      Txcount = Sound->putSamples ( Output, length );
      stopTx();
      return;
    }
  }
  else
    length = BUF_SIZE;

  Txcount = Sound->putSamples ( Output, length ); // If Txcount >= 0 and length < BUF_SIZE
  // we've reached end of Transmiision

}

void LinPSK::generalSettings()
{
  GeneralSettings *LocalSettings = new GeneralSettings ( this );
  if ( LocalSettings->exec() != 0 )
    settings = LocalSettings->getSettings();
  apply_settings();
}
void LinPSK::chooseColor()
{
  int ID = settings.ActChannel->getID();
  QColor color = QColorDialog::getColor ( WindowColors[ID], this );
  if ( color.isValid() )
  {
    settings.ActChannel->setWindowColor ( color );
    WindowColors[ID] = color;
    RxDisplay->repaint();
    RxDisplay->newColor();
  }

}
void LinPSK::FontSetup()
{
  bool ok;
  QFont f = QFontDialog::getFont ( &ok, font(), this );
  if ( ok )
  {
    settings.ApplicationFont->setFamily ( f.family() );
    int ii = f.pixelSize();
    if ( ii <= 0 )
    {
      ii = f.pointSize();
      settings.ApplicationFont->setPointSize ( ii );
    }
    else
      settings.ApplicationFont->setPixelSize ( ii );

    qApp->setFont ( f );
  }
}
void LinPSK::stopTx()
{
  Modulator->disconnect();
  if ( Sound != 0 )
  {
    Sound->PTT ( false );
    Sound->close_Device();
  }
  delete Modulator;
  Modulator = 0;
}

void LinPSK::apply_settings()
{
  selectPTTDevice();
}
void LinPSK::setChannelParams()
{
  Control->Display->setPhasePointer ( settings.ActChannel->getPhasePointer() );
  Control->QSO->newChannel();
  if ( settings.ActChannel != 0 )
  {
    QString Info;
    switch ( settings.ActChannel->getModulationType() )
    {

      case QPSK:
        Info = "QPSK";
        break;

      case BPSK:
        Info = "BPSK";
        break;

      case RTTY:
        Info = "RTTY";
        break;

      case MFSK16:
        Info = "MFSK16";
        break;
      default:
        Info = "undefined";
    }
    RxDisplay->RxFreq->setAfcDisplayMode ( settings.ActChannel->AfcProperties() );
    msg->setText ( tr ( "Receiving " ) + Info );
  }
}
void LinPSK::setRxMode()
{
  QString Info;
  ModeMenu Menu ;
  ExtraParameter *Param;
  Param = ( ExtraParameter * ) settings.ActChannel->getParameter ( Extra );
  if ( Param != 0 )
    Menu.setParameter ( *Param );
  if ( Menu.exec() != 0 )
  {
    Mode rxmode = ( Mode ) Menu.selectedMode();
    settings.ActChannel->setMode ( rxmode );
    RxDisplay->RxFreq->setAfcDisplayMode ( settings.ActChannel->AfcProperties() );
    RxDisplay->RxFreq->setAfcMode ( settings.ActChannel->getAfcMode() );
    Control->Display->setPhasePointer ( settings.ActChannel->getPhasePointer() );
    settings.ActChannel->setParameter ( Extra, &Menu.getParameter() );
  }
  if ( settings.ActChannel != 0 )
    switch ( settings.ActChannel->getModulationType() )
    {
      case QPSK:
        Info = "QPSK";
        break;

      case BPSK:
        Info = "BPSK";
        break;

      case RTTY:
        Info = "RTTY";
        break;

      case MFSK16:
        Info = "MFSK16";
        break;
/* RIP 
    case RTTY2:
        Info = "RTTY2";
        break;
*/
      default:
        Info = "undefined";
    }

  msg->setText ( tr ( "Receiving " ) + Info );
}

void LinPSK::save_config()
{
  int i;
  QSettings config ( "DL1KSV", "LinPSK" );
  /** Windows Parameter **/
  config.beginGroup ( "WindowsParameter" );
// config.setValue ( "MinimumWindowWidth", settings.MinimumWindowWidth );
  config.setValue ( "WindowWidth", width() );
// config.setValue ( "MinimumWindowHeight", settings.MinimumWindowHeight );
  config.setValue ( "Xpos", this->x() );
  config.setValue ( "Ypos", this->y() );
  config.setValue ( "WindowHeight", height() );
  config.setValue ( "FontName", qApp->font().family() );
  config.setValue ( "FontSize",  qApp->font().pointSize() );
  config.endGroup();
  /** DemoMode **/
  config.setValue ( "DemoMode", settings.DemoMode );
  config.setValue ( "DemoTypeNumber", settings.DemoTypeNumber );
//Operating
  config.setValue ( "Callsign", settings.callsign );
  config.setValue ( "MyLocator", settings.myLocator );

  config.setValue ( "PTTDevice", settings.SerialDevice );
//Logging
  config.setValue ( "Directory", settings.Directory );
  config.setValue ( "QSoFileName", settings.QSOFileName );
  config.setValue ( "FileLog", settings.fileLog );
  config.setValue ( "LinLog", settings.LinLog );
  if ( settings.LinLog )
  {
    config.setValue ( "Host", settings.Host );
    config.setValue ( "Port", settings.Port );
  }

  config.setValue ( "TimeOffset", settings.timeoffset );
  config.setValue ( "dateFormat", settings.dateFormat );
  config.setValue ( "Slashed0", settings.slashed0 );
  config.setValue ("autoCrLf",settings.autoCrLf);
  /** Colors **/
  if ( WindowColors.size() > 0 )
  {
    config.beginWriteArray ( "Colors" );
    for ( i = 0; i < WindowColors.size();i++ )
    {
      config.setArrayIndex ( i );
      config.setValue ( "r", WindowColors[i].red() );
      config.setValue ( "g", WindowColors[i].green() );
      config.setValue ( "b", WindowColors[i].blue() );
    }
    config.endArray();
  }
// Macros
  if ( ( Macro->count() > 0 ) )
  {
    config.beginWriteArray ( "Macros" );
    for ( i = 0; i < Macro->count();i++ )
    {
      config.setArrayIndex ( i );
      config.setValue ( "Name", Macro->getMacroName ( i ) );
      QString s = Macro->getDefinition ( i );
      config.setValue ( "Definition", s );
      config.setValue ( "Accelerator", Macro->getAccelerator ( i ) );
    }
    config.endArray();
  }

}

void LinPSK::executeMacro ( int MacroNumber )
{
  Macro->executeMacro ( MacroNumber, TxBuffer );
}

void LinPSK::addMacro()
{

  AddMacro *NewMacro = new AddMacro ( Macro );
// NewMacro->setKeywords ( Macro );
  if ( NewMacro->exec() != 0 )
  {
    Macro->insert ( NewMacro->macroName(),
                    NewMacro->macroDefinition(), NewMacro->accelerator(), NewMacro->position() );
    Control->MacroBox->updateMacroWindow ( Macro );
  }
}
void LinPSK::editMacro()
{
  if ( Macro->count() > 0 )
  {
    EditMacro *Edit = new EditMacro ( Macro );

    if ( Edit->exec() != 0 )
//   Control->MacroBox->updateMacroWindow ( Macro );
      Control->MacroBox->setMacroWindow ( Macro );
  }
}
void LinPSK::deleteMacro()
{
  if ( Macro->count() > 0 )
  {
    DeleteMacro *Del = new DeleteMacro ( Macro );

    if ( Del->exec() != 0 )
      Control->MacroBox->setMacroWindow ( Macro );
  }
}
void LinPSK::renameMacro()
{
  if ( Macro->count() > 0 )
  {
    RenameMacro *Ren = new RenameMacro ( Macro );

    if ( Ren->exec() != 0 )
      Control->MacroBox->setMacroWindow ( Macro );
  }
}

void LinPSK::read_config()
{
  QSettings config ( "DL1KSV", "LinPSK" );

  int HeighttoSet = 0;
  int WidthtoSet = 0;
  int X = -1;
  int Y = -1;
  int i, size;
// Try to read settings from Configfile
  /** Windows Parameter **/
  config.beginGroup ( "WindowsParameter" );
// settings.MinimumWindowWidth = config.value ( "MinimumWindowWidth" , 600 ).toInt();
  WidthtoSet = config.value ( "WindowWidth" ).toInt();
// settings.MinimumWindowHeight = config.value ( "MinimumWindowHeight", 400 ).toInt();
  X = config.value ( "Xpos" ).toInt();
  Y = config.value ( "Ypos" ).toInt();
  HeighttoSet = config.value ( "WindowHeight" ).toInt();
  if ( config.contains ( "FontName" ) )
  {
    settings.ApplicationFont = new QFont ( config.value ( "FontName" ).toString() );
    size = config.value ( "FontSize" ).toInt();
    if ( size < 10 )
      size = 10;
    settings.ApplicationFont->setPointSize ( size );

  }
  config.endGroup();
  /** DemoMode **/
  settings.DemoMode = config.value ( "DemoMode" ).toBool();
  settings.DemoTypeNumber = config.value ( "DemoTypeNumber" ).toInt();
//Operating
  settings.callsign = config.value ( "Callsign" ).toString();
  settings.myLocator = config.value ( "MyLocator" ).toString();

  settings.SerialDevice = config.value ( "PTTDevice" ).toString();
//Logging
  settings.Directory = config.value ( "Directory", QDir::homePath() ).toString();

  settings.QSOFileName = config.value ( "QSoFileName" ).toString();
  settings.fileLog = config.value ( "FileLog", true ).toBool();
  settings.LinLog = config.value ( "LinLog", false ).toBool();
  if ( settings.LinLog )
  {
    settings.Host = config.value ( "Host", "localhost" ).toString();
    settings.Port = config.value ( "Port", 8080 ).toInt();
  }
  settings.timeoffset = config.value ( "TimeOffset" ).toInt();
  settings.dateFormat = config.value ( "dateFormat", "dd.MM.yyyy" ).toString();
  settings.slashed0 = config.value ( "Slashed0" ).toBool();
  settings.autoCrLf = config.value("autoCrLf").toBool();
  /** Colors **/
  if ( config.contains ( "Colors" ) )
  {
    size = config.beginReadArray ( "Colors" );
    for ( i = 0; i < size;i++ )
    {
      config.setArrayIndex ( i );
      int r = config.value ( "r" ).toInt();
      int g = config.value ( "g" ).toInt();
      int b = config.value ( "b" ).toInt();
      WindowColors.push_back ( QColor ( r, g, b ) );

    }
    config.endArray();
  }
// Macros
  size = config.beginReadArray ( "Macros" );
  if ( size > 0 )
  {
    QString name, def, acc;

    for ( i = 0; i < size;i++ )
    {
      config.setArrayIndex ( i );
      name = config.value ( "Name" ).toString();
      def = config.value ( "Definition" ).toString();
      acc = config.value ( "Accelerator" ).toString();
      Macro->insert ( name, def, acc, i + 1 );
    }
    config.endArray();
  }

  if ( ( HeighttoSet > 0 ) && ( WidthtoSet > 0 ) )
    resize ( WidthtoSet, HeighttoSet );
  if ( ( X >= 0 ) && ( Y >= 0 ) )
    move ( X, Y );

}
void LinPSK::selectPTTDevice()
{

//if (settings.serial >0 )
//  close(settings.serial);
  settings.serial = -1;
  if ( settings.SerialDevice != "None" )
//{
// QMessageBox::information(0,"LinPsk","Trying to open Device " + settings.SerialDevice);
    settings.serial = open ( settings.SerialDevice.toAscii().data(), O_EXCL | O_WRONLY );
// if (settings.serial > 0 )
//  QMessageBox::information(0,"LinPsk","Opening successfull");
// else
//  QMessageBox::information(0,"LinPsk","Opening unsuccessfull");
//}
  int flags = TIOCM_RTS | TIOCM_DTR;
  if ( settings.serial > 0 )
    ioctl ( settings.serial, TIOCMBIC, &flags );
  else
    settings.SerialDevice = "None"; //Their seems to be a wrong Value in the ConfigFile
}
void LinPSK::recording ( bool on )
{
  settings.ActChannel->record ( on );
}
void LinPSK::HelpAbout()
{
  QMessageBox::about ( this, tr ( "About..." ),
                       ProgramName + QString ( VERSION ) + "\n written by Volker Schroer, DL1KSV\n" );
//  setActiveWindow();
}

void LinPSK::saveSettings()
{
  *SaveParameters = settings;
  save_config();
}

void LinPSK::closeEvent ( QCloseEvent *e )
{
  if ( settings.Status != ON ) // We are not just transmitting
  {
    if ( RxDisplay != 0 )
      RxDisplay->stop_process_loop();
    e->accept();
    save_config();
  }
  else
  {
    QMessageBox::information ( 0, ProgramName,
      "You should stop transmitting before closing this window!" );
    e->ignore();
  }
  return;
}

