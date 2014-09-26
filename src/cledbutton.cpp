/***************************************************************************
                          cledbutton.cpp  -  description
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

#include "cledbutton.h"

#include <qpainter.h>

CLedButton::CLedButton(QWidget *parent) : QPushButton(parent)
{
status=UNDEF;
setText("&RX");
setFlat(false);
connect(this,SIGNAL ( clicked() ),this,SLOT(rxtx()));
}
CLedButton::~CLedButton(){
}
/** paint the button and the led in it */
void CLedButton::paintEvent(QPaintEvent *e)
{
QPushButton::paintEvent(e);
QPainter paint(this);

switch (status)
	{
	case ON:
		paint.setPen(Qt::black);
		paint.setBrush(Qt::red);
		paint.drawEllipse(4,4,14,14);
		break;
	case OFF:
		paint.setPen(Qt::black);
		paint.setBrush(Qt::green);		
		paint.drawEllipse(4,4,14,14);
		break;
	case UNDEF:
		paint.setPen(Qt::black);
    paint.setBrush(Qt::gray);
		paint.drawEllipse(4,4,14,14);
		break;
  case SW:
    paint.setPen(Qt::black);
    paint.setBrush(Qt::yellow);
    paint.drawEllipse(4,4,14,14);
    break;
	}
}
/** sets the buttonstatus */
void CLedButton::setStatus(BUTTONSTATUS state)
{
status=state;
switch(status)
  {
  case UNDEF:
  case ON:
    setText("&RX");
    break;
  case OFF:
    setText("&TX");
    break;
  case SW:
    setText("&A");
    break;
  }
repaint();
}
/** No descriptions */
void CLedButton::rxtx()
{
switch(status)
  {
  case UNDEF:
  case ON:
//    setStatus(OFF);
    emit startRx();
    break;
  case OFF:
//    setStatus(ON);
    emit startTx();
    break;
  case SW:
    emit abortTx();
    break;
  }
}
BUTTONSTATUS CLedButton::getstatus()
{
return status;
}
