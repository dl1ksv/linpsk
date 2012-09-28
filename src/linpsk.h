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

class QActionGroup;
class QToolBar;
class QMenu;
class QCloseEvent;

class CRxDisplay;
class CTxDisplay;
class ControlPanel;
class QLabel;
class CModulator;
class CTxBuffer;
class Input;
class Macros;
class Parameter;


class LinPSK : public QMainWindow
{
    Q_OBJECT

public:
    LinPSK( QWidget* parent = 0,Qt::WFlags fl = 0);
    ~LinPSK();

    CRxDisplay* RxDisplay;
    CTxDisplay* TxDisplay;
    ControlPanel* Control;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *changeRxParams;
    QMenu *helpMenu;

public slots:
    virtual void fileOpen();
    virtual void Exit();
///    virtual void helpIndex();
///    virtual void helpContents();
    virtual void helpAbout();
    virtual void helpAboutQt();
    virtual void addRxWindow();
    virtual void generalSettings();
    virtual void chooseColor();

protected:
    void resizeEvent(QResizeEvent *);

protected slots:
  virtual void languageChange();
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
  /** Font Settings **/
  void FontSetup();
  /** Switching from TX to RX after TX- Bufefr ist empty **/
  void stopTx();
  /** apply the settings **/
  void apply_settings();
  /** Changig some Channel Parameters **/
  void setChannelParams();
  /** Changing Rx Mode ( Moulation type ) **/
  void setRxMode();
  /** Save Settings **/
  void saveSettings();
  /** Executing Macros **/
  void executeMacro(int);
  /** Add Macro **/
  void addMacro();
  /** Edit Macro **/
  void editMacro();
  /** Delete Macro **/
  void deleteMacro();
  /** Rename Macro **/
  void renameMacro();
  /** Recording **/
  void recording(bool);
  /** About Info **/
  void HelpAbout();
  
  /** Closing this window **/
  void closeEvent( QCloseEvent *);

private:

    void calculateSizeofComponents();
    void read_config();
    void save_config();
    bool inAction;
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
Macros *Macro;
QList<QColor> WindowColors;
};

#endif // LINPSK_H
