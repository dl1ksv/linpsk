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
 ***************************************************************************/

#include "csquelch.h"
#include "color.h"
#include <QPalette>
#include <QRadioButton>
#include <QLabel>
#include <QPainter>
#include <QColor>
#include <QPolygon>
#include <QLabel>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QStyle>
#include <QCleanlooksStyle>


#define Cyan  color[127]
#define Yellow color[192]

mySlider::mySlider ( QWidget *parent ) :
    QSlider ( Qt::Vertical, parent )
{
  setStyle(new QCleanlooksStyle); // Has each style handles the drawing different,
                                  // I fix the slder style
  setTickInterval ( 10 );
  setTickPosition ( QSlider::NoTicks );
  setMinimum ( 0 );
  setMaximum ( 100 );

  setValue ( 50 );
  SquelchLevel = 0;

}

mySlider::~mySlider()
{}
/** Painting Slider Background depending on Threshold and Signal Strength
*/

void mySlider::paintEvent ( QPaintEvent * )
{
  QStylePainter p;
  int pos;
  p.begin ( this );
// Get Parameters of the Slider
  initStyleOption ( &option );
  pos=option.sliderPosition;
  option.sliderPosition=SquelchLevel;
  option.type = QStyleOption::SO_Slider;
  option.subControls = QStyle::SC_SliderGroove;
  if ( SquelchLevel < pos )
    option.palette.setColor( QPalette::Normal,QPalette::Highlight,Qt::yellow);
  else
    option.palette.setColor( QPalette::Normal,QPalette::Highlight,Qt::cyan);
  p.drawComplexControl ( QStyle::CC_Slider, option );
  option.sliderPosition=pos;
  option.subControls = QStyle::SC_SliderHandle;
  option.palette.setColor ( QPalette::Button, Qt::red );
  p.drawComplexControl ( QStyle::CC_Slider, option );
  p.end();

}

void mySlider::setSquelchLevel ( int level )
{
  SquelchLevel = level;
  repaint();
}

int mySlider::getThreshold()
{
  return value();
}

/*
 *  Constructs a CSquelch which is a child of 'parent', with the
 *  name 'name'.'
 */
CSquelch::CSquelch (QWidget* parent )
    : QGroupBox ( parent )
{
  setAlignment ( int ( Qt::AlignHCenter ) );

  Activate = new QRadioButton ( tr ( "On/Off" ), this );
  Activate->setGeometry ( QRect ( 120, 340, 60, 41 ) );


  Squelch = new mySlider ( this );
//connect(Squelch,SIGNAL(sliderMoved(int)),this,SLOT(setThreshold(int)));

  languageChange();

}

/*
 *  Destroys the object and frees any allocated resources
 */
CSquelch::~CSquelch()
{
  // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CSquelch::languageChange()
{
  setTitle ( tr ( "Squelch" ) );
}
void CSquelch::resizeEvent ( QResizeEvent * )
{
  calculateSizeofComponents();
}

void CSquelch::calculateSizeofComponents()
{
  /** Margins **/
#define TOPMARGIN 10
  /** WIDTH and Height in % **/
#define SQUELCHWIDTH 25
#define SQUELCHHEIGHT 70
#define BUTTONWIDTH 75
#define BUTTONHEIGHT 10

  int xpos, ypos, width, height, innerwidth, innerheight;

  width = this->width();
  height = this->height();
  innerwidth = width * SQUELCHWIDTH / 100;
  xpos = ( width - innerwidth ) / 2;
  ypos = height * TOPMARGIN / 100;
  innerheight = height * SQUELCHHEIGHT / 100;
  Squelch->setGeometry ( xpos, ypos, innerwidth, innerheight );
  ypos = ypos + innerheight + TOPMARGIN;
  innerwidth = width * BUTTONWIDTH / 100;
  xpos = ( width - innerwidth ) / 2;
  innerheight = height * BUTTONHEIGHT / 100;
  Activate->setGeometry ( xpos, ypos, innerwidth, innerheight );
  resize ( width, height );
}

void CSquelch::setSquelchLevel ( int level )
{
  Squelch->setSquelchLevel ( level );
//repaint();

}
int CSquelch::getThreshold()
{
  return Squelch->getThreshold();
}
bool CSquelch::getSquelchState()
{
  return Activate->isChecked();
}
void CSquelch::setSquelchState ( bool state )
{
  Activate->setChecked ( state );
}
void CSquelch::setThreshold ( int Threshold )
{
  Squelch->setValue ( Threshold );
}
