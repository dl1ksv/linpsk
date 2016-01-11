/***************************************************************************
                          spectrumdisplay.h  -  description
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

#ifndef SPECTRUMDISPLAY_H
#define SPECTRUMDISPLAY_H



#include <QFrame>
#include <complex>
#include "ui_spectrumdisplay.h"

class QRadioButton;
class QSpinBox;
class QWidget;

/**
  *@author Volker Schroer
  */
const int fftsize=1024; // Check size to protect arrays

class SpectrumDisplay : public QFrame , private Ui::SpectrumDisplay
{
 Q_OBJECT
public: 
  SpectrumDisplay( QWidget* parent = 0);
  ~SpectrumDisplay();
  void setPhasePointer(std::complex<float> *);
  void setColorList(QList<QColor> *c);
  QByteArray spectrumSplitterState() const;
  void restoreSplitterState(const QByteArray & spectrumState);
  void showSpectren(bool p);

public slots:
  void startPlot(double *,bool);



protected:
    void resizeEvent( QResizeEvent * );

protected slots:
  void setnewFrequency(int);
private:
 /** Pointer to the Input for the FFT **/
 double *inputdata; 
 int displayWidth;
 void translate(void);
 void plotVector(QPainter *p);
  /** Results of FFT */
  void calcFFT();

 float fftdata[fftsize];    // Not all elements are used, it differs
 int xtranslate[fftsize]; // depending on the display width
 float smoothedfft[fftsize];
 int oldMinfreq;
 int oldMaxfreq;

signals:
void FrequencyChanged(double);

};

#endif
