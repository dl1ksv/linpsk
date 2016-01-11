/***************************************************************************
 *   Copyright (C) 2012 by Volker Schroer, DL1KSV                          *
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

#ifndef DELETEMACRO_H
#define DELETEMACRO_H

#include <QDialog>

#include "constants.h"

namespace Ui {
  class DeleteMacro;
}
class ReadOnlyStringListModel;
class DeleteMacro : public QDialog
{
  Q_OBJECT

public:
  DeleteMacro(QVector<Macro> *macroList,QWidget* parent = 0, Qt::WindowFlags fl = 0 );
  ~DeleteMacro();

public slots:
  /*$PUBLIC_SLOTS$*/

protected:

ReadOnlyStringListModel *model;
ReadOnlyStringListModel *deleteList;

protected slots:
  virtual void          accept();
  void removefromList();
  void addtoList();

private:
  Ui::DeleteMacro *ui;
  QVector<Macro> *mL;
};

#endif

