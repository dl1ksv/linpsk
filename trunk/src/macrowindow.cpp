/***************************************************************************
                          macros.cpp  -  description
                             -------------------
    begin                : Sam Mai 3 2003
    copyright            : (C) 2003 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "macrowindow.h"
#include <QPushButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <QScrollBar>
#include "parameter.h"
#include <QScrollArea>

#include "macros.h"

extern Parameter settings;

MacroWindow::MacroWindow ( QWidget *parent ) : QFrame ( parent )
{
  scrollArea = new QScrollArea(this);
  scrollArea->setFixedSize(this->width()-3,this->height()-3);
  Area = new QGroupBox ( this);
  Area->setAlignment ( Qt::AlignHCenter );
  Area->setFixedWidth(this->width()-5);
  DisplayBox = new QButtonGroup ( Area );
  scrollArea-> setWidget ( Area );
  scrollArea-> setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  scrollArea-> setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );
  NumberOfMacros=0;
  scrollArea->show();
  connect ( DisplayBox, SIGNAL ( buttonClicked ( int ) ), this, SIGNAL ( callMacro ( int ) ) );
}
MacroWindow::~MacroWindow()
{}


void MacroWindow::resizeEvent ( QResizeEvent * )
{
	if ( NumberOfMacros == 0 )
		return; // Nothing to do


	int width, height, y;

    width = this->width()-3;
    scrollArea->setFixedSize(width,this->height()-3);
//    y = scrollArea->verticalScrollBar()->width();
    width = width - 16;
    if ( width <= 0) // In an early state width may be to small
      return;
	y = 5;
	if ( settings.ApplicationFont != 0 )
		height = QFontMetrics ( *settings.ApplicationFont ).height();
	else
		height = QFontMetrics ( this->font() ).height();

    if(NumberOfMacros > 0)
	for ( int i = 0; i < NumberOfMacros; i++ )
	{
		QPushButton *pb;
		pb = static_cast<QPushButton *> ( DisplayBox->button ( i ) );
//  if (settings.ApplicationFont != 0)
//   pb->setFont(*settings.ApplicationFont);
        if (pb > 0)
        {
		pb->setFixedSize ( width - 20, height );
		pb->move ( 8, y );
        }
		y = y + height + 5;
    }

    Area->setFixedSize(width,y);
    Area->show();
}

void MacroWindow::updateMacroWindow ( Macros *Macro )
{
	QPushButton *pb;

	NumberOfMacros = Macro->count();
    pb = new QPushButton ( Macro->getMacroName ( NumberOfMacros - 1 ), this );
	DisplayBox->addButton ( pb, NumberOfMacros - 1 );

	pb->show();
	resizeEvent ( 0 );
}
void MacroWindow::fontChange ( const QFont & )
{
	/**
	 if (settings.ApplicationFont != 0)
	 {
	  DisplayBox->setFont(*settings.ApplicationFont);
	  int anzahl = DisplayBox->count();
	  for(int i=0; i < anzahl; i++)
	  {
	   QButton *pb;
	   pb=DisplayBox->find(i);
	   pb->setFont(*settings.ApplicationFont);
	  }
	 }
	 **/
}
void MacroWindow::setMacroWindow ( Macros *M )
{
	int i, anzahl;
    QPushButton *pb = 0;
    if (NumberOfMacros > 0)
    {
     anzahl = DisplayBox->buttons().size();
     if ( anzahl > 0)
     for ( i = 0; i < anzahl; i++ )
     {
      pb= static_cast<QPushButton *> ( DisplayBox->button ( i ) );
      DisplayBox->removeButton (pb  );
      delete pb;
     }
    }
	NumberOfMacros = M->count();

	if ( NumberOfMacros > 0 )
    {
      scrollArea->show();
      for ( i = 0; i < NumberOfMacros;i++ )
      {
        pb = new QPushButton ( M->getMacroName ( i ), Area );
        DisplayBox->addButton ( pb, i );
        pb->show();
      }
    }
    else
    scrollArea->hide();
	resizeEvent ( 0 );
}
