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
#include "addmacro.h"
#include "deletemacro.h"
#include "activatemacros.h"
#include "qsodata.h"
#include "editmacro.h"
#include "deletemacro.h"
#include "renamemacro.h"
#include "color.h"
#include "definebandlist.h"
#include "rigcontrol.h"
#include <unistd.h>


#include <QtGui>
#include <QEventLoop>
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>

#define VERSION "1.3.1"


#define ProgramName "LinPSK "

extern Parameter settings;
/*
 *  Constructs a LinPSK as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
LinPSK::LinPSK ( QWidget* parent)
  : QMainWindow ( parent), Ui::LinPSK()
{
  Sound = 0;
  Modulator = 0;
  inAction=false;
  settings.rig =new RigControl();
  /** To avoid multipe Macro clicking **/
  blockMacros=false;
  /************************************/
  SaveParameters = new Parameter();

  setupUi(this);
  read_config();
  apply_settings();
  if((settings.rigModelNumber > 0) && settings.rigDevice != "None")
    {
     int rc=-settings.rig->connectRig();
     if(rc != RIG_OK)
       {
         switch(rc) {
           case RIG_ETIMEOUT:
             QMessageBox::warning(0,"Connection time out",QLatin1String("Could not connect to rig"));
             break;
           case RIG_EINVAL :
             QMessageBox::warning(0,"Invalid parameter",QLatin1String("Probably unknown rig"));
             break;
           default:
             QMessageBox::warning(0,"Connection time out",QLatin1String(rigerror(rc)));
             break;

        }
       }
    }
  Control->initQsoData();
  /** Fixme: handle font change: fontChange() **/

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
  setWindowTitle ( QString ( ProgramName ) + QString ( VERSION ) );
  setWindowIcon ( QIcon ( ":/images/linpsk.png" ) );
// Create Statusbar
  statusBar()->setFixedHeight(18);
  statusbar->setSizeGripEnabled ( true );


//Messages
  msg = new QLabel ( );
  statusbar->addPermanentWidget ( msg, 2 );
  msg->setText ( tr ( "Ready" ) );

// IMD
  IMD = new QLabel ( statusbar );
  statusbar -> addPermanentWidget ( IMD, 1 );

// Time
  zeit = new QLabel ( statusbar );
  statusbar->addPermanentWidget ( zeit, 1 );

// date
  datum = new QLabel ( statusbar );
  statusbar ->addPermanentWidget ( datum, 1 );
  setclock();

// Let the time pass
  QTimer *clock = new QTimer ( this );
  connect ( clock, SIGNAL ( timeout() ), SLOT ( setclock() ) );
  clock->start ( 60000 );

  RxDisplay->setAfcProperties(settings.ActChannel->AfcProperties());
  RxDisplay->setAfc(settings.ActChannel->getAfcMode());
  RxDisplay->setColorList ( &WindowColors );

  settings.QslData = settings.ActChannel->getQsoData();
  Control->setPhasePointer ( settings.ActChannel->getPhasePointer() );

  Control->setColorList ( &WindowColors );
  languageChange();
// signals and slots connections
//========== Macro Processing ======================================
  tokenList << "@CALLSIGN@" << "@DATE@" << "@Replace by filename@" << "@RX@" << "@THEIRCALL@" << "@THEIRNAME@";
  tokenList << "@TIMELOCAL@" << "@TIMEUTC@" << "@TX@" << "@RSTGIVEN@" << "@RSTRCVD@" <<"@TXPWR@";
  Control->insertMacros( &macroList );
  connect ( this, SIGNAL ( StartRx() ), this, SLOT ( startRx() ) );
  connect ( this, SIGNAL ( StartTx() ), this, SLOT ( startTx() ) );

