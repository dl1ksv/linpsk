/***************************************************************************
                          viterbi.cpp  -  description
                             -------------------
    begin                : Sam Mär 8 2003
    copyright            : (C) 2003 by Volker Schroer
    email                : dl1ksv@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "viterbisoft.h"
#include <iostream>
using namespace std;


ViterbiSoft::ViterbiSoft ( const double ( *d ) ( double *, int ) )
{
	int i;
	dist = d;
	NumberofStates = NumberOfRTTYSampleStates;


	for ( i = 0; i < NumberofStates ; i++ )
	{
		Metric[i] = 10000.;
		PathMem[i][0] = Stop16;
//  bitestimate[i][0] = Stop3;
//  bitestimate[i+NumberofStates][0]=Stop3;
	}
	Metric[Stop16] = 0.0;
	pathPointer = 1;
	/** Set State Table **/
	StateTable[0][Start1] = Start2;
	StateTable[1][Start1] = Start2; // ??
	StateTable[0][Start2] = Start3;
	StateTable[1][Start2]= Start3;
	StateTable[0][Start3]= Start4;
	StateTable[1][Start3]= Start4;
	StateTable[0][Start4]= Start5;
	StateTable[1][Start4]= Start5;
	StateTable[0][Start5]= Start6;
	StateTable[1][Start5]= Start6;
	StateTable[0][Start6]= Start7;
	StateTable[1][Start6]= Start7;
	StateTable[0][Start7]= Start8;
	StateTable[1][Start7]= Start8;
	StateTable[0][Start8]= Start9;
	StateTable[1][Start8]= Start9;
	StateTable[0][Start9]= Start10;
	StateTable[1][Start9]= Start10;
	StateTable[0][Start10]= Start11;
	StateTable[1][Start10]= Start11;
	StateTable[0][Start11] = D1_0_1;
	StateTable[1][Start11] = D1_1_1;
	StateTable[0][Stop1] = Stop2; // ??
	StateTable[1][Stop1] = Stop2;
	StateTable[0][Stop2] = Stop3; // ??
	StateTable[1][Stop2] = Stop3;
	StateTable[0][Stop3] = Stop4;
	StateTable[1][Stop3] = Stop4;
	StateTable[0][Stop4] = Stop5;
	StateTable[1][Stop4] = Stop5;
	StateTable[0][Stop5] = Stop6;
	StateTable[1][Stop5] = Stop6;
	StateTable[0][Stop6] = Stop7;
	StateTable[1][Stop6] = Stop7;
	StateTable[0][Stop7] = Stop8;
	StateTable[1][Stop7] = Stop8;
	StateTable[0][Stop8] = Stop9;
	StateTable[1][Stop8] = Stop9;
	StateTable[0][Stop9] = Stop10;
	StateTable[1][Stop9] = Stop10;
	StateTable[0][Stop10] = Stop11;
	StateTable[1][Stop10] = Stop11;
	StateTable[0][Stop11] = Stop12;
	StateTable[1][Stop11] = Stop12;
	StateTable[0][Stop12] = Stop13;
	StateTable[1][Stop12] = Stop13;
	StateTable[0][Stop13] = Stop14;
	StateTable[1][Stop13] = Stop14;
	StateTable[0][Stop14] = Stop15;
	StateTable[1][Stop14] = Stop15;
	StateTable[0][Stop15] = Stop16;
	StateTable[1][Stop15] = Stop16;
	StateTable[0][Stop16] = Start1;
	StateTable[1][Stop16] = Stop16;
	StateTable[0][D1_0_1] = D1_0_2;
	StateTable[1][D1_0_1] = D1_0_2; //??
	StateTable[0][D1_0_2] = D1_0_3;
	StateTable[1][D1_0_2] = D1_0_3;
	StateTable[0][D1_0_3] = D1_0_4;
	StateTable[1][D1_0_3] = D1_0_4;
	StateTable[0][D1_0_4] = D1_0_5;
	StateTable[1][D1_0_4] = D1_0_5;
	StateTable[0][D1_0_5] = D1_0_6;
	StateTable[1][D1_0_5] = D1_0_6;
	StateTable[0][D1_0_6] = D1_0_7;
	StateTable[1][D1_0_6] = D1_0_7;
	StateTable[0][D1_0_7] = D1_0_8;
	StateTable[1][D1_0_7] = D1_0_8;
	StateTable[0][D1_0_8] = D1_0_9;
	StateTable[1][D1_0_8] = D1_0_9;
	StateTable[0][D1_0_9] = D1_0_10;
	StateTable[1][D1_0_9] = D1_0_10;
	StateTable[0][D1_0_10] = D1_0_11;
	StateTable[1][D1_0_10] = D1_0_11;
StateTable[0][D1_0_11] = D2_0_1;
StateTable[1][D1_0_11] = D2_1_1;
	StateTable[0][D1_1_1] = D1_1_2;
	StateTable[1][D1_1_1] = D1_1_2;
	StateTable[0][D1_1_2] = D1_1_3;
	StateTable[1][D1_1_2] = D1_1_3;
	StateTable[0][D1_1_3] = D1_1_4;
	StateTable[1][D1_1_3] = D1_1_4;
	StateTable[0][D1_1_4] = D1_1_5;
	StateTable[1][D1_1_4] = D1_1_5;
	StateTable[0][D1_1_5] = D1_1_6;
	StateTable[1][D1_1_5] = D1_1_6;
	StateTable[0][D1_1_6] = D1_1_7;
	StateTable[1][D1_1_6] = D1_1_7;
	StateTable[0][D1_1_7] = D1_1_8;
	StateTable[1][D1_1_7] = D1_1_8;
	StateTable[0][D1_1_8] = D1_1_9;
	StateTable[1][D1_1_8] = D1_1_9;
	StateTable[0][D1_1_9] = D1_1_10;
	StateTable[1][D1_1_9] = D1_1_10;
	StateTable[0][D1_1_10] = D1_1_11;
	StateTable[1][D1_1_10] = D1_1_11;
