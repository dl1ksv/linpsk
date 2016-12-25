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

#include "definebandlist.h"
#include "ui_definebandlist.h"
#include "parameter.h"
#include <QLineEdit>

extern Parameter settings;

DefineBandList::DefineBandList(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DefineBandList)
{
  int rows;
  int i,j;
  ui->setupUi(this);

  rows = settings.bandList.size();
  for(i=0; i < rows; i++)
  {
   ui->bandDefinitions->insertRow(i);
//   if(i < (rows-1))
     ui->bandDefinitions->setItem(i,0,new QTableWidgetItem(settings.bandList.at(i).bandName));
   for(j=1; j < 4; j++)
     {
       QLineEdit *line =new QLineEdit;
       line->setAlignment(Qt::AlignRight);
       line->setInputMask("00000000000D");
       ui->bandDefinitions->setCellWidget(i,j,line);
 //      if(i < (rows-1) )
 //        {
           if (j == 1 )
             line->setText(QString("%1").arg(settings.bandList.at(i).bandStart));
           else if ( j == 2 )
              line->setText(QString("%1").arg(settings.bandList.at(i).bandEnd));
           else line->setText(QString("%1").arg(settings.bandList.at(i).preferedFreq));
 //        }

     }
  }
  ui->bandDefinitions->setCurrentCell(rows-1,0);
  ui->bandDefinitions->setFocus();
  ui->bandDefinitions->resizeColumnsToContents();

}

DefineBandList::~DefineBandList()
{
  delete ui;
}
void DefineBandList::accept()
{

 QLineEdit *lE;

 settings.bandList.clear();

 for(int i=0; i < ui->bandDefinitions->rowCount(); i++)
   {
     Band b;
     QTableWidgetItem *it=ui->bandDefinitions->item(i,0);
     if( it != NULL)
      b.bandName = it->text();
     else
      break;

     lE=  (QLineEdit *) ui->bandDefinitions->cellWidget(i,1);
     if ( lE != NULL)
      b.bandStart= lE->text().toInt();
     else
       break;
     lE = (QLineEdit *) ui->bandDefinitions->cellWidget(i,2);
     if ( lE != NULL)
       b.bandEnd= lE->text().toInt();
     else
       break;
     lE = (QLineEdit *) ui->bandDefinitions->cellWidget(i,3);
     if ( it != NULL)
      b.preferedFreq= lE->text().toInt();
     else
       break;
     // To do: Check if definition ist consistent
     settings.bandList << b;
   }

 QDialog::accept();
}
void DefineBandList::addRow()
{
  int row;
  row= ui->bandDefinitions->rowCount();
  ui->bandDefinitions->insertRow(row);
  for(int j=1; j < 4; j++)
    {
      QLineEdit *line =new QLineEdit;
      line->setAlignment(Qt::AlignRight);
      line->setInputMask("00000000000D");
      ui->bandDefinitions->setCellWidget(row,j,line);
    }
}

void DefineBandList::deleteRow()
{
  int row;
  row=ui->bandDefinitions->currentRow();
  ui->bandDefinitions->removeRow(row);
}

void DefineBandList::rowUp()
{
  QTableWidgetItem *it;
  QLineEdit *line1;
  QLineEdit *line0;
  int row;
  row=ui->bandDefinitions->currentRow();
  if(row == 0)
    return;
  it=ui->bandDefinitions->takeItem(row-1,0);
  ui->bandDefinitions->setItem(row-1,0,ui->bandDefinitions->takeItem(row,0));
  ui->bandDefinitions->setItem(row,0,it);
  for (int i=1; i < 4; i++)
    {
      line1 = (QLineEdit *)ui->bandDefinitions->cellWidget(row-1,i);
      line0 = (QLineEdit *)ui->bandDefinitions->cellWidget(row,i);
      QString s = line1->text();
      line1->setText(line0->text());
      line0->setText(s);

    }
}

void DefineBandList::rowDown()
{
  QTableWidgetItem *it;
  QLineEdit *line1;
  QLineEdit *line0;
  int row;
  row=ui->bandDefinitions->currentRow();
  if(row >=(ui->bandDefinitions->rowCount()-1) )
    return;
  it=ui->bandDefinitions->takeItem(row+1,0);
  ui->bandDefinitions->setItem(row+1,0,ui->bandDefinitions->takeItem(row,0));
  ui->bandDefinitions->setItem(row,0,it);
  for (int i=1; i < 4; i++)
    {
      line1 = (QLineEdit *)ui->bandDefinitions->cellWidget(row+1,i);
      line0 = (QLineEdit *)ui->bandDefinitions->cellWidget(row,i);
      QString s = line1->text();
      line1->setText(line0->text());
      line0->setText(s);

    }
}
