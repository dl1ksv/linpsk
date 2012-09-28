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

#include "ctxdisplay.h"


#include "cledbutton.h"
#include "ctxwindow.h"
#include "frequencyselect.h"
/* 
 *  Constructs a CTxDisplay which is a child of 'parent', with the 
 *  name 'name'.' 
 */
CTxDisplay::CTxDisplay( QWidget* parent )
    : QFrame( parent)
{
    setMinimumSize( QSize( 540, 96 ) );
    setBaseSize( QSize( 540, 96 ) );
    setFrameShape( QFrame::Panel );
    setFrameShadow( QFrame::Sunken );

    TxWindow = new CTxWindow( this );

    TxFreq = new FrequencySelect( this );
    TxFreq->setFunctionText("Net");
    TxFreq->setTitle("Tx Freq");
    TxFreq->setAfcDisplayMode(Narrow);
    TxFreq->setAfcMode(Narrow);
//    TxFunctions = new CTxFunctions( "Rx / Tx", this );
     TxFunctions =new CLedButton(this);
    connect(TxFunctions,SIGNAL(startRx()),this,SIGNAL(startRx()));
    connect(TxFunctions,SIGNAL(startTx()),this,SIGNAL(startTx()));

    TxFunctions->setGeometry( QRect( 100, 20, 51, 71 ) );
    languageChange();
}

/*
 *  Destroys the object and frees any allocated resources
 */
CTxDisplay::~CTxDisplay()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CTxDisplay::languageChange()
{
}

void CTxDisplay::calculateSizeofComponents()
{
/** in percent of whole widget **/
/** TX Function part **/
#define TXFUNCTIONWIDTH 10
/** TxFrequency (Width) **/
#define TXFREQWIDTH 18
#define TXFREQHEIGHT 70
/** Left and Right Margin **/
#define LEFTANDRIGHTMARGIN 1
/** Top and Bottom Margin **/
#define TOPANDBOTTOMMARGIN 2
/** Inner distance **/
#define distance 1

int xpos,ypos,width,height,innerheight,innerwidth;
width=this->width();
height=this->height();
xpos=width*LEFTANDRIGHTMARGIN/100;

/** Frequency select Box **/

innerheight=height*TXFREQHEIGHT/100;
ypos=(height-innerheight)/2;
innerwidth=width*TXFREQWIDTH/100;

TxFreq->setGeometry(xpos,ypos,innerwidth,innerheight);

/** TX- Functions **/


//ypos=height*TOPANDBOTTOMMARGIN/100;
//innerheight=height-2*ypos;
innerheight=height*TXFREQHEIGHT/100;
xpos=xpos+innerwidth+width*distance/100;
innerwidth=width*TXFUNCTIONWIDTH/100;

TxFunctions->setGeometry(xpos,ypos,innerwidth,innerheight);
/** TX- Window **/
xpos=xpos+innerwidth+width*distance/100;
innerwidth=width-xpos-width*LEFTANDRIGHTMARGIN/100;
TxWindow->setGeometry(xpos,ypos,innerwidth,innerheight);

}

void CTxDisplay::resizeEvent( QResizeEvent * )
{
calculateSizeofComponents();

}
void CTxDisplay::abbruch()
{
TxFunctions->setStatus(UNDEF);
}
