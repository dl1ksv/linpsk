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


#include "qsodata.h"
#include "parameter.h"
#include "crxchannel.h"
#include "processlogdata.h"
#include "constants.h"
#ifdef WITH_HAMLIB
#include "rigcontrol.h"
#endif

#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QValidator>
#include <QMessageBox>
#include <QTcpSocket>

extern Parameter settings;

QSOData::QSOData ( QWidget* parent )
    : QGroupBox ( parent ), Ui::QSOData()
{
  int i;
  setupUi ( this );
  QRegExp rx ( "^[A-R][A-R][0-9][0-9][A-X][A-X]$" );
  validator = new QRegExpValidator ( rx, this );
  Loc->setValidator ( validator );
  Loc->setStyleSheet ( "QLineEdit{color: black ; }" );
  QsoDate->setDisplayFormat ( settings.dateFormat );
  QsoTime->setDisplayFormat(QString("hh:mm"));
  if ( settings.QslData )
  {
    RemoteCallsign->setText ( settings.QslData->RemoteCallsign );
    OpName->setText ( settings.QslData->OpName );
    Qth->setText ( settings.QslData->Qth );
    Loc->setText ( settings.QslData->Locator );
    HisRST->setText ( settings.QslData->HisRST );
    MyRST->setText ( settings.QslData->MyRST );
    dokName->setText(settings.QslData->dokName);
  }
  logBookCommunication = new ProcessLogData();
  connectionError = false;
  connect ( Clear, SIGNAL ( clicked() ), this, SLOT ( clear() ) );
  connect ( Qth, SIGNAL ( editingFinished () ), this, SLOT ( QTHchanged() ) );
  connect ( QsoFrequency, SIGNAL ( activated (int) ), this, SLOT ( frequencyChanged(int) ) );
  connect ( txPwr,SIGNAL( valueChanged(int) ), this ,SLOT( pwrChanged(int) ) );
  connect ( MyRST, SIGNAL ( editingFinished () ), this, SLOT ( MyRSTchanged() ) );
  connect ( OpName, SIGNAL ( editingFinished () ), this, SLOT ( Namechanged() ) );
  connect ( Save, SIGNAL ( clicked() ), this, SLOT ( save() ) );
  connect ( Loc, SIGNAL ( editingFinished () ), this, SLOT ( Locatorchanged() ) );
  connect ( HisRST, SIGNAL ( editingFinished () ), this, SLOT ( HisRSTchanged() ) );
  connect ( QsoDate, SIGNAL ( dateChanged ( const QDate & ) ), this, SLOT ( Datechanged() ) );
  connect ( QsoTime, SIGNAL ( timeChanged ( const QTime & ) ), this, SLOT ( Timechanged() ) );
  connect ( dokName, SIGNAL ( editingFinished()),this ,SLOT(dokChanged()));

  connect ( RemoteCallsign, SIGNAL ( editingFinished ( ) ), this, SLOT ( sendRequest() ) );
  connect ( logBookCommunication, SIGNAL ( unabletoConnect() ), this , SLOT ( stopTrial() ) );
  connect ( logBookCommunication, SIGNAL ( answerAvailable() ), this, SLOT ( copyAnswer() ) );
  //====== Control the rig ================================== //
  for(i=0; i< settings.bandList.size(); i++)
    QsoFrequency->addItem(settings.bandList.at(i).bandName);
//  QsoFrequency->setCurrentIndex(findBand());

//  txPwr->setValue(rigControl->get_pwr());

}

