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

#ifndef WATERFALLWINDOW_H
#define WATERFALLWINDOW_H

#include <QFrame>

class QPixmap;
class WaterfallWindow : public QFrame
{
  Q_OBJECT
public:
  explicit WaterfallWindow(QWidget *parent = 0);
  void plotWaterfall(float *fftdata);
  void mousePressEvent ( QMouseEvent *e );

protected:
  void resizeEvent( QResizeEvent * );
  void paintEvent(QPaintEvent *);
  void wheelEvent ( QWheelEvent * e );

private:
  QPixmap *pwaterfall;
  
signals:
 void frequencyChanged(double);
 void frequencyChanged(int);
public slots:
  
};

#endif // WATERFALLWINDOW_H
