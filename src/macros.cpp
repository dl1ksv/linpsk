/***************************************************************************
                          Macros.cpp  -  description
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
 ***************************************************************************/

#include "macros.h"
#include "parameter.h"
#include "ctxbuffer.h"
#include <QDateTime>
#include <QMessageBox>
#include "constants.h"
#include <QFile>
#include <QTextStream>
extern Parameter settings;

Macros::Macros()
{

  Words << "@CALLSIGN@" << "@DATE@" << "@Replace by filename@" << "@RX@" << "@THEIRCALL@" << "@THEIRNAME@" << "@TIMELOCAL@" << "@TIMEUTC@" << "@TX@" << "@RSTGIVEN@" << "@RSTRCVD@";
  NumberofMacros = 0;
}
Macros::~Macros()
{
}
void Macros::insert ( QString Name, QString Definition, QString Acc, int position )
{
  if ( position <= NumberofMacros )
  {
    MacroNames.replace ( position - 1, Name );
    MacroText.replace ( position - 1, Definition );
    Accelerator.replace ( position - 1, Acc );
  }
  else
  {
    MacroNames << Name;
    MacroText << Definition;
    Accelerator << Acc;
  }
  NumberofMacros++;

}
void Macros::executeMacro ( int MacroNumber, CTxBuffer *TxBuffer )
{
  QString Macro = MacroText[MacroNumber];
  QString Token;

  int anzahl = Macro.count ( '@' ) / 2;
  if ( anzahl > 0 )
  {
    int indexbis, indexvon = 0;
    for ( int i = 0; i < anzahl; i++ )
    {
      indexbis = Macro.indexOf ( '@', indexvon );
      indexvon = indexbis;
      indexbis = Macro.indexOf ( '@', indexvon + 1 );
      Token = Macro.mid ( indexvon, indexbis - indexvon + 1 );
      if ( Token == Words[0] )         // callsign
      {
        Macro.replace ( indexvon, Words[0].length(), settings.callsign );

      }
      else
        if ( Token == Words[1] )         // Date
        {
          QDate t = QDate::currentDate();
          Macro.replace ( indexvon, Words[1].length(), t.toString ( QString ( "d.MM.yyyy" ) ) );

        }
        else
          if ( Token == Words[3] )         // RX
          {
            Macro.remove ( indexvon, 4 );
            if ( settings.Status == ON )
            {
              TxBuffer->insert ( Macro, indexvon );
              emit StartRx();
            }
            return;
          }
          else
            if ( Token == Words[4] )        // Remote callsign
            {
              Macro.replace ( indexvon, Words[4].length(), settings.QslData->RemoteCallsign );

            }
            else
              if ( Token == Words[5] )        // Remote op's name
              {
                Macro.replace ( indexvon, Words[5].length(), settings.QslData->OpName );

              }
              else
                if ( Token == Words[6] )        // Time Local
                {
                  QDateTime t;
                  t = QDateTime::currentDateTime();

                  Macro.replace ( indexvon, Words[6].length(), t.toString ( "h:mm" ) );

                }
                else
                  if ( Token == Words[7] )        // Time UTC
                  {
                    QDateTime t;
                    t = QDateTime::currentDateTime();
                    t = t.addSecs ( settings.timeoffset * 3600 );
                    Macro.replace ( indexvon, Words[7].length(), t.toString ( "h:mm" ) );
                  }
                  else
                    if ( Token == Words[8] )        // TX
                    {
                      if ( settings.Status == UNDEF )
                      {
                        QMessageBox::warning ( 0, "Warning", "Please listen before transmitting",
                                               QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton );

                        return;
                      }
                      Macro.remove ( indexvon, 4 );
                      if ( settings.Status == OFF )
                        emit StartTx();
                    }
                    else
                      if ( Token == Words[9] )        // RST given
                      {
                        Macro.replace ( indexvon, Words[9].length(), settings.QslData->MyRST );

                      }
                      else
                        if ( Token == Words[10] )        // RST received
                        {
                          Macro.replace ( indexvon, Words[10].length(), settings.QslData->HisRST );

                        }
                        else  // Must be File
                        {
                          Macro.remove ( indexvon, Token.length() );
                          if ( indexvon > 0 )
                          {
                            TxBuffer->insert ( Macro, indexvon );
                            Macro.remove ( 0, indexvon );
                            indexvon = 0;
                          }
                          Token = Token.mid ( 1, Token.length() - 2 );
                          QFile MacroFile ( settings.Directory + QString ( "/" ) + Token );
                          if ( MacroFile.open ( QIODevice::ReadOnly ) )
                          {
                            QTextStream line ( &MacroFile );
                            while ( !line.atEnd() )
                            {
                              QString Txstring = line.readLine();
                              if(settings.autoCrLf) // AutoCrLF ??
                                Txstring += '\r';
                              Txstring += "\n";
                              TxBuffer->insert ( Txstring, Txstring.length() );
                            }
                            MacroFile.close();
                          }
                          else
                            QMessageBox::warning ( 0, "Warning", "File " + Token + " not found ",
                                                   QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton );

                        }
    }


  }
  TxBuffer->insert ( Macro, Macro.length() );

}

void Macros::deleteMacro ( int number )
{
  if ( number < 0 || number >= NumberofMacros )
    return;

  MacroNames.erase ( MacroNames.begin() + number );
  MacroText.erase ( MacroText.begin() + number );
  Accelerator.erase ( Accelerator.begin() + number );

  if ( NumberofMacros > 0 )
    NumberofMacros--;
}

QString Macros::getMacroName ( int number )
{

  if ( number < 0 || number >= NumberofMacros )
    return QString ( "" );
  QString s = MacroNames.at ( number );
  return  s;
}
int Macros::count()
{
  return NumberofMacros;
}
QString Macros::getDefinition ( int number )
{

  if ( number < 0 || number >= NumberofMacros )
    return QString ( "" );
  return MacroText[number];
}
QString Macros::getAccelerator ( int number )
{
  if ( number < 0 || number >= NumberofMacros )
    return QString ( "" );
  return Accelerator[number];
}
int Macros::Keywordcount()
{
  return Words.size();;
}
QString Macros::getKeyword ( int number )
{
  if ( number < 0 || number >= Keywordcount() )
    return QString ( "" );
  return Words[number];
}
void Macros::setDefinition ( QString s, int position )
{
  if ( position < MacroText.size() )
    MacroText.replace ( position, s );
  else
    MacroText << s;
}
void Macros::setAccelerator ( QString s, int position )
{
  if ( position < Accelerator.size() )
    Accelerator.replace ( position, s );
  else
    Accelerator << s;
}
void Macros::setMacroName ( QString s, int MacroNumber )
{
  if ( MacroNumber < MacroNames.size() )
    MacroNames.replace ( MacroNumber, s );
  else
    MacroNames << s;
}
QStringList Macros::getKeyWordList()
{
  return Words;
}
QStringList Macros::getMacroList()
{
  return MacroNames;
}
