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


#include "addmacro.h"
#include "readonlystringlistmodel.h"
#include "ui_addmacro.h"

#include <QMessageBox>
#include <QButtonGroup>

AddMacro::AddMacro(QVector<Macro> *macroList,QStringList tokenList, QWidget* parent, Qt::WindowFlags fl)
: QDialog( parent, fl ),  ui(new Ui::AddMacro)
{
  ui->setupUi(this);
  ui->bG->setId(ui->lang0,0);
  ui->bG->setId(ui->lang1,1);
  ui->bG->setId(ui->lang2,2);
  mL=macroList;
  ReadOnlyStringListModel *model= new ReadOnlyStringListModel();
  model->setStringList(tokenList);
  ui->KeywordDisplay->setModel(model);
  model= new ReadOnlyStringListModel();
  QStringList macroName;
  int numberofMacros=macroList->size();
  if (numberofMacros > 0 )
  for(int i=0; i < numberofMacros;i++)
    macroName.append((*macroList).at(i).name);
  model->setStringList(macroName);
  ui->MacroDisplay->setModel(model);
  ui->Position->setMaximum(numberofMacros);
  ui->Position->setValue(numberofMacros);

}

AddMacro::~AddMacro()
{
  delete ui;
}
void AddMacro::accept()
{
  int anzahl;
  if (ui->MacroName->text().length()== 0)
  {
    QMessageBox::warning(this,"Incomplete Macro Definition","Name of Macro is missing \n Enter Name of Macro",
      QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    return;
  }
  if (ui->MacroDefinition->toPlainText().length()== 0)
  {
    QMessageBox::warning(this,"Incomplete Macro Definition","Macrodefinition is missing \n Enter  Macrodefinition",
      QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    return;
  }
  anzahl=ui->MacroDefinition->toPlainText().count(QLatin1Char('@'));

  if (( anzahl > 0) && ((anzahl/2)*2 != anzahl) )
        {
    QMessageBox::warning(this,"Error in Macro Definition","Error in Macrodefinition. Incorrect number of @  \n Enter  Macrodefinition",
      QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
    return;
  }
  Macro macro;
  macro.name=ui->MacroName->text();
  macro.text=ui->MacroDefinition->toPlainText();
  macro.accelerator=ui->Accelerator->text();
  macro.languageType=ui-> bG->checkedId();
//  mL->append(macro);
  mL->insert(ui->Position->value(),macro);

  QDialog::accept();
}

void AddMacro::insertKeyword(QModelIndex index)
{
 QString s=index.data().toString();
 ui->MacroDefinition->insertPlainText(s);
 ui->MacroDefinition->setFocus();
}

