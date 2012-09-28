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

#ifndef EDITMACRO_H
#define EDITMACRO_H

#include <QDialog>
#include "ui_editmacro.h"

class Macros;
class ReadOnlyStringListModel;


class EditMacro : public QDialog, private Ui::EditMacro
{
  Q_OBJECT

public:
  EditMacro(Macros *M,QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~EditMacro();


public slots:
  /*$PUBLIC_SLOTS$*/

protected:
Macros *AllMacros;
ReadOnlyStringListModel *model;

protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void accept();
void setText( int Number);
void insertKeyword(const QModelIndex &);
};

#endif

