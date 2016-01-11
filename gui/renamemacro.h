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
#include "constants.h"


namespace Ui {
  class RenameMacro;
}
class ReadOnlyStringListModel;

class RenameMacro : public QDialog
{
  Q_OBJECT

public:
  RenameMacro(QVector<Macro> *macroList,QWidget* parent = 0, Qt::WindowFlags fl = 0 );
  ~RenameMacro();
  int getMacroNumber();


protected:
  ReadOnlyStringListModel *model;

  int macroNumber;
protected slots:
  /*$PROTECTED_SLOTS$*/
  virtual void          accept();
void selectMacro(const QModelIndex &);
private:
  Ui::RenameMacro *ui;
  QVector<Macro> *mL;
};

#endif