QSOData::~QSOData()
{}
void QSOData::enableSaveData()
{
  if ( ! ( settings.fileLog || settings.LinLog ) )
    Save->setDisabled ( true );
  else
    Save->setDisabled(false);
}
void QSOData::clear()
{
  RemoteCallsign->setText ( "" );
  settings.QslData->RemoteCallsign = RemoteCallsign->text();
  OpName->setText ( "" );
  settings.QslData->OpName = OpName->text();
  Qth->setText ( "" );
  settings.QslData->Qth = Qth->text();
  Loc->setText ( "" );
  settings.QslData->Locator = Loc->text();
  QsoFrequency->setCurrentIndex(findBand());
#ifdef WITH_HAMLIB
  settings.pwr=settings.rigctl->get_pwr();
  txPwr->setValue(settings.pwr);
#endif
  settings.QslData->Locator = Loc->text();
  HisRST->setText ( "" );
  settings.QslData->HisRST = HisRST->text();
  MyRST->setText ( "" );
  settings.QslData->MyRST = MyRST->text();
  Distance->setText ( "" );
  continent->setText ( "" );
  settings.QslData->continent = continent->text();
  wazZone->setText ( "" );
  settings.QslData->wazZone = wazZone->text();
  ituZone->setText ( "" );
  settings.QslData->ituZone = ituZone->text();
  countryName->setText ( "" );
  settings.QslData->countryName = countryName->text();
  worked->setText ( "" );
  settings.QslData->worked = worked->text();
  mainPrefix->setText ( "" );
  settings.QslData->mainPrefix = mainPrefix->text();
  dokName->clear();
  eQsl->setChecked(false);
  bureau->setChecked(false);
  refreshDateTime();
}

void QSOData::Callsignchanged()
{
  settings.QslData->RemoteCallsign = RemoteCallsign->text();
  RemoteCallsign->setModified ( true );
  sendRequest();
}

void QSOData::Namechanged()
{
  settings.QslData->OpName = OpName->text();
}

void QSOData::QTHchanged()
{
  settings.QslData->Qth = Qth->text();

}

void QSOData::Locatorchanged()
{
   int i=0;
  QString toCheck= Loc->text();
  QValidator::State status=validator->validate(toCheck,i);
  if(status==QValidator::Invalid)
    Loc->setStyleSheet ( "QLineEdit{color: red ; }" );
  else
  {
    Loc->setStyleSheet ( "QLineEdit{color: black ; }" );
    settings.QslData->Locator = toCheck;
    calculateDistance ( toCheck );
  }
}

void QSOData::frequencyChanged(int index)
{
#ifdef WITH_HAMLIB
  settings.rigctl->set_frequency(settings.bandList.at(index).preferedFreq);
#endif
  settings.QsoFrequency = index;
}

void QSOData::HisRSTchanged()
{
  settings.QslData->HisRST = HisRST->text();
}

void QSOData::MyRSTchanged()
{
  settings.QslData->MyRST = MyRST->text();
}

void QSOData::Datechanged()
{
  settings.QslData->QsoDate = QsoDate->date();
}

