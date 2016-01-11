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


#include "addrxwindow.h"
#include "parameter.h"

extern Parameter settings;

AddRxWindow::AddRxWindow(QWidget* parent, Qt::WindowFlags fl)
: QDialog( parent, fl ), Ui::AddRxWindow()
{
	setupUi(this);
RxMode->insertItem(0,"BPSK");
RxMode->insertItem(1,"QPSK");
RxMode->insertItem(2,"RTTY");
RxMode->insertItem(3,"MFSK16");
RxMode->setCurrentRow(0);
TitleText->setText("Rx " + QString().setNum(settings.RxChannels+1));

}

AddRxWindow::~AddRxWindow()
{
}
Mode AddRxWindow::selectedMode()
{
return (Mode) RxMode->currentRow();
}
QString AddRxWindow::titleText()
{
 return TitleText->text();
}
int AddRxWindow::frequency()
{
 return Frequency->value();
}

