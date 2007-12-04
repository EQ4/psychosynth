/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) 2007 Juan Pedro Bolivar Puente                          *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "net/OSCBroadcast.h"
#include "net/OSCMisc.h"

using namespace std;

void OSCBroadcast::broadcastMessage(const char* path, lo_message msg)
{
    for (list<lo_address>::iterator it = m_dest.begin();
	 it != m_dest.end(); ++it)
	lo_send_message(*it, path, msg);
}

void OSCBroadcast::broadcastMessageFrom(const char* path, lo_message msg, lo_address source)
{    
    for (list<lo_address>::iterator it = m_dest.begin();
	 it != m_dest.end(); ++it)
	if (!lo_address_equals(*it, source))
	    lo_send_message(*it, path, msg);
}
