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

#include "editmacro.h"
#include "ui_editmacro.h"

#include "readonlystringlistmodel.h"
#include <QMessageBox>

EditMacro::EditMacro(QVector<Macro> *macroList,QStringList tokenList, QWidget* parent, Qt::WindowFlags fl)
  : QDialog( parent, fl ), ui(new Ui::EditMacro)
{
  ui->setupUi(this);
  mL=macroList;
  int anzahl=macroList->size();
  model=new ReadOnlyStringListModel();
  model->setStringList(tokenList);
  ui->Keywords->setModel(model);

  ui->Position->setMaximum(anzahl);
  ui->SelectMacro->insertItem(0,"Choose macro");

  for(int i=0; i < anzahl; i++)
    ui->SelectMacro->insertItem(i+1,(*macroList).at(i).name);

  ui->bG->setId(ui->lang0,0);
  ui->bG->setId(ui->lang1,1);
  ui->bG->setId(ui->lang2,2);
}

EditMacro::~EditMacro()
{
  delete ui;
}

void EditMacro::accept()
{
  int aktPosition,newPosition;
  aktPosition=ui->SelectMacro->currentIndex()-1;
  if (aktPosition < 0)
    {
      QMessageBox::warning(this,"Edit macro","No macro selected \n Select correct macro",
        QMessageBox::Ok,QMessageBox::NoButton,QMessageBox::NoButton);
      return;
    }
  Macro macro;
  newPosition=ui->Position->value();
  macro=(*mL).at(aktPosition);
  macro.text=ui->Definition->toPlainText();
  macro.accelerator=ui->Accelerator->text();
  int langType=ui->bG->checkedId();
  if(macro.languageType < 0) //Deactivated
    macro.languageType = langType-3;
  if(aktPosition < newPosition) // Macro should be moved behind
    {
      mL->insert(newPosition,macro); //First insert then remove
      mL->remove(aktPosition);
    }
  else if (aktPosition > newPosition) // Macro should be moved forward
    {
      mL->remove(aktPosition); // First remove, then insert
      mL->insert(newPosition,macro);
    }
    else
      mL->replace(aktPosition, macro);
  QDialog::accept();
}

void EditMacro::setText( int Number)
{
  int langType;
if( Number > 0)
    {
      Number--;
      ui->Position->setValue(Number);
      ui->Definition->setText((*mL).at(Number).text);
      ui->Accelerator->setText((*mL).at(Number).accelerator);
      langType=(*mL).at(Number).languageType;
      if(langType <0 )
        langType +=3;
      if ((langType <0) || (langType > 2 ) ) //Check range if config file was wrongly modified
        langType=0;
      ui->bG->button(langType)->setChecked(true);
  }
}
void EditMacro::insertKeyword(QModelIndex index)
{
 QString s=index.data().toString();
 ui->Definition->insertPlainText(s);
 ui->Definition->setFocus();
}
