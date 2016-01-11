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


#include "modemenu.h"
#include "parameter.h"
#include "crxchannel.h"

extern Parameter settings;

ModeMenu::ModeMenu(QWidget* parent, Qt::WindowFlags fl)
: QDialog( parent, fl ), Ui::ModeMenu()
{
	setupUi(this);
RxMode->insertItem(0,"BPSK");
RxMode->insertItem(1,"QPSK");
RxMode->insertItem(2,"RTTY");
RxMode->insertItem(3,"MFSK16");

RxMode->setCurrentRow(settings.ActChannel->getModulationType() );
connect(RxMode,SIGNAL(itemSelectionChanged ()),this,SLOT(changeView()));
Stopbits=new QButtonGroup(this);
Stopbits->addButton(One,0);
Stopbits->addButton(Onepoint5,1);
Stopbits->addButton(Two,2);

Parity=new QButtonGroup(this);
Parity->addButton(None,0);
Parity->addButton(Odd,1);
Parity->addButton(Even,2);

if ( settings.ActChannel->getModulationType() != RTTY)
 {
  ParityLayout->hide();
  Spacing->hide();
  Reverse->hide();
  StopbitsLayout->hide();
 } 
}

ModeMenu::~ModeMenu()
{
}
void ModeMenu::changeView()
{
int index=RxMode->currentRow();
if ( index == RTTY )
 {
  ParityLayout->show();
  Spacing->show();
  Reverse->show();
  StopbitsLayout->show(); 
 }
else
 {
  ParityLayout->hide();
  Spacing->hide();
  Reverse->hide();
  StopbitsLayout->hide();

 } 
}
void ModeMenu::setParameter(ExtraParameter Param)
{
RTTYSpacing->setValue(Param.offset);
/** FIXME 
Parity->setChecked(Param.parity);
Stopbits->setChecked(Param.stopbits);
**/
Reverse->setChecked(Param.reverse);
}
ExtraParameter ModeMenu::getParameter()
{
ExtraParameter Param;

Param.offset=RTTYSpacing->value();
Param.reverse=Reverse->isChecked();
Param.parity=(Paritaet) Parity->checkedId();
Param.stopbits=(StopBits) Stopbits->checkedId();
return Param;
}
Mode ModeMenu::selectedMode()
{
 return (Mode) RxMode->currentRow();
}