//===================================================================

  TxBuffer = new CTxBuffer();
  TxDisplay->txWindow->setTxBuffer ( TxBuffer );
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
    modus = RxDisplay->getAfcMode();
    settings.ActChannel->setAfcMode ( modus );
    Mode rxmode = ( Mode ) Channel->selectedMode();
    if ( WindowColors.size() <= settings.RxChannels )
      WindowColors << color[ ( settings.RxChannels*51 ) % 256];

    RxDisplay->addRxWindow ( Channel->frequency(), rxmode, Channel->titleText() );

    settings.ActChannel->setWindowColor ( WindowColors.at ( settings.RxChannels ) );
    settings.RxChannels++;
    RxDisplay->setAfcProperties( settings.ActChannel->AfcProperties() );
  }

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
  if ( Modulator != 0 && !settings.DemoMode)
  {
    TxBuffer->insert ( TXOFF_CODE );
    if ( Sound != NULL )  // Switch Trx to rx
      {
        TxDisplay->switch2Rx();
        msg->setText ( tr ( "Switching to receive" ) );
        while ( Sound->isRunning() ) // Wait for buffer to be cleared
          qApp->processEvents ( QEventLoop::AllEvents, 100 );
      }
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
  Control->present(true);
  settings.Status = TxDisplay->TxFunctions->getstatus();
  TxDisplay->txWindow->setFocus();
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
  if ( Sound <= NULL ) // Only create Sound Device once for output
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
  if ( Sound <= NULL )
  {
    QMessageBox::critical ( 0, " Programm Error! LinPsk", "Could not create Sound Device for Output" );
    TxDisplay->TxFunctions->setStatus ( ON );
    return;
  }

  if ( !Sound->open_Device_write ( &errorstring ) )
  {
    QMessageBox::information ( 0, ProgramName, errorstring );
    stopTx();
     emit  StartRx();
    return;
  }

  connect ( Modulator, SIGNAL ( charSend ( char ) ), settings.ActChannel, SLOT ( updateRx ( char ) ) );
  TxDisplay->TxFunctions->setStatus ( ON );
  msg->setText ( tr ( "Transmitting " ) + Info );
  TxDisplay->txWindow->setFocus();
  settings.Status = TxDisplay->TxFunctions->getstatus();
  Control->present(false);

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
    {
      int modelNr = settings.rigModelNumber;
      settings = LocalSettings->getSettings();
      if(modelNr != settings.rigModelNumber) //Rig has changed
        {
          settings.rig->disconnectRig();
          int rc = -settings.rig->connectRig();
          if(rc != RIG_OK)
            {
              switch(rc) {
                case RIG_ETIMEOUT:
                  QMessageBox::warning(0,"Connection time out",QLatin1String("Could not connect to rig"));
                  break;
                case RIG_EINVAL :
                  QMessageBox::warning(0,"Invalid parameter",QLatin1String("Probably unknown rig.\nTrying to keep previous one"));
                  break;
                default:
                  QMessageBox::warning(0,"Connection time out",QLatin1String("Unknown reason"));
                  break;

             }
              settings.rigModelNumber=modelNr; // Reconnect here ?
              if(settings.rig->connectRig() != RIG_OK)
                return;
            }
        }

    }
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
  TxDisplay->txWindow->clearBuffers();
}

void LinPSK::apply_settings()
{
  checkControlDevices();
  Control->enableSaveData();
  Control->setAutoDate();
}
void LinPSK::setChannelParams()
{
  Control->setPhasePointer ( settings.ActChannel->getPhasePointer() );
  Control->newChannel();
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
    RxDisplay->setAfcProperties( settings.ActChannel->AfcProperties() );
    msg->setText ( tr ( "Receiving " ) + Info );
  }
}
void LinPSK::setRxMode()
{
  QString Info;
  ModeMenu Menu ;
  ExtraParameter *Param;
  ExtraParameter parameter;

  Param = ( ExtraParameter * ) settings.ActChannel->getParameter ( Extra );
  if ( Param != 0 )
    Menu.setParameter ( *Param );
  if ( Menu.exec() != 0 )
  {
    Mode rxmode = ( Mode ) Menu.selectedMode();
    settings.ActChannel->setMode ( rxmode );
    RxDisplay->setAfcProperties( settings.ActChannel->AfcProperties() );
    RxDisplay->setAfc ( settings.ActChannel->getAfcMode() );
    Control->setPhasePointer ( settings.ActChannel->getPhasePointer() );
    parameter= Menu.getParameter();
    settings.ActChannel->setParameter ( Extra, &parameter );
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
      default:
        Info = "undefined";
    }

  msg->setText ( tr ( "Receiving " ) + Info );
}

