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

#ifndef FREQUENCYSELECT_H
#define FREQUENCYSELECT_H

#include <QGroupBox>
#include "constants.h"
class QRadioButton;
class QSpinBox;

class FrequencySelect : public QGroupBox
{
    Q_OBJECT

public:
    FrequencySelect( QWidget* parent = 0, AfcMode WithMode = Wide);
    ~FrequencySelect();

    QRadioButton* Activate;
    QSpinBox* Frequency;

    void setFunctionText( QString text );
    AfcMode getAfcMode();
    void setAfcMode(AfcMode);
    void setAfcDisplayMode(AfcMode);

public slots:
    double getFrequency();
    void setFrequency( double );

protected:
    void resizeEvent( QResizeEvent * );


protected slots:
    virtual void languageChange();
    void checkFrequency(int);
    void toggleWide();
    void toggleActivate();

private:
    double frequency;
    QRadioButton* AfcWide;
    void calculateSizeofComponents();
    AfcMode modus;
signals:
 void FrequencyChanged(double);

};

#endif // FREQUENCYSELECT_H
