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



#include "ctxbuffer.h"
#include "ctxwindow.h"

#include <QPushButton>
#include <QLineEdit>
#include <QApplication>
#include <QKeyEvent>
#include "parameter.h"

extern Parameter settings;
/*
 *  Constructs a CTxWindow which is a child of 'parent', with the
 *  name 'name'.'
 */


CTxWindow::CTxWindow ( QWidget* parent )
    : QFrame ( parent )
{
  setFrameShape ( QFrame::Panel );
  setFrameShadow ( QFrame::Sunken );
//    setLineWidth( 2 );
//    setMargin( 2 );
//    setMidLineWidth( 1 );

  Clear = new QPushButton ( "TXClear", this );


  Zeile1 = new QLineEdit ( this );
  Zeile1->setFrame ( false );

  Zeile2 = new QLineEdit ( this );
  Zeile2->setFrame ( false );


  Zeile3 = new QLineEdit ( this );
  Zeile3->setFrame ( false );
  languageChange();
  Zeile = 0;
  Spalte = 0;
  Zeile1->setFocusProxy ( this );
  Zeile2->setFocusProxy ( this );
  Zeile3->setFocusProxy ( this );
  setFocusPolicy(Qt::ClickFocus);

  // signals and slots connections
  connect ( Clear , SIGNAL ( clicked() ), this, SLOT ( clear() ) );
  connect ( Zeile1, SIGNAL ( returnPressed() ), this, SLOT ( gotoNextLine() ) );
  connect ( Zeile2, SIGNAL ( returnPressed() ), this, SLOT ( gotoNextLine() ) );
  connect ( Zeile3, SIGNAL ( returnPressed() ), this, SLOT ( gotoNextLine() ) );

  setEnabled ( true );
}

void CTxWindow::clear()
{
  Zeile1->clear();
  Zeile2->clear();
  Zeile3->clear();
  Zeile = 0;
  Spalte = 0;
  Zeile1->setCursorPosition ( Spalte );
  Buffer->clear();
}


void CTxWindow::resizeEvent ( QResizeEvent * )
{
  calculateSizeofComponents();
}

void CTxWindow::gotoNextLine()
{
  if ( Zeile == 2 ) // Reached last Line
  {
    Zeile1->setText ( Zeile2->text() );
    Zeile2->setText ( Zeile3->text() );
    Zeile3->clear();
    Spalte = 0;
    Zeile3->setCursorPosition ( Spalte );
  }
  else
  {
    Zeile++;
    Spalte = 0;
    if ( Zeile == 1 )
      Zeile2->setCursorPosition ( Spalte );
    else
      Zeile3->setCursorPosition ( Spalte );
  }
}

void CTxWindow::calculateSizeofComponents()
{
#define MARGINS 3
#define DISTANCE 2
#define BUTTOMWIDTH 20
  int xpos, ypos, width, height, innerheight, innerwidth;
  width = this->width();
  height = this->height();
  xpos = DISTANCE;
  ypos = height * MARGINS / 100;
  innerheight = ( height - 2 * ypos - height * DISTANCE / 100 ) / 4;
  Zeile1->setGeometry ( xpos, ypos, width - 2*xpos, innerheight );
  ypos = ypos + innerheight - 2;
  Zeile2->setGeometry ( xpos, ypos, width - 2*xpos, innerheight );
  ypos = ypos + innerheight - 2;
  Zeile3->setGeometry ( xpos, ypos, width - 2*xpos, innerheight );
  ypos = ypos + innerheight + height * DISTANCE / 100;
  innerwidth = 100;
  xpos = width - innerwidth - 2 * xpos;
  Clear->setGeometry ( xpos, ypos, innerwidth, innerheight );
}

void CTxWindow::mousePressEvent ( QMouseEvent * )
{
  if ( Zeile == 0 ) Zeile1->setCursorPosition ( Spalte );
  if ( Zeile == 1 ) Zeile2->setCursorPosition ( Spalte );
  if ( Zeile == 2 ) Zeile3->setCursorPosition ( Spalte );
}



/*
 *  Destroys the object and frees any allocated resources
 */
CTxWindow::~CTxWindow()
{
  // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void CTxWindow::languageChange()
{
  Clear->setText ( tr ( "Clear" ) );
}


void CTxWindow::keyPressEvent ( QKeyEvent *e ) // Bearbeiten der Eingaben im
// TX Window
{
  unsigned char c;
  static bool CapsLock = false;
  if ( Buffer->Filled() )
  {
    QApplication::beep();
    e->ignore();
    return;
  }
  /**
  if ( e->state() & ControlButton )
   {
    if ( e->key() == Key_V )
     pasteText( QApplication::clipboard()->text( QClipboard::Clipboard ) );
   }
  else
  **/
  switch ( e->key() )
  {
    case Qt::Key_Enter:
    case Qt::Key_Return:
      gotoNextLine();
      if(settings.autoCrLf)
        Buffer->insert('\r');
      Buffer->insert ( '\n' );
      e->accept();
      break;
  case Qt::Key_Home:
    Buffer->insert('\r');
    e->accept();
    break;
    case Qt::Key_CapsLock:
      CapsLock = !CapsLock;
      e->accept();
      break;

    case Qt::Key_Backspace:
      c = '\b';
      backspace();
      e->accept();
      Buffer->insert ( c );
      break;

    default:
//    ButtonState cc=e->state();
//    if ( (cc == ShiftButton) )
      if ( ( e->modifiers() == Qt::ShiftModifier ) || CapsLock )
      {
        if ( !e->text().isEmpty() )
          c = e->text().toUpper().at ( 0 ).toAscii();
        else
          c = 0;
      }
      else
      {
        if ( !e->text().isEmpty() )
          c = e->text().toLower().at ( 0 ).toAscii();
        else
          c = 0;
      }
      if ( c != 0 )
      {
        Spalte++;
        if ( Spalte > 80 )
        {
          Buffer->insert ( '\n' ); //This newline Char will  be Transmitted
          gotoNextLine();
        }
        Buffer->insert ( c );
        insert ( c );
        e->accept();
      }
  } // End Case

} // keyPressEvent


void CTxWindow::backspace()
{

  QLineEdit * line;
  if ( Zeile == 0 )
    line = Zeile1;
  if ( Zeile == 1 )
    line = Zeile2;
  if ( Zeile == 2 )
    line = Zeile3;
  line->backspace();

}
void CTxWindow::insert ( unsigned char c )
{
  QLineEdit *line;
  if ( Zeile == 0 )
    line = Zeile1;
  if ( Zeile == 1 )
    line = Zeile2;
  if ( Zeile == 2 )
    line = Zeile3;
  QString s = line->text() + QString ( QChar ( c ) );
  line->setText ( s );
}
void CTxWindow::setTxBuffer ( CTxBuffer *p )
{
  Buffer = p;
}

void CTxWindow::pasteText ( const QString &Text )
{
  int length;
  length = Text.length();
  for ( int i = 0; i < length; i++ )
  {
    Spalte++;
    if ( Spalte > 80 )
    {
      Buffer->insert ( '\n' ); //This newline Char will  be Transmitted
      gotoNextLine();
    }
    char c = Text.at ( i ).toAscii();
    Buffer->insert ( c );
    insert ( c );
  }
}
