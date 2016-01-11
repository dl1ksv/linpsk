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
  connect ( Demomode, SIGNAL ( clicked ( bool ) ), this, SLOT ( selectDemomode ( bool ) ) );
  connect ( AvailableDevices, SIGNAL ( clicked ( const QModelIndex & ) ), this, SLOT ( setPTTDevice ( const QModelIndex & ) ) );

  if ( Demomode->isChecked() )
    selectDemomode(true);
  else
    selectDemomode(false);

  //PTT
  SelectedDevice->setText ( LocalSettings.SerialDevice );
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
  connect(fileLog,SIGNAL(clicked(bool)),this,SLOT(selectFileLogging(bool)));
  Directory->setDisabled ( !LocalSettings.fileLog );
  QsoFile->setDisabled ( !LocalSettings.fileLog );
  LinLog->setChecked ( LocalSettings.LinLog );
  connect(LinLog,SIGNAL(clicked(bool)),this,SLOT(selectLinLogLogging(bool)));
  Port->setDisabled ( !LocalSettings.LinLog );
  Host->setDisabled ( !LocalSettings.LinLog );

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
  LocalSettings.SerialDevice = SelectedDevice->text();
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

void GeneralSettings::setPTTDevice ( const QModelIndex &index )
{
  QString s = index.data().toString();

  SelectedDevice->clear();
  SelectedDevice->setText ( s );
  LocalSettings.SerialDevice = s;
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
