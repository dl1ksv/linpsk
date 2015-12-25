/***************************************************************************
 *   Copyright (C) 2014 by Volker Schroer, DL1KSV                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef RIGCONTROL_H
#define RIGCONTROL_H

#include <hamlib/rig.h>

class RigControl
{
public:
  RigControl();
  int get_frequency();
  void set_frequency(int f);
  int get_pwr();
  void set_pwr(int p);
  int connectRig();
  void disconnectRig();
private:

RIG *rig;

int frequency;
int pwr;

bool connected;
};

#endif // RIGCONTROL_H
