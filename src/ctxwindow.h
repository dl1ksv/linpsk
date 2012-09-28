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

#ifndef CTXWINDOW_H
#define CTXWINDOW_H

#include <QFrame>
class QLineEdit;
class QPushButton;
class CTxBuffer;
class QString;

class CTxWindow : public QFrame
{
    Q_OBJECT

public:
    CTxWindow( QWidget* parent = 0);
    ~CTxWindow();

    QPushButton* Clear;
    void setTxBuffer(CTxBuffer *);

public slots:

signals:


protected:
    virtual void resizeEvent( QResizeEvent * e );
    virtual void mousePressEvent( QMouseEvent * );



protected slots:
    virtual void languageChange();
    void pasteText(const QString &);
private:
    unsigned int Zeile;
    unsigned int Spalte;
    QLineEdit* Zeile2;
    QLineEdit* Zeile1;
    QLineEdit* Zeile3;

    void calculateSizeofComponents();
    void keyPressEvent(QKeyEvent *);
    CTxBuffer * Buffer;
    void backspace();
    void insert(unsigned char);

private slots:
    virtual void clear();
    void gotoNextLine();

};

#endif // CTXWINDOW_H
