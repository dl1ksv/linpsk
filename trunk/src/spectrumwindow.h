/***************************************************************************
                          spectrumwindow.h -  description
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

#ifndef SPECTRUMWINDOW_H
#define SPECTRUMWINDOW_H

#include <QFrame>
#include <QColor>
#include <QList>
#include <complex>

class QPixmap;

class SpectrumWindow : public QFrame
{
  Q_OBJECT
public:
  explicit SpectrumWindow(QWidget *parent = 0);
  void plotSpectrum(bool overload, float *fftdata, int minfreq, int maxfreq);
  void setColorList(QList<QColor> *c);
  void mousePressEvent ( QMouseEvent *e );
  void setPhasePointer(std::complex<float> *);

private:
  void paintLineal(QPainter* ,int,int,int,int);
  void plotVector ( QPainter *p );
  QPixmap *pdisplay;   // Display for double buffering
  int baseline; // Y Position of X- Axis in window
  std::complex<float> *Phase;

protected:
    void resizeEvent( QResizeEvent * );
    void paintEvent(QPaintEvent *);
    void wheelEvent ( QWheelEvent * e );
  
    QList<QColor> *Farbe;
signals:
  void frequencyChanged(int);
  void frequencyChanged(double);
public slots:
  
};

#endif // SPECTRUMWINDOW_H
