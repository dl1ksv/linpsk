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

#include "crecording.h"

#include <qvariant.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>


/* 
 *  Constructs a CRecording which is a child of 'parent', with the 
 *  name 'name'.' 
 */
CRecording::CRecording( QWidget* parent )
    : QGroupBox( parent )
{

    Record = new QRadioButton( this);
    languageChange();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CRecording::~CRecording()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CRecording::languageChange()
{

    Record->setText( tr( "Record QSO" ) );
}
void CRecording::resizeEvent( QResizeEvent * )
{
calculateSizeofComponents();

}

void CRecording::calculateSizeofComponents()
{
/** Margins **/
#define LEFTMARGIN 5
#define TOPMARGIN 30
int innerwidth,innerheight,xpos,ypos,width,height;

width=this->width();
height=this->height();
xpos=width*LEFTMARGIN/100;
ypos=height*TOPMARGIN/100;

innerwidth=width-2*xpos;
innerheight=height-3*ypos/2;
Record->setGeometry(xpos,ypos,innerwidth,innerheight);
}
