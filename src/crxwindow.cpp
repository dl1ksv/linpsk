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
#include <QVBoxLayout>
#include <QClipboard>

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
  rowHeight = fm.height()+3;
  int pixelwidth=82*fm.horizontalAdvance("A");
  DisplayBox = new QWidget ();
  DisplayBox-> setFocusPolicy( Qt::NoFocus);
  DisplayBox->setGeometry( 0,0 ,pixelwidth, RXWINDOWBUFFER*rowHeight );
  DisplayBox->setContextMenuPolicy ( Qt::CustomContextMenu );
  connect ( DisplayBox, SIGNAL ( customContextMenuRequested ( QPoint ) ), this, SLOT ( contextMenu ( QPoint ) ) );

  setWidget ( DisplayBox );
  linesLayout = new QVBoxLayout(DisplayBox);
  linesLayout->setSpacing(0);
  linesLayout->setContentsMargins(0, 0, 0, 0);
  linesLayout->setObjectName(QString::fromUtf8("linesLayout"));
  linesLayout->setSizeConstraint(QLayout::SetNoConstraint);


  setHorizontalScrollBarPolicy ( Qt::ScrollBarAsNeeded );
  setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );

  trigger = false;
  TriggerText = "";
  TexttoTrigger = "";
  save = false;
  stream = 0;
  Row = 0;
  Column = 0;
  AutoScroll = true;
  crFound = false;

  for ( int i = 0;i < RXWINDOWBUFFER;i++ )
  {

    ScrollBuffer[i] = new QLineEdit ( DisplayBox );
    ScrollBuffer[i]->setFixedWidth(pixelwidth);
    ScrollBuffer[i]->setMaximumHeight(rowHeight);
    ScrollBuffer[i]->setFrame ( false );
    ScrollBuffer[i]->setReadOnly ( true );
    linesLayout->addWidget(ScrollBuffer[i]);
    ScrollBuffer[i]->setContextMenuPolicy ( Qt::NoContextMenu );
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
  A = menu->addAction ("Dok");
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( copyDok() ) );
  A = menu->addAction ("Clipboard");
  connect ( A, SIGNAL ( triggered() ), this, SLOT ( copy() ) );

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
      if( ! crFound )
        NeueZeile();
      else
        crFound = false;
      break;
    case '\r':
      crFound = true;
      NeueZeile();
      break;

    case '\b':
      crFound =false;
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
      crFound = false;
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
      QString s = ScrollBuffer[Row]->text() + QString ( QLatin1Char ( c ) );
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

  ensureWidgetVisible ( ( QWidget* ) ScrollBuffer[Row] );
}
void CRxWindow::setColor ( QColor color )
{

  for ( int i = 0;i < RXWINDOWBUFFER;i++ )
    ScrollBuffer[i]->setStyleSheet ( "QLineEdit{color: " + color.name() + "; }" );
}
/**
void CRxWindow::fontChange ( const QFont & )
{
  QFontMetrics fm ( font() );
  int pixelwidth=82*fm.width("A");
  for ( int i = 0;i < RXWINDOWBUFFER;i++ )
    ScrollBuffer[i]->setFixedWidth( pixelwidth );
  DisplayLineHeight = ScrollBuffer[0]->height()-1;

}
**/
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


void CRxWindow::contextMenu ( QPoint p )
{
  int selectedLine, selectedColumn;
  char c = 0xF8;
  QPoint p1 = QCursor::pos();
//  selectedLine = (p.y()-3) / (ScrollBuffer[0]->height()-1);
  selectedLine = p.y() / ScrollBuffer[0]->height();

#ifndef QT_NO_DEBUG
  qDebug("Line %d selected, lineheight, %d ",selectedLine,ScrollBuffer[0]->height());
  qDebug("*****Cursor: x: %i y: %i",p.x(),p.y());
  int start;
  start=selectedLine-4;
  if(start <0)
    start=0;
  for(int i=start;i <=selectedLine;i++)
    {
      qDebug("Zeile: %i, %s",i,qPrintable(ScrollBuffer[i]->text()));
    }
  qDebug("***Menuposition x: %i y: %i",p1.x(),p1.y());
#endif

  if(selectedLine >= RXWINDOWBUFFER)
    selectedLine=RXWINDOWBUFFER-1;
  selectedColumn=0;
  if(!ScrollBuffer[selectedLine]->hasSelectedText())
    {
      selectedColumn = ScrollBuffer[selectedLine]->cursorPositionAt ( QPoint ( p.x(), 2 ) );
      ScrollBuffer[selectedLine]->setCursorPosition ( selectedColumn );
      ScrollBuffer[selectedLine]->cursorWordForward ( true );
    }
  selectedString = ScrollBuffer[selectedLine]->selectedText();
  if(!selectedString.endsWith(QChar(' ')))
  {
    ScrollBuffer[selectedLine]->cursorWordForward ( true );
    selectedString = ScrollBuffer[selectedLine]->selectedText();
    if(!selectedString.endsWith(QChar(' ')))  // / + extension might belong to the text (callsign)
    {
      ScrollBuffer[selectedLine]->cursorWordForward ( true );
      selectedString = ScrollBuffer[selectedLine]->selectedText();
    }
  }
  selectedString = selectedString.replace ( QChar ( c ), QLatin1String ( "0" ) ); // Replace Slashed zero by zero
  if ( selectedString == QString ( " " ) )
  {
    ScrollBuffer[selectedLine]->setCursorPosition ( selectedColumn + 1 );
    ScrollBuffer[selectedLine]->cursorWordForward ( true );
    selectedString = ScrollBuffer[selectedLine]->selectedText();
  }


  menu->exec ( p1 );
  ScrollBuffer[selectedLine]->deselect();
}
void CRxWindow::copyCallSign()
{
  emit setQsoData(CallSign,  selectedString );
}
void CRxWindow::copyQTH()
{
  emit setQsoData( QTH, selectedString );
}
void CRxWindow::copyName()
{
  emit setQsoData( Name, selectedString );
}
void CRxWindow::copyLocator()
{
  emit setQsoData( Locator, selectedString );
}
void CRxWindow::copyRST()
{
  emit setQsoData( RST, selectedString );
}
void CRxWindow::copyDok()
{
  emit setQsoData( DOK, selectedString );
}
void CRxWindow::copy()
{
  QApplication::clipboard()->setText(selectedString);
}
