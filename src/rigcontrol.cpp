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

#include "rigcontrol.h"
#include "parameter.h"

extern Parameter settings;

RigControl::RigControl()
{
  frequency=0;
  pwr=0;
  connected=false;
  rig= NULL;
}
int RigControl::connectRig()
{
   int rc;
   rc = -1;
   if( (settings.rigModelNumber == 0) || (settings.rigDevice == "none") )
     return RIG_OK;
  rig=rig_init(settings.rigModelNumber);
  if(rig->caps->port_type != RIG_PORT_NONE)
  {
    if(rig->caps->port_type == RIG_PORT_SERIAL)
      {
        strcpy(rig->state.rigport.pathname,settings.rigDevice.toLatin1());
        rig->state.rigport.parm.serial.rate=settings.baudrate;
        rig->state.rigport.parm.serial.handshake = (serial_handshake_e) settings.handshake;
        rig->state.rigport.retry=3;

      }
    if (rig)
      {
        rc=rig_open(rig);
        if(rc == RIG_OK )
          connected=true;
      }
  }
  return rc;
}
int RigControl::get_frequency()
{
  int rc;
  freq_t freq;
  if(connected)
    {
      rc = rig_get_freq(rig, RIG_VFO_CURR, &freq);
      if( rc == RIG_OK)
        frequency = (int) freq;
    }
  return frequency;
}

void RigControl::set_frequency(int f)
{

  freq_t freq;
  if(connected)
    {
     freq=f;
     if(rig_set_freq(rig, RIG_VFO_CURR, freq) == RIG_OK)
      frequency=f;
    }
}

int RigControl::get_pwr()
{
  value_t p;
  if(connected)
    {
      if(rig_get_level(rig,RIG_VFO_CURR,RIG_LEVEL_RFPOWER,&p)== RIG_OK )
        pwr=p.f*100;
    }
  return pwr;
}

void RigControl::set_pwr(int p)
{
 value_t pset;
 if( connected )
   {
     pset.f = (float) p/100.;
     if( rig_set_level(rig,RIG_VFO_CURR,RIG_LEVEL_RFPOWER,pset)== RIG_OK)
       pwr=p;
   }

}
void RigControl::disconnectRig()
{
  if (rig != NULL)
    rig_close(rig);
  rig = NULL;
}
