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
#include "readonlystringlistmodel.h"
#include "rigcontrol.h"
#include <QButtonGroup>
#include <QDir>
#include <QMessageBox>
#include <QModelIndex>
#include <QString>
#include <QTextStream>


extern Parameter settings;

int register_callback (rig_caps const * caps, void * callback_data)
{
  QString key;
  GeneralSettings::Riglist *rigs=reinterpret_cast<GeneralSettings::Riglist *> (callback_data);
  if( RIG_MODEL_DUMMY == caps->rig_model)
    {
      (*rigs)["None"].model = 0;
      (*rigs)["None"].port_type = RIG_PORT_NONE;
    }
  else
    {
      if((caps->port_type == RIG_PORT_SERIAL) || (caps->port_type == RIG_PORT_NETWORK)
          || (caps->port_type == RIG_PORT_USB))
        {
          key=QString::fromLatin1 (caps->mfg_name).trimmed ()
                + ' '+ QString::fromLatin1 (caps->model_name).trimmed ();
          (*rigs)[key].model=caps->rig_model;
          (*rigs)[key].port_type=caps->port_type;
        }
    }
    return 1;
}
int unregister_callback (rig_caps const * caps, void *)
{
  rig_unregister (caps->rig_model);
  return 1;                   // keep them coming
}

GeneralSettings::GeneralSettings ( QWidget* parent, Qt::WindowFlags fl )
  : QDialog ( parent, fl ), Ui::GeneralSettings()
{
  setupUi ( this );
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
  //PTT and Rig
  // First look in the /dev Directory
  DirectoryName = "/dev/";

  dir.setPath ( DirectoryName );
  QStringList filenames;
  //Check for serial, usb to serial and bluetooth to serial devices
  filenames << "ttyS*" << "ttyUSB*" << "rfcomm*";
  QStringList Files = dir.entryList ( filenames, QDir::System | QDir::CaseSensitive, QDir::Name );

  for ( int kk = 0; kk < Files.size(); kk++ )
    Files.replace ( kk, DirectoryName + Files.at ( kk ) );

  // Rig
  rigPort->addItems(Files);
  rigPort->addItem(QLatin1String("None"));
  index=rigPort->findText(LocalSettings.config.rigPort);
  if(index >= 0)
    rigPort->setCurrentIndex(index);
  else
   {
    index=rigPort->count();
    rigPort->setCurrentIndex(index);
   }
  // Sound Devices
  QStringList cards=getSoundCards();
  soundInputDeviceName->addItems(cards);
//  soundInputDeviceName->addItem(QLatin1String("LinPSK_Record"));
  index=soundInputDeviceName->findText(LocalSettings.InputDeviceName);
  if(index >=0)
    soundInputDeviceName->setCurrentIndex(index);
  else
    {
      soundInputDeviceName->addItem(LocalSettings.InputDeviceName);
      qDebug("Index= %d",soundInputDeviceName->count());
      soundInputDeviceName->setCurrentIndex(soundInputDeviceName->count()-1);
    }
  soundOutputDeviceName->addItems(cards);
//  soundOutputDeviceName->addItem(QLatin1String("LinPSK_Play"));
  index=soundOutputDeviceName->findText(LocalSettings.OutputDeviceName);
  if(index >= 0)
    soundOutputDeviceName->setCurrentIndex(index);
  else
    {
      soundOutputDeviceName->addItem(LocalSettings.OutputDeviceName);
      soundOutputDeviceName->setCurrentIndex(soundInputDeviceName->count()-1);
    }
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

  rig_load_all_backends ();
  rig_list_foreach (register_callback, &riglist);

  for(auto r = riglist.cbegin();r != riglist.cend();++r)
    {
      if ( r.key() == "None")
        modelNr->insertItem(0,r.key());
      else
        modelNr->addItem(r.key());
    }
  index=modelNr->findText(LocalSettings.rig->rigKey());
  modelNr->setCurrentIndex(index);

  // Rig- Serial
  if (riglist[LocalSettings.rig->rigKey()].port_type ==
      RIG_PORT_SERIAL ) {
      portParameter->setEnabled(true);
      portText->setText("Serial port");
      switch (LocalSettings.config.handshake) {
        case RIG_HANDSHAKE_NONE:
          handshakeNone->setChecked(true);
        break;
        case RIG_HANDSHAKE_XONXOFF:
          handshakeSoft->setChecked(true);
        break;
      case RIG_HANDSHAKE_HARDWARE:
          handshakeHard->setChecked(true);
        break;
    }
    baudRate->setCurrentIndex(baudRate->findText(QString("%1").arg(LocalSettings.config.baudrate),Qt::MatchExactly));
  }
  else
    {
     portParameter->setEnabled(false);
     if (riglist[LocalSettings.rig->rigKey()].port_type == RIG_PORT_NETWORK )
          portText->setText("Network");
     else if (riglist[LocalSettings.rig->rigKey()].port_type == RIG_PORT_USB )
       portText->setText("USB");
    }
  // PTT
  pttDevice->addItems(Files);
  pttDevice->addItem(QLatin1String("None"));
  index=pttDevice->findText(LocalSettings.config.pttDevice);
  if(index >= 0)
    pttDevice->setCurrentIndex(index);
  else
   {
    index=pttDevice->count();
    pttDevice->setCurrentIndex(index);
   }
   switch (LocalSettings.config.ptt)  {
     case RIG_PTT_SERIAL_DTR:
       dtr->setChecked(true);
       break;
     case RIG_PTT_SERIAL_RTS:
       rts->setChecked(true);
       break;
     case RIG_PTT_RIG:
       catPTT->setChecked(true);
       break;
     case RIG_PTT_RIG_MICDATA:
       voxPTT->setChecked(true);
       break;
     default:
       break;
   }
}


