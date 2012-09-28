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


#include "addmacro.h"
#include "readonlystringlistmodel.h"
#include <QMessageBox>
AddMacro::AddMacro(Macros *k,QWidget* parent, Qt::WFlags fl)
: QDialog( parent, fl ), Ui::AddMacro()
{
	setupUi(this);
ReadOnlyStringListModel *model= new ReadOnlyStringListModel();
model->setStringList(k->getKeyWordList());
KeywordDisplay->setModel(model);
connect(KeywordDisplay,SIGNAL(clicked(const QModelIndex &)),this,SLOT(insertKeyword(const QModelIndex &)));
model= new ReadOnlyStringListModel();
model->setStringList(k->getMacroList());
MacroDisplay->setModel(model);
Position->setMaximum(k->getMacroList().size()+1);
Position->setValue(k->getMacroList().size()+1);

}

AddMacro::~AddMacro()
{
}
void AddMacro::accept()
{
if (MacroName->text().length()== 0)
  {
    QMessageBox::warning(this,"Incomplete Macro Definition","Name of Macro is missing \n Enter Name of Macro",
      QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    return;  
  }
if (MacroDefinition->toPlainText().length()== 0)
  {
    QMessageBox::warning(this,"Incomplete Macro Definition","Macrodefinition is missing \n Enter  Macrodefinition",
      QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    return;
  }
int anzahl=MacroDefinition->toPlainText().count(QLatin1Char('@'));

 if (( anzahl > 0) && ((anzahl/2)*2 != anzahl) )
	{
    QMessageBox::warning(this,"Error in Macro Definition","Error in Macrodefinition. Incorrect number of @  \n Enter  Macrodefinition",
      QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    return;
  }
  QDialog::accept();
}

void AddMacro::insertKeyword(const QModelIndex &index)
{
 QString s=index.data().toString();
 MacroDefinition->insertPlainText(s);
 MacroDefinition->setFocus();
}

QString AddMacro::macroName()
{
 return MacroName->text();
}
QString AddMacro::macroDefinition()
{
 return MacroDefinition->toPlainText();
}
int AddMacro::position()
{
 return Position->value();
}
QString AddMacro::accelerator()
{
 return Accelerator->text();
}
