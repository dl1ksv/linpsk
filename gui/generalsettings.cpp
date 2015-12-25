/***************************************************************************
 *   Copyright (C) 2007 by volker, DL1KSV   *
 *   schroer@tux64   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "generalsettings.h"
#include <QString>
#include <QDir>
#include <QButtonGroup>
#include "readonlystringlistmodel.h"
#include <QModelIndex>
#include <QMenu>

#include <hamlib/rig.h>

extern Parameter settings;
GeneralSettings::GeneralSettings ( QWidget* parent, Qt::WindowFlags fl )
  : QDialog ( parent, fl ), Ui::GeneralSettings()
{
  setupUi ( this );
  QString DirectoryName;
  QDir dir;
  QString s;
  LocalSettings = settings;
  FileFormat = new QButtonGroup ( FileFormatLayout );
  FileFormat->setExclusive ( true );
  FileFormat->addButton ( Wav, 0 );
  FileFormat->addButton ( Text, 1 );
  if ( LocalSettings.DemoTypeNumber == 0 )
    Wav->setChecked ( true );
  else
    Text->setChecked ( true );
  Callsign->setText ( LocalSettings.callsign );
  myLocator->setText ( LocalSettings.myLocator );
  UTC->setValue ( LocalSettings.timeoffset );
  SlashedZero->setAutoExclusive(false);
  SlashedZero->setChecked ( LocalSettings.slashed0 );
  autoCrLf->setAutoExclusive(false);
  autoCrLf->setChecked(LocalSettings.autoCrLf);
  autoDate->setChecked(LocalSettings.autoDate);
  QRegExp rx ( "^[A-R][A-R][0-9][0-9][A-X][A-X]$" );
  QValidator *validator = new QRegExpValidator ( rx, this );
  myLocator->setValidator ( validator );
  myLocator->setText ( LocalSettings.myLocator );
  Demomode->setChecked ( LocalSettings.DemoMode );

  if ( Demomode->isChecked() )
    selectDemomode(true);
  else
    selectDemomode(false);

  //PTT
  pttDevice->setText ( LocalSettings.SerialDevice );
  // First look in the /dev Directory
  DirectoryName = "/dev/";

  dir.setPath ( DirectoryName );
  QStringList filenames;
  //Check for serial and usb to serial devices
  filenames << "ttyS*" << "ttyUSB*";
  QStringList Files = dir.entryList ( filenames, QDir::System | QDir::CaseSensitive, QDir::Name );

  for ( int kk = 0; kk < Files.size(); kk++ )
    Files.replace ( kk, DirectoryName + Files.at ( kk ) );
  ReadOnlyStringListModel *m = new ReadOnlyStringListModel ( this );
  m->setStringList ( Files );
  AvailableDevices->setModel ( m );
  AvailableDevices->show();
  // Sound Devices
  soundInputDeviceName->setText(LocalSettings.InputDeviceName);
  soundOutputDeviceName->setText(LocalSettings.OutputDeviceName);
  //Logging
  Directory->setText ( LocalSettings.Directory );
  QsoFile->setText ( LocalSettings.QSOFileName );
  fileLog->setChecked ( LocalSettings.fileLog );
  Directory->setDisabled ( !LocalSettings.fileLog );
  QsoFile->setDisabled ( !LocalSettings.fileLog );
  LinLog->setChecked ( LocalSettings.LinLog );
  Port->setDisabled ( !LocalSettings.LinLog );
  Host->setDisabled ( !LocalSettings.LinLog );
  /** +++++ Rig +++ */
  // Rig number
  modelNr->setText(QString("%1").arg(LocalSettings.rigModelNumber));
  // PTT
  pttDevice->setText ( LocalSettings.SerialDevice );
  // Rig device
  rigControl->setText(LocalSettings.rigDevice);
  // Rig- Serial
  handShake->setCurrentIndex(LocalSettings.handshake);
  baudRate->setCurrentIndex(baudRate->findText(QString("%1").arg(LocalSettings.baudrate),Qt::MatchExactly));

  selectionMenu= new QMenu(AvailableDevices);
  QAction *A = selectionMenu->addAction ( tr ( "Set PTT device" ) );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( setPTTDevice() ) );
  A = selectionMenu->addAction ( "Set Rig device" );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( setRigDevice() ) );
}


