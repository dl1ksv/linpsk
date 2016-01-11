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

#ifndef LINPSK_H
#define LINPSK_H

#include <QMainWindow>
#include <QList>
#include "constants.h"
#include "ui_linpsk.h"
#include <complex>

class QActionGroup;
class QToolBar;
class QMenu;
class QCloseEvent;

class QLabel;
class CModulator;
class CTxBuffer;
class Input;
class Macros;
class Parameter;


class LinPSK : public QMainWindow, private Ui::LinPSK
{
    Q_OBJECT

public:
    LinPSK( QWidget* parent = 0,Qt::WindowFlags fl = 0);
    ~LinPSK();


public slots:
    virtual void Exit();

protected:

protected slots:
  virtual void languageChange();
  void fileOpen();
  void helpAboutQt();
  void addRxWindow();
  void generalSettings();
  void chooseColor();
  void helpAbout();
  /** Show Time */
  void setclock();
  /** Set IMD */
  void setIMD(float);
  /** Start Rx **/
  void startRx();
  /** Start Tx **/
  void startTx();
  /** Calculate the TX Data **/
  void process_txdata();
  /** Switching from TX to RX after TX- Bufefr ist empty **/
  void stopTx();
  /** apply the settings **/
  void apply_settings();
  /** Changig some Channel Parameters **/
  void setChannelParams();
  /** Changing Rx Mode ( Moulation type ) **/
  void setRxMode();
  /** Executing Macros **/
  void executeMacro(int id);
  /** Add Macro **/
  void addMacro();
  /** Edit Macro **/
  void editMacro();
  /** Delete Macro **/
  void deleteMacro();
  /** Rename Macro **/
  void renameMacro();
  /**Activate /Deactivate Macros**/
  void actdeactMacros();
  /** Font Settings **/
  void FontSetup();
  /** Save Settings **/
  void saveSettings();
  /** Closing this window **/
  void closeEvent( QCloseEvent *);

private slots:
  void on_RxDisplay_newActiveChannel();
  void unblockMacros();
private:

    void read_config();
    void save_config();
    bool inAction;
    bool blockMacros;
Parameter *SaveParameters;    
/** Modulator **/
CModulator *Modulator;
/** Characters to transmit **/
CTxBuffer *TxBuffer;
/** Signalbuffer **/
double Output[BUF_SIZE];
/** Numbers of Char to transmit **/
unsigned int Txcount;
/** Sound Device **/
Input *Sound;
/** To show messages in Statusbar */
QLabel *msg;
/** TxTimer **/
///QTimer *TxTimer;

void selectPTTDevice();

/** To show date and time **/
QLabel *datum;
QLabel *zeit;
QLabel *IMD;

QLabel *clockadj;
QList<QColor> WindowColors;
/** Macros **/
QVector<Macro> macroList;
QStringList tokenList;

signals:
void StartRx();
void StartTx();

};

#endif // LINPSK_H
