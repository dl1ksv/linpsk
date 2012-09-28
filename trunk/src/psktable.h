#if !defined(PSKTablesXYZ)
#define PSKTablesXYZ
//////////////////////////////////////////////////////////////////////
// Copyright 1999.    Moe Wheatley AE4JY  <ae4jy@mindspring.com>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//////////////////////////////////////////////////////////////////////
//
////////////////////  Various constant tabels  //////////////////////////
// The word in the CW table is divided into 8 groups of two bits starting
// at the msb side.  The two bits represent one of four possible states.
//  00 - end of character
//  01 - DOT
//  10 - DASH 
//  11 - SPACE of two dot times
const unsigned short int CW_TABLE[59]=
{
	0xF000,		// 1111 0000 0000 0000b  ( 32)       WORD SPACE
	0x0000,		// 0000 0000 0000 0000b  ( 33)  !
	0x0000,		// 0000 0000 0000 0000b  ( 34)  "
	0x0000,		// 0000 0000 0000 0000b  ( 35)  #
	0x0000,		// 0000 0000 0000 0000b  ( 36)  $
	0x0000,		// 0000 0000 0000 0000b  ( 37)  %
	0x0000,		// 0000 0000 0000 0000b  ( 38)  &
	0x0000,		// 0000 0000 0000 0000b  ( 39)  '
	0x0000,		// 0000 0000 0000 0000b  ( 40)  (
	0x0000,		// 0000 0000 0000 0000b  ( 41)  )
	0x566C,		// 0101 0110 0110 1100b  ( 42)  *    ...-.-  SK
	0x6670,		// 0110 0110 0111 0000b  ( 43)  +    .-.-.   AR
	0xA5AC,		// 1010 0101 1010 1100b  ( 44)  ,    --..--
	0x0000,		// 0000 0000 0000 0000b  ( 45)  -
	0x666C,		// 0110 0110 0110 1100b  ( 46)  .    .-.-.-
	0x9670,		// 1001 0110 0111 0000b  ( 47)  /    -..-.
	0xAAB0,		// 1010 1010 1011 0000b  ( 48)  0    -----
	0x6AB0,		// 0110 1010 1011 0000b  ( 49)  1    .----
	0x5AB0,		// 0101 1010 1011 0000b  ( 50)  2    ..---
	0x56B0,		// 0101 0110 1011 0000b  ( 51)  3    ...--
	0x55B0,		// 0101 0101 1011 0000b  ( 52)  4    ....-
	0x5570,		// 0101 0101 0111 0000b  ( 53)  5    .....
	0x9570,		// 1001 0101 0111 0000b  ( 54)  6    -....
	0xA570,		// 1010 0101 0111 0000b  ( 55)  7    --...
	0xA970,		// 1010 1001 0111 0000b  ( 56)  8    ---..
	0xAA70,		// 1010 1010 0111 0000b  ( 57)  9    ----.
	0x0000,		// 0000 0000 0000 0000b  ( 58)  :
	0x0000,		// 0000 0000 0000 0000b  ( 59)  ;
	0x0000,		// 0000 0000 0000 0000b  ( 60)  <    
	0x95B0,		// 1001 0101 1011 0000b  ( 61)  =    -...-   BT
	0x0000,		// 0000 0000 0000 0000b  ( 62)  >
	0x5A5C,		// 0101 1010 0101 1100b  ( 63)  ?    ..--..
	0x0000,		// 0000 0000 0000 0000b  ( 64)  @
	0x6C00,		// 0110 1100 0000 0000b  ( 65)  A    .-
	0x95C0,		// 1001 0101 1100 0000b  ( 66)  B    -...
	0x99C0,		// 1001 1001 1100 0000b  ( 67)  C    -.-.
	0x9700,		// 1001 0111 0000 0000b  ( 68)  D    -..
	0x7000,		// 0111 0000 0000 0000b  ( 69)  E    .
	0x59C0,		// 0101 1001 1100 0000b  ( 70)  F    ..-.
	0xA700,		// 1010 0111 0000 0000b  ( 71)  G    --.
	0x55C0,		// 0101 0101 1100 0000b  ( 72)  H    ....
	0x5C00,		// 0101 1100 0000 0000b  ( 73)  I    ..
	0x6AC0,		// 0110 1010 1100 0000b  ( 74)  J    .---
	0x9B00,		// 1001 1011 0000 0000b  ( 75)  K    -.-
	0x65C0,		// 0110 0101 1100 0000b  ( 76)  L    .-..
	0xAC00,		// 1010 1100 0000 0000b  ( 77)  M    --
	0x9C00,		// 1001 1100 0000 0000b  ( 78)  N    -.
	0xAB00,		// 1010 1011 0000 0000b  ( 79)  O    ---
	0x69C0,		// 0110 1001 1100 0000b  ( 80)  P    .--.
	0xA6C0,		// 1010 0110 1100 0000b  ( 81)  Q    --.-
	0x6700,		// 0110 0111 0000 0000b  ( 82)  R    .-.
	0x5700,		// 0101 0111 0000 0000b  ( 83)  S    ...
	0xB000,		// 1011 0000 0000 0000b  ( 84)  T    -
	0x5B00,		// 0101 1011 0000 0000b  ( 85)  U    ..-
	0x56C0,		// 0101 0110 1100 0000b  ( 86)  V    ...-
	0x6B00,		// 0110 1011 0000 0000b  ( 87)  W    .--
	0x96C0,		// 1001 0110 1100 0000b  ( 88)  X    -..-
	0x9AC0,		// 1001 1010 1100 0000b  ( 89)  Y    -.--
	0xA5C0		// 1010 0101 1100 0000b  ( 90)  Z    --..
};



// For the QPSK modulator/demodulator, rate 1/2 constraint length 5 
//   convolutional FEC coding is used.
// The generator polynomials used are:
//  g1(x) = x^4 + x^3           + 1		= 0x19
//  g0(x) = x^4	+       x^2 + x + 1		= 0x17
// 
//                                g1(x)
//              /----+--------+--------------------------+
//            /      |        |                          |
// symbol msb       ---      ---      ---      ---      ---
//                 | b4|<---| b3|<---| b2|<---| b1|<---| b0| <-- inverted data in
// symbol lsb       ---      ---      ---      ---      ---
//            \      |                 |        |        |
//              \----+-----------------+--------+--------+
//                                g0(x)
//
// Lookup table to get symbol from non-inverted data stream
/**
static const unsigned char ConvolutionCodeTable[32] =
{
	2, 1, 3, 0, 3, 0, 2, 1,
	0, 3, 1, 2, 1, 2, 0, 3,
	1, 2, 0, 3, 0, 3, 1, 2,
	3, 0, 2, 1, 2, 1, 3, 0 
};
**/
#endif