void LinPSK::save_config()
{
  int i,size;
  QSettings config ( "DL1KSV", "LinPSK" );
  /** Windows Parameter **/
  config.beginGroup ( "WindowsParameter" );
  config.setValue ( "WindowWidth", width() );
  config.setValue ( "Xpos", this->x() );
  config.setValue ( "Ypos", this->y() );
  config.setValue ( "WindowHeight", height() );
  config.setValue ( "FontName", qApp->font().family() );
  config.setValue ( "FontSize",  qApp->font().pointSize() );
  config.setValue ("ControlSplitter",Control->controlSplitterState());
  config.setValue ("SpectrumSplitter",Control->spectrumSplitterState());
  config.endGroup();
  /** DemoMode **/
  config.setValue ( "DemoMode", settings.DemoMode );
  config.setValue ( "DemoTypeNumber", settings.DemoTypeNumber );
//Operating
  config.setValue ( "Callsign", settings.callsign );
  config.setValue ( "MyLocator", settings.myLocator );
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
  config.setValue ("autoDate",settings.autoDate);
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
   size=macroList.size();
   if ( size > 0 )
   {
     config.beginWriteArray ( "Macros" );
     for ( i = 0; i < size;i++ )
     {
       config.setArrayIndex ( i );
       config.setValue ( "Name", macroList.at( i ).name );
       config.setValue ( "Definition", macroList.at(i).text );
       config.setValue ( "Accelerator", macroList.at (i ).accelerator );
       config.setValue ("Language",macroList.at( i ).languageType);
     }
     config.endArray();
   }
  // Bandlist
   size=settings.bandList.size();
   if(size >0)
     {
       config.beginWriteArray ("Bandlist");
       for(i=0;i < size;i++)
         {
           config.setArrayIndex ( i );
           config.setValue ( "Bandname", settings.bandList.at( i ).bandName );
           config.setValue ( "Bandstart", settings.bandList.at(i).bandStart );
           config.setValue ( "Bandend", settings.bandList.at(i).bandEnd );
           config.setValue ( "PreferedFrequency",settings.bandList.at(i).preferedFreq);

         }
       config.endArray();
     }
   // Rig parameter for use with hamlib
     config.beginGroup ( "Rigparameter" );
     config.setValue ( "PTTDevice", settings.SerialDevice );
     config.setValue( "RIGDevice", settings.rigDevice);
     config.setValue("Modelnr",settings.rigModelNumber);
     config.setValue("BaudRate",settings.baudrate);
     config.setValue("Handshake",settings.handshake);
     config.endGroup();

  // SoundDevices
   config .beginGroup("SoundDevices");
   config.setValue("InputDeviceName",settings.InputDeviceName);
   config.setValue("sampleRate",settings.sampleRate);
   config.setValue("OutputDeviceName",settings.OutputDeviceName);
   config.setValue("ComplexFormat",settings.complexFormat);
   config.endGroup();
}

