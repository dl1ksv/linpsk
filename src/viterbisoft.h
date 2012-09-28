/***************************************************************************
                          viterbisoft.h  -  description
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

#ifndef viterbisoft_H
#define viterbisoft_H

#define MEMORYLENGTH 256
#define NumberOfRTTYSampleStates 269 
/**
  *@author Volker Schroer
  */

class ViterbiSoft
{
	public:
		ViterbiSoft ( const double ( * ) ( double *, int ) );
		~ViterbiSoft();
//  void decode(int,const double (*)(int,int));
		void decode ( double * );

		int getBit ( unsigned int i );
		enum RTTYSampleStates {Start1 = 0,Start2,Start3,Start4,Start5,Start6,Start7,Start8,Start9,Start10,Start11,
Stop1,Stop2,Stop3,Stop4,Stop5,Stop6,Stop7,Stop8,Stop9,Stop10,Stop11,Stop12,Stop13,Stop14,Stop15,Stop16,
 D1_0_1,D1_0_2,D1_0_3,D1_0_4,D1_0_5,D1_0_6,D1_0_7,D1_0_8,D1_0_9,D1_0_10,D1_0_11,
 D1_1_1,D1_1_2,D1_1_3,D1_1_4,D1_1_5,D1_1_6,D1_1_7,D1_1_8,D1_1_9,D1_1_10,D1_1_11,
 D2_0_1,D2_0_2,D2_0_3,D2_0_4,D2_0_5,D2_0_6,D2_0_7,D2_0_8,D2_0_9,D2_0_10,D2_0_11,
 D2_1_1,D2_1_2,D2_1_3,D2_1_4,D2_1_5,D2_1_6,D2_1_7,D2_1_8,D2_1_9,D2_1_10,D2_1_11,
 D3_0_1,D3_0_2,D3_0_3,D3_0_4,D3_0_5,D3_0_6,D3_0_7,D3_0_8,D3_0_9,D3_0_10,D3_0_11,
 D3_1_1,D3_1_2,D3_1_3,D3_1_4,D3_1_5,D3_1_6,D3_1_7,D3_1_8,D3_1_9,D3_1_10,D3_1_11,
 D4_0_1,D4_0_2,D4_0_3,D4_0_4,D4_0_5,D4_0_6,D4_0_7,D4_0_8,D4_0_9,D4_0_10,D4_0_11,
 D4_1_1,D4_1_2,D4_1_3,D4_1_4,D4_1_5,D4_1_6,D4_1_7,D4_1_8,D4_1_9,D4_1_10,D4_1_11,
 D5_0_1,D5_0_2,D5_0_3,D5_0_4,D5_0_5,D5_0_6,D5_0_7,D5_0_8,D5_0_9,D5_0_10,D5_0_11,
 D5_1_1,D5_1_2,D5_1_3,D5_1_4,D5_1_5,D5_1_6,D5_1_7,D5_1_8,D5_1_9,D5_1_10,D5_1_11,
 D6_0_1,D6_0_2,D6_0_3,D6_0_4,D6_0_5,D6_0_6,D6_0_7,D6_0_8,D6_0_9,D6_0_10,D6_0_11,
 D6_1_1,D6_1_2,D6_1_3,D6_1_4,D6_1_5,D6_1_6,D6_1_7,D6_1_8,D6_1_9,D6_1_10,D6_1_11,
 D7_0_1,D7_0_2,D7_0_3,D7_0_4,D7_0_5,D7_0_6,D7_0_7,D7_0_8,D7_0_9,D7_0_10,D7_0_11,
 D7_1_1,D7_1_2,D7_1_3,D7_1_4,D7_1_5,D7_1_6,D7_1_7,D7_1_8,D7_1_9,D7_1_10,D7_1_11,
 D8_0_1,D8_0_2,D8_0_3,D8_0_4,D8_0_5,D8_0_6,D8_0_7,D8_0_8,D8_0_9,D8_0_10,D8_0_11,
 D8_1_1,D8_1_2,D8_1_3,D8_1_4,D8_1_5,D8_1_6,D8_1_7,D8_1_8,D8_1_9,D8_1_10,D8_1_11,
 D9_0_1,D9_0_2,D9_0_3,D9_0_4,D9_0_5,D9_0_6,D9_0_7,D9_0_8,D9_0_9,D9_0_10,D9_0_11,
 D9_1_1,D9_1_2,D9_1_3,D9_1_4,D9_1_5,D9_1_6,D9_1_7,D9_1_8,D9_1_9,D9_1_10,D9_1_11,
 D10_0_1,D10_0_2,D10_0_3,D10_0_4,D10_0_5,D10_0_6,D10_0_7,D10_0_8,D10_0_9,D10_0_10,D10_0_11,
 D10_1_1,D10_1_2,D10_1_3,D10_1_4,D10_1_5,D10_1_6,D10_1_7,D10_1_8,D10_1_9,D10_1_10,D10_1_11,
 D11_0_1,D11_0_2,D11_0_3,D11_0_4,D11_0_5,D11_0_6,D11_0_7,D11_0_8,D11_0_9,D11_0_10,D11_0_11,
 D11_1_1,D11_1_2,D11_1_3,D11_1_4,D11_1_5,D11_1_6,D11_1_7,D11_1_8,D11_1_9,D11_1_10,D11_1_11};
    RTTYSampleStates getState(unsigned int i);
	private:
		int BitCount;
		int StopBitCount;
		int NumberofStates;
		int MinimumPath;
int BitTable[NumberOfRTTYSampleStates];
RTTYSampleStates StateTable[2][NumberOfRTTYSampleStates];
		const double ( *dist ) ( double [], int );
		double Metric[NumberOfRTTYSampleStates];
		RTTYSampleStates PathMem[NumberOfRTTYSampleStates][MEMORYLENGTH];
		int pathPointer;

};

#endif
