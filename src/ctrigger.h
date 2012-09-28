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

#ifndef CTRIGGER_H
#define CTRIGGER_H


#include <QGroupBox>
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLineEdit;
class QRadioButton;

class CTrigger : public QGroupBox
{
    Q_OBJECT

public:
    CTrigger(const QString, QWidget* parent = 0  );
    ~CTrigger();

    QLineEdit* TriggerText;
    QRadioButton* Activate;

public slots:


protected:
    void resizeEvent( QResizeEvent * );


protected slots:
    virtual void languageChange();
private:
    void calculateSizeofComponents();

};

#endif // CTRIGGER_H