void LinPSK::executeMacro ( int id )
{
//  qDebug("Execute macro %d",id);
  if(blockMacros) {
    qDebug("Macro %d blocked",id);
    return;
  }
  else
  {
    blockMacros=true;
    QTimer::singleShot(1000,this,SLOT(unblockMacros()));
  }
  QString macro = macroList.at(id).text;
  QString Token;
  QFile *MacroFile;
  int i,anzahl;
  bool switchtoRx=false;
  anzahl = macro.count ( '@' ) / 2;
  if ( anzahl > 0 )
  {
    int indexbis, indexvon = 0;
    for (  i = 0; i < anzahl; i++ )
    {
      indexbis = macro.indexOf ( '@', indexvon );
      indexvon = indexbis;
      indexbis = macro.indexOf ( '@', indexvon + 1 );
      Token = macro.mid ( indexvon, indexbis - indexvon + 1 );
      if ( Token == tokenList.at(0) )         // callsign
      {
        macro.replace ( indexvon, tokenList.at(0).length(), settings.callsign );

      }
      else
        if ( Token == tokenList.at(1) )         // Date
        {
          QDate t = QDate::currentDate();
          macro.replace ( indexvon, tokenList.at(1).length(), t.toString ( QString ( "d.MM.yyyy" ) ) );

        }
        else
          if ( Token == tokenList.at(3) )         // RX
          {
            macro.remove ( indexvon, 4 );
            switchtoRx=true;
          }
          else
            if ( Token == tokenList.at(4) )        // Remote callsign
            {
             macro.replace ( indexvon, tokenList.at(4).length(), settings.QslData->RemoteCallsign );

            }
            else
              if ( Token == tokenList.at(5) )        // Remote op's name
                {
                  macro.replace ( indexvon, tokenList.at(5).length(), settings.QslData->OpName );

                }
                else
                  if ( Token == tokenList.at(6) )        // Time Local
                  {
                    QDateTime t;
                    t = QDateTime::currentDateTime();

                    macro.replace ( indexvon, tokenList.at(6).length(), t.toString ( "h:mm" ) );

                  }
                  else
                    if ( Token == tokenList.at(7) )        // Time UTC
                    {
                      QDateTime t;
                      t = QDateTime::currentDateTime();
                      t = t.addSecs ( settings.timeoffset * 3600 );
                      macro.replace ( indexvon, tokenList.at(7).length(), t.toString ( "h:mm" ) );
                    }
                    else
                      if ( Token == tokenList.at(8) )        // TX
                      {
                          if ( settings.Status == UNDEF )
                        {
                          QMessageBox::warning ( 0, "Warning", "Please listen before transmitting",
                                                 QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton );
                          return;
                        }
                         if(settings.callsign.isEmpty())
                         {
                            QMessageBox::warning(0,"Parameter error",QLatin1String("Callsign not set"));
                            TxDisplay->TxFunctions->setStatus ( OFF );
                            return;
                          }
                         if(macro.contains(tokenList.at(4)) && settings.QslData->RemoteCallsign.isEmpty())  //Check if macro contains remote callsign when switching to tx
                          {
                            QMessageBox::critical(0,"Parameter error",QLatin1String("Remote callsign not set"));
                            TxDisplay->TxFunctions->setStatus ( OFF );
                            return;
                          }
                        macro.remove ( indexvon, 4 );
                        if(settings.Status == OFF)
                          emit StartTx();
                        else //We are alredy transmitting
                          if (!TxBuffer->isEmpty())
                            return;
                      }
                      else
                        if ( Token == tokenList.at(9) )        // RST given
                        {
                          macro.replace ( indexvon, tokenList.at(9).length(),settings.QslData->MyRST );

                        }
                        else
                          if ( Token == tokenList.at(10) )        // RST received
                          {
                           macro.replace ( indexvon, tokenList.at(10).length(),settings.QslData->HisRST );

                          }
                          else
                            if( Token == tokenList.at(11) )  // TXPWR
                            {
                             QString pwr;
                             pwr.setNum(settings.pwr);
                             macro.replace ( indexvon, tokenList.at(11).length(),pwr);
                            }
                          else  // Must be File
                          {
                            macro.remove ( indexvon, Token.length() );
                            if ( indexvon > 0 )
                            {
                              TxBuffer->insert ( macro, indexvon );
                              macro.remove ( 0, indexvon );
                              indexvon = 0;
                            }
                            Token = Token.mid ( 1, Token.length() - 2 );
                            if(Token.startsWith(QChar('/'))) // Distinguish between relative and absolute path
                                MacroFile =new QFile( Token );
                            else
                               MacroFile =new QFile( settings.Directory + QString ( "/" ) + Token );
                            if ( MacroFile->open ( QIODevice::ReadOnly ) )
                            {
                              QTextStream line ( MacroFile );
                              while ( !line.atEnd() )
                              {
                                QString Txstring = line.readLine();
                                if(settings.autoCrLf) // AutoCrLF ??
                                  Txstring += '\r';
                                Txstring += "\n";
                                TxBuffer->insert ( Txstring, Txstring.length() );
                                TxDisplay->insert(Txstring);
                              }
                              MacroFile->close();
                              delete MacroFile;
                            }
                            else
                              QMessageBox::warning ( 0, "Warning", "File " + Token + " not found ",
                                                     QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton );

                        }
      }
    }
    anzahl=macro.length();
    TxBuffer->insert(macro,anzahl);
    TxDisplay->setTxFocus();
    TxDisplay->insert(macro);
    if(switchtoRx & (settings.Status == ON))
      {
        TxDisplay->TxFunctions->setStatus(SW);
        msg->setText ( tr ( "Switching to receive" ) );
        emit  StartRx();
      }

}