StateTable[0][D1_1_11] = D2_0_1;
StateTable[1][D1_1_11] = D2_1_1;
	StateTable[0][D2_0_1] = D2_0_2;
	StateTable[1][D2_0_1] = D2_0_2;
	StateTable[0][D2_0_2] = D2_0_3;
	StateTable[1][D2_0_2] = D2_0_3;
	StateTable[0][D2_0_3] = D2_0_4;
	StateTable[1][D2_0_3] = D2_0_4;
	StateTable[0][D2_0_4] = D2_0_5;
	StateTable[1][D2_0_4] = D2_0_5;
	StateTable[0][D2_0_5] = D2_0_6;
	StateTable[1][D2_0_5] = D2_0_6;
	StateTable[0][D2_0_6] = D2_0_7;
	StateTable[1][D2_0_6] = D2_0_7;
	StateTable[0][D2_0_7] = D2_0_8;
	StateTable[1][D2_0_7] = D2_0_8;
	StateTable[0][D2_0_8] = D2_0_9;
	StateTable[1][D2_0_8] = D2_0_9;
	StateTable[0][D2_0_9] = D2_0_10;
	StateTable[1][D2_0_9] = D2_0_10;
	StateTable[0][D2_0_10] = D2_0_11;
	StateTable[1][D2_0_10] = D2_0_11;
StateTable[0][D2_0_11] = D3_0_1;
StateTable[1][D2_0_11] = D3_1_1;
	StateTable[0][D2_1_1] = D2_1_2;
	StateTable[1][D2_1_1] = D2_1_2;
	StateTable[0][D2_1_2] = D2_1_3;
	StateTable[1][D2_1_2] = D2_1_3;
	StateTable[0][D2_1_3] = D2_1_4;
	StateTable[1][D2_1_3] = D2_1_4;
	StateTable[0][D2_1_4] = D2_1_5;
	StateTable[1][D2_1_4] = D2_1_5;
	StateTable[0][D2_1_5] = D2_1_6;
	StateTable[1][D2_1_5] = D2_1_6;
	StateTable[0][D2_1_6] = D2_1_7;
	StateTable[1][D2_1_6] = D2_1_7;
	StateTable[0][D2_1_7] = D2_1_8;
	StateTable[1][D2_1_7] = D2_1_8;
	StateTable[0][D2_1_8] = D2_1_9;
	StateTable[1][D2_1_8] = D2_1_9;
	StateTable[0][D2_1_9] = D2_1_10;
	StateTable[1][D2_1_9] = D2_1_10;
	StateTable[0][D2_1_10] = D2_1_11;
	StateTable[1][D2_1_10] = D2_1_11;
StateTable[0][D2_1_11] = D3_0_1;
StateTable[1][D2_1_11] = D3_1_1;
	StateTable[0][D3_0_1] = D3_0_2;
	StateTable[1][D3_0_1] = D3_0_2; // ??
	StateTable[0][D3_0_2] = D3_0_3;
	StateTable[1][D3_0_2] = D3_0_3;
	StateTable[0][D3_0_3] = D3_0_4;
	StateTable[1][D3_0_3] = D3_0_4;
	StateTable[0][D3_0_4] = D3_0_5;
	StateTable[1][D3_0_4] = D3_0_5;
	StateTable[0][D3_0_5] = D3_0_6;
	StateTable[1][D3_0_5] = D3_0_6;
	StateTable[0][D3_0_6] = D3_0_7;
	StateTable[1][D3_0_6] = D3_0_7;
	StateTable[0][D3_0_7] = D3_0_8;
	StateTable[1][D3_0_7] = D3_0_8;
	StateTable[0][D3_0_8] = D3_0_9;
	StateTable[1][D3_0_8] = D3_0_9;
	StateTable[0][D3_0_9] = D3_0_10;
	StateTable[1][D3_0_9] = D3_0_10;
	StateTable[0][D3_0_10] = D3_0_11;
	StateTable[1][D3_0_10] = D3_0_11;
StateTable[0][D3_0_11] = D4_0_1; // ??
StateTable[1][D3_0_11] = D4_1_1;
	StateTable[0][D3_1_1] = D3_1_2;
	StateTable[1][D3_1_1] = D3_1_2;
	StateTable[0][D3_1_2] = D3_1_3;
	StateTable[1][D3_1_2] = D3_1_3;
	StateTable[0][D3_1_3] = D3_1_4;
	StateTable[1][D3_1_3] = D3_1_4;
	StateTable[0][D3_1_4] = D3_1_5;
	StateTable[1][D3_1_4] = D3_1_5;
	StateTable[0][D3_1_5] = D3_1_6;
	StateTable[1][D3_1_5] = D3_1_6;
	StateTable[0][D3_1_6] = D3_1_7;
	StateTable[1][D3_1_6] = D3_1_7;
	StateTable[0][D3_1_7] = D3_1_8;
	StateTable[1][D3_1_7] = D3_1_8;
	StateTable[0][D3_1_8] = D3_1_9;
	StateTable[1][D3_1_8] = D3_1_9;
	StateTable[0][D3_1_9] = D3_1_10;
	StateTable[1][D3_1_9] = D3_1_10;
	StateTable[0][D3_1_10] = D3_1_11;
	StateTable[1][D3_1_10] = D3_1_11;
