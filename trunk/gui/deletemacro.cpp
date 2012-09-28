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


#include "deletemacro.h"
#include "macros.h"
//#include <QStringListModel>
#include "readonlystringlistmodel.h"
DeleteMacro::DeleteMacro ( Macros *M, QWidget* parent, Qt::WFlags fl )
		: QDialog ( parent, fl ), Ui::DeleteMacro()
{
	setupUi ( this );
	AllMacros = M;
//	model = new QStringListModel ( AllMacros->getMacroList() );
	model = new ReadOnlyStringListModel ( AllMacros->getMacroList() );
//	deleteList = new QStringListModel ( this );
	deleteList = new ReadOnlyStringListModel ( this );
	MacroBox->setModel ( model );
	Macrostodelete->setModel ( deleteList );
	connect ( Add, SIGNAL ( clicked ( bool ) ), this, SLOT ( addtoList() ) );
	connect ( Remove, SIGNAL ( clicked ( bool ) ), this, SLOT ( removefromList() ) );
}

DeleteMacro::~DeleteMacro()
{
	if ( model != 0 )
		delete model;
	delete deleteList;
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
			for ( int k = 0;k < AllMacros->count();k++ )
			{
				if ( AllMacros->getMacroName ( k ) == s )
				{
					AllMacros->deleteMacro ( k );
					break;
				}
			}

		}
// Store list to macros ??
	}
	QDialog::accept();
}

void DeleteMacro::addtoList()
{
	QStringList l = deleteList->stringList();
	l << MacroBox->currentIndex().data().toString();
	deleteList->setStringList ( l );
}

void DeleteMacro::removefromList()
{
	deleteList->removeRows ( Macrostodelete->currentIndex().row(), 1 );
}


