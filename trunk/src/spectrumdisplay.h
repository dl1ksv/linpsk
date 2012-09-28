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
#include <QColor>
#include <QList>
using namespace std;

class QRadioButton;
class QSpinBox;
class QPixmap;
class QWidget;

/**
  *@author Volker Schroer
  */

class SpectrumDisplay : public QFrame  {
 Q_OBJECT
public: 
  SpectrumDisplay( QWidget* parent = 0);
  ~SpectrumDisplay();
  
  void setColorList(QList<QColor> *c);
   
  void mousePressEvent(QMouseEvent *);



public slots:
  void startPlot(double *,bool);
  void setPhasePointer(std::complex<float> *);



protected:
    void resizeEvent( QResizeEvent * );
   void paintEvent(QPaintEvent *);

protected slots:
    virtual void languageChange();
  void setnewFrequency(int);
private:
 void calculateSizeofComponents();
 QSpinBox* MaxFreq;
 QSpinBox* MinFreq;
QRadioButton *Smooth;


 void plotspectrum(bool);
 /** Pointer to the Input for the FFT **/
 double *inputdata; 
 complex<float> *Phase;
 void translate(void);
 void paintLineal(QPainter* ,int,int);
 void plotVector(QPainter *p);
  /** Results of FFT */
  void calcFFT();

  int fftdata[1024];    // Not all elements are used, it differs 
	int xtranslate[1024]; // depending on the display width
  float smoothedfft[1024];
// Pointer to the different Colors
QList<QColor> *Farbe;

QPixmap pdisplay;   // Pixmap for Display for double buffering
QPixmap pwaterfall; // Pixmap for Waterfall Display
//QPixmap *pellipse;   // Pixmap for crossed ellipse in RTTY(2) Mode

signals:
void FrequencyChanged(double);

void frequencyChanged(int);

};

#endif
