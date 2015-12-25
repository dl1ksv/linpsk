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

#ifndef DEFINEBANDLIST_H
#define DEFINEBANDLIST_H

#include <QDialog>

namespace Ui {
  class DefineBandList;
}

class DefineBandList : public QDialog
{
  Q_OBJECT

public:
  explicit DefineBandList(QWidget *parent = 0);
  ~DefineBandList();

private:
  Ui::DefineBandList *ui;
public slots:
  virtual void accept();
  void addRow();
  void deleteRow();
  void rowUp();
  void rowDown();
};

#endif // DEFINEBANDLIST_H
