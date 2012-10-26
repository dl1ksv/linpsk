/***************************************************************************
                          crxchannel.h  -  description
                             -------------------
    begin                : Sam Jan 4 2003
    copyright            : (C) 2003 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef CRXCHANNEL_H
#define CRXCHANNEL_H

#include <complex>
#include "constants.h"
#include <QObject>

#include "cdemodulator.h"
/**Class to administrate the different RX Channels
  *@author Volker Schroer
  */
class CRxWindow;
class QWidget;
#include <qcolor.h>

class CRxChannel : public QObject
 {
 Q_OBJECT
public: 
	CRxChannel(int ID,QWidget* parent=0,Mode DemodulatorType= BPSK,unsigned int Freq=1000);
virtual ~CRxChannel();
  void insertChannel(CRxChannel * );
  void setPrevChannel(CRxChannel *);
  void changeChain(CRxChannel *);
  void setGeometry(int,int,int,int);
  void processInput(double *, double *);
  AfcMode getAfcMode();
  AfcMode AfcProperties();
  CRxChannel *getNextChannel();
  double getRxFrequency();
  int get2RxFrequency();
  Mode getModulationType();
  int getID();
  CRxWindow *getWindow();
  bool getSquelchState();
  int getSquelchValue();
  void setThreshold(int);
  int getThreshold();
  QColor getWindowColor();
  void setWindowColor(QColor);
  void setParameter(RxTxParameterType,void *);
  void *getParameter(RxTxParameterType);
  CRxWindow *RxWindow;
  float getIMD();
  virtual void *getBuffer();
  std::complex<float>* getPhasePointer();
public slots:
  void setRxFrequency(double);
  void hide();
  void show();
  class CRxChannel *getChannel(int);
  void setAfcMode(AfcMode);
  void setSquelch(bool);
  void updateRx(char);
  void clearRxWindow();
  void setMode(Mode);
  void setQsoData(QsoInfo *);
  QsoInfo *getQsoData();
  void record(bool);
protected:

CDemodulator *Demodulator;

private:
CRxChannel *nextChannel;
CRxChannel *prevChannel;
Mode RxMode;
int WindowsID;
QsoInfo QsoData;
private slots:
void triggered();
signals:
void Triggered(int);

};

#endif
