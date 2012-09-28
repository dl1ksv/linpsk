/***************************************************************************
                          crxwindow.cpp  -  description
                             -------------------
    begin                : Mon Mar 6 2000
    copyright            : (C) 2000 by Volker Schroer
    email                : DL1KSV@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "crxwindow.h"
#include <QLineEdit>
#include <QFontMetrics>
#include <QMenu>

#include "parameter.h"

extern Parameter settings;
/*
 *  Constructs a CRxWindow which is a child of 'parent', with the
 *  name 'name'.'
 */
CRxWindow::CRxWindow ( QWidget* parent )
    : QScrollArea ( parent )
{
  QFontMetrics fm ( font() );
  rowHeight = fm.height();
  DisplayBox = new QWidget ( this );
  DisplayBox->setFixedSize ( 642, RXWINDOWBUFFER*rowHeight );
  DisplayBox->setContextMenuPolicy ( Qt::CustomContextMenu );
  connect ( DisplayBox, SIGNAL ( customContextMenuRequested ( QPoint ) ), this, SLOT ( contextMenu ( QPoint ) ) );

  setWidget ( DisplayBox );

  setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );

  trigger = false;
  TriggerText = "";
  TexttoTrigger = "";
  save = false;
  stream = 0;
  Row = 0;
  Column = 0;
  AutoScroll = true;
  DisplayLineHeight = rowHeight - 2;
  for ( int i = 0;i < RXWINDOWBUFFER;i++ )
  {

    ScrollBuffer[i] = new QLineEdit ( DisplayBox );
    ScrollBuffer[i]->move ( 1, 1 + DisplayLineHeight*i );

    ScrollBuffer[i]->setFrame ( false );
    ScrollBuffer[i]->setReadOnly ( true );
    ScrollBuffer[i]->setFixedHeight ( rowHeight );
    ScrollBuffer[i]->setContextMenuPolicy ( Qt::NoContextMenu );
    ScrollBuffer[i]->show();
  }
  setBackgroundRole ( ScrollBuffer[0]->backgroundRole() );
  menu = new QMenu ( tr ( "Log value" ), DisplayBox );
  QAction *A = menu->addAction ( tr ( "Callsign" ) );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( copyCallSign() ) );
  A = menu->addAction ( "QTH" );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( copyQTH() ) );
  A = menu->addAction ( "Name" );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( copyName() ) );
  A = menu->addAction ( tr ( "Locator" ) );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( copyLocator() ) );
  A = menu->addAction ( "Raport" );
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( copyRST() ) );

  selectedString.clear();

}

/*
 *  Destroys the object and frees any allocated resources
 */
CRxWindow::~CRxWindow()
{
  for ( int i = 0;i < RXWINDOWBUFFER;i++ )
  {

    if ( ScrollBuffer[i] )
      delete ScrollBuffer[i];
    ScrollBuffer[i] = 0;
  }

  // no need to delete child widgets, Qt does it all for us
}
void CRxWindow::updateRx ( char c )
{
  switch ( c )
  {
    case '\n':
    case '\r':
      NeueZeile();
      break;

    case '\b':
      if ( Column > 0 )
      {
        Column--;
        QString s = ScrollBuffer[Row]->text();
        s.truncate ( Column );
        ScrollBuffer[Row]->setText ( s );
        if ( trigger && ( settings.Status == OFF ) )
        {
          if ( TriggerText.length() > 0 )
            TriggerText.remove ( TriggerText.length(), 1 );
        }
      }
      break;

    case '0':
      if ( settings.slashed0 )
        c = 0xF8;
    default:
      if ( trigger && ( settings.Status == OFF ) )
      {
        TriggerText.append ( c );
        if ( TriggerText.length() > TexttoTrigger.length() )
        {
          TriggerText.remove ( 0, 1 );
          if ( TriggerText.toLower() ==  TexttoTrigger.toLower() )
          {
            trigger = false; // Stop
            emit Triggered();
          }
        }
      }
      QString s = ScrollBuffer[Row]->text() + QString ( QChar ( c ) );
      ScrollBuffer[Row]->setText ( s );
      Column++;

      if ( Column >= 80 )
        NeueZeile();
  }


}

void CRxWindow::clearRxWindow()
{
  Column = 0;
  Row = 0;
  for ( int i = 0;i < RXWINDOWBUFFER;i++ )
    ScrollBuffer[i]->setText ( "" );
  ensureVisible ( 0, Row*rowHeight, 20, 15 );
}


