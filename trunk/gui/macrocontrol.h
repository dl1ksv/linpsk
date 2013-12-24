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

#ifndef MACROCONTROL_H
#define MACROCONTROL_H

#include <QGroupBox>
#include "constants.h"

class QButtonGroup;

namespace Ui {
  class MacroControl;
}

class MacroControl : public QGroupBox
{
  Q_OBJECT

public:
  explicit MacroControl(QWidget *parent = 0);
  ~MacroControl();
  void insertMacros(QVector<Macro> *macroList);
  void updateMacroWindow(int macroNumber);

public slots:
  void setMacroLanguage(int lang);
signals:
  void executeMacro(int);

private:
  Ui::MacroControl *ui;
  QVector<Macro> *mL;
  int macroLang;
  QButtonGroup *displayBox;
  void displayMacros();

};

#endif // MACROCONTROL_H
