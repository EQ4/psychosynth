/**
 *  Time-stamp:  <2011-06-13 16:50:07 raskolnikov>
 *
 *  @file        port_fwd.hpp
 *  @author      Juan Pedro Bolívar Puente <raskolnikov@es.gnu.org>
 *  @date        Mon Jun 13 16:47:22 2011
 *
 *  @brief Port forward declarations.
 */

/*
 *  Copyright (C) 2011 Juan Pedro Bolívar Puente
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

#ifndef PSYNTH_GRAPH_PORT_FWD_HPP_
#define PSYNTH_GRAPH_PORT_FWD_HPP_

#include <psynth/base/declare.hpp>

namespace psynth
{
namespace graph
{

PSYNTH_DECLARE_TYPE (port_base);
PSYNTH_DECLARE_TYPE (out_port_base);
PSYNTH_DECLARE_TYPE (in_port_base);

} /* namespace graph */
} /* namespace psynth */

#endif /* PSYNTH_GRAPH_PORT_FWD_HPP_ */