void QSOData::Timechanged()
{
  settings.QslData->QsoTime = QsoTime->time();
}
void QSOData::dokChanged()
{
  settings.QslData->dokName = dokName->text();
}
void QSOData::refreshDateTime()
{
  QDateTime t1;
  QDateTime t;
  t = QDateTime::currentDateTime();

  t1 = t.addSecs ( settings.timeoffset * 3600 );
  QsoDate->setDisplayFormat(settings.dateFormat);
  QsoDate->setDate ( t.date() );
  QsoTime->setTime ( t1.time() );
  if ( settings.QslData )
  {
    settings.QslData->QsoTime = QsoTime->time();
    settings.QslData->QsoDate = QsoDate->date();
  }
}
void QSOData::save()
{
  QMessageBox msgBox;
  if ( ( ( settings.QSOFileName == "" ) && ( !settings.LinLog ) ) || connectionError )
    return ; // No Filename specified, and no LinLog logging required
// Get the most recent values
  settings.QslData->RemoteCallsign = RemoteCallsign->text();
  Namechanged(); // Later ?, not saved at the moment
  QTHchanged();
  Locatorchanged();
  HisRSTchanged();
  MyRSTchanged();
  if(settings.autoDate)
    refreshDateTime();
  Datechanged();
  Timechanged();
  QString saveString, s;

  if ( RemoteCallsign->text().length() > 0 )
    saveString = QString ( "<CALL:%1>%2\n" ).arg ( RemoteCallsign->text().length() ).arg ( RemoteCallsign->text() );

  if ( settings.callsign != "" )
  {
    s = QString ( "<OPERATOR:%1>%2\n" ).arg ( settings.callsign.length() ).arg ( settings.callsign );
    saveString.append ( s );
  }
  if ( !OpName->text().isEmpty() )
  {
    s = QString ( "<NAME:%1>%2\n" ).arg ( OpName->text().length() ).arg ( OpName->text() );
    saveString.append ( s );
  }
  if ( Qth->text() != "" )
  {
    s = QString ( "<QTH:%1>%2\n" ).arg ( Qth->text().length() ).arg ( Qth->text() );
    saveString.append ( s );
  }
  if ( Loc->text() != "" )
  {
    s = QString ( "<GRIDSQUARE:%1>%2\n" ).arg ( Loc->text().length() ).arg ( Loc->text() );
    saveString.append ( s );
  }
  if ( QsoFrequency->currentText() != "" )
  {
    s = QString ( "<BAND:%1>%2\n" ).arg ( QsoFrequency->currentText().length() ).arg ( QsoFrequency->currentText() );
    saveString.append ( s );
    float freq = settings.bandList[QsoFrequency->currentIndex()].preferedFreq/1000000.;
    if( freq > 0 )
    {
        s=QString("%1").arg(freq,0,'f',3);
        saveString.append (QString("<FREQ:%1>%2\n").arg(s.length()).arg(s));
    }
  }
  // TxPwr
  s.setNum(txPwr->value());
  s = QString ("<TX_PWR:%1>%2\n").arg(s.length()).arg(s);
  saveString.append(s);
  if ( QsoDate->text() == "" )
  {
    s = QString ( "<QSO_DATE:8:d>%1\n" ).arg ( QDateTime::currentDateTime().toString ( "yyyyMMdd" ) );
    saveString.append ( s );
  }
  else
  {
    s = QString ( "<QSO_DATE:8:d>%1\n" ).arg ( QsoDate->date().toString ( "yyyyMMdd" ) );
    saveString.append ( s );
  }
  if ( QsoTime->text() != "" )
  {
    QString timeValue= QsoTime->text().remove ( ':' );
    s = QString ( "<TIME_ON:%1>%2\n" ).arg ( timeValue.length() ).arg ( timeValue );
    saveString.append ( s );
  }

  if ( HisRST->text() != "" )
  {
    s = QString ( "<RST_RCVD:%1>%2\n" ).arg ( HisRST->text().length() ).arg ( HisRST->text() );
    saveString.append ( s );
  }
  if ( MyRST->text() != "" )
  {
    s = QString ( "<RST_SENT:%1>%2\n" ).arg ( MyRST->text().length() ).arg ( MyRST->text() );
    saveString.append ( s );
  }
  if ( settings.ActChannel != 0 )
  {
    s = QString ( "<MODE:" );
    switch ( settings.ActChannel->getModulationType() )
    {
      case QPSK:
        s.append( "6>QPSK31");
        break;
      case BPSK31:
        s.append ( "5>PSK31\n" );
        break;
      case BPSK63:
        s.append ( "5>PSK63\n" );
        break;
      case RTTY:
        s.append ( "4>RTTY\n" );
        break;

      case MFSK16:
        s.append ( "6>MFSK16\n" );
        break;

      default:
        s.append ( "5>PSK31\n" );
    }
    saveString.append ( s );
  }
  if(dokName->text() !="")
   saveString.append(QString("<APP_LinLog_DOK:%1>%2\n").arg(dokName->text().length()).arg(dokName->text()));
  saveString.append(QString("<EOR>"));
  if ( settings.fileLog )
  {
    QDir d;
    if ( !d.cd ( settings.Directory ) )
      d.mkdir ( settings.Directory );
    QFile f ( settings.Directory + "/" + settings.QSOFileName );
    if ( !f.open ( QIODevice::WriteOnly | QIODevice::Append ) )
    {
      msgBox.setText("Could not open file " + settings.QSOFileName );
      msgBox.exec();
      return;
    }
    QTextStream stream ( &f );
    if( f.size() == 0) //Empty file, write preamble
      {
        stream << "ADIF export from  " << settings.callsign << Qt::endl;
        stream << "Generated on " << QDateTime::currentDateTime().toUTC().toString() << " UTC" << Qt::endl;
        stream << "<ADIF_Ver:5>3.1.1" << Qt::endl;
        stream << "<PROGRAMID:6>LinPSK" << Qt::endl;
        stream  << "<eoh>" << Qt::endl;
      }
    stream << saveString  << Qt::endl;
    f.close();

    msgBox.setText("Saved qso with " +  RemoteCallsign->text());
    msgBox.exec();
  }
  if ( settings.LinLog )
  {

    saveString.append("<QSL_SENT:1>");
    if(bureau->isChecked())
       saveString.append("R\n");
    else
       saveString.append("N\n");
    saveString.append("<QSL_RCVD:1>");
    if(bureau->isChecked())
       saveString.append("R\n");
    else
       saveString.append("N\n");
    saveString.append("<EQSL_QSL_SENT:1>");
    if(eQsl->isChecked())
       saveString.append("R\n");
     else
       saveString.append("N\n");
    saveString.append("<EQSL_QSL_RCVD:1>");
    if(eQsl->isChecked())
       saveString.append("R\n");
    else
    saveString.append("N\n");
    saveString.append ( "<eor>\n" );
    if ( !logBookCommunication->isRunning() )
      {
	logBookCommunication->start();
#if QT_VERSION >= 0x050000
        QThread::usleep(2000);
#endif
      }
#ifndef QT_NO_DEBUG
    qDebug ( "Written to Logbook\n%s", qPrintable ( saveString ) );
#endif
    logBookCommunication->saveQsoData ( saveString );
  }
}

