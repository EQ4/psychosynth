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

#ifndef LOGGER_H
#define LOGGER_H

#include <map>
#include <list>
#include <iostream>
#include <common/Singleton.h>

class Log;

class LogDumper
{
public:
    virtual void dump(Log& log, int level, const std::string& msg) = 0;
};

class Log
{
    std::map<std::string, Log> m_childs;
    std::list<LogDumper*> m_dumpers;
    Log* m_parent;
    std::string m_name;
    bool m_isinit;
    
public:
    enum Level {
	INFO,
	WARNING,
	FATAL
    };

    static const char* levelName(int level) {
	switch(level) {
	case INFO:
	    return "INFO";
	case WARNING:
	    return "WARNING";
	case FATAL:
	    return "FATAL";
	default:
	    return "UNKNOWN";
	};
    };
    
    Log() :
	m_parent(NULL), m_isinit(false) {}

    ~Log();
    
    void init(const std::string& name, Log* parent) {
	m_name = name;
	m_parent = parent;
    }

    bool isInit() {
	return m_isinit;
    }
    
    const std::string& getName() const {
	return m_name;
    }

    void addDumper(LogDumper* d) {
	m_dumpers.push_back(d);
    }

    void deleteDumper(LogDumper* d) {
	m_dumpers.remove(d);
    }
       
    const Log* getParent() const {
	return m_parent;
    }
    
    Log& getChild(const std::string& name) {
	if (!m_childs[name].isInit())
	    m_childs[name].init(name, this);
	return m_childs[name];
    };

    Log* getParent() {
	return m_parent;
    }
        
    void log(std::string& child, int level, const std::string& msg) {
	getChild(child).log(level, msg);
    }

    std::string getPathName() {
	if (m_parent)
	    return m_parent->getPathName() + "/" + m_name; 
	else
	    return "/" + m_name;
    }
    
    void log(int level, const std::string& msg);
    
    Log& getPath(std::string path);
};

class Logger : public Singleton<Logger>,
	       public Log
{
public:
};

class LogDefaultDumper : public LogDumper {
public:
    void dump(Log& log, int level, const std::string& msg) {
	if (level > Log::INFO)
	    std::cerr << '[' << log.getPathName() << "] "
		      << Log::levelName(level)
		      << ": " << msg << std::endl;
	else
	    std::cout << '[' << log.getPathName() << "] "
		      << Log::levelName(level)
		      << ": " << msg << std::endl;
    }
};

#endif


