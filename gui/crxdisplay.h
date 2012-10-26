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

#ifndef CRXDISPLAY_H
#define CRXDISPLAY_H

#include "constants.h"
#include <QList>
#include <QFrame>
#include <fftw3.h>
#include "constants.h"
#include "ui_crxdisplay.h"

class Input;
class QTimer;
class QPushButton;
class CRxChannel;

class CRxDisplay : public QFrame , private Ui::CRxDisplay
{
  Q_OBJECT

  public:
		CRxDisplay ( QWidget* parent = 0 );
		~CRxDisplay();
		/** Returns the pointer to the values for Calculating the Spectrum **/
		double * FFTValues();
		bool start_process_loop();
		void stop_process_loop();
		void newColor();
        void setColorList ( QList<QColor> *c );
        AfcMode getAfcMode();
        void stopRecording();
  public slots:
		/** Starting receiving/transmitting */
		void addRxWindow ( int Frequency, Mode Modulation, QString Heading );
		void setRxFrequency ( double );
        void setAfc(AfcMode mode);
        void setAfcProperties(AfcMode mode);

  protected:

  protected slots:
		virtual void languageChange();
		void process_rxdata();
		void changeActiveRxWindow ( int );
        void trigger();
		void clearRxWindow();
        void record(bool);
  private:
    QList<QColor> *Farbe;
		/**
		Sound is a pointer to the Input Source, may be a File with
        Demo Samples (text or wav), or the soundcard
		**/
        Input *Sound;
	/** Pointer to input Buffer **/
	double inbuf[BUF_SIZE];
	CRxChannel *RxChannel;
	/** Decimation Filter to reduce samplerate */
	void ProcDec2Fir ( double *pIn, double *pOut, int BlockSize );
	double *dec2fir; // queue for decimation by 2 filter
	double outbuf[BUF_SIZE];
	double *m_pDec2InPtr;
	double output[BUF_SIZE/2];
	fftw_plan plan;

	signals:

      void startPlotting ( double *, bool );
      void newActiveChannel();
      void new_IMD ( float );
      void setQsoData(QsoData,QString);

};

#endif // CRXDISPLAY_H
