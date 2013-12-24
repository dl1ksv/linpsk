/***************************************************************************
                          waterfallwindow.h -  description
                             -------------------
    begin                : Sat Aug 26 2012
    copyright            : (C) 2012 by Volker Schroer
    email                : DL1KSV@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include "waterfallwindow.h"
#include "color.h"
#include "parameter.h"
#include "crxchannel.h"

extern Parameter settings;

WaterfallWindow::WaterfallWindow(QWidget *parent) :
  QFrame(parent)
{
  int x=2*frameWidth();
  pwaterfall=new QPixmap (width()-x,height()-x);
  pwaterfall->fill(Qt::black);
}

void WaterfallWindow::plotWaterfall(float *fftdata)
{
  int xmax = pwaterfall->width();
  int y1;
  QPainter p(pwaterfall);
  p.drawPixmap(0,2,pwaterfall->copy(0,0,pwaterfall->width(),pwaterfall->height()-2));
  for (int i=0;i<xmax;i++)
  {
   y1=31*fftdata[i]; // Scale is 256/8.1
   if ( y1 > 255)
     {
  //     qDebug("y1: %i, fft: %f",y1,fftdata[i]);
    y1 = 255;
     }
   else if  ( y1 < 0 )
    y1= 0;
   p.setPen(color[y1]);
   p.drawPoint(i,0);
   p.drawPoint(i,1);
  }

  p.end();
  update();
}
void WaterfallWindow::paintEvent(QPaintEvent *e)
{
  int x=frameWidth();
  QFrame::paintEvent(e);
  QPainter p(this);
  p.drawPixmap(x,x,pwaterfall->copy(0,0,pwaterfall->width(),pwaterfall->height()));
  p.end();
}
void WaterfallWindow::resizeEvent(QResizeEvent *)
{
  int x=2*frameWidth();
  if(pwaterfall != 0)
    delete pwaterfall;
  pwaterfall =new QPixmap (this->width()-x,this->height()-x);
  pwaterfall->fill(Qt::black);
}
void WaterfallWindow::mousePressEvent ( QMouseEvent *e )
{
  emit frequencyChanged ( e->x() - lineWidth() );
}
void WaterfallWindow::wheelEvent ( QWheelEvent * e )
{
  double freq=settings.ActChannel->getRxFrequency();
  if(e->delta() >0 )
    freq =freq+1;
  else
    freq = freq -1;
  emit frequencyChanged(freq);
  settings.ActChannel->setRxFrequency(freq);
    e->accept();
}