GeneralSettings::~GeneralSettings()
{
  rig_list_foreach (unregister_callback, nullptr);
}

Parameter GeneralSettings::getSettings()
{
  LocalSettings.callsign = Callsign->text();
  LocalSettings.myLocator = myLocator->text();
  LocalSettings.InputDeviceName=soundInputDeviceName->currentText();
  LocalSettings.OutputDeviceName=soundOutputDeviceName->currentText();

  LocalSettings.timeoffset = UTC->value();
  LocalSettings.slashed0 = SlashedZero->isChecked();
  LocalSettings.autoCrLf = autoCrLf->isChecked();
  LocalSettings.autoDate=autoDate->isChecked();


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
  LocalSettings.config.rigPort=rigPort->currentText();
  LocalSettings.config.rigModelNumber = riglist[modelNr->currentText()].model;
  LocalSettings.config.baudrate=baudRate->currentText().toInt();
  if (handshakeHard->isChecked())
    LocalSettings.config.handshake=RIG_HANDSHAKE_HARDWARE;
  else if(handshakeSoft->isChecked())
    LocalSettings.config.handshake=RIG_HANDSHAKE_XONXOFF;
  else
    LocalSettings.config.handshake=RIG_HANDSHAKE_NONE;
  // PTT
  LocalSettings.config.pttDevice = pttDevice->currentText();
  if(dtr->isChecked())
    LocalSettings.config.ptt = RIG_PTT_SERIAL_DTR;
  else if (rts->isChecked())
    LocalSettings.config.ptt = RIG_PTT_SERIAL_RTS;
  else if ( catPTT->isChecked())
    LocalSettings.config.ptt = RIG_PTT_RIG;
  else if ( voxPTT->isChecked() )
    LocalSettings.config.ptt = RIG_PTT_RIG_MICDATA;
  else
    LocalSettings.config.ptt = RIG_PTT_NONE;

  return LocalSettings;
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

void GeneralSettings::accept()
{

  QString key = modelNr->currentText();
  if ( riglist[key].port_type ==  RIG_PORT_SERIAL) // As a serial port is required, check , if it's set
    {
      if ( rigPort->currentText() == QLatin1String("None") )
        {
          QMessageBox::warning(this,"Error !","Rig requires a serial port\nSelect a port",
            QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton) ;
          return;
        }
    }
  QDialog::accept();
}
void GeneralSettings::rigChanged(QString r)
{
  if (riglist[r].port_type ==
      RIG_PORT_SERIAL ) {
      portParameter->setEnabled(true);
      portText->setText("Serial port");
      rigPort->show();
      switch (LocalSettings.config.handshake) {
        case RIG_HANDSHAKE_NONE:
          handshakeNone->setChecked(true);
        break;
        case RIG_HANDSHAKE_XONXOFF:
          handshakeSoft->setChecked(true);
        break;
      case RIG_HANDSHAKE_HARDWARE:
          handshakeHard->setChecked(true);
        break;
    }
    baudRate->setCurrentIndex(baudRate->findText(QString("%1").arg(LocalSettings.config.baudrate),Qt::MatchExactly));
  }
  else
    {
     portParameter->setEnabled(false);
     if (riglist[r].port_type == RIG_PORT_NETWORK ) {
          portText->setText("Network");
          rigPort->hide();
       }
     else if (riglist[r].port_type == RIG_PORT_USB ) {
       portText->setText("USB");
       rigPort->hide();
       }
    }
}
