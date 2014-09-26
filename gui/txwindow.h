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

#ifndef TXWINDOW_H
#define TXWINDOW_H

#include <QFrame>
#include "constants.h"

class CTxBuffer;
class QString;
class QLineEdit;

namespace Ui {
  class TxWindow;
}

class TxWindow : public QFrame
{
  Q_OBJECT

public:
  explicit TxWindow(QWidget *parent = 0);
  ~TxWindow();
  void setTxBuffer(CTxBuffer *);
  void insertString(QString string);
  void insert(unsigned char);


private:
  Ui::TxWindow *ui;

  CTxBuffer * Buffer;
  QLineEdit* scrollBuffer[TXWINDOWBUFFER];
  int zeile,spalte; // Position in  window buffer

  void gotoNextLine();
  void backspace();

  void keyPressEvent(QKeyEvent *); // Bearbeiten der Eingaben im TX Window
  protected:
  virtual void focusInEvent(QFocusEvent *e);
  virtual void focusOutEvent(QFocusEvent *e);
//  virtual void mousePressEvent(QMouseEvent *);
public slots:
  void clearBuffers();
};

#endif // TXWINDOW_H
