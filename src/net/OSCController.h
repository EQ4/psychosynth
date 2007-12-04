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

#ifndef OSCCONTROLLER_H
#define OSCCONTROLLER_H

#include <map>

#include "table/Table.h"
#include "net/OSCMisc.h"
#include "net/OSCBroadcast.h"

class OSCController : public TableListener,
		      public TableObjectListener,
		      public OSCBroadcast
{
    std::map<std::pair<int,int>, int> m_local_id;
    std::map<int, std::pair<int,int> > m_net_id;
    Table* m_table;
    int m_skip;
    int m_id;
    bool m_is_server;
    
    LO_HANDLER(OSCController, add);
    LO_HANDLER(OSCController, delete);
    LO_HANDLER(OSCController, move);
    LO_HANDLER(OSCController, param);
    LO_HANDLER(OSCController, activate);
    LO_HANDLER(OSCController, deactivate);

    void addToTable(Table* table) {
	table->addTableListener(this);
	table->addTableObjectListener(this);
    }

    void deleteFromTable(Table* table) {
	table->deleteTableListener(this);
	table->deleteTableObjectListener(this);
    }
    
public:
    OSCController(bool m_is_server = false);
    ~OSCController();

    void setID(int id) {
	m_id = id;
    }

    void setTable(Table* table) {
	if (m_table)
	    deleteFromTable(m_table);
	if (table)
	    addToTable(table);
	m_table = table;
    }

    void activate() {
	if (m_table)
	    addToTable(m_table);
    }

    void deactivate() {
	if (m_table)
	    deleteFromTable(m_table);
    }

    void clear() {
	deactivate();
	m_local_id.clear();
	m_net_id.clear();
	m_skip = 0;
	OSCBroadcast::clear();
    }

    void addToServer(lo_server s);
    
    void handleAddObject(TableObject& obj);
    void handleDeleteObject(TableObject& obj);
    void handleMoveObject(TableObject& obj);
    void handleActivateObject(TableObject& obj);
    void handleDeactivateObject(TableObject& obj);
    void handleSetParamObject(TableObject& ob, int param_id);
};

#endif /* OSCCONTROLLER_H */