GeneralSettings::~GeneralSettings()
{}

/*$SPECIALIZATION$*/


Parameter GeneralSettings::getSettings()
{
  LocalSettings.callsign = Callsign->text();
  LocalSettings.myLocator = myLocator->text();
  if ( Demomode->isChecked() )
    {
      LocalSettings.DemoMode = true;
      LocalSettings.DemoTypeNumber = FileFormat->checkedId();
      LocalSettings.inputFilename = "";
    }
  else
    {
      LocalSettings.DemoMode = false;
      LocalSettings.InputDeviceName=soundInputDeviceName->text();
      LocalSettings.OutputDeviceName=soundOutputDeviceName->text();
    }

  LocalSettings.timeoffset = UTC->value();
  LocalSettings.slashed0 = SlashedZero->isChecked();
  LocalSettings.autoCrLf = autoCrLf->isChecked();
  LocalSettings.autoDate=autoDate->isChecked();
  LocalSettings.SerialDevice = pttDevice->text();
  LocalSettings.fileLog = fileLog->isChecked();
  if ( LocalSettings.fileLog )
    {
      LocalSettings.Directory = Directory->text();
      LocalSettings.QSOFileName = QsoFile->text();
    }

  LocalSettings.LinLog = LinLog->isChecked();
  if ( LocalSettings.LinLog )
    {
      LocalSettings.Host = Host->text();
      LocalSettings.Port = Port->value();
    }
  LocalSettings.dateFormat=dateFormat->currentText();
  // Rig parameter
  LocalSettings.rigModelNumber=modelNr->text().toInt();
  LocalSettings.baudrate=baudRate->currentText().toInt();
  LocalSettings.handshake=static_cast<serial_handshake_e>(handShake->currentIndex());
  return LocalSettings;
}

void GeneralSettings::selectDemomode ( bool mode )
{
  if ( mode )
    {
      FileFormatLayout->show();
      SoundDeviceBox->hide();
    }
  else
    {
      FileFormatLayout->hide();
      SoundDeviceBox->show();
    }
}

void GeneralSettings::setControlDevice (QModelIndex index )
{
  selectedDevice=index;
  /**
  QString s = index.data().toString();

  pttDevice->clear();
  pttDevice->setText ( s );
  LocalSettings.SerialDevice = s;
  **/
  selectionMenu->popup(mapToGlobal( mapFromParent(QCursor::pos())));
}

void GeneralSettings::selectFileLogging ( bool b)
{
  Directory->setDisabled ( !b);
  QsoFile->setDisabled ( !b );
}
void GeneralSettings::selectLinLogLogging ( bool b )
{
  Port->setDisabled ( !b );
  Host->setDisabled ( !b );
}
void GeneralSettings::setSampleRate(QString s)
{
  LocalSettings.sampleRate=s.toInt();
}
void GeneralSettings::setComplexFormat(bool b)
{
  LocalSettings.complexFormat=b;
}
void GeneralSettings::setRigDevice()
{
  QString s = selectedDevice.data().toString();

  rigControl->clear();
  rigControl->setText ( s );
  LocalSettings.rigDevice=s;
  if(LocalSettings.SerialDevice == s)
    {
      LocalSettings.SerialDevice = "None";
      pttDevice->setText(LocalSettings.SerialDevice);
    }

}

void GeneralSettings::setPTTDevice()
{
  QString s = selectedDevice.data().toString();

  pttDevice->clear();
  pttDevice->setText ( s );
  LocalSettings.SerialDevice = s;
  if(LocalSettings.rigDevice == s)
    {
      LocalSettings.rigDevice="";
      rigControl->setText(LocalSettings.rigDevice);
    }
}
/**
void GeneralSettings::setRigNumber()
{
  QString s = modelNr->text();
  int ii=-1;
  ii=s.toInt();
  LocalSettings.rigModelNumber = ii;
}
**/
