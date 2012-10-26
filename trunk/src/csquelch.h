/***************************************************************************
                          |FILENAME|  -  description
                             -------------------
    begin                : |DATE|
    copyright            : (C) |YEAR| by |AUTHOR|
    email                : |EMAIL|
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   The PSK part is based on WinPSK 1.0 by Moe Wheatley, AE4JY            *
 ***************************************************************************/

#ifndef CSQUELCH_H
#define CSQUELCH_H


#include <QSlider>
#include <QGroupBox>
class QRadioButton;
#include <QStyleOptionSlider>

class mySlider : public QSlider {
   Q_OBJECT
public:
	mySlider(QWidget *parent);
 virtual ~mySlider();
 void setSquelchLevel(int);
 int getThreshold();
private:

int SquelchLevel;
QStyleOptionSlider option;
protected:
/** Painter for Squelch  */
  void paintEvent(QPaintEvent *);

};


class CSquelch : public QGroupBox
{
    Q_OBJECT

public:
    CSquelch(QWidget* parent = 0);
    ~CSquelch();
    void setSquelchLevel(int);
    int getThreshold();
    bool getSquelchState();
    void setSquelchState(bool);
    
public slots:
void setThreshold(int);

protected:
    void resizeEvent( QResizeEvent * );


protected slots:
    virtual void languageChange();

private:
    void calculateSizeofComponents();
    mySlider* Squelch;
    QRadioButton* Activate;

 


};

#endif // CSQUELCH_H
