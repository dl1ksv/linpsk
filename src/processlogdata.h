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
#ifndef PROCESSLOGDATA_H
#define PROCESSLOGDATA_H

#include <QThread>
#include <QTcpSocket>
class QString;

class QLabel;
/**
Sends logdata to the linlogbook server and requests infos for a callsign from the linlogbook server

 @author Volker Schroer <dl1ksv@gmx.de>
*/
class ProcessLogData : public QThread
{
    Q_OBJECT
  public:
    ProcessLogData ( QObject *parent = 0 );

    ~ProcessLogData();
    void saveQsoData ( QString s );
    void requestCallSign ( QLabel **, QString s );
    void run();
  private:
    enum RequestType {Save = 0, Request};
    RequestType requestType;
    QString actionString;
    QTcpSocket *tcpSocket;
    QLabel * results[6];
    bool connectionEstablished;
    bool connectionError;
  private slots:
    void doAction();
    void connectionClosedbyHost();
    void readAnswer();
    void setConnected();
    void setError ( QAbstractSocket::SocketError );
signals:
void unabletoConnect();
void answerAvailable();
};

#endif