StateTable[0][D3_1_11] = D4_0_1;
StateTable[1][D3_1_11] = D4_1_1;
	StateTable[0][D4_0_1] = D4_0_2;
	StateTable[1][D4_0_1] = D4_0_2; // ??
	StateTable[0][D4_0_2] = D4_0_3;
	StateTable[1][D4_0_2] = D4_0_3;
	StateTable[0][D4_0_3] = D4_0_4;
	StateTable[1][D4_0_3] = D4_0_4;
	StateTable[0][D4_0_4] = D4_0_5;
	StateTable[1][D4_0_4] = D4_0_5;
	StateTable[0][D4_0_5] = D4_0_6;
	StateTable[1][D4_0_5] = D4_0_6;
	StateTable[0][D4_0_6] = D4_0_7;
	StateTable[1][D4_0_6] = D4_0_7;
	StateTable[0][D4_0_7] = D4_0_8;
	StateTable[1][D4_0_7] = D4_0_8;
	StateTable[0][D4_0_8] = D4_0_9;
	StateTable[1][D4_0_8] = D4_0_9;
	StateTable[0][D4_0_9] = D4_0_10;
	StateTable[1][D4_0_9] = D4_0_10;
	StateTable[0][D4_0_10] = D4_0_11;
	StateTable[1][D4_0_10] = D4_0_11;
StateTable[0][D4_0_11] = D5_0_1;
StateTable[1][D4_0_11] = D5_1_1;
	StateTable[0][D4_1_1] = D4_1_2; // ??
	StateTable[1][D4_1_1] = D4_1_2;
	StateTable[0][D4_1_2] = D4_1_3;
	StateTable[1][D4_1_2] = D4_1_3;
	StateTable[0][D4_1_3] = D4_1_4;
	StateTable[1][D4_1_3] = D4_1_4;
	StateTable[0][D4_1_4] = D4_1_5;
	StateTable[1][D4_1_4] = D4_1_5;
	StateTable[0][D4_1_5] = D4_1_6;
	StateTable[1][D4_1_5] = D4_1_6;
	StateTable[0][D4_1_6] = D4_1_7;
	StateTable[1][D4_1_6] = D4_1_7;
	StateTable[0][D4_1_7] = D4_1_8;
	StateTable[1][D4_1_7] = D4_1_8;
	StateTable[0][D4_1_8] = D4_1_9;
	StateTable[1][D4_1_8] = D4_1_9;
	StateTable[0][D4_1_9] = D4_1_10;
	StateTable[1][D4_1_9] = D4_1_10;
	StateTable[0][D4_1_10] = D4_1_11;
	StateTable[1][D4_1_10] = D4_1_11;
StateTable[0][D4_1_11] = D5_0_1;
StateTable[1][D4_1_11] = D5_1_1;
	StateTable[0][D5_0_1] = D5_0_2;
	StateTable[1][D5_0_1] = D5_0_2; // ??
	StateTable[0][D5_0_2] = D5_0_3;
	StateTable[1][D5_0_2] = D5_0_3;
	StateTable[0][D5_0_3] = D5_0_4;
	StateTable[1][D5_0_3] = D5_0_4;
	StateTable[0][D5_0_4] = D5_0_5;
	StateTable[1][D5_0_4] = D5_0_5;
	StateTable[0][D5_0_5] = D5_0_6;
	StateTable[1][D5_0_5] = D5_0_6;
	StateTable[0][D5_0_6] = D5_0_7;
	StateTable[1][D5_0_6] = D5_0_7;
	StateTable[0][D5_0_7] = D5_0_8;
	StateTable[1][D5_0_7] = D5_0_8;
	StateTable[0][D5_0_8] = D5_0_9;
	StateTable[1][D5_0_8] = D5_0_9;
	StateTable[0][D5_0_9] = D5_0_10;
	StateTable[1][D5_0_9] = D5_0_10;
	StateTable[0][D5_0_10] = D5_0_11;
	StateTable[1][D5_0_10] = D5_0_11;
StateTable[0][D5_0_11] = D6_0_1; // ??
StateTable[1][D5_0_11] = D6_1_1;
	StateTable[0][D5_1_1] = D5_1_2; // ??
	StateTable[1][D5_1_1] = D5_1_2;
	StateTable[0][D5_1_2] = D5_1_3;
	StateTable[1][D5_1_2] = D5_1_3;
	StateTable[0][D5_1_3] = D5_1_4;
	StateTable[1][D5_1_3] = D5_1_4;
	StateTable[0][D5_1_4] = D5_1_5;
	StateTable[1][D5_1_4] = D5_1_5;
	StateTable[0][D5_1_5] = D5_1_6;
	StateTable[1][D5_1_5] = D5_1_6;
	StateTable[0][D5_1_6] = D5_1_7;
	StateTable[1][D5_1_6] = D5_1_7;
	StateTable[0][D5_1_7] = D5_1_8;
	StateTable[1][D5_1_7] = D5_1_8;
	StateTable[0][D5_1_8] = D5_1_9;
	StateTable[1][D5_1_8] = D5_1_9;
	StateTable[0][D5_1_9] = D5_1_10;
	StateTable[1][D5_1_9] = D5_1_10;
	StateTable[0][D5_1_10] = D5_1_11;
	StateTable[1][D5_1_10] = D5_1_11;
StateTable[0][D5_1_11] = D6_0_1; // ??
StateTable[1][D5_1_11] = D6_1_1;

	StateTable[0][D6_0_1] = D6_0_2;
	StateTable[1][D6_0_1] = D6_0_2; // ??
	StateTable[0][D6_0_2] = D6_0_3;
	StateTable[1][D6_0_2] = D6_0_3;
	StateTable[0][D6_0_3] = D6_0_4;
	StateTable[1][D6_0_3] = D6_0_4;
	StateTable[0][D6_0_4] = D6_0_5;
	StateTable[1][D6_0_4] = D6_0_5;
	StateTable[0][D6_0_5] = D6_0_6;
	StateTable[1][D6_0_5] = D6_0_6;
	StateTable[0][D6_0_6] = D6_0_7;
	StateTable[1][D6_0_6] = D6_0_7;
	StateTable[0][D6_0_7] = D6_0_8;
	StateTable[1][D6_0_7] = D6_0_8;
	StateTable[0][D6_0_8] = D6_0_9;
	StateTable[1][D6_0_8] = D6_0_9;
	StateTable[0][D6_0_9] = D6_0_10;
	StateTable[1][D6_0_9] = D6_0_10;
	StateTable[0][D6_0_10] = D6_0_11;
	StateTable[1][D6_0_10] = D6_0_11;
