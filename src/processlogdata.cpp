/***************************************************************************
 *   Copyright (C) 2005 by Volker Schroer   *
 *   dl1ksv@gmx.de   *
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
#include "processlogdata.h"
#include <QString>
#include <QLabel>
#include <QHostAddress>
#include <QEventLoop>
#include <QMessageBox>
#include <QMetaType>

ProcessLogData::ProcessLogData ( QObject *parent )
    : QThread ( parent )
{
  tcpSocket = 0;
  connectionEstablished = false;
  connectionError = false;
}


ProcessLogData::~ProcessLogData()
{
}
void ProcessLogData::saveQsoData ( QString s )
{
  qDebug ( "Request save" );
  requestType = Save;
  actionString = s;
  doAction();
}
void ProcessLogData::requestCallSign ( QLabel **r, QString s )
{
  qDebug ( "Request Callsign" );
  requestType = Request;
  actionString.clear();
  actionString.append ( "@@@@" );
  actionString.append ( s );
  actionString.append ( "\r\n" );

  for ( int i = 0; i < 6;i++ )
    results[i] = r[i];
  doAction();
}
void ProcessLogData::run()
{
  connectionEstablished = false;
  connectionError = false;
  qRegisterMetaType<QAbstractSocket::SocketError> ( "QAbstractSocket::SocketError" );
  tcpSocket = new QTcpSocket();
  connect ( tcpSocket, SIGNAL ( disconnected() ), this, SLOT ( connectionClosedbyHost() ) );
  connect ( tcpSocket, SIGNAL ( readyRead() ), this, SLOT ( readAnswer() ) );
  connect ( tcpSocket, SIGNAL ( connected() ), this, SLOT ( setConnected() ) );
  connect ( tcpSocket, SIGNAL ( error ( QAbstractSocket::SocketError ) ), this, SLOT ( setError ( QAbstractSocket::SocketError ) ) );
  tcpSocket->connectToHost ( QHostAddress::LocalHost, 8080, QIODevice::ReadWrite );
  exec();
}
void ProcessLogData::doAction()
{
  if ( !connectionEstablished )
    usleep ( 6000 );
  while (tcpSocket == 0) // On heavy loaded systems it might last some times until socket is created
    usleep(6000);
  if ( !connectionEstablished && tcpSocket->state() != 3)
  {
 //   qDebug ( "Waiting for Socket timed out: %d",tcpSocket->state() );
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question ( 0, "LinPSK", tr ( "Cannot connect to LinLogBook\nTry again later ?" ), QMessageBox::Yes | QMessageBox::No );
    if ( reply == QMessageBox::No )
      emit unabletoConnect();
  if ( tcpSocket != 0 )
    delete tcpSocket;
  tcpSocket = 0;

    exit();
    return;
  }

//  qDebug ( "SocketError %d", tcpSocket->state() );
  if ( tcpSocket->state() == QAbstractSocket::UnconnectedState )
  {
    QMessageBox::information ( 0, "LinPSK", tr ( "Cannot connect to LinLogBook" ) );
    return;
  }
  int n = tcpSocket->write ( actionString.toLatin1(), actionString.length() );
//  qDebug ( "Written %d, to be written %d", n, actionString.length() );
  if ( n < 0 ) // Retry
  {
    usleep ( 100 );
    n = tcpSocket->write ( actionString.toLatin1(), actionString.length() );
//    qDebug ( "Written %d, to be written %d", n, actionString.length() );
  }
 /** qt 4.7 lets flush write data again*/
//  tcpSocket->flush();
}
void ProcessLogData::connectionClosedbyHost()
{
  connectionEstablished = false;
  if ( tcpSocket != 0 )
    delete tcpSocket;
  tcpSocket = 0;
  quit();

}

void ProcessLogData::readAnswer()
{
//  qDebug ( "Read Answer" );
//Has to be improved, to get safer
  QString s;
  int i;
  for ( i = 0; i < 6; i++ )
  {
    while ( ! tcpSocket->canReadLine() )
      usleep ( 100 );
    s = QLatin1String ( tcpSocket->readLine() );
    s.remove ( QLatin1Char ( '\n' ) );
    results[i]->setText ( s );
    results[i]->show();
  }
//  qDebug ( "%d Zeilen gelesen", i );
emit answerAvailable();
}
void ProcessLogData::setConnected()
{
  connectionEstablished = true;
}
void ProcessLogData::setError ( QAbstractSocket::SocketError  )
{
  connectionEstablished = false;
  connectionError = true;
//  if ( tcpSocket != 0 )
//    qDebug ( "SocketError %d", tcpSocket->state() );
}


