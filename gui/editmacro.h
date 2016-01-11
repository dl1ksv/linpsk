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

#ifndef EDITMACRO_H
#define EDITMACRO_H

#include <QDialog>
#include <QModelIndex>
#include <QStringList>

#include "constants.h"

namespace Ui {
  class EditMacro;
}
class ReadOnlyStringListModel;

class EditMacro : public QDialog
{
  Q_OBJECT

public:
  EditMacro(QVector<Macro> *macroList,QStringList tokenList,QWidget* parent = 0, Qt::WindowFlags fl = 0 );
  ~EditMacro();


public slots:

protected:

ReadOnlyStringListModel *model;

protected slots:
  virtual void accept();
void setText( int Number);
void insertKeyword(QModelIndex);

private:
  Ui::EditMacro *ui;
  QVector<Macro> *mL;
};

#endif

