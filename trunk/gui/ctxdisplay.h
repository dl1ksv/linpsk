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
 *   The PSK part is based on WinPSK 1.0 by Moe Wheatley, AE4JY            *
 ***************************************************************************/

#ifndef CTXDISPLAY_H
#define CTXDISPLAY_H


#include <QFrame>
#include "ui_ctxdisplay.h"

class CTxDisplay : public QFrame, public Ui::CTxDisplay

{
    Q_OBJECT

public:
    CTxDisplay( QWidget* parent = 0);
    ~CTxDisplay();
    void insert(QString s);
    void setTxFocus();
    void switch2Rx();
  public slots:
 //   void abbruch();
  protected:
    void resizeEvent( QResizeEvent * );


protected slots:
    virtual void languageChange();
  private:
  signals:
    void startRx();
    void startTx();
    void abortTx();

};

#endif // CTXDISPLAY_H