StateTable[0][D6_0_11] = D7_0_1; // ??
StateTable[1][D6_0_11] = D7_1_1;
	StateTable[0][D6_1_1] = D6_1_2; // ??
	StateTable[1][D6_1_1] = D6_1_2;
	StateTable[0][D6_1_2] = D6_1_3;
	StateTable[1][D6_1_2] = D6_1_3;
	StateTable[0][D6_1_3] = D6_1_4;
	StateTable[1][D6_1_3] = D6_1_4;
	StateTable[0][D6_1_4] = D6_1_5;
	StateTable[1][D6_1_4] = D6_1_5;
	StateTable[0][D6_1_5] = D6_1_6;
	StateTable[1][D6_1_5] = D6_1_6;
	StateTable[0][D6_1_6] = D6_1_7;
	StateTable[1][D6_1_6] = D6_1_7;
	StateTable[0][D6_1_7] = D6_1_8;
	StateTable[1][D6_1_7] = D6_1_8;
	StateTable[0][D6_1_8] = D6_1_9;
	StateTable[1][D6_1_8] = D6_1_9;
	StateTable[0][D6_1_9] = D6_1_10;
	StateTable[1][D6_1_9] = D6_1_10;
	StateTable[0][D6_1_10] = D6_1_11;
	StateTable[1][D6_1_10] = D6_1_11;
StateTable[0][D6_1_11] = D7_0_1; // ??
StateTable[1][D6_1_11] = D7_1_1;

	StateTable[0][D7_0_1] = D7_0_2;
	StateTable[1][D7_0_1] = D7_0_2; // ??
	StateTable[0][D7_0_2] = D7_0_3;
	StateTable[1][D7_0_2] = D7_0_3;
	StateTable[0][D7_0_3] = D7_0_4;
	StateTable[1][D7_0_3] = D7_0_4;
	StateTable[0][D7_0_4] = D7_0_5;
	StateTable[1][D7_0_4] = D7_0_5;
	StateTable[0][D7_0_5] = D7_0_6;
	StateTable[1][D7_0_5] = D7_0_6;
	StateTable[0][D7_0_6] = D7_0_7;
	StateTable[1][D7_0_6] = D7_0_7;
	StateTable[0][D7_0_7] = D7_0_8;
	StateTable[1][D7_0_7] = D7_0_8;
	StateTable[0][D7_0_8] = D7_0_9;
	StateTable[1][D7_0_8] = D7_0_9;
	StateTable[0][D7_0_9] = D7_0_10;
	StateTable[1][D7_0_9] = D7_0_10;
	StateTable[0][D7_0_10] = D7_0_11;
	StateTable[1][D7_0_10] = D7_0_11;
StateTable[0][D7_0_11] = D8_0_1; // ??
StateTable[1][D7_0_11] = D8_1_1;
	StateTable[0][D7_1_1] = D7_1_2; // ??
	StateTable[1][D7_1_1] = D7_1_2;
	StateTable[0][D7_1_2] = D7_1_3;
	StateTable[1][D7_1_2] = D7_1_3;
	StateTable[0][D7_1_3] = D7_1_4;
	StateTable[1][D7_1_3] = D7_1_4;
	StateTable[0][D7_1_4] = D7_1_5;
	StateTable[1][D7_1_4] = D7_1_5;
	StateTable[0][D7_1_5] = D7_1_6;
	StateTable[1][D7_1_5] = D7_1_6;
	StateTable[0][D7_1_6] = D7_1_7;
	StateTable[1][D7_1_6] = D7_1_7;
	StateTable[0][D7_1_7] = D7_1_8;
	StateTable[1][D7_1_7] = D7_1_8;
	StateTable[0][D7_1_8] = D7_1_9;
	StateTable[1][D7_1_8] = D7_1_9;
	StateTable[0][D7_1_9] = D7_1_10;
	StateTable[1][D7_1_9] = D7_1_10;
	StateTable[0][D7_1_10] = D7_1_11;
	StateTable[1][D7_1_10] = D7_1_11;
StateTable[0][D7_1_11] = D8_0_1; // ??
StateTable[1][D7_1_11] = D8_1_1;

	StateTable[0][D8_0_1] = D8_0_2;
	StateTable[1][D8_0_1] = D8_0_2; // ??
	StateTable[0][D8_0_2] = D8_0_3;
	StateTable[1][D8_0_2] = D8_0_3;
	StateTable[0][D8_0_3] = D8_0_4;
	StateTable[1][D8_0_3] = D8_0_4;
	StateTable[0][D8_0_4] = D8_0_5;
	StateTable[1][D8_0_4] = D8_0_5;
	StateTable[0][D8_0_5] = D8_0_6;
	StateTable[1][D8_0_5] = D8_0_6;
	StateTable[0][D8_0_6] = D8_0_7;
	StateTable[1][D8_0_6] = D8_0_7;
	StateTable[0][D8_0_7] = D8_0_8;
	StateTable[1][D8_0_7] = D8_0_8;
	StateTable[0][D8_0_8] = D8_0_9;
	StateTable[1][D8_0_8] = D8_0_9;
	StateTable[0][D8_0_9] = D8_0_10;
	StateTable[1][D8_0_9] = D8_0_10;
	StateTable[0][D8_0_10] = D8_0_11;
	StateTable[1][D8_0_10] = D8_0_11;
