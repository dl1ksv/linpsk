/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef CRXWINDOW_H
#define CRXWINDOW_H

#include <QScrollArea>
#include "constants.h"

#include <QFile>
#include <QTextStream>

class QLineEdit;
class QWidget;
class QString;
class QMenu;
class QVBoxLayout;

class CRxWindow : public QScrollArea
{
  Q_OBJECT

public:
  CRxWindow ( QWidget* parent = 0 );
  ~CRxWindow();
  bool getTriggerStatus();
  QString getTriggerText();
  void stopRecording();
  void startRecording ( QString );
  bool getRecordingState();
public slots:
  void updateRx ( char );
  void clearRxWindow();
  void setColor ( QColor );
  void activateTrigger ( QString );
  void deactivateTrigger();
  void contextMenu ( QPoint );

protected:
//  virtual void fontChange ( const QFont & );

protected slots:
  void copyCallSign();
  void copyQTH();
  void copyName();
  void copyLocator();
  void copyRST();
  void copyDok();
  void copy();
private:

  QVBoxLayout *linesLayout;
  QLineEdit* ScrollBuffer[RXWINDOWBUFFER];

  int Row, Column, rowHeight;
  void removeLines(int);
  void NeueZeile();
  QWidget* DisplayBox;
  bool AutoScroll;
  bool trigger;
  QString TriggerText;
  QString TexttoTrigger;
  bool save;
  bool crFound;
  QFile File;
  QTextStream *stream;

  QMenu *menu;
  QString selectedString;

signals:
  void Triggered();
  void setQsoData(QsoData,QString);
};

#endif // CRXWINDOW_H
