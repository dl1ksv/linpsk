/***************************************************************************
                          constants.h  -  description
                             -------------------
    begin                : Fri Apr 21 2000
    copyright            : (C) 2000 by Volker Schroer
    email                : DL1KSV@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *   based on the work of Moe Wheatley, AE4JY                              *
 ***************************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>                        // for some constants like PI
#include <QString>
#include <QDate>
#include <QTime>

#define TXOFF_CODE -1			// control codes that can be placed in the input
#define TXON_CODE -2			// queue for various control functions
#define TXTOG_CODE -3

#define FFT_SIZE 2048
#define FFT_SAMPLERATEX2 11025	// 2 x the fft sample rate(so is an integer)

#define TXBUFFER_LENGTH 1000
#define BUF_SIZE 4096	//size of data chunks to process at a time from
                        // the soundcard
#define RXWINDOWBUFFER 50 // Max Number of Lines in the RX ScrollBuffer

// define some constants
#define PI2 ( M_PI + M_PI )		// 2 Pi

#define Ts (.032+.000000)			// Ts == symbol period(correct for +/- .23%error)
#define M_PI_3_2	(M_PI_2+M_PI)		// 3Pi/2
#define M_PI_3_4  (M_PI_2+M_PI_4) // 3Pi/4
#define M_PI_5_4	(M_PI + M_PI_4) // 5Pi/4
#define M_PI_7_4	(M_PI + M_PI_2 + M_PI_4) // 7Pi/4

enum Mode {BPSK=0,QPSK,RTTY,MFSK16};
enum AfcMode {Off,Narrow,Wide};

enum Paritaet {None,Even,Odd};

enum StopBits {One,Onepoint5,Two};

enum RxTxParameterType{Reverse,Offset,Parity,Extra,Standard};
enum StateOfReception { WaitingForMark,WaitingForSpace, CheckingStartBit,SkipRestOfStartBit,CollectingByte,
                        CheckingParity, WaitingForStopBits,ThrowHalfBit};


typedef struct
{
bool reverse;
Paritaet parity;
int offset;
StopBits stopbits;
}  ExtraParameter;

typedef struct
{
AfcMode afc;
bool Squelch;
int Threshold;

} Controls;

typedef struct
{
 QString RemoteCallsign;
 QString OpName;
 QString Qth;
 QString Locator;
 QString QsoFrequency;
 QString HisRST;
 QString MyRST;
 QDate QsoDate;
 QTime QsoTime;
QString mainPrefix;
QString continent;
QString wazZone;
QString ituZone;
QString countryName;
QString worked;
} QsoInfo;

#define HilbertFilterLength 37

#endif
