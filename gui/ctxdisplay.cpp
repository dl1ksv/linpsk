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
#include "txwindow.h"
#include "frequencyselect.h"
/* 
 *  Constructs a CTxDisplay which is a child of 'parent', with the 
 *  name 'name'.' 
 */
CTxDisplay::CTxDisplay( QWidget* parent )
    : QFrame( parent), Ui::CTxDisplay()
{
  setupUi(this);

    TxFreq->setFunctionText("Net");
    TxFreq->setAfcDisplayMode(Narrow);
    TxFreq->setAfcMode(Narrow);
    connect(TxFunctions,SIGNAL(startRx()),this,SIGNAL(startRx()));
    connect(TxFunctions,SIGNAL(startTx()),this,SIGNAL(startTx()));
    connect(TxFunctions,SIGNAL(abortTx()),this,SIGNAL(abortTx()));
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
void CTxDisplay::resizeEvent( QResizeEvent * )
{
TxFunctions->setFixedSize(QSize(height()-20,height()-20));
}

void CTxDisplay::insert(QString s)
{
 txWindow->insertString(s);
}
void CTxDisplay::setTxFocus()
{
 txWindow->setFocus();
}
void CTxDisplay::switch2Rx()
{
  TxFunctions->setStatus(SW);
}
