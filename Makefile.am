#
#   PSYCHOSYNTH
#   ===========
#              
#   Copyright (C) 2007, 2008 by Juan Pedro Bolivar Puente
#                                                        
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#                                      
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

AUTOMAKE_OPTIONS = 

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src data doc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libpsynth.pc

noinst_DATA = ChangeLog

AM_RECURSIVE_TARGETS = doc

.PHONY: log

log:
	\bzr log --gnu-changelog -r97.. > ChangeLog

