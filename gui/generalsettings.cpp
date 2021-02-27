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
#include <QTextStream>

#ifdef WITH_HAMLIB
#include <hamlib/rig.h>
#endif

extern Parameter settings;
GeneralSettings::GeneralSettings ( QWidget* parent)
  : QDialog ( parent), Ui::GeneralSettings()
{
  setupUi ( this );
#ifndef WITH_HAMLIB
  Rignumber->hide();
  modelNr->hide();
  commParams->hide();
  Rigdevice->hide();
  rigControl->hide();
#endif
  QString DirectoryName;
  QDir dir;
  QString s;
  int index;
  LocalSettings = settings;
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

  if ( Demomode->isChecked() )
    selectDemomode(true);
  else
    selectDemomode(false);

  //PTT and Rig
  // First look in the /dev Directory
  DirectoryName = "/dev/";

  dir.setPath ( DirectoryName );
  QStringList filenames;
  //Check for serial and usb to serial devices
  filenames << "ttyS*" << "ttyUSB*";
  QStringList Files = dir.entryList ( filenames, QDir::System | QDir::CaseSensitive, QDir::Name );

  for ( int kk = 0; kk < Files.size(); kk++ )
    Files.replace ( kk, DirectoryName + Files.at ( kk ) );
  // PTT
  pttDevice->addItem(QLatin1String("None"));
  pttDevice->addItems(Files);
  index=pttDevice->findText(LocalSettings.SerialDevice);
  if(index > 0)
    pttDevice->setCurrentIndex(index);
  else
    pttDevice->setCurrentIndex(0);
  // Rig
  rigControl->addItem(QLatin1String("None"));
  rigControl->addItems(Files);
  index=rigControl->findText(LocalSettings.rigDevice);
  if(index > 0)
    rigControl->setCurrentIndex(index);
  else
    rigControl->setCurrentIndex(0);
  // Sound Devices
  QStringList cards=getSoundCards();
  soundInputDeviceName->addItems(cards);
  soundInputDeviceName->addItem(QLatin1String("LinPSK_Record"));
  index=soundInputDeviceName->findText(LocalSettings.InputDeviceName);
  if(index >=0)
    soundInputDeviceName->setCurrentIndex(index);
  soundOutputDeviceName->addItems(cards);
  soundOutputDeviceName->addItem(QLatin1String("LinPSK_Play"));
  index=soundOutputDeviceName->findText(LocalSettings.OutputDeviceName);
  if(index >= 0)
    soundOutputDeviceName->setCurrentIndex(index);
  //Logging
  Directory->setText ( LocalSettings.Directory );
  QsoFile->setText ( LocalSettings.QSOFileName );
  fileLog->setChecked ( LocalSettings.fileLog );
  Directory->setDisabled ( !LocalSettings.fileLog );
  QsoFile->setDisabled ( !LocalSettings.fileLog );
  LinLog->setChecked ( LocalSettings.LinLog );
  Port->setDisabled ( !LocalSettings.LinLog );
  Host->setDisabled ( !LocalSettings.LinLog );

  // Rig number
  modelNr->setText(QString("%1").arg(LocalSettings.rigModelNumber));



  // Rig- Serial
  handShake->setCurrentIndex(LocalSettings.handshake);
  baudRate->setCurrentIndex(baudRate->findText(QString("%1").arg(LocalSettings.baudrate),Qt::MatchExactly));

}


GeneralSettings::~GeneralSettings()
{}



Parameter GeneralSettings::getSettings()
{
  LocalSettings.callsign = Callsign->text();
  LocalSettings.myLocator = myLocator->text();
  if ( Demomode->isChecked() )
    {
      LocalSettings.DemoMode = true;
      LocalSettings.inputFilename = "";
    }
  else
    {
      LocalSettings.DemoMode = false;
      LocalSettings.InputDeviceName=soundInputDeviceName->currentText();
      LocalSettings.OutputDeviceName=soundOutputDeviceName->currentText();
    }

  LocalSettings.timeoffset = UTC->value();
  LocalSettings.slashed0 = SlashedZero->isChecked();
  LocalSettings.autoCrLf = autoCrLf->isChecked();
  LocalSettings.autoDate=autoDate->isChecked();
  LocalSettings.SerialDevice = pttDevice->currentText();
  LocalSettings.rigDevice=rigControl->currentText();
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
#ifdef WITH_HAMLIB
  LocalSettings.rigModelNumber=modelNr->text().toInt();
  LocalSettings.baudrate=baudRate->currentText().toInt();
  LocalSettings.handshake=static_cast<serial_handshake_e>(handShake->currentIndex());
#else
  LocalSettings.rigModelNumber=0;
#endif
  return LocalSettings;
}

void GeneralSettings::selectDemomode ( bool mode )
{
  if ( mode )
    SoundDeviceBox->hide();
  else
    SoundDeviceBox->show();
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
QStringList GeneralSettings::getSoundCards()
{
  QString line;
  QFile cards( "/proc/asound/cards" );
  QStringList cardList;
  int pos;
  cardList << "None";
  if (!cards.open(QIODevice::ReadOnly | QIODevice::Text))
          return cardList;

  QTextStream in(&cards);
  cardList.clear();
  do {
      line = in.readLine();
      pos=line.indexOf(QLatin1String(": "));
      if(pos > 0)
        cardList << line.mid(pos+1);
  } while (!in.atEnd());
  if(cardList.isEmpty())
    cardList << "None";
  cards.close();
  return cardList;
  }

