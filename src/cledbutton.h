/***************************************************************************
                          cledbutton.h  -  description
                             -------------------
    begin                : Sun Jan 14 2001
    copyright            : (C) 2001 by Volker Schroer
    email                : DL1KSV@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef CLEDBUTTON_H
#define CLEDBUTTON_H

#include <qpushbutton.h>
#include "constants.h"
/**Constucts a pushbutton with a led in it
  *@author Volker Schroer
  */

class CLedButton : public QPushButton  {
   Q_OBJECT
public: 
	CLedButton(QWidget *parent=0);
	~CLedButton();
  /** paint the button and the led in it */
  void paintEvent(QPaintEvent *e);
  /** sets the buttonstatus */
  void setStatus(BUTTONSTATUS state);
BUTTONSTATUS getstatus();    
private:
BUTTONSTATUS status;	
private slots: // Private slots
  /** No descriptions */
  void rxtx();

signals:
void startRx();
void startTx();
void abortTx();
};

#endif
