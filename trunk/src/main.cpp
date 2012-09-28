/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Sam Nov 23 11:37:46 CET 2002
    copyright            : (C) 2002 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   The PSK part is based on WinPSk 1.0 by Moe Wheatly, AE4JY             *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QApplication>
#include "linpsk.h"
#include "parameter.h"




Parameter settings;

int main ( int argc, char *argv[] )
{
	QApplication a ( argc, argv );

	LinPSK *w = new LinPSK ( 0 );
	w->show();
	a.connect ( &a, SIGNAL ( lastWindowClosed() ), &a, SLOT ( quit() ) );
	a.exec();
	return 0;
}
