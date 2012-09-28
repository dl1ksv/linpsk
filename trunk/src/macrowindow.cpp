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

#include "macros.h"

extern Parameter settings;

MacroWindow::MacroWindow ( Macros *Macro, QWidget *parent ) : QScrollArea ( parent )
{
	QPushButton *pb;
	Area = new QGroupBox ( this );
	Area->setAlignment ( Qt::AlignHCenter );
	DisplayBox = new QButtonGroup ( Area );
	setWidget ( Area );
	setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );
	NumberOfMacros = Macro->count();
	if ( NumberOfMacros > 0 )
		for ( int i = 0; i < NumberOfMacros;i++ )
		{

			pb = new QPushButton ( Macro->getMacroName ( i ), Area );
			DisplayBox->addButton ( pb, i );
		}
	else
		Area->hide();
	connect ( DisplayBox, SIGNAL ( buttonClicked ( int ) ), this, SIGNAL ( callMacro ( int ) ) );
}
MacroWindow::~MacroWindow()
{}


void MacroWindow::resizeEvent ( QResizeEvent * )
{
	if ( NumberOfMacros == 0 )
		return; // Nothing to do


	int width, height, y;

	width = this->width();
	width = width - verticalScrollBar()->width();
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

		pb->setFixedSize ( width - 20, height );
		pb->move ( 8, y );
		y = y + height + 5;
	}
	Area->setFixedSize ( width-5, y );
	Area->show();
}

void MacroWindow::updateMacroWindow ( Macros *Macro )
{
	QPushButton *pb;

	NumberOfMacros = Macro->count();
	pb = new QPushButton ( Macro->getMacroName ( NumberOfMacros - 1 ), Area );
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
	QPushButton *pb;
	anzahl = DisplayBox->buttons().size();
if ( anzahl > 0)
	for ( i = 0; i < anzahl; i++ )
{
 pb= static_cast<QPushButton *> ( DisplayBox->button ( i ) );
		DisplayBox->removeButton (pb  );
delete pb;
}
	NumberOfMacros = M->count();

	if ( NumberOfMacros > 0 )
		for ( i = 0; i < NumberOfMacros;i++ )
		{
			pb = new QPushButton ( M->getMacroName ( i ), Area );
			DisplayBox->addButton ( pb, i );
pb->show();
		}
	else
		Area->hide();
	resizeEvent ( 0 );
}