void QSOData::calculateDistance ( QString loc )
{
  coordinates mine, his;
  double dist;
  if ( settings.myLocator.isEmpty() )
  {
    Distance->setText ( "" );
    return;
  }
  if ( loc.length() != 6 )
    return;
  QString s ( "%1 km" );
  mine = loc2coordinates ( settings.myLocator.constData() );
  his = loc2coordinates ( loc.constData() );
  dist = 6371. * acos ( sin ( mine.breite ) * sin ( his.breite ) + cos ( mine.breite ) * cos ( his.breite ) * cos ( mine.laenge - his.laenge ) );
  Distance->setText ( s.arg ( ( int ) dist, 5 ) );
}
QSOData::coordinates QSOData::loc2coordinates ( const QChar *l )
{
  coordinates c;
  c.laenge = ( 20. * ( l[0].toLatin1() - 'A' ) + 2. * ( l[2].toLatin1() - '0' ) + ( 1. / 12. ) * ( l[4].toLatin1() - 'A' ) + 1. / 24. );
  c.laenge *= M_PI / 180.;
  c.breite = 90. - ( 10. * ( l[1].toLatin1() - 'A' ) + 1. * ( l[3].toLatin1() - '0' ) + ( 1. / 24. ) * ( l[5].toLatin1() - 'A' ) + 1. / 48. );
  c.breite *= M_PI / 180.;
  return c;
}

void QSOData::setQsoData(QsoData value,QString s)
{

  switch (value)
  {
    case CallSign :
      RemoteCallsign->setText ( s.remove(QChar(' ')) );
      Callsignchanged();
      break;
    case QTH :
      Qth->setText ( s );
      QTHchanged();
      break;
    case Name:
      OpName->setText ( s );
      Namechanged();
      break;
    case Locator:
      Loc->setText ( s.left(6).toUpper());
      Locatorchanged();
      break;
    case RST:
      HisRST->setText ( s );
      HisRSTchanged();
      break;
    case DOK :
      dokName->setText(s);
      dokChanged();
      break;
  }
 }

