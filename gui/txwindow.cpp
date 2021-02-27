/***************************************************************************
 *   Copyright (C) 2012 by Volker Schroer, DL1KSV                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "txwindow.h"
#include "ui_txwindow.h"
#include <QLineEdit>
#include <QKeyEvent>
#include "parameter.h"
#include "ctxbuffer.h"
#include <QFontMetrics>

extern Parameter settings;

TxWindow::TxWindow(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::TxWindow)
{
  QFontMetrics fm ( font() );
  int pixelwidth=82*fm.horizontalAdvance("A");
  int height=fm.height();
  ui->setupUi(this);
  zeile=0;
  spalte=0;
  for ( int i = 0;i < TXWINDOWBUFFER;i++ )
  {
    scrollBuffer[i] = new QLineEdit ( ui->txLines );
    scrollBuffer[i]->setFrame ( false );
    scrollBuffer[i]->setFocusProxy(this);
    scrollBuffer[i]->setMaximumHeight(2*height);
    scrollBuffer[i]->setFixedWidth(pixelwidth);
    ui->linesLayout->addWidget(scrollBuffer[i]);
  }
  ui->txArea->ensureWidgetVisible(scrollBuffer[zeile]);
}

TxWindow::~TxWindow()
{
  delete ui;
}
void TxWindow::insert ( unsigned char c )
{
  QString s = scrollBuffer[zeile]->text() + QString ( QChar ( c ) );
  scrollBuffer[zeile]->setText ( s );
  spalte++;
  if(c == '\n')
    {
 //     Buffer->insert(c);
      gotoNextLine();
    }
  else if (spalte >= 80)
    {
      if(settings.autoCrLf)
        Buffer->insert('\r');
      Buffer->insert ( '\n' );
      gotoNextLine();
    }
}

void TxWindow::insertString (QString string )
{
  for(int i=0;i < string.length();i++)
    insert(string.at(i).toLatin1());
}
void TxWindow::setTxBuffer ( CTxBuffer *p )
{
  Buffer = p;
}
void TxWindow::gotoNextLine()
{
  int i;
  zeile++;
  spalte=0;
  if( zeile >= TXWINDOWBUFFER) //Scroll two lines
    {
      for(i=2; i< TXWINDOWBUFFER;i++)
        scrollBuffer[i-2]->setText(scrollBuffer[i]->text());
      zeile = TXWINDOWBUFFER-2;
      for(i=zeile; i < TXWINDOWBUFFER;i++)
        scrollBuffer[i]->clear();
    }
  scrollBuffer[zeile]->setCursorPosition(spalte);
  ui->txArea->ensureWidgetVisible(scrollBuffer[zeile]);
}
void TxWindow::clearBuffers()
{
  for ( int i = 0;i < TXWINDOWBUFFER;i++ )
    scrollBuffer[i]->clear();
  Buffer->clear();
  zeile=0;
  spalte=0;
  scrollBuffer[zeile]->setCursorPosition(spalte);
  ui->txArea->ensureWidgetVisible(scrollBuffer[zeile]);
}
void TxWindow::keyPressEvent ( QKeyEvent *e )
{
  unsigned char c;
  bool CapsLock = false; // Fixme checking CapsLock state
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
      Buffer->insert ( c );
      backspace();
      e->accept();
      break;

    default:
//    ButtonState cc=e->state();
//    if ( (cc == ShiftButton) )
      if ( ( e->modifiers() == Qt::ShiftModifier ) || CapsLock )
      {
        if ( !e->text().isEmpty() )
          c = e->text().toUpper().at ( 0 ).toLatin1();
        else
          c = 0;
      }
      else
      {
        if ( !e->text().isEmpty() )
          c = e->text().toLower().at ( 0 ).toLatin1();
        else
          c = 0;
      }
      if ( c != 0 )
      {
        Buffer->insert ( c );
        insert ( c );
        e->accept();
      }
  } // End Case

} // keyPressEvent
void TxWindow::backspace()
{
  if(spalte > 0)
  {
    spalte--;
    scrollBuffer[zeile]->backspace();
  }
  else if (zeile > 0)
  {
    zeile--;
    spalte=(scrollBuffer[zeile]->text()).length();
    ui->txArea->ensureWidgetVisible(scrollBuffer[zeile]);
  }
}
void TxWindow::focusInEvent(QFocusEvent *e)
{
  setStyleSheet("TxWindow{border: 1px solid green;}");
  QFrame::focusInEvent(e);
}

void TxWindow::focusOutEvent(QFocusEvent *e)
{

  setStyleSheet("TxWindow{border: 1px solid red;}");
  QFrame::focusOutEvent(e);
}
