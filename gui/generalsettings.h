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

#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QDialog>
#include "ui_generalsettings.h"
class Parameter;
class QButtonGroup;
class QModelIndex;
class GeneralSettings : public QDialog, private Ui::GeneralSettings
{
  Q_OBJECT

public:
  GeneralSettings(QWidget* parent = 0, Qt::WindowFlags fl = 0 );
  ~GeneralSettings();
Parameter getSettings();

public slots:
  /*$PUBLIC_SLOTS$*/

protected:
QButtonGroup *FileFormat;
  /*$PROTECTED_FUNCTIONS$*/
Parameter LocalSettings;
protected slots:
void selectDemomode(bool);
void setPTTDevice(const QModelIndex &);
void selectFileLogging(bool);
void selectLinLogLogging(bool);
void setSampleRate(QString s);
void setComplexFormat(bool);
};

#endif

