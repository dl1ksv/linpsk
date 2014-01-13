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

#ifndef QSODATA_H
#define QSODATA_H

#include <QGroupBox>
#include "ui_qsodata.h"
#include "constants.h"

class QChar;
class ProcessLogData;
class QValidator;
class QSOData : public QGroupBox, private Ui::QSOData
{
  Q_OBJECT
struct coordinates
{
        double laenge;
        double breite;
};

public:
  QSOData(QWidget* parent = 0);
  ~QSOData();
  void enableSaveData();
  void setAutoDate();

public slots:
  /*$PUBLIC_SLOTS$*/
  void Timechanged();
  void Datechanged();
  void MyRSTchanged();
  void HisRSTchanged();
  void frequencyChanged(int);
  void Locatorchanged();
  void QTHchanged();
  void Namechanged();
  void Callsignchanged();
  void clear();
  void refreshDateTime();
  void save();
  void calculateDistance(QString);
  void dokChanged();

  void setQsoData(QsoData,QString);
  void newChannel();


protected:
  coordinates loc2coordinates ( const QChar *l );

protected slots:
  void sendRequest();
  void stopTrial();
  void copyAnswer();
private:
  ProcessLogData *logBookCommunication;
  bool connectionError;
  QValidator *validator;
};

#endif

