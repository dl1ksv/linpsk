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

#include "macrocontrol.h"
#include "ui_macrocontrol.h"
#include <QPushButton>
#include <QButtonGroup>

MacroControl::MacroControl(QWidget *parent) :
  QGroupBox(parent),
  ui(new Ui::MacroControl)
{
  ui->setupUi(this);
  displayBox = new QButtonGroup(ui->area);
  ui->languageBox->setId(ui->lang0,0);
  ui->languageBox->setId(ui->lang1,1);
  ui->languageBox->setId(ui->lang2,2);
  macroLang=ui->languageBox->checkedId();
  connect(displayBox,SIGNAL(buttonPressed(int)),this,SIGNAL(executeMacro(int)));
}

MacroControl::~MacroControl()
{
  delete displayBox;
  delete ui;
}
void MacroControl::setMacroLanguage(int lang)
{
  macroLang=lang;
  displayMacros();
}
void MacroControl::insertMacros(QVector<Macro> *macroList)
{
  QPushButton *pb = 0;
  int anzahl,i;
  /**
    First Check, if box is empty,
    otherwise delete all entries,as order or even number of macros
    might have changed ( Delete or edit )
    So we don't need extra processing for these cases.
  **/
  anzahl = displayBox->buttons().size();
  if ( anzahl > 0)
    for ( i = 0; i < anzahl; i++ ) // Clean the display box
    {
      pb= static_cast<QPushButton *> (displayBox->button ( i ) );
      displayBox->removeButton (pb  );
      delete pb;
    }
  mL=macroList;
  anzahl=mL->size();
  if ( anzahl > 0 )
    {
      for (i = 0; i < anzahl;i++ ) // insert macro buttons
        {
          pb = new QPushButton ( mL->at(i).name, ui->area );
          pb ->setCheckable(false);
          pb->setMaximumHeight(20);
          ui->arrangeButtons->addWidget(pb);
          displayBox->addButton ( pb, i );
        }
      displayMacros();
    }
}
void MacroControl::displayMacros()
{
  int i, anzahl,lT;
  anzahl=mL->size();
  if ( anzahl > 0 )
    {
      ui->scrollArea->show();
      for ( i = 0; i < anzahl;i++ ) // insert macro buttons
      {
         lT=mL->at(i).languageType;
         if(((macroLang == 0) && (lT >0)) || ( lT == macroLang)|| (lT == 0))
             displayBox->button(i)->show();
          else
           displayBox->button(i) ->hide();

      }
    }
  else
    ui->scrollArea->hide();
}
void MacroControl::updateMacroWindow(int macroNumber)
{
  QPushButton *pb = static_cast<QPushButton *> (displayBox->button ( macroNumber ) );
  pb->setText(mL->at(macroNumber).name);
}