void QSOData::newChannel()
{
  Distance->setText ( "" );
  if ( settings.QslData )
  {
    RemoteCallsign->setText ( settings.QslData->RemoteCallsign );
    OpName->setText ( settings.QslData->OpName );
    Qth->setText ( settings.QslData->Qth );
    Loc->setText ( settings.QslData->Locator );
    Locatorchanged(); // Check Locator and set Color
    HisRST->setText ( settings.QslData->HisRST );
    MyRST->setText ( settings.QslData->MyRST );
    QsoDate->setDate ( settings.QslData->QsoDate );
    QsoTime->setTime ( settings.QslData->QsoTime );
    mainPrefix->setText ( settings.QslData->mainPrefix );
    continent->setText ( settings.QslData->continent );
    wazZone->setText ( settings.QslData->wazZone );
    ituZone->setText ( settings.QslData->ituZone );
    countryName->setText ( settings.QslData->countryName );
    worked->setText ( settings.QslData->worked );
  }
  else
    clear();
}

void QSOData::sendRequest()
{
  bool test;
  test = RemoteCallsign->isModified();
  if ( test )
  {
    RemoteCallsign->setModified ( false ); //To avoid calling twice
    settings.QslData->RemoteCallsign = RemoteCallsign->text();
  }
  if ( !settings.LinLog || connectionError || !test ) // No request to LinLogbook if disabled in the settings
    return;
//  qDebug ( "Trying to request %s", qPrintable ( RemoteCallsign->text().toUpper() ) );
  QLabel *results[6];
  results[0] = mainPrefix;
  results[1] = wazZone;
  results[2] = ituZone;
  results[3] = countryName;
  results[4] = continent;
  results[5] = worked;
  if ( !logBookCommunication->isRunning() )
    {
      logBookCommunication->start();
#if QT_VERSION >= 0x050000
      QThread::usleep(2000);
#endif
    }
  logBookCommunication->requestCallsign ( results, RemoteCallsign->text().toUpper() );

}
void QSOData::stopTrial()
{
  connectionError = true;
}
void QSOData::copyAnswer()
{
  settings.QslData->mainPrefix = mainPrefix->text();
  settings.QslData->wazZone = wazZone->text();
  settings.QslData->ituZone = ituZone->text();
  settings.QslData->countryName = countryName->text();
  settings.QslData->continent = continent->text();
  settings.QslData->worked = worked->text();
}
void QSOData::setAutoDate()
{
  if(settings.autoDate)
    {
      dateLabel->hide();
      QsoDate->hide();
      timeLabel->hide();
      QsoTime->hide();
    }
  else
    {
      dateLabel->show();
      QsoDate->show();
      timeLabel->show();
      QsoTime->show();
    }
}
void QSOData::pwrChanged(int p)
{
#ifdef WITH_HAMLIB
  settings.rigctl->set_pwr(p);
#endif
  settings.pwr=p;
}
int QSOData::findBand()
{
#ifdef WITH_HAMLIB
  int freq = settings.rigctl->get_frequency();
  for(int i=0; i < settings.bandList.size();i++)
    {
      if( (settings.bandList.at(i).bandStart <= freq) && (settings.bandList.at(i).bandEnd >= freq))
          return i;
    }
#endif
  return settings.bandList.size()-1;
}
void QSOData::initQsoData()
{
  QsoFrequency->clear();
  for(int i=0; i< settings.bandList.size(); i++)
    QsoFrequency->addItem(settings.bandList.at(i).bandName);
  QsoFrequency->setCurrentIndex(findBand());
#ifdef WITH_HAMLIB
  settings.pwr=settings.rigctl->get_pwr();
#endif
  txPwr->setValue(settings.pwr);

}
