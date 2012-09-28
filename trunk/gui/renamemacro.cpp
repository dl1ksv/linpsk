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
#include "macros.h"
#include "readonlystringlistmodel.h"
#include <QMessageBox>

RenameMacro::RenameMacro(Macros *M,QWidget* parent, Qt::WFlags fl)
: QDialog( parent, fl ), Ui::RenameMacro()
{
	setupUi(this);
model=new ReadOnlyStringListModel();
model->setStringList(M->getMacroList());
AllMacros=M;
MacroBox->setModel(model);
connect(MacroBox,SIGNAL(clicked(const QModelIndex &)),this,SLOT(selectMacro(const QModelIndex &)));
}

RenameMacro::~RenameMacro()
{
}


void RenameMacro::accept()
{
if (NewName->text().length() ==0 )
 {
  QMessageBox::warning(this,
  "Error", "New Name of Macro is missing. \n Enter new Name of  Macro",QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    return;
 }
else 
 AllMacros->setMacroName(NewName->text(),MacroNumber);
  QDialog::accept();
}

void RenameMacro::selectMacro(const QModelIndex &index)
{
MacroNumber=index.row();
OldName->setText(index.data().toString());
}