StateTable[0][D8_0_11] = D9_0_1; // ??
StateTable[1][D8_0_11] = D9_1_1;
	StateTable[0][D8_1_1] = D8_1_2; // ??
	StateTable[1][D8_1_1] = D8_1_2;
	StateTable[0][D8_1_2] = D8_1_3;
	StateTable[1][D8_1_2] = D8_1_3;
	StateTable[0][D8_1_3] = D8_1_4;
	StateTable[1][D8_1_3] = D8_1_4;
	StateTable[0][D8_1_4] = D8_1_5;
	StateTable[1][D8_1_4] = D8_1_5;
	StateTable[0][D8_1_5] = D8_1_6;
	StateTable[1][D8_1_5] = D8_1_6;
	StateTable[0][D8_1_6] = D8_1_7;
	StateTable[1][D8_1_6] = D8_1_7;
	StateTable[0][D8_1_7] = D8_1_8;
	StateTable[1][D8_1_7] = D8_1_8;
	StateTable[0][D8_1_8] = D8_1_9;
	StateTable[1][D8_1_8] = D8_1_9;
	StateTable[0][D8_1_9] = D8_1_10;
	StateTable[1][D8_1_9] = D8_1_10;
	StateTable[0][D8_1_10] = D8_1_11;
	StateTable[1][D8_1_10] = D8_1_11;
StateTable[0][D8_1_11] = D9_0_1; // ??
StateTable[1][D8_1_11] = D9_1_1;

	StateTable[0][D9_0_1] = D9_0_2;
	StateTable[1][D9_0_1] = D9_0_2; // ??
	StateTable[0][D9_0_2] = D9_0_3;
	StateTable[1][D9_0_2] = D9_0_3;
	StateTable[0][D9_0_3] = D9_0_4;
	StateTable[1][D9_0_3] = D9_0_4;
	StateTable[0][D9_0_4] = D9_0_5;
	StateTable[1][D9_0_4] = D9_0_5;
	StateTable[0][D9_0_5] = D9_0_6;
	StateTable[1][D9_0_5] = D9_0_6;
	StateTable[0][D9_0_6] = D9_0_7;
	StateTable[1][D9_0_6] = D9_0_7;
	StateTable[0][D9_0_7] = D9_0_8;
	StateTable[1][D9_0_7] = D9_0_8;
	StateTable[0][D9_0_8] = D9_0_9;
	StateTable[1][D9_0_8] = D9_0_9;
	StateTable[0][D9_0_9] = D9_0_10;
	StateTable[1][D9_0_9] = D9_0_10;
	StateTable[0][D9_0_10] = D9_0_11;
	StateTable[1][D9_0_10] = D9_0_11;
StateTable[0][D9_0_11] = D10_0_1; // ??
StateTable[1][D9_0_11] = D10_1_1;
	StateTable[0][D9_1_1] = D9_1_2; // ??
	StateTable[1][D9_1_1] = D9_1_2;
	StateTable[0][D9_1_2] = D9_1_3;
	StateTable[1][D9_1_2] = D9_1_3;
	StateTable[0][D9_1_3] = D9_1_4;
	StateTable[1][D9_1_3] = D9_1_4;
	StateTable[0][D9_1_4] = D9_1_5;
	StateTable[1][D9_1_4] = D9_1_5;
	StateTable[0][D9_1_5] = D9_1_6;
	StateTable[1][D9_1_5] = D9_1_6;
	StateTable[0][D9_1_6] = D9_1_7;
	StateTable[1][D9_1_6] = D9_1_7;
	StateTable[0][D9_1_7] = D9_1_8;
	StateTable[1][D9_1_7] = D9_1_8;
	StateTable[0][D9_1_8] = D9_1_9;
	StateTable[1][D9_1_8] = D9_1_9;
	StateTable[0][D9_1_9] = D9_1_10;
	StateTable[1][D9_1_9] = D9_1_10;
	StateTable[0][D9_1_10] = D9_1_11;
	StateTable[1][D9_1_10] = D9_1_11;
StateTable[0][D9_1_11] = D10_0_1; // ??
StateTable[1][D9_1_11] = D10_1_1;

	StateTable[0][D10_0_1] = D10_0_2;
	StateTable[1][D10_0_1] = D10_0_2; // ??
	StateTable[0][D10_0_2] = D10_0_3;
	StateTable[1][D10_0_2] = D10_0_3;
	StateTable[0][D10_0_3] = D10_0_4;
	StateTable[1][D10_0_3] = D10_0_4;
	StateTable[0][D10_0_4] = D10_0_5;
	StateTable[1][D10_0_4] = D10_0_5;
	StateTable[0][D10_0_5] = D10_0_6;
	StateTable[1][D10_0_5] = D10_0_6;
	StateTable[0][D10_0_6] = D10_0_7;
	StateTable[1][D10_0_6] = D10_0_7;
	StateTable[0][D10_0_7] = D10_0_8;
	StateTable[1][D10_0_7] = D10_0_8;
	StateTable[0][D10_0_8] = D10_0_9;
	StateTable[1][D10_0_8] = D10_0_9;
	StateTable[0][D10_0_9] = D10_0_10;
	StateTable[1][D10_0_9] = D10_0_10;
	StateTable[0][D10_0_10] = D10_0_11;
	StateTable[1][D10_0_10] = D10_0_11;
StateTable[0][D10_0_11] = D11_0_1; // ??
StateTable[1][D10_0_11] = D11_1_1;
	StateTable[0][D10_1_1] = D10_1_2; // ??
	StateTable[1][D10_1_1] = D10_1_2;
	StateTable[0][D10_1_2] = D10_1_3;
	StateTable[1][D10_1_2] = D10_1_3;
	StateTable[0][D10_1_3] = D10_1_4;
	StateTable[1][D10_1_3] = D10_1_4;
	StateTable[0][D10_1_4] = D10_1_5;
	StateTable[1][D10_1_4] = D10_1_5;
	StateTable[0][D10_1_5] = D10_1_6;
	StateTable[1][D10_1_5] = D10_1_6;
	StateTable[0][D10_1_6] = D10_1_7;
	StateTable[1][D10_1_6] = D10_1_7;
	StateTable[0][D10_1_7] = D10_1_8;
	StateTable[1][D10_1_7] = D10_1_8;
	StateTable[0][D10_1_8] = D10_1_9;
	StateTable[1][D10_1_8] = D10_1_9;
	StateTable[0][D10_1_9] = D10_1_10;
	StateTable[1][D10_1_9] = D10_1_10;
	StateTable[0][D10_1_10] = D10_1_11;
	StateTable[1][D10_1_10] = D10_1_11;
