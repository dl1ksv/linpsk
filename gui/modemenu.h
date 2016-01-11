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

#ifndef MODEMENU_H
#define MODEMENU_H

#include <QDialog>
#include "ui_modemenu.h"
#include "constants.h"

class QButtonGroup;

class ModeMenu : public QDialog, private Ui::ModeMenu
{
  Q_OBJECT

public:
  ModeMenu(QWidget* parent = 0, Qt::WindowFlags fl = 0 );
  ~ModeMenu();
  /*$PUBLIC_FUNCTIONS$*/
void setParameter(ExtraParameter Param);
ExtraParameter getParameter();
Mode selectedMode();
public slots:
  /*$PUBLIC_SLOTS$*/

protected:

QButtonGroup *Stopbits;
QButtonGroup *Parity;
protected slots:

void changeView();
};

#endif

