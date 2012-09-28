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

#ifndef RENAMEMACRO_H
#define RENAMEMACRO_H

#include <QDialog>
#include "ui_renamemacro.h"

class Macros;
class ReadOnlyStringListModel;

class RenameMacro : public QDialog, private Ui::RenameMacro
{
  Q_OBJECT

public:
  RenameMacro(Macros *M,QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~RenameMacro();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
  /*$PUBLIC_SLOTS$*/

protected:
ReadOnlyStringListModel *model;
Macros *AllMacros;
int MacroNumber;
protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void          accept();
void selectMacro(const QModelIndex &);
};

#endif