void CRxWindow::removeLines ( int number2Remove )
{
  int i;
  for ( i = number2Remove; i < RXWINDOWBUFFER; i++ )
    ScrollBuffer[i-number2Remove]->setText ( ScrollBuffer[i]->text() );
  for ( i = RXWINDOWBUFFER - number2Remove;i < RXWINDOWBUFFER;i++ )
    ScrollBuffer[i]->setText ( "" );
}

void CRxWindow::NeueZeile()
{

  Column = 0;
  Row++;
  if ( Row >= RXWINDOWBUFFER )
  {
    if ( save && stream != 0 )
      for ( int i = 0;i < 10; i++ )
        *stream << ScrollBuffer[i]->text() << "\n";
    removeLines ( 10 );
    Row -= 10;
  }
  if ( ! AutoScroll )
    return;
//  ensureVisible ( 0, Row*rowHeight, 20, 15 );
  ensureWidgetVisible ( ( QWidget* ) ScrollBuffer[Row] );
}
void CRxWindow::setColor ( QColor color )
{

  for ( int i = 0;i < RXWINDOWBUFFER;i++ )
    ScrollBuffer[i]->setStyleSheet ( "QLineEdit{color: " + color.name() + "; }" );
}

void CRxWindow::fontChange ( const QFont & )
{
  QFontMetrics fm ( font() );
  for ( int i = 0;i < RXWINDOWBUFFER;i++ )
    ScrollBuffer[i]->setFixedHeight ( fm.height() );
}
void CRxWindow::activateTrigger ( QString s )
{
  trigger = true;
  TexttoTrigger = s;
  TriggerText = "";
}
void CRxWindow::deactivateTrigger()
{
  trigger = false;
}
bool CRxWindow::getTriggerStatus()
{
  return trigger;
}
QString CRxWindow::getTriggerText()
{
  return TexttoTrigger;
}

void CRxWindow::stopRecording()
{
  /** Save all ScrollBuffer in Buffer **/
  if ( save )
  {
    for ( int i = 0; i <= Row; i++ )
      *stream << ScrollBuffer[i]->text() << "\n";
    File.close();
    save = false;
    delete stream;
  }
}
void CRxWindow::startRecording ( QString Datei )
{
  if ( !save )
  {
    File.setFileName ( Datei );
    File.open ( QIODevice::WriteOnly );
    save = true;
    stream = new QTextStream ( &File );
  }
}
bool CRxWindow::getRecordingState()
{
  return save;
}

void CRxWindow::resizeEvent ( QResizeEvent * )
{
  int width = this->width();
  DisplayBox->setFixedWidth ( width );;
  for ( int i = 0;i < RXWINDOWBUFFER;i++ )
    ScrollBuffer[i]->setFixedWidth ( width );
}
void CRxWindow::contextMenu ( QPoint p )
{
  int selectedLine, selectedColumn;
  char c = 0xF8;
  QPoint p1 = QCursor::pos();
  selectedLine = p.y() / DisplayLineHeight;
  selectedColumn = ScrollBuffer[selectedLine]->cursorPositionAt ( QPoint ( p.x(), 2 ) );
  ScrollBuffer[selectedLine]->setCursorPosition ( selectedColumn );
  ScrollBuffer[selectedLine]->cursorWordForward ( true );
  selectedString = ScrollBuffer[selectedLine]->selectedText();
  selectedString = selectedString.replace ( QChar ( c ), QLatin1String ( "0" ) ); // Replace Slashed zero by zero
  if ( selectedString == QString ( " " ) )
  {
    ScrollBuffer[selectedLine]->setCursorPosition ( selectedColumn + 1 );
    ScrollBuffer[selectedLine]->cursorWordForward ( true );
    selectedString = ScrollBuffer[selectedLine]->selectedText();
  }
  qDebug ( "Copy %s", qPrintable ( selectedString ) );
  menu->exec ( p1 );
}
void CRxWindow::copyCallSign()
{
  emit copyCallSign ( selectedString );
}
void CRxWindow::copyQTH()
{
  emit copyQTH ( selectedString );
}
void CRxWindow::copyName()
{
  emit copyName ( selectedString );
}
void CRxWindow::copyLocator()
{
  emit copyLocator ( selectedString );
}
void CRxWindow::copyRST()
{
  emit copyRST ( selectedString );
}
