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
#ifndef READONLYSTRINGLISTMODEL_H
#define READONLYSTRINGLISTMODEL_H

#include <QStringListModel>

/**
ReadOnlyStringListModel provides a readonly StringList model, so that views of this model cannot change the model's data

	@author volker, DL1KSV <schroer@tux64>
*/
class ReadOnlyStringListModel : public QStringListModel
{
Q_OBJECT
public:
    ReadOnlyStringListModel(QObject *parent = 0);
ReadOnlyStringListModel ( const QStringList & strings, QObject * parent = 0 );
 //   ~ReadOnlyStringListModel();
 Qt::ItemFlags flags ( const QModelIndex & index ) const;

};

#endif
