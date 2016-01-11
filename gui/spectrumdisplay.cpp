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
#include <QSplitter>
#include "spectrumdisplay.h"
#include "crxchannel.h"
#include "parameter.h"
#include "color.h"
extern Parameter settings;

SpectrumDisplay::SpectrumDisplay ( QWidget* parent ) : QFrame ( parent ), Ui::SpectrumDisplay()
{
  setupUi(this);
  inputdata = 0;
  for(int i=0; i< fftsize;i++)
    smoothedfft[i]=0.;
  displayWidth=spectrumWindow->width()-2*spectrumWindow->frameWidth();
  connect(spectrumWindow,SIGNAL(frequencyChanged(double)),this ,SIGNAL(FrequencyChanged(double)));
  connect(waterfallWindow,SIGNAL(frequencyChanged(double)),this ,SIGNAL(FrequencyChanged(double)));
}

/*
 *  Destroys the object and frees any allocated resources
 */
SpectrumDisplay::~SpectrumDisplay()
{
	// no need to delete child widgets, Qt does it all for us
}

void SpectrumDisplay::resizeEvent ( QResizeEvent * )
{
  if ( inputdata == 0 )
    return;          // No data available
  displayWidth=spectrumWindow->width()-2*spectrumWindow->frameWidth();
  if(fftsize < displayWidth)
  {
    qDebug("+++ SpectrumDisplay: Spectrum array %d lower than displaywidth %d",fftsize,displayWidth);
    return;
  }
  translate();
  calcFFT();
  spectrumWindow->plotSpectrum(false,fftdata, MinFreq->value(),MaxFreq->value());
  waterfallWindow->plotWaterfall(fftdata);
}

void SpectrumDisplay::calcFFT()
{
  int i;
  float x,gain;
  if ( inputdata == 0 )
     return;          // No data available
  if( Smooth->isChecked())
    {
      for ( i = 0;i < displayWidth;i++ )
      {
        if(inputdata[xtranslate[i]]> 0.02)
           x=log10 ( inputdata[xtranslate[i]])+2;
          else
           x=0.;
        gain=(1. - exp(-(0.2 * x)));
        smoothedfft[i]=smoothedfft[i]*(1.-gain) + gain *x;
        fftdata[i] = smoothedfft[i];
      }
     }
   else
    {
      for ( i = 0;i < displayWidth;i++ )
        {
          if(inputdata[xtranslate[i]]> 0.02)
            fftdata[i]=log10 ( inputdata[xtranslate[i]])+2;
          else
            fftdata[i]=0.;
        }
    }
  i=spectrumWindow->height();
  i=0;
}

void SpectrumDisplay::translate ( void )
{
    int i, to, minfreq, maxfreq,tmp;
	minfreq = MinFreq->value();
	maxfreq = MaxFreq->value();
    to = int ( maxfreq * fftsize *4/ settings.sampleRate );
    for ( i = 0;i < displayWidth;i++ )
    {
      tmp= ( ( ( maxfreq - minfreq ) * i * to / displayWidth ) + minfreq * to ) / maxfreq;
      if( tmp < fftsize)
        xtranslate[i] = tmp;
      else
      {
        qDebug("+++ SpectrumDisplay: translation array size %d lower than requested size %d",fftsize,tmp);
        return;
      }
    }
}

void SpectrumDisplay::startPlot ( double *x, bool overload )
{
	inputdata = x;
    int tmp=spectrumWindow->width()-2*spectrumWindow->frameWidth();
    if( tmp != displayWidth)
    {
      displayWidth=tmp;
      translate();
    }
	calcFFT();
    spectrumWindow->plotSpectrum(overload,fftdata, MinFreq->value(),MaxFreq->value());
    waterfallWindow->plotWaterfall(fftdata);
}

void SpectrumDisplay::setnewFrequency ( int position )
{

	double freq;
    freq = ( position * ( MaxFreq->value() - MinFreq->value() ) ) / displayWidth + MinFreq->value();
	settings.ActChannel->setRxFrequency ( freq );
	emit FrequencyChanged ( freq );
}

void SpectrumDisplay::setPhasePointer ( std::complex<float> *p )
{
   spectrumWindow->setPhasePointer(p);
}
void SpectrumDisplay::setColorList(QList<QColor> *c)
{
  spectrumWindow->setColorList(c);
}
QByteArray SpectrumDisplay::spectrumSplitterState() const
{
   return spectrumSplitter->saveState();
}
void SpectrumDisplay::restoreSplitterState(const QByteArray & spectrumState)
{
  spectrumSplitter->restoreState(spectrumState);
}
void SpectrumDisplay::showSpectren(bool p)
{
  if(p)
    {
      spectrumWindow->show();
      waterfallWindow->show();
    }
  else
    {
      spectrumWindow->hide();
      waterfallWindow->hide();
    }
}
