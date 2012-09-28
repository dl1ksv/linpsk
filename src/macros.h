/***************************************************************************
                          keywords.h  -  description
                             -------------------
    begin                : Sam Mai 10 2003
    copyright            : (C) 2003 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   The PSK part is based on WinPSK 1.0 by Moe Wheatley, AE4JY            *              
 ***************************************************************************/

#ifndef MACROS_H
#define MACROS_H
#include <QObject>

#include <QStringList>

/**Contains the Keywords for Macro Processing 
Offers the methods for processing the Keywords
  *@author Volker Schroer
  */
class CTxBuffer;
class Macros : public QObject
 {
  Q_OBJECT
public: 
	Macros();
	~Macros();
void executeMacro(int,CTxBuffer *);
void insert(QString ,QString , QString,int);
void deleteMacro(int);
QString getMacroName(int);
QString getDefinition(int);
void setDefinition(QString,int);
void setAccelerator(QString,int);
void setMacroName(QString,int);
QString getAccelerator(int);
QString getKeyword(int);
QStringList getKeyWordList();
QStringList getMacroList();
int count();
int Keywordcount();
private:	
QStringList Words;
QStringList MacroNames ;
QStringList MacroText;
QStringList Accelerator;
int NumberofMacros;
signals:
void StartRx();
void StartTx();
  
};

#endif