StateTable[0][D10_1_11] = D11_0_1; // ??
StateTable[1][D10_1_11] = D11_1_1;

	StateTable[0][D11_0_1] = D11_0_2;
	StateTable[1][D11_0_1] = D11_0_2; // ??
	StateTable[0][D11_0_2] = D11_0_3;
	StateTable[1][D11_0_2] = D11_0_3;
	StateTable[0][D11_0_3] = D11_0_4;
	StateTable[1][D11_0_3] = D11_0_4;
	StateTable[0][D11_0_4] = D11_0_5;
	StateTable[1][D11_0_4] = D11_0_5;
	StateTable[0][D11_0_5] = D11_0_6;
	StateTable[1][D11_0_5] = D11_0_6;
	StateTable[0][D11_0_6] = D11_0_7;
	StateTable[1][D11_0_6] = D11_0_7;
	StateTable[0][D11_0_7] = D11_0_8;
	StateTable[1][D11_0_7] = D11_0_8;
	StateTable[0][D11_0_8] = D11_0_9;
	StateTable[1][D11_0_8] = D11_0_9;
	StateTable[0][D11_0_9] = D11_0_10;
	StateTable[1][D11_0_9] = D11_0_10;
	StateTable[0][D11_0_10] = D11_0_11;
	StateTable[1][D11_0_10] = D11_0_11;
StateTable[0][D11_0_11] = Stop1; // ??
StateTable[1][D11_0_11] = Stop1;
	StateTable[0][D11_1_1] = D11_1_2; // ??
	StateTable[1][D11_1_1] = D11_1_2;
	StateTable[0][D11_1_2] = D11_1_3;
	StateTable[1][D11_1_2] = D11_1_3;
	StateTable[0][D11_1_3] = D11_1_4;
	StateTable[1][D11_1_3] = D11_1_4;
	StateTable[0][D11_1_4] = D11_1_5;
	StateTable[1][D11_1_4] = D11_1_5;
	StateTable[0][D11_1_5] = D11_1_6;
	StateTable[1][D11_1_5] = D11_1_6;
	StateTable[0][D11_1_6] = D11_1_7;
	StateTable[1][D11_1_6] = D11_1_7;
	StateTable[0][D11_1_7] = D11_1_8;
	StateTable[1][D11_1_7] = D11_1_8;
	StateTable[0][D11_1_8] = D11_1_9;
	StateTable[1][D11_1_8] = D11_1_9;
	StateTable[0][D11_1_9] = D11_1_10;
	StateTable[1][D11_1_9] = D11_1_10;
	StateTable[0][D11_1_10] = D11_1_11;
	StateTable[1][D11_1_10] = D11_1_11;
StateTable[0][D11_1_11] = Stop1; // ??
StateTable[1][D11_1_11] = Stop1;

	BitCount = 0;
