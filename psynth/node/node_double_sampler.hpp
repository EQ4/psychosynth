/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) Juan Pedro Bolivar Puente 2007                          *
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

#ifndef PSYNTH_NODE_DOUBLE_SAMPLER_H
#define PSYNTH_NODE_DOUBLE_SAMPLER_H

#include <psynth/node/node.hpp>
#include <psynth/input/file_reader_any.hpp>
#include <psynth/input/file_reader_fetcher.hpp>
#include <psynth/node/node_factory.hpp>
#include <psynth/common/scaler_st.hpp>

namespace psynth
{

class mutex;

class node_double_sampler : public node
{
public:
    enum in_audio_socket_id {
	N_IN_A_SOCKETS
    };
	
    enum in_control_socket_id {
	IN_C_BLEND,
	IN_C_TRIGGER,
	N_IN_C_SOCKETS
    };

    enum param_id {
	PARAM_FILE_ONE = node::N_COMMON_PARAMS,
	PARAM_FILE_TWO,
	PARAM_AMPLITUDE,
	PARAM_BLEND,
	N_PARAM
    };
    
    enum out_audio_socket_id {
	OUT_A_OUTPUT,
	N_OUT_A_SOCKETS
    };

    enum out_control_socket_id {
	N_OUT_C_SOCKETS
    };
    
private:
    file_reader_any m_reader_one;
    file_reader_fetcher m_fetcher_one;
    file_reader_any m_reader_two;
    file_reader_fetcher m_fetcher_two;  
    audio_buffer m_inbuf_one;
    audio_buffer m_inbuf_two;
        
    float m_param_ampl;
    float m_param_blend;

    bool m_restart;
    
    std::string m_param_file_one;
    std::string m_param_file_two;
    
    boost::mutex m_update_mutex;
    
    void on_file_one_change (node_param& par);
    void on_file_two_change (node_param& par);
    void read (audio_buffer& buf, int start, int end);
    void restart();
    
    void do_update (const node* caller, int caller_port_type, int caller_port);
    void do_advance ();
    void on_info_change ();
    
public:
    node_double_sampler (const audio_info& info);
    ~node_double_sampler ();
};

PSYNTH_DECLARE_NODE_FACTORY (node_double_sampler, "double_sampler");

} /* namespace psynth */

#endif /* PSYNTH_OBJECTSAMPLER_H */