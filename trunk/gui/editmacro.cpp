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


#include "editmacro.h"
#include "macros.h"
#include "readonlystringlistmodel.h"

EditMacro::EditMacro(Macros *M,QWidget* parent, Qt::WFlags fl)
: QDialog( parent, fl ), Ui::EditMacro()
{
	setupUi(this);
AllMacros=M;
model=new ReadOnlyStringListModel();
model->setStringList(M->getKeyWordList());
Keywords->setModel(model);
int anzahl=AllMacros->count();

Position->setMaximum(anzahl);
SelectMacro->insertItem(0," ");
connect(SelectMacro,SIGNAL(activated(int)),this,SLOT(setText(int)));
connect(Keywords,SIGNAL(activated(const QModelIndex &)),this,SLOT(insertKeyword(const QModelIndex &)));
 for(int i=0; i < anzahl; i++)
  SelectMacro->insertItem(i+1,AllMacros->getMacroName(i));
}

EditMacro::~EditMacro()
{
}

void EditMacro::accept()
{
int AktPosition;
AktPosition=SelectMacro->currentIndex()-1;
AllMacros->setDefinition(Definition->toPlainText(),AktPosition);
AllMacros->setAccelerator(Accelerator->text(),AktPosition);
  QDialog::accept();
}

void EditMacro::setText( int Number)
{
if( Number > 0)
    { 
      Number--;
      Position->setValue(Number);
      Definition->setText(AllMacros->getDefinition(Number));
      Accelerator->setText(AllMacros->getAccelerator(Number));
  } 
}
void EditMacro::insertKeyword(const QModelIndex &index)
{
 QString s=index.data().toString();
 Definition->insertPlainText(s);
 Definition->setFocus();
}
