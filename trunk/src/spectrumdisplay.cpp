/***************************************************************************
                          spectrumdisplay.cpp  -  description
                             -------------------
    begin                : Fr March 19 2004
    copyright            : (C) 2004 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


#include <QRadioButton>
#include <QSpinBox>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QSplitter>
#include "spectrumdisplay.h"
#include "crxchannel.h"
#include "parameter.h"
#include "color.h"

#define SPECTRUMHEIGHT 70
#define WATERFALLHEIGHT 20
#define LABELHEIGHT 10
#define DISTANCE 3

extern Parameter settings;

SpectrumDisplay::SpectrumDisplay ( QWidget* parent ) : QFrame ( parent )
{
    setFrameShape( QFrame::StyledPanel);
setFrameShadow(QFrame::Sunken );


	MaxFreq = new QSpinBox ( parent );
	MaxFreq->setMaximum ( 2500 );
	MaxFreq->setMinimum ( 1300 );
	MaxFreq->setValue ( 2500 );
	MaxFreq->setSingleStep ( 100 );

	MinFreq = new QSpinBox ( parent );
	MinFreq->setButtonSymbols ( QSpinBox::UpDownArrows );
	MinFreq->setMaximum ( 1200 );
	MinFreq->setMinimum ( 100 );
	MinFreq->setSingleStep ( 100 );

Smooth=new QRadioButton(tr("Smooth"),parent);
	languageChange();


	inputdata = 0;


	Phase = 0;
 Farbe=0;
// Connections
	connect ( this, SIGNAL ( frequencyChanged ( int ) ), this, SLOT ( setnewFrequency ( int ) ) );
for(int i=0; i< 1024;i++)
 smoothedfft[i]=0.;
}

/*
 *  Destroys the object and frees any allocated resources
 */