//	StopBitCount = SAMPLESPERSTOPBIT;

	BitTable[Start1] = 0;
	BitTable[Start2] = 0;
	BitTable[Start3]=0;
	BitTable[Start4]=0;
	BitTable[Start5]=0;
	BitTable[Start6]=0;
	BitTable[Start7]=0;
	BitTable[Start8]=0;
	BitTable[Start9]=0;
	BitTable[Start10]=0;
	BitTable[Start11]=0;

	BitTable[ Stop1] = 1;
	BitTable[Stop2] = 1;
	BitTable[Stop3] = 1;
	BitTable[Stop4] = 1;
	BitTable[Stop5] = 1;
	BitTable[Stop6] = 1;
	BitTable[Stop7] = 1;
	BitTable[Stop8] = 1;
	BitTable[Stop9] = 1;
	BitTable[Stop10] = 1;
	BitTable[Stop11] = 1;
	BitTable[Stop12] = 1;
	BitTable[Stop13] = 1;
	BitTable[Stop14] = 1;
	BitTable[Stop15] = 1;
	BitTable[Stop16] = 1;

	BitTable[ D1_0_1] = 0;
	BitTable[D1_0_2] = 0;
	BitTable[D1_0_3] = 0;
	BitTable[D1_0_4] = 0;
	BitTable[D1_0_5] = 0;
	BitTable[D1_0_6] = 0;
	BitTable[D1_0_7] = 0;
	BitTable[D1_0_8] = 0;
	BitTable[D1_0_9] = 0;
	BitTable[D1_0_10] = 0;
	BitTable[D1_0_11] = 0;

	BitTable[ D1_1_1] = 1;
	BitTable[D1_1_2] = 1;
	BitTable[ D1_1_3] = 1;
	BitTable[ D1_1_4] = 1;
	BitTable[ D1_1_5] = 1;
	BitTable[ D1_1_6] = 1;
	BitTable[ D1_1_7] = 1;
	BitTable[ D1_1_8] = 1;
	BitTable[ D1_1_9] = 1;
	BitTable[ D1_1_10] = 1;
	BitTable[ D1_1_11] = 1;

	BitTable[ D2_0_1] = 0;
	BitTable[D2_0_2] = 0;
	BitTable[D2_0_3] = 0;
	BitTable[D2_0_4] = 0;
	BitTable[D2_0_5] = 0;
	BitTable[D2_0_6] = 0;
	BitTable[D2_0_7] = 0;
	BitTable[D2_0_8] = 0;
	BitTable[D2_0_9] = 0;
	BitTable[D2_0_10] = 0;
	BitTable[D2_0_11] = 0;

	BitTable[ D2_1_1] = 1;
	BitTable[D2_1_2] = 1;
	BitTable[D2_1_3] = 1;
	BitTable[D2_1_4] = 1;
	BitTable[D2_1_5] = 1;
	BitTable[D2_1_6] = 1;
	BitTable[D2_1_7] = 1;
	BitTable[D2_1_8] = 1;
	BitTable[D2_1_9] = 1;
	BitTable[D2_1_10] = 1;
	BitTable[D2_1_11] = 1;

	BitTable[ D3_0_1] = 0;
	BitTable[D3_0_2] = 0;
	BitTable[D3_0_3] = 0;
	BitTable[D3_0_4] = 0;
	BitTable[D3_0_5] = 0;
	BitTable[D3_0_6] = 0;
	BitTable[D3_0_7] = 0;
	BitTable[D3_0_8] = 0;
	BitTable[D3_0_9] = 0;
	BitTable[D3_0_10] = 0;
	BitTable[D3_0_11] = 0;

	BitTable[ D3_1_1] = 1;
	BitTable[D3_1_2] = 1;
	BitTable[D3_1_3] = 1;
	BitTable[D3_1_4] = 1;
	BitTable[D3_1_5] = 1;
	BitTable[D3_1_6] = 1;
	BitTable[D3_1_7] = 1;
	BitTable[D3_1_8] = 1;
	BitTable[D3_1_9] = 1;
	BitTable[D3_1_10] = 1;
	BitTable[D3_1_11] = 1;

	BitTable[D4_0_1] = 0;
	BitTable[D4_0_2] = 0;
	BitTable[D4_0_3] = 0;
	BitTable[D4_0_4] = 0;
	BitTable[D4_0_5] = 0;
	BitTable[D4_0_6] = 0;
	BitTable[D4_0_7] = 0;
	BitTable[D4_0_8] = 0;
	BitTable[D4_0_9] = 0;
	BitTable[D4_0_10] = 0;
	BitTable[D4_0_11] = 0;

	BitTable[ D4_1_1] = 1;
	BitTable[D4_1_2] = 1;
	BitTable[D4_1_3] = 1;
	BitTable[D4_1_4] = 1;
	BitTable[D4_1_5] = 1;
	BitTable[D4_1_6] = 1;
	BitTable[D4_1_7] = 1;
	BitTable[D4_1_8] = 1;
	BitTable[D4_1_9] = 1;
	BitTable[D4_1_10] = 1;
	BitTable[D4_1_11] = 1;

	BitTable[ D5_0_1] = 0;
	BitTable[D5_0_2] = 0;
	BitTable[D5_0_3] = 0;
	BitTable[D5_0_4] = 0;
	BitTable[D5_0_5] = 0;
	BitTable[D5_0_6] = 0;
	BitTable[D5_0_7] = 0;
	BitTable[D5_0_8] = 0;
	BitTable[D5_0_9] = 0;
	BitTable[D5_0_10] = 0;
	BitTable[D5_0_11] = 0;

	BitTable[ D5_1_1] = 1;
	BitTable[D5_1_2] = 1;
	BitTable[D5_1_3] = 1;
	BitTable[D5_1_4] = 1;
	BitTable[D5_1_5] = 1;
	BitTable[D5_1_6] = 1;
	BitTable[D5_1_7] = 1;
	BitTable[D5_1_8] = 1;
	BitTable[D5_1_9] = 1;
	BitTable[D5_1_10] = 1;
	BitTable[D5_1_11] = 1;

	BitTable[ D6_0_1] = 0;
	BitTable[D6_0_2] = 0;
	BitTable[D6_0_3] = 0;
	BitTable[D6_0_4] = 0;
	BitTable[D6_0_5] = 0;
	BitTable[D6_0_6] = 0;
	BitTable[D6_0_7] = 0;
	BitTable[D6_0_8] = 0;
	BitTable[D6_0_9] = 0;
	BitTable[D6_0_10] = 0;
	BitTable[D6_0_11] = 0;

	BitTable[ D6_1_1] = 1;
	BitTable[D6_1_2] = 1;
	BitTable[D6_1_3] = 1;
	BitTable[D6_1_4] = 1;
	BitTable[D6_1_5] = 1;
	BitTable[D6_1_6] = 1;
	BitTable[D6_1_7] = 1;
	BitTable[D6_1_8] = 1;
	BitTable[D6_1_9] = 1;
	BitTable[D6_1_10] = 1;
	BitTable[D6_1_11] = 1;


	BitTable[ D7_0_1] = 0;
	BitTable[D7_0_2] = 0;
	BitTable[D7_0_3] = 0;
	BitTable[D7_0_4] = 0;
	BitTable[D7_0_5] = 0;
	BitTable[D7_0_6] = 0;
	BitTable[D7_0_7] = 0;
	BitTable[D7_0_8] = 0;
	BitTable[D7_0_9] = 0;
	BitTable[D7_0_10] = 0;
	BitTable[D7_0_11] = 0;

	BitTable[ D7_1_1] = 1;
	BitTable[D7_1_2] = 1;
	BitTable[D7_1_3] = 1;
	BitTable[D7_1_4] = 1;
	BitTable[D7_1_5] = 1;
	BitTable[D7_1_6] = 1;
	BitTable[D7_1_7] = 1;
	BitTable[D7_1_8] = 1;
	BitTable[D7_1_9] = 1;
	BitTable[D7_1_10] = 1;
	BitTable[D7_1_11] = 1;

	BitTable[ D8_0_1] = 0;
	BitTable[D8_0_2] = 0;
	BitTable[D8_0_3] = 0;
	BitTable[D8_0_4] = 0;
	BitTable[D8_0_5] = 0;
	BitTable[D8_0_6] = 0;
	BitTable[D8_0_7] = 0;
	BitTable[D8_0_8] = 0;
	BitTable[D8_0_9] = 0;
	BitTable[D8_0_10] = 0;
	BitTable[D8_0_11] = 0;

	BitTable[ D8_1_1] = 1;
	BitTable[D8_1_2] = 1;
	BitTable[D8_1_3] = 1;
	BitTable[D8_1_4] = 1;
	BitTable[D8_1_5] = 1;
	BitTable[D8_1_6] = 1;
	BitTable[D8_1_7] = 1;
	BitTable[D8_1_8] = 1;
	BitTable[D8_1_9] = 1;
	BitTable[D8_1_10] = 1;
	BitTable[D8_1_11] = 1;

	BitTable[ D9_0_1] = 0;
	BitTable[D9_0_2] = 0;
	BitTable[D9_0_3] = 0;
	BitTable[D9_0_4] = 0;
	BitTable[D9_0_5] = 0;
	BitTable[D9_0_6] = 0;
	BitTable[D9_0_7] = 0;
	BitTable[D9_0_8] = 0;
	BitTable[D9_0_9] = 0;
	BitTable[D9_0_10] = 0;
	BitTable[D9_0_11] = 0;

	BitTable[ D9_1_1] = 1;
	BitTable[D9_1_2] = 1;
	BitTable[D9_1_3] = 1;
	BitTable[D9_1_4] = 1;
	BitTable[D9_1_5] = 1;
	BitTable[D9_1_6] = 1;
	BitTable[D9_1_7] = 1;
	BitTable[D9_1_8] = 1;
	BitTable[D9_1_9] = 1;
	BitTable[D9_1_10] = 1;
	BitTable[D9_1_11] = 1;

	BitTable[ D10_0_1] = 0;
	BitTable[D10_0_2] = 0;
	BitTable[D10_0_3] = 0;
	BitTable[D10_0_4] = 0;
	BitTable[D10_0_5] = 0;
	BitTable[D10_0_6] = 0;
	BitTable[D10_0_7] = 0;
	BitTable[D10_0_8] = 0;
	BitTable[D10_0_9] = 0;
	BitTable[D10_0_10] = 0;
	BitTable[D10_0_11] = 0;

	BitTable[ D10_1_1] = 1;
	BitTable[D10_1_2] = 1;
	BitTable[D10_1_3] = 1;
	BitTable[D10_1_4] = 1;
	BitTable[D10_1_5] = 1;
	BitTable[D10_1_6] = 1;
	BitTable[D10_1_7] = 1;
	BitTable[D10_1_8] = 1;
	BitTable[D10_1_9] = 1;
	BitTable[D10_1_10] = 1;
	BitTable[D10_1_11] = 1;

	BitTable[ D11_0_1] = 0;
	BitTable[D11_0_2] = 0;
	BitTable[D11_0_3] = 0;
	BitTable[D11_0_4] = 0;
	BitTable[D11_0_5] = 0;
	BitTable[D11_0_6] = 0;
	BitTable[D11_0_7] = 0;
	BitTable[D11_0_8] = 0;
	BitTable[D11_0_9] = 0;
	BitTable[D11_0_10] = 0;
	BitTable[D11_0_11] = 0;

	BitTable[ D11_1_1] = 1;
	BitTable[D11_1_2] = 1;
	BitTable[D11_1_3] = 1;
	BitTable[D11_1_4] = 1;
	BitTable[D11_1_5] = 1;
	BitTable[D11_1_6] = 1;
	BitTable[D11_1_7] = 1;
	BitTable[D11_1_8] = 1;
	BitTable[D11_1_9] = 1;
	BitTable[D11_1_10] = 1;
	BitTable[D11_1_11] = 1;
}
ViterbiSoft::~ViterbiSoft()
{}