void LinPSK::addMacro()
{

  AddMacro *nM = new AddMacro ( &macroList,tokenList);
  if (nM->exec() != 0 )
    Control->insertMacros( &macroList );
  delete nM;
}
void LinPSK::editMacro()
{

  EditMacro * eM = new EditMacro(&macroList,tokenList);
  if(eM->exec() != 0)
    Control->insertMacros( &macroList );
  delete eM;
}
void LinPSK::deleteMacro()
{
  DeleteMacro * dL = new DeleteMacro(&macroList);
  if ( dL->exec() !=0)
  {
   Control->insertMacros( &macroList );
  }
  delete dL;
}
void LinPSK::actdeactMacros()
{
  ActivateMacros *act =new ActivateMacros(&macroList);
  if(act->exec() !=0 ) {
    Control->insertMacros( &macroList );
  }
  delete act;
}
void LinPSK::renameMacro()
{
  RenameMacro *rM = new RenameMacro(&macroList);
  if(rM->exec() != 0)
   {
     Control->updateMacroWindow(rM->getMacroNumber());
   }
  delete rM;

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
    Control->restoreSplitterStates(config.value("ControlSplitter").toByteArray(),config.value("SpectrumSplitter").toByteArray());
  }
  config.endGroup();
  /** DemoMode **/
  settings.DemoMode = config.value ( "DemoMode" ).toBool();
  settings.DemoTypeNumber = config.value ( "DemoTypeNumber" ).toInt();
//Operating
  settings.callsign = config.value ( "Callsign" ).toString();
  settings.myLocator = config.value ( "MyLocator" ).toString();

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
  settings.autoDate = config.value("autoDate").toBool();
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
      Macro macro;
      for ( i = 0; i < size;i++ )
      {
        config.setArrayIndex ( i );
        macro.name = config.value ( "Name" ).toString();
        macro.text = config.value ( "Definition" ).toString();
        macro.accelerator = config.value ( "Accelerator" ).toString();
        macro.languageType=config.value("Language",0).toInt();
        macroList.append(macro);
        }
    }
    config.endArray();
