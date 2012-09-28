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
 *   The PSK part is based on WinPSK 1.0 by Moe Wheatly, AE4JY             *
 ***************************************************************************/
#include "frequencyselect.h"

#include <QRadioButton>
#include <QSpinBox>


/* 
 *  Constructs a FrequencySelect which is a child of 'parent', with the 
 *  name 'name'.' 
 */
FrequencySelect::FrequencySelect( QWidget* parent , AfcMode WithMode)
    : QGroupBox( parent )
{
 //   setFrameShape( QGroupBox::WinPanel );
 //   setFrameShadow( QGroupBox::Raised );
    setAlignment( Qt::AlignCenter );

    Activate = new QRadioButton(  "Activate", this );
Activate->setAutoExclusive(false);
    AfcWide = new QRadioButton("Wide", this);
AfcWide->setAutoExclusive(false);
    modus=Wide;
    switch (WithMode)
    {
     case Off:
      Activate->hide();
      AfcWide->hide();
      break;
     case Narrow:
      Activate->show();
      AfcWide->hide();
      break;
     case Wide:
      Activate->show();
      AfcWide->show();
      break;
    }
    Frequency = new QSpinBox( this );
    Frequency->setMaximum( 2500 );
    Frequency->setMinimum( 300 );
    frequency = 1000;
    Frequency->setValue( (int) frequency );
    languageChange();
    connect(Frequency,SIGNAL(valueChanged(int)),this,SLOT(checkFrequency(int)));
    connect(AfcWide,SIGNAL(clicked()),this,SLOT(toggleWide()));
    connect(Activate,SIGNAL(clicked()),this,SLOT(toggleActivate()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
FrequencySelect::~FrequencySelect()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FrequencySelect::languageChange()
{
    Activate->setText( tr( "Frequency" ) );
}


double FrequencySelect::getFrequency()
{
return frequency;
}


void FrequencySelect::setFunctionText( QString Text)
{
Activate->setText(Text);
}



void FrequencySelect::calculateSizeofComponents()
{
#define LEFTANDRIGHTMARGIN 6
#define TOPANDBOTTOM 30
#define LABELHEIGHT 35
int width,height,xpos,ypos,innerwidth,innerheight;
width=this->width();
height=this->height();
xpos=width*LEFTANDRIGHTMARGIN/100;
ypos=height*TOPANDBOTTOM/100;
innerwidth=width-2*xpos;
innerheight=height*LABELHEIGHT/100;
Frequency->setGeometry(xpos,ypos,innerwidth,innerheight);
ypos=ypos+innerheight;
innerwidth=innerwidth/2;
xpos = xpos -2;
Activate->setGeometry(xpos,ypos,innerwidth,innerheight);
AfcWide->setText("Wide");
AfcWide->setGeometry(xpos+innerwidth+1,ypos,innerwidth,innerheight);
  
resize(width,height);

}

void FrequencySelect::resizeEvent( QResizeEvent * )
{
calculateSizeofComponents();
}


void FrequencySelect::setFrequency( double freq)
{
Frequency->setValue( (int) freq);
frequency=freq;
}

void FrequencySelect::checkFrequency(int freq)
{
if ((unsigned int) freq != (unsigned int)frequency)
  {
   frequency= freq;  
   emit FrequencyChanged(frequency);
  }
}

AfcMode FrequencySelect::getAfcMode()
{
return modus;
}

void FrequencySelect::setAfcMode(AfcMode mode)
{
modus=mode;
if ( modus == Narrow )
 {
  Activate->setChecked(true);
  AfcWide->setChecked(false);
 }
 else if ( modus == Wide )
  {
   Activate->setChecked(false);
   AfcWide->setChecked(true);
  }
  else
   {
    Activate->setChecked(false);
    AfcWide->setChecked(false);
   }
}
void FrequencySelect::setAfcDisplayMode(AfcMode Mode)
{
 switch (Mode)
 {
  case Off:
   Activate->hide();
   AfcWide->hide();
   break;
  case Narrow:
   Activate->show();
   AfcWide->hide();
   break;
  case Wide:
   Activate->show();
   AfcWide->show();
   break;
 } 
}

void FrequencySelect::toggleWide()
{
 bool OnOff=AfcWide->isChecked();
 Activate->setChecked(false);
 AfcWide->setChecked(OnOff);
 if( OnOff )
  modus=Wide;
 else
  modus=Off;  
}
void FrequencySelect::toggleActivate()
{
 bool OnOff=Activate->isChecked();
 AfcWide->setChecked(false);
 Activate->setChecked(OnOff);
 if( OnOff )
  modus=Narrow;
 else
  modus=Off;  
}