SpectrumDisplay::~SpectrumDisplay()
{
	// no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SpectrumDisplay::languageChange()
{

	MaxFreq->setSuffix ( " Hz" );
	MinFreq->setSuffix ( " Hz" );
}
void SpectrumDisplay::resizeEvent ( QResizeEvent * )
{

	calculateSizeofComponents();
	translate();
	calcFFT();
	plotspectrum ( false );

}

void SpectrumDisplay::calculateSizeofComponents()
{
	int xpos, ypos, width, height, innerheight, innerwidth;
	width = this->width();
	height =this->height();
	/** Display **/
	xpos = 0;
	ypos = height * DISTANCE / 100;
  innerwidth=width-2*this->frameWidth();

	innerheight = height * SPECTRUMHEIGHT / 100;
pdisplay =QPixmap ( innerwidth,innerheight);
pwaterfall=QPixmap(innerwidth,height-innerheight);
pwaterfall.fill(Qt::black);
	/** Controlelements of the display **/
	ypos = ypos + height + DISTANCE * height / 100;
	innerheight = height * LABELHEIGHT / 100;
	innerwidth = ( width - 2 * xpos ) / 4;
	xpos = x();
	MinFreq->setGeometry ( xpos, ypos, innerwidth, innerheight );
//xpos=this->width()-innerwidth;
Smooth->setGeometry(xpos+width/2-innerwidth/2,ypos,innerwidth, innerheight );
	xpos = xpos + width - innerwidth;
	MaxFreq->setGeometry ( xpos, ypos, innerwidth, innerheight );

}

// Plot Spectrum of decimated Input
void SpectrumDisplay::plotspectrum ( bool overload )
{


	QPainter p;


	int dist, y1, y2, z, ymax, xmax;
	int minfreq, maxfreq;

	double scale;
	
	scale = ( pdisplay.height() - 15 ) / 100.;
	minfreq = MinFreq->value();
	maxfreq = MaxFreq->value();
	ymax = pdisplay.height()-4; // Subtract Frame
	dist = ymax / 10;
	xmax = pdisplay.width();

	pdisplay.fill();
	p.begin ( &pdisplay );
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
		p.setPen ( Qt::blue );
// Plot Lineal and grid
	p.setPen ( Qt::black );
	paintLineal ( &p, xmax, ymax );

	if ( inputdata != 0 )
	{
		for ( int i = 1;i < xmax;i++ )
		{

			y1 = ymax - ( int ) ( scale * fftdata[i-1] ) - 15;
			y2 = ymax - ( int ) ( scale * fftdata[i] ) - 15;

			p.drawLine ( i - 1, y1, i, y2 );
		}

		if ( Phase != 0 )
			plotVector ( &p );
	}

p.end();
	 if ( inputdata != 0 )
	 {
	xmax = pwaterfall.width();

	  p.begin(&pwaterfall);
      p.drawPixmap(0,2,pwaterfall.copy(0,0,pwaterfall.width(),pwaterfall.height()-2)); // Qt changed behaviour
	  for (int i=0;i<xmax;i++)
	  {
	   y1=4*fftdata[i];
	   if ( y1 > 255)
	    y1 = 255;
	   else if  ( y1 < 0 )
	    y1= 0;
	   p.setPen(color[y1]);
	   p.drawPoint(i,0);
	   p.drawPoint(i,1);
	  }

	p.end();
}
update();
}
void SpectrumDisplay::calcFFT()
{

	if ( inputdata == 0 )
		return;          // No data available
	for ( int i = 0;i < pdisplay.width();i++ )
  if( Smooth->isChecked())
 {
float x=( log10 ( inputdata[xtranslate[i]] + 100. ) - 2. );
 float gain=(1. - exp(-(0.2 * x)));
  smoothedfft[i]=smoothedfft[i]*(1.-gain) + gain *x;
 fftdata[i] = ( int ) (20.*smoothedfft[i]);
 }
else
//18.4 scales to a range from 0 - 100, as max(inputdata ) =  fft_length/4 ^ 2
		fftdata[i] = ( int ) ( 18.4 * ( log10 ( inputdata[xtranslate[i]] + 100. ) - 2. ) );
// For Color scale should be 18.4 *2.55
}

void SpectrumDisplay::translate ( void )
{
	int i, to, minfreq, maxfreq, displaywidth;
	minfreq = MinFreq->value();
	maxfreq = MaxFreq->value();
	to = int ( maxfreq * 1024 / 2756.25 );
	displaywidth = pdisplay.width();
	for ( i = 0;i < displaywidth;i++ )
		xtranslate[i] = ( ( ( maxfreq - minfreq ) * i * to / displaywidth ) + minfreq * to ) / maxfreq;
}

void SpectrumDisplay::startPlot ( double *x, bool overload )
{
	inputdata = x;
	translate();
	calcFFT();
	plotspectrum ( overload );
}

void SpectrumDisplay::setnewFrequency ( int position )
{

	double freq;
	int ii;
	ii = pdisplay.width();
	freq = ( position * ( MaxFreq->value() - MinFreq->value() ) ) / ii + MinFreq->value();
	settings.ActChannel->setRxFrequency ( freq );
	emit FrequencyChanged ( freq );
}

void SpectrumDisplay::paintLineal ( QPainter* p, int xmax, int ymax )
{

	int stepfrequency;
	float stepwidth;
	int i, ix, NumberofFreqs, diff;
	int y;
	QString frequency;
	QFontMetrics fm ( this->font() );
	int minfreq = MinFreq->value();
// Calcalute Frequency- Steps
	diff = ( MaxFreq->value() - minfreq );
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
	y = ymax - diff;


	for ( i = 1; i < NumberofFreqs; i++ )
	{
		ix = ( int ) ( i * stepwidth + 0.5 );
		p->drawLine ( ix, y, ix, y - 3 );
		frequency.setNum ( minfreq + stepfrequency*i );
		ix = ix - fm.width ( frequency ) / 2;
		p->drawText ( ix, ymax, frequency );

	}

// Plot Grid
	for ( i = 1;i < 10; i++ )
	{

		p->drawLine ( 0, y, xmax, y );
		y -= diff;
	}
}

void SpectrumDisplay::plotVector ( QPainter *p )
{
	int xc, yc;
	double mag;

	xc = pdisplay.width() / 8;
	yc = pdisplay.height() / 8;
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

void SpectrumDisplay::setPhasePointer ( std::complex<float> *p )
{
	Phase = p;
}
void SpectrumDisplay::setColorList ( QList<QColor> *c )
{
  Farbe=c;
}
void SpectrumDisplay::mousePressEvent ( QMouseEvent *e )

{
	emit frequencyChanged ( e->x() - lineWidth() );
}
void SpectrumDisplay::paintEvent(QPaintEvent *e)
{
QFrame::paintEvent(e);
QPainter p(this);
p.drawPixmap(frameWidth(),frameWidth(),pdisplay);
p.drawPixmap(frameWidth(),frameWidth()+pdisplay.height()-4,pwaterfall);
}

