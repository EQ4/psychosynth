/**
 *  Time-stamp:  <2010-10-18 17:45:55 raskolnikov>
 *
 *  @file        exception.hpp
 *  @author      Juan Pedro Bolívar Puente <raskolnikov@es.gnu.org>
 *  @date        2009
 *
 *  Basic exception facility that permits the ease definition of
 *  new exceptions that are hierarchically distributed and
 *  cooperate with the logging system.
 */

/*
 *  Copyright (C) 2009 Juan Pedro Bolívar Puente
 *
 *  This file is part of Psychosynth.
 *   
 *  Psychosynth is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Psychosynth is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef PSYNTH_EXCEPTION_H_
#define PSYNTH_EXCEPTION_H_

#include <exception>
#include <string>

namespace psynth
{
namespace base
{

class exception : public std::exception
{
public:
    virtual ~exception () throw () {}
    
    virtual void log () const {}
    virtual const char* what () const throw () = 0;
    virtual const char* where () const throw () = 0;
};

class error : public exception
{
public:
    virtual ~error () throw () {}

    virtual void log () const;
    
    virtual const char* what () const throw ()
    {
	return m_what.c_str ();
    }

    virtual const char* where () const throw ()
    {
	return m_where.c_str ();
    }

protected:    
    error (const std::string& where, const std::string& what) throw ();
    std::string default_error ();
    
private:
    std::string m_what;
    std::string m_where;
};
    
#define PSYNTH_DECLARE_ERROR(d_parent, d_error)				\
    class d_error : public d_parent					\
    {									\
    public:								\
	typedef d_parent base;						\
	d_error (const std::string& what);				\
	d_error ();							\
									\
    protected:								\
	d_error (const std::string& where, const std::string& what);	\
    };

#define PSYNTH_DEFINE_ERROR_WHERE_WHAT(d_error, d_where, d_what)	\
    d_error::d_error (const std::string& where, const std::string& what) \
	: base (d_where + std::string (".") + where, what)		\
    {}									\
    d_error::d_error (const std::string& what)				\
	: base (d_where, what)						\
    {}									\
    d_error::d_error ()							\
	: base (d_where, d_what)					\
    {}

#define PSYNTH_DEFINE_ERROR_WHERE(d_error, d_where)			\
    d_error::d_error (const std::string& where, const std::string& what) \
	: base (d_where + std::string (".") + where, what)		\
    {}									\
    d_error::d_error (const std::string& what)				\
	: base (d_where, what)						\
    {}									\
    d_error::d_error ()							\
	: base (d_where, default_error ())				\
    {}

#define PSYNTH_DEFINE_ERROR_WHAT(d_error, d_what)			\
    d_error::d_error (const std::string& where, const std::string& what) \
	: base (where, what)						\
    {}									\
    d_error::d_error (const std::string& what)				\
	: base (what)							\
    {}									\
    d_error::d_error ()							\
	: base (d_what)							\
    {}

#define PSYNTH_DEFINE_ERROR(d_error)					\
    d_error::d_error (const std::string& where, const std::string& what) \
	: base (where, what)						\
    {}									\
    d_error::d_error (const std::string& what)				\
	: base (what)							\
    {}									\
    d_error::d_error ()							\
	: base ()							\
    {}

PSYNTH_DECLARE_ERROR (error, psynth_error);
PSYNTH_DECLARE_ERROR (psynth_error, base_error);

} /* namespace base */
} /* namespace psynth */

#endif /* PSYNTH_EXCEPTION_H_ */