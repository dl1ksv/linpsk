/***************************************************************************
 *   Copyright (C) 2014 by Volker Schroer, DL1KSV                          *
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

#include "activatemacros.h"
#include "ui_activatemacros.h"

#include "readonlystringlistmodel.h"

ActivateMacros::ActivateMacros(QVector<Macro> *macroList, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ActivateMacros)
{
  ui->setupUi(this);
  mL = macroList;
  activeMacros=new ReadOnlyStringListModel ( );
  deactivatedMacros = new ReadOnlyStringListModel ( );

//  activeList = new QStringList();
// QStringList deactivateList;
  int numberofMacros=macroList->size();
  if (numberofMacros > 0 )
  for(int i=0; i < numberofMacros;i++) {
    if( (*macroList).at(i).languageType >= 0) {
     activeList << (*macroList).at(i).name;
     deactivateList << "";
    }
    else {
     deactivateList << (*macroList).at(i).name;
     activeList << "";
    }
  }
    activeMacros->setStringList(activeList);
    deactivatedMacros->setStringList(deactivateList);
    ui->activeList->setModel(activeMacros);
    ui->passiveList->setModel(deactivatedMacros);

}

ActivateMacros::~ActivateMacros()
{
  delete activeMacros;
  delete deactivatedMacros;
  delete ui;
}
void ActivateMacros::addtoList()
{
  if(ui->activeList->currentIndex().data().toString().isEmpty())
    return;
  deactivatedMacros->setData(ui->activeList->currentIndex(),ui->activeList->currentIndex().data());
  deactivateList.replace(ui->activeList->currentIndex().row(),ui->activeList->currentIndex().data().toString());
  activeMacros->setData(ui->activeList->currentIndex(),"");
  ui->activeList->clearSelection();

}
void ActivateMacros::removefromList()
{
  if(ui->passiveList->currentIndex().data().toString().isEmpty())
    return;
  activeMacros->setData(ui->passiveList->currentIndex(),ui->passiveList->currentIndex().data());
  activeList.replace(ui->passiveList->currentIndex().row(),ui->passiveList->currentIndex().data().toString());
  deactivatedMacros->setData(ui->passiveList->currentIndex(),"");
  ui->passiveList->clearSelection();

}
void ActivateMacros::accept()
{
  int numberofMacros=mL->size();
  if (numberofMacros > 0 )
    for(int i=0; i < numberofMacros;i++) {
      if(((*mL).at(i).languageType >= 0) && (!deactivateList.at(i).isEmpty())) {
        Macro macro;
        macro=(*mL).at(i);
        macro.languageType -=3;
        mL->replace(i,macro);
      }
      else if (((*mL).at(i).languageType < 0) && (!activeList.at(i).isEmpty())) {
        Macro macro;
        macro=(*mL).at(i);
        macro.languageType +=3;
        mL->replace(i,macro);

      }

    }
  QDialog::accept();
}