void ViterbiSoft::decode ( double dibit[2] )
{
	double min;
	int i, k;
	min = 1.0E99;
	MinimumPath = 0;
	double d[2];
	d[0] = dibit[0];
	d[1] = dibit[1];
	double x;
	RTTYSampleStates s;
	double distance[2*NumberOfRTTYSampleStates];
  RTTYSampleStates bitestimate[2*NumberOfRTTYSampleStates][MEMORYLENGTH];

	for ( i = 0;i < 2*NumberOfRTTYSampleStates;i++ )
		distance[i] = 10000.;
	for ( i = 0; i < NumberofStates; i++ )
	{
		for ( int j = 0; j < 2;j++ )
		{
			s = StateTable[j][i];

//  x = Metric[i] + ( *dist ) ( dibit,BitTable[StateTable[j][i]]  );
			x = Metric[i] + ( *dist ) ( dibit, j );
			if ( x < distance[s+j*NumberofStates] )
			{
				distance[s+j*NumberofStates] = x;

				if ( x < min )
				{
					min = x;
					MinimumPath = s;
				}
				for ( k = 0;k < MEMORYLENGTH;k++ )
					bitestimate[s+j*NumberofStates][k] = PathMem[i][k];
				bitestimate[s+j*NumberofStates][pathPointer] = StateTable[j][i];
			}
		}
	}
	for ( i = 0;i < NumberofStates ;i++ )
	{
		if ( distance[i] < distance[i+NumberofStates] )
		{
			Metric[i] = distance[i] - min;
			for ( k = 0;k < MEMORYLENGTH;k++ )
				PathMem[i][k] = bitestimate[i][k];
		}
		else
		{
			Metric[i] = distance[NumberofStates+i] - min;
			for ( k = 0;k < MEMORYLENGTH;k++ )
				PathMem[i][k] = bitestimate[NumberofStates+i][k];

		}
	}
	pathPointer = ( pathPointer + 1 ) % MEMORYLENGTH;
}
int ViterbiSoft::getBit ( unsigned int i )
{
	int pos;
	pos = ( pathPointer + 127 - i ) % MEMORYLENGTH;
	return ( BitTable[ PathMem[MinimumPath][pos]] );
}
ViterbiSoft::RTTYSampleStates ViterbiSoft::getState(unsigned int i)
{
	int pos;
	pos = ( pathPointer + 127 - i ) % MEMORYLENGTH;
	return ( PathMem[MinimumPath][pos] );
}