// Bandlist
    size = config.beginReadArray ("Bandlist");

    if(size >0)
      {
        Band band;
        for(i=0;i < size;i++)
          {
            config.setArrayIndex ( i );
            band.bandName = config.value ( "Bandname" ).toString();
            band.bandStart = config.value ( "Bandstart",1).toInt();
            band.bandEnd = config.value ( "Bandend",1).toInt();
            band.preferedFreq = config.value ( "PreferedFrequency",1).toInt();
            settings.bandList.append(band);
          }
      }
     else
      {
        Band none;
        none.bandName   = "--";
        none.bandStart  = 0;
        none.bandEnd    = 60000000;
        none.preferedFreq=0;
        settings.bandList.append(none);
      }
     config.endArray();

 if ( ( HeighttoSet > 0 ) && ( WidthtoSet > 0 ) )
    resize ( WidthtoSet, HeighttoSet );
 if ( ( X >= 0 ) && ( Y >= 0 ) )
    move ( X, Y );
 // SoundDevices
 // Rig parameter for use with hamlib
   config.beginGroup ( "Rigparameter" );
   settings.SerialDevice = config.value ( "PTTDevice" ).toString();
   settings.rigDevice = config.value( "RIGDevice").toString();
   settings.rigModelNumber = config.value("Modelnr",0).toInt();
   settings.baudrate = config.value("BaudRate",9600).toInt();
   settings.handshake= config.value("Handshake",1).toInt();
   config.endGroup();

  config .beginGroup("SoundDevices");
  settings.InputDeviceName=config.value("InputDeviceName","LinPSK_Record").toString();
  settings.sampleRate=config.value("sampleRate",11025).toInt();
  settings.OutputDeviceName=config.value("OutputDeviceName","LinPSK_Play").toString();
  settings.complexFormat=config.value("ComplexFormat").toBool();
  config.endGroup();
}
void LinPSK::checkControlDevices()
{

#ifdef WITH_HAMLIB  
  int  err;
  settings.serial = -1;
  int flags = TIOCM_RTS | TIOCM_DTR;
  if( (settings.SerialDevice != "None") && (settings.SerialDevice == settings.rigDevice))
    {
      QMessageBox::critical ( 0, "LinPsk", "PTT device and device to control the rig must be different\nPlease change configuration" );
      settings.SerialDevice = "None";
      settings.rigDevice="None";
      return;
    }
#endif    
  if ( settings.SerialDevice != "None" )
  {
    settings.serial = open( settings.SerialDevice.toLatin1().data(), O_EXCL | O_WRONLY );
    if ( settings.serial > 0 )
      err=ioctl ( settings.serial, TIOCMBIC, &flags );
    if( (settings.serial <0) || (err < 0 ))
    {
      QMessageBox::critical ( 0, "LinPsk", "Unable to open PTT device " + settings.SerialDevice+"\nCheck if device is available or permission of device" );
      settings.SerialDevice = "None"; //Their seems to be a wrong Value in the ConfigFile
      settings.serial=-1;
    }
   }
#ifdef WITH_HAMLIB   
  if ( settings.rigDevice != "None")
  {
    int fd = open( settings.rigDevice.toLatin1().data(), O_EXCL | O_WRONLY );
    if ( fd > 0 )
      err=ioctl ( fd, TIOCMBIC, &flags );
    if( (fd <0) || (err < 0 ))
    {
      QMessageBox::critical ( 0, "LinPsk", "Unable to open serial port to control  rig" + settings.rigDevice+"\nCheck if device is available or permission of device" );
      settings.rigDevice = "None"; //Their seems to be a wrong Value in the ConfigFile
    }
    if(fd >0)
      ::close(fd);
   }
#endif      
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
    RxDisplay->stopRecording();
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


void LinPSK::on_RxDisplay_newActiveChannel()
{

}
void LinPSK::unblockMacros()
{
 blockMacros=false;
}
void LinPSK::defineBandlist()
{
  DefineBandList *dF = new DefineBandList();
  if (dF->exec() != 0 )
    Control->initQsoData();
  delete dF;
}
