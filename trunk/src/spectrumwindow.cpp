/***************************************************************************
                          spectrumwindow.cpp -  description
                             -------------------
    begin                : Sat Aug 25 2012
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

#include "spectrumwindow.h"
#include "parameter.h"

#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include "crxchannel.h"
#include <cmath>

extern Parameter settings;

SpectrumWindow::SpectrumWindow(QWidget *parent) :
  QFrame(parent)
{
  int x=2*frameWidth();
  pdisplay=new QPixmap (width()-x,height()-x);
  pdisplay->fill();
  Farbe=0;
}
void SpectrumWindow::resizeEvent(QResizeEvent *)
{
  int x=2*frameWidth();
  if(pdisplay != 0)
    delete pdisplay;
  pdisplay =new QPixmap (this->width()-x,this->height()-x);
  pdisplay->fill();
  QPainter p(pdisplay);
  paintLineal(&p,pdisplay->width(),pdisplay->height(),100,2500);
  p.end();
}

void SpectrumWindow::paintLineal ( QPainter* p, int xmax, int ymax,int minfreq,int maxfreq )
{

    int stepfrequency;
    float stepwidth;
    int i, ix, NumberofFreqs, diff;
    int y;
    QString frequency;
    QFontMetrics fm ( this->font() );
// Calcalute Frequency- Steps
    diff = ( maxfreq - minfreq );
    NumberofFreqs = 7;
    while ( NumberofFreqs > 4 )
    {
        stepfrequency = diff / NumberofFreqs;
        if ( (stepfrequency * NumberofFreqs) != diff )
            NumberofFreqs--;
        else
            break;
    }
    stepwidth = float ( xmax ) / NumberofFreqs;

    diff = ymax / 10;
    if( diff < 12)
        diff = 12;
    y = ymax -3* diff/2;
    y=(y/diff)*diff;
    for ( i = 1; i < NumberofFreqs; i++ )
    {
        ix = ( int ) ( i * stepwidth + 0.5 );
        p->drawLine ( ix, y, ix, y + 3 );
        frequency.setNum ( minfreq + stepfrequency*i );
        ix = ix - fm.width ( frequency ) / 2;
        p->drawText ( ix, y+diff+diff/2, frequency );

    }

// Plot Grid

    for ( i = diff;i <=y; i +=diff )
    {
        p->drawLine ( 0, i, xmax, i );
    }
  baseline=y;
}
void SpectrumWindow::plotSpectrum(bool overload,float *fftdata,int minfreq,int maxfreq)
{
  float scale;
  int y1,y2,z;
  int ymax = pdisplay->height();
  int xmax = pdisplay-> width();
 // scale = ymax / 100.;
  scale=baseline/8.03;
  pdisplay->fill();
  QPainter p(pdisplay);
  p.setBrush ( Qt::white );

//Plot Frequencylines for the different Rx- Windows

    for (CRxChannel *pRx=settings.ChannelChain;pRx != 0;pRx= pRx->getNextChannel())
    {

  if ( Farbe->size() > 0)
       {
         int ID = pRx->getID();
         if ( ID >= 0 && ID < Farbe->size() )
           p.setPen(Farbe->at(ID));
       }
      // Calculate Centerfrequency Coordinates
      z=(int)(((  pRx->getRxFrequency()-minfreq)*xmax)/(maxfreq-minfreq)+0.5);
      p.drawLine(z,0,z,ymax);

      if ( (z =  pRx->get2RxFrequency()) != 0 ) // RTTY demands to lines
        {
          z=(( z-minfreq)*xmax)/(maxfreq-minfreq);
          p.drawLine(z,0,z,ymax);
        }
   }
    if ( overload )
            p.setPen ( Qt::red );
    else
      p.setPen ( Qt::black );
    for ( int i = 1;i < xmax;i++ )
    {

        y1 = baseline - ( int ) ( scale * fftdata[i-1] );
        y2 = baseline - ( int ) ( scale * fftdata[i] );

        p.drawLine ( i - 1, y1, i, y2 );
    }

  p.setPen ( Qt::black );
  paintLineal ( &p, xmax, ymax,minfreq,maxfreq );
  if ( Phase != 0 )
    plotVector ( &p );
  p.end();
  update();
}

void SpectrumWindow::paintEvent(QPaintEvent *e)
{
  int x=frameWidth();
  QFrame::paintEvent(e);
  QPainter p(this);
  p.drawPixmap(x,x,pdisplay->copy(0,0,pdisplay->width(),pdisplay->height()));
  p.end();
 }

void SpectrumWindow::setColorList ( QList<QColor> *c )
{
  Farbe=c;
}
void SpectrumWindow::setPhasePointer ( std::complex<float> *p )
{
  Phase = p;
}
void SpectrumWindow::plotVector ( QPainter *p )
{
  int xc, yc;
  double mag;
  xc = pdisplay->width() / 8;
  yc = pdisplay->height() / 8;
  p->drawEllipse ( xc, yc, 40, 40 );
  xc = xc + 20;
  yc = yc + 20;
  p->setPen ( Qt::green );
  for(int i=0; i< 11; i++)
  {
    mag=abs(Phase[i]);
    if ( mag > 0.001)
    p->drawLine(xc,yc,xc - (int)(20.*Phase[i].imag()/mag), yc - (int)(20.*Phase[i].real()/mag) );
   }
}

void SpectrumWindow::mousePressEvent ( QMouseEvent *e )
{
  emit frequencyChanged ( e->x() - lineWidth() );
}
void SpectrumWindow::wheelEvent ( QWheelEvent * e )
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
