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

#include "deletemacro.h"
#include "ui_deletemacro.h"

#include "readonlystringlistmodel.h"
DeleteMacro::DeleteMacro (QVector<Macro> *macroList, QWidget* parent, Qt::WindowFlags fl )
                : QDialog ( parent, fl ),  ui(new Ui::DeleteMacro)
{
  ui->setupUi ( this );
  mL = macroList;
  model = new ReadOnlyStringListModel ( );
  QStringList macroName;
  int numberofMacros=macroList->size();
  if (numberofMacros > 0 )
  for(int i=0; i < numberofMacros;i++)
    macroName.append((*macroList).at(i).name);
  model->setStringList(macroName);
  deleteList = new ReadOnlyStringListModel ( this );
  ui->MacroBox->setModel ( model );
  ui->Macrostodelete->setModel ( deleteList );
}

DeleteMacro::~DeleteMacro()
{
  if ( model != 0 )
    delete model;
  delete deleteList;
  delete ui;
}


void DeleteMacro::accept()
{

  QStringList d = deleteList->stringList();

  if ( !d.isEmpty() )
  {
    QString s;
    for ( int i = 0; i < d.size(); i++ )
    {
        s = d.at ( i );
        for ( int k = 0;k < mL->size();k++ )
        {
          if ( mL->at ( k ).name == s )
          {
            mL->remove ( k ,1);
            break;
          }
        }
    }
	}
  QDialog::accept();
}

void DeleteMacro::addtoList()
{
  QStringList l = deleteList->stringList();
  l << ui->MacroBox->currentIndex().data().toString();
  deleteList->setStringList ( l );
}

void DeleteMacro::removefromList()
{
  deleteList->removeRows ( ui->Macrostodelete->currentIndex().row(), 1 );
}


