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
//class QVBoxLayout;
//class QHBoxLayout;
//class QGridLayout;
//class CTxFunctions;
class CLedButton;
class CTxWindow;
class FrequencySelect;

class CTxDisplay : public QFrame
{
    Q_OBJECT

public:
    CTxDisplay( QWidget* parent = 0);
    ~CTxDisplay();

    CTxWindow* TxWindow;
    FrequencySelect* TxFreq;
 //   CTxFunctions* TxFunctions;
CLedButton *TxFunctions;

public slots:
void abbruch();
protected:
    void resizeEvent( QResizeEvent * );


protected slots:
    virtual void languageChange();
private:
    void calculateSizeofComponents();
signals:
void startRx();
void startTx();

};

#endif // CTXDISPLAY_H
