/***************************************************************************
                          macros.h  -  description
                             -------------------
    begin                : Sam Mai 3 2003
    copyright            : (C) 2003 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MACROWINDOW_H
#define MACROWINDOW_H

#include <QFrame>

class Macros;
class QGroupBox;
class QButtonGroup;
class QScrollArea;

/**This class implements simple text and file macros.
The macros are startet by a pushbutton and the pushbuttons are arranged in a scrollview.
  *@author Volker Schroer
  */


class MacroWindow : public QFrame  {
   Q_OBJECT
public: 
    MacroWindow(QWidget *parent=0);
	~MacroWindow();
void setMacroWindow ( Macros *M);
private:  
QButtonGroup* DisplayBox;
QGroupBox* Area;
QScrollArea *scrollArea;
int NumberOfMacros;
protected:
void fontChange(const QFont &);

public slots:
virtual void resizeEvent( QResizeEvent * );
void updateMacroWindow(Macros *);

signals:
void callMacro(int);  
};

#endif
