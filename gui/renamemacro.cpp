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


#include "renamemacro.h"
#include "readonlystringlistmodel.h"
#include <QMessageBox>

RenameMacro::RenameMacro(QVector<Macro> *macroList,QWidget* parent, Qt::WindowFlags fl)
  : QDialog( parent, fl ), ui(new Ui:: RenameMacro())
{
  ui->setupUi(this);
  mL = macroList;
  model=new ReadOnlyStringListModel();
  QStringList macroName;
  int numberofMacros=macroList->size();
  if (numberofMacros > 0 )
  for(int i=0; i < numberofMacros;i++)
    macroName.append((*macroList).at(i).name);
  model->setStringList(macroName);
  ui->MacroBox->setModel(model);
  connect(ui->MacroBox,SIGNAL(clicked(const QModelIndex &)),this,SLOT(selectMacro(const QModelIndex &)));
  macroNumber=-1;
}

RenameMacro::~RenameMacro()
{
}


void RenameMacro::accept()
{
if (ui->NewName->text().length() ==0 )
 {
  QMessageBox::warning(this,
  "Error", "New Name of Macro is missing. \n Enter new Name of  Macro",QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    return;
 }
  Macro macro;
  macro=(*mL).at(macroNumber);
  macro.name=ui->NewName->text();
  mL->replace(macroNumber,macro);
  QDialog::accept();
}

void RenameMacro::selectMacro(const QModelIndex &index)
{
  macroNumber=index.row();
  ui->OldName->setText(index.data().toString());
}
int RenameMacro::getMacroNumber()
{
  return macroNumber;
}
