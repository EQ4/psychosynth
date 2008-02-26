/**************************************************************************
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

#ifndef PSYNTH_MISC_H
#define PSYNTH_MISC_H

#include <string>

namespace psynth
{

class NoCopy
{
protected:
    NoCopy() {}

private:
    NoCopy(const NoCopy&);
    NoCopy& operator= (const NoCopy&);
};

char* itoa(int val, int base);
char* ftoa(double f, double sigfigs);

std::string str_dirname(std::string str);

} /* namespace psynth */

#endif /* PSYNTH_MISC_H */