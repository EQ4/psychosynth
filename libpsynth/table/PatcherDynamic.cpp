/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) 2007 by Juan Pedro Bolivar Puente                       *
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

#include <algorithm>
#include "table/PatcherDynamic.h"

#include "node/node.h"
#include "node/node_output.h"
#include "node/node_audio_mixer.h"
#include "node/node_control_mixer.h"
#include "node/node_audio_oscillator.h"
#include "node/node_lfo.h"
#include "node/node_filter.h"
#include "node/node_sampler.h"
#include "node/node_step_seq.h"
#include "node/node_audio_noise.h"
#include "node/node_control_noise.h"
#include "node/node_echo.h"
#include "node/node_delay.h"

using namespace std;

namespace psynth
{

const int PATCHER_ANY  = -1;

struct PatcherData {
    int socket_type;
    int src_socket;
    int dest_socket;
};

/*
 * Each object has several inputs and output sockets. On a dinamically patched synth we only
 * want one output connected with one input. This table tells the dinamic patcher which
 * output should be connected with which inpunt on each object.
 * The first dimension is the source object type and the second is the destiny object type.
 */
const PatcherData PATCHER_TABLE[N_NODES][N_NODES] =
{
    /* node_output */
    {
	{node::LINK_NONE, 0, 0},  /* node_output */
	{node::LINK_NONE, 0, 0},  /* node_audio_mixer */
	{node::LINK_NONE, 0, 0}, /* node_control_mixer */
	{node::LINK_NONE, 0, 0},  /* node_audio_oscillator */
	{node::LINK_NONE, 0, 0},  /* node_lfo */
	{node::LINK_NONE, 0, 0},   /* node_filter */
	{node::LINK_NONE, 0, 0}, /* node_sampler */
	{node::LINK_NONE, 0, 0}, /* node_step_seq */
	{node::LINK_NONE, 0, 0}, /* node_audio_noise */	
	{node::LINK_NONE, 0, 0}, /* node_control_noise */
	{node::LINK_NONE, 0, 0}, /* node_echo */
	{node::LINK_NONE, 0, 0} /* node_delay */
    },

    /* node_audio_mixer */
    {
	{node::LINK_AUDIO, node_audio_mixer::OUT_A_OUTPUT, node_output::IN_A_INPUT},  /* node_output */
	{node::LINK_AUDIO, node_audio_mixer::OUT_A_OUTPUT, PATCHER_ANY}, /* node_audio_mixer */
	{node::LINK_NONE, 0, 0}, /* node_control_mixer */
	{node::LINK_NONE, 0, 0}, /* node_audio_oscillator */
	{node::LINK_NONE, 0, 0}, /* node_lfo */
	{node::LINK_AUDIO, node_audio_mixer::OUT_A_OUTPUT, node_filter::IN_A_INPUT}, /* node_filter */
	{node::LINK_NONE, 0, 0}, /* node_sampler */
	{node::LINK_NONE, 0, 0}, /* node_step_seq */
	{node::LINK_NONE, 0, 0}, /* node_audio_noise */
	{node::LINK_NONE, 0, 0}, /* node_control_noise */
	{node::LINK_AUDIO, node_audio_mixer::OUT_A_OUTPUT, node_echo::IN_A_INPUT}, /* node_echo */
	{node::LINK_AUDIO, node_audio_mixer::OUT_A_OUTPUT, node_delay::IN_A_INPUT} /* node_delay */
    },

    /* node_control_mixer */
    {
	{node::LINK_NONE, 0, 0},  /* node_output */
	{node::LINK_CONTROL, node_control_mixer::OUT_C_OUTPUT, node_mixer::IN_C_AMPLITUDE}, /* node_audio_mixer */
	/* TODO: Decide a way to allow controlling control signals amplitude. */
	{node::LINK_CONTROL, node_control_mixer::OUT_C_OUTPUT, PATCHER_ANY}, /* node_control_mixer */
	{node::LINK_CONTROL, node_control_mixer::OUT_C_OUTPUT, node_oscillator::IN_C_FREQUENCY},/* node_audio_oscillator */
	{node::LINK_CONTROL, node_control_mixer::OUT_C_OUTPUT, node_oscillator::IN_C_FREQUENCY}, /* node_lfo */
	{node::LINK_CONTROL, node_control_mixer::OUT_C_OUTPUT, node_filter::IN_C_CUTOFF}, /* node_filter */
	{node::LINK_CONTROL, node_control_mixer::OUT_C_OUTPUT, node_sampler::IN_C_RATE}, /* node_sampler */
	{node::LINK_CONTROL, node_control_mixer::OUT_C_OUTPUT, node_step_seq::IN_C_BPM}, /* node_step_seq */
	{node::LINK_CONTROL, node_control_mixer::OUT_C_OUTPUT, node_audio_noise::IN_C_AMPLITUDE}, /* node_audio_noise */
	{node::LINK_CONTROL, node_control_mixer::OUT_C_OUTPUT, node_control_noise::IN_C_AMPLITUDE}, /* node_control_noise */
	{node::LINK_CONTROL, node_control_mixer::OUT_C_OUTPUT, node_echo::IN_C_FEEDBACK}, /* node_echo */
	{node::LINK_CONTROL, node_control_mixer::OUT_C_OUTPUT, node_echo::IN_C_DELAY} /* node_delay */
    },
    
    /* node_audio_oscillator */
    {
	{node::LINK_NONE, 0, 0}, /* node_output */
	{node::LINK_AUDIO, node_audio_oscillator::OUT_A_OUTPUT, PATCHER_ANY}, /* node_audio_mixer */
	{node::LINK_NONE, 0, 0}, /* node_control_mixer */
	{node::LINK_NONE, 0, 0}, /* node_audio_oscillator */
	{node::LINK_NONE, 0, 0}, /* node_lfo */
	{node::LINK_AUDIO, node_audio_oscillator::OUT_A_OUTPUT, node_filter::IN_A_INPUT}, /* node_filter */
	{node::LINK_NONE, 0, 0}, /* node_sampler */
	{node::LINK_NONE, 0, 0}, /* node_step_seq */
	{node::LINK_NONE, 0, 0}, /* node_audio_noise */
	{node::LINK_NONE, 0, 0}, /* node_control_noise */
	{node::LINK_AUDIO, node_audio_oscillator::OUT_A_OUTPUT, node_echo::IN_A_INPUT}, /* node_echo */
	{node::LINK_AUDIO, node_audio_oscillator::OUT_A_OUTPUT, node_delay::IN_A_INPUT}, /* node_delay */
    },

    /* node_lfo */
    {
	{node::LINK_NONE, 0, 0}, /* node_output */	
	{node::LINK_CONTROL, node_lfo::OUT_C_OUTPUT, node_mixer::IN_C_AMPLITUDE}, /* node_audio_mixer */
	{node::LINK_CONTROL, node_lfo::OUT_C_OUTPUT, PATCHER_ANY}, /* node_control_mixer */
	{node::LINK_CONTROL, node_lfo::OUT_C_OUTPUT, node_oscillator::IN_C_FREQUENCY}, /* node_audio_oscillator */
	{node::LINK_CONTROL, node_lfo::OUT_C_OUTPUT, node_oscillator::IN_C_FREQUENCY}, /* node_lfo */
	{node::LINK_CONTROL, node_lfo::OUT_C_OUTPUT, node_filter::IN_C_CUTOFF},  /* node_filter */
	{node::LINK_CONTROL, node_lfo::OUT_C_OUTPUT, node_sampler::IN_C_RATE}, /* node_sampler */
	{node::LINK_CONTROL, node_lfo::OUT_C_OUTPUT, node_step_seq::IN_C_BPM}, /* node_step_seq */
	{node::LINK_CONTROL, node_lfo::OUT_C_OUTPUT, node_audio_noise::IN_C_AMPLITUDE}, /* node_audio_noise */
	{node::LINK_CONTROL, node_lfo::OUT_C_OUTPUT, node_control_noise::IN_C_AMPLITUDE}, /* node_control_noise */
	{node::LINK_CONTROL, node_lfo::OUT_C_OUTPUT, node_echo::IN_C_FEEDBACK}, /* node_echo */
	{node::LINK_CONTROL, node_lfo::OUT_C_OUTPUT, node_delay::IN_C_DELAY}, /* node_delay */
    },

    /* node_filter */
    {
	{node::LINK_NONE, 0, 0}, /* node_output */
	{node::LINK_AUDIO, node_filter::OUT_A_OUTPUT, PATCHER_ANY}, /* node_audio_mixer */
	{node::LINK_NONE, 0, 0}, /* node_control_mixer */
	{node::LINK_NONE, 0, 0}, /* node_audio_oscillator */
	{node::LINK_NONE, 0, 0}, /* node_lfo */
	{node::LINK_AUDIO, node_filter::OUT_A_OUTPUT, node_filter::IN_A_INPUT}, /* node_filter */
	{node::LINK_NONE, 0, 0}, /* node_sampler */
	{node::LINK_NONE, 0, 0}, /* node_step_seq */
	{node::LINK_NONE, 0, 0}, /* node_audio_noise */
	{node::LINK_NONE, 0, 0}, /* node_control_noise */
	{node::LINK_AUDIO, node_filter::OUT_A_OUTPUT, node_echo::IN_A_INPUT}, /* node_echo */
	{node::LINK_AUDIO, node_filter::OUT_A_OUTPUT, node_delay::IN_A_INPUT} /* node_delay */
    },

    /* node_sampler */
    {
	{node::LINK_NONE, 0, 0}, /* node_output */
	{node::LINK_AUDIO, node_sampler::OUT_A_OUTPUT, PATCHER_ANY}, /* node_audio_mixer */
	{node::LINK_NONE, 0, 0}, /* node_control_mixer */
	{node::LINK_NONE, 0, 0}, /* node_audio_oscillator */
	{node::LINK_NONE, 0, 0}, /* node_lfo */
	{node::LINK_AUDIO, node_sampler::OUT_A_OUTPUT, node_filter::IN_A_INPUT}, /* node_filter */
	{node::LINK_NONE, 0, 0}, /* node_sampler */
	{node::LINK_NONE, 0, 0}, /* node_step_seq */
	{node::LINK_NONE, 0, 0}, /* node_audio_noise */
	{node::LINK_NONE, 0, 0}, /* node_control_noise */
	{node::LINK_AUDIO, node_sampler::OUT_A_OUTPUT, node_echo::IN_A_INPUT}, /* node_echo */
	{node::LINK_AUDIO, node_sampler::OUT_A_OUTPUT, node_delay::IN_A_INPUT}, /* node_delay */
    },

    /* node_step_seq */
    {
	{node::LINK_NONE, 0, 0}, /* node_output */
	{node::LINK_NONE, 0, 0}, /* node_audio_mixer */
	{node::LINK_NONE, 0, 0}, /* node_control_mixer */
	{node::LINK_CONTROL, node_step_seq::OUT_C_OUTPUT, node_oscillator::IN_C_TRIGGER}, /* node_audio_oscillator */
	{node::LINK_CONTROL, node_step_seq::OUT_C_OUTPUT, node_oscillator::IN_C_TRIGGER}, /* node_lfo */
	{node::LINK_NONE, 0, 0}, /* node_filter */
	{node::LINK_CONTROL, node_step_seq::OUT_C_OUTPUT, node_sampler::IN_C_TRIGGER}, /* node_sampler */
	{node::LINK_NONE, 0, 0}, /* node_step_seq */
	{node::LINK_CONTROL, node_step_seq::OUT_C_OUTPUT, node_noise::IN_C_TRIGGER}, /* node_audio_noise */
	{node::LINK_CONTROL, node_step_seq::OUT_C_OUTPUT, node_noise::IN_C_TRIGGER}, /* node_control_noise */
	{node::LINK_NONE, 0, 0}, /* node_echo */
	{node::LINK_NONE, 0, 0} /* node_delay */
    },

    /* node_audio_noise */
    {
	{node::LINK_NONE, 0, 0}, /* node_output */
	{node::LINK_AUDIO, node_audio_noise::OUT_A_OUTPUT, PATCHER_ANY}, /* node_audio_mixer */
	{node::LINK_NONE, 0, 0}, /* node_control_mixer */
	{node::LINK_NONE, 0, 0}, /* node_audio_oscillator */
	{node::LINK_NONE, 0, 0}, /* node_lfo */
	{node::LINK_AUDIO, node_audio_noise::OUT_A_OUTPUT, node_filter::IN_A_INPUT}, /* node_filter */
	{node::LINK_NONE, 0, 0}, /* node_sampler */
	{node::LINK_NONE, 0, 0}, /* node_step_seq */
	{node::LINK_NONE, 0, 0}, /* node_audio_noise */
	{node::LINK_NONE, 0, 0}, /* node_control_noise */
	{node::LINK_AUDIO, node_audio_noise::OUT_A_OUTPUT, node_echo::IN_A_INPUT}, /* node_echo */
	{node::LINK_AUDIO, node_audio_noise::OUT_A_OUTPUT, node_delay::IN_A_INPUT}, /* node_delay */
    },

    /* node_control_noise */
    {
	{node::LINK_NONE, 0, 0}, /* node_output */	
	{node::LINK_CONTROL, node_control_noise::OUT_C_OUTPUT, node_mixer::IN_C_AMPLITUDE}, /* node_audio_mixer */
	{node::LINK_CONTROL, node_control_noise::OUT_C_OUTPUT, PATCHER_ANY}, /* node_control_mixer */
	{node::LINK_CONTROL, node_control_noise::OUT_C_OUTPUT, node_oscillator::IN_C_FREQUENCY}, /* node_audio_oscillator */
	{node::LINK_CONTROL, node_control_noise::OUT_C_OUTPUT, node_oscillator::IN_C_FREQUENCY}, /* node_lfo */
	{node::LINK_CONTROL, node_control_noise::OUT_C_OUTPUT, node_filter::IN_C_CUTOFF},  /* node_filter */
	{node::LINK_CONTROL, node_control_noise::OUT_C_OUTPUT, node_sampler::IN_C_RATE}, /* node_sampler */
	{node::LINK_CONTROL, node_control_noise::OUT_C_OUTPUT, node_step_seq::IN_C_BPM}, /* node_step_seq */
	{node::LINK_CONTROL, node_control_noise::OUT_C_OUTPUT, node_audio_noise::IN_C_AMPLITUDE}, /* node_audio_noise */
	{node::LINK_CONTROL, node_control_noise::OUT_C_OUTPUT, node_control_noise::IN_C_AMPLITUDE}, /* node_control_noise */
	{node::LINK_CONTROL, node_control_noise::OUT_C_OUTPUT, node_echo::IN_C_FEEDBACK}, /* node_echo */
	{node::LINK_CONTROL, node_control_noise::OUT_C_OUTPUT, node_delay::IN_C_DELAY} /* node_delay */
    },

    /* node_echo */
    {
	{node::LINK_NONE, 0, 0}, /* node_output */
	{node::LINK_AUDIO, node_echo::OUT_A_OUTPUT, PATCHER_ANY}, /* node_audio_mixer */
	{node::LINK_NONE, 0, 0}, /* node_control_mixer */
	{node::LINK_NONE, 0, 0}, /* node_audio_oscillator */
	{node::LINK_NONE, 0, 0}, /* node_lfo */
	{node::LINK_AUDIO, node_echo::OUT_A_OUTPUT, node_filter::IN_A_INPUT}, /* node_filter */
	{node::LINK_NONE, 0, 0}, /* node_sampler */
	{node::LINK_NONE, 0, 0}, /* node_step_seq */
	{node::LINK_NONE, 0, 0}, /* node_audio_noise */
	{node::LINK_NONE, 0, 0}, /* node_control_noise */
	{node::LINK_AUDIO, node_echo::OUT_A_OUTPUT, node_echo::IN_A_INPUT}, /* node_echo */
	{node::LINK_AUDIO, node_echo::OUT_A_OUTPUT, node_delay::IN_A_INPUT}, /* node_delay */
    },

    /* node_delay */
    {
	{node::LINK_NONE, 0, 0}, /* node_output */
	{node::LINK_AUDIO, node_delay::OUT_A_OUTPUT, PATCHER_ANY}, /* node_audio_mixer */
	{node::LINK_NONE, 0, 0}, /* node_control_mixer */
	{node::LINK_NONE, 0, 0}, /* node_audio_oscillator */
	{node::LINK_NONE, 0, 0}, /* node_lfo */
	{node::LINK_AUDIO, node_delay::OUT_A_OUTPUT, node_filter::IN_A_INPUT}, /* node_filter */
	{node::LINK_NONE, 0, 0}, /* node_sampler */
	{node::LINK_NONE, 0, 0}, /* node_step_seq */
	{node::LINK_NONE, 0, 0}, /* node_audio_noise */
	{node::LINK_NONE, 0, 0}, /* node_control_noise */
	{node::LINK_AUDIO, node_delay::OUT_A_OUTPUT, node_echo::IN_A_INPUT}, /* node_echo */
	{node::LINK_AUDIO, node_delay::OUT_A_OUTPUT, node_delay::IN_A_INPUT}, /* node_delay */
    }
};

PatcherDynamic::PatcherDynamic() :
    m_changed(false)
{
}

PatcherDynamic::~PatcherDynamic()
{
    for (multiset<Link*, LinkPtrCmp>::iterator i = m_links.begin(); i != m_links.end(); ++i)
	delete *i;
}

bool PatcherDynamic::addNode(node* obj)
{
    if (m_nodes.insert(pair<int, Node>(obj->get_id(), Node(obj))).second) {
	map<int, Node>::iterator i;
	int this_type = obj->get_type ();
	int other_type;
	
	for (i = m_nodes.begin(); i != m_nodes.end(); ++i) {
	    if (i->first == obj->get_id ())
		continue;
	    
	    other_type = i->second.obj->get_type (); 

	    if (PATCHER_TABLE[this_type][other_type].socket_type != node::LINK_NONE) {
		m_links.insert(new Link(obj, (*i).second.obj,
					obj->sqr_distance_to(*i->second.obj),
					i->second.obj->sqr_distance_to_center (),
					PATCHER_TABLE[this_type][other_type].socket_type,
					PATCHER_TABLE[this_type][other_type].src_socket,
					PATCHER_TABLE[this_type][other_type].dest_socket));
	    }

	    if (PATCHER_TABLE[other_type][this_type].socket_type != node::LINK_NONE) {
		m_links.insert(new Link(i->second.obj, obj,
					i->second.obj->sqr_distance_to(*obj),
					obj->sqr_distance_to_center(),
					PATCHER_TABLE[other_type][this_type].socket_type,
					PATCHER_TABLE[other_type][this_type].src_socket,
					PATCHER_TABLE[other_type][this_type].dest_socket));
	    }
	}
	
	m_changed = true;
	return true;
    }
    
    return false;
}

bool PatcherDynamic::deleteNode(node* obj)
{
    if (m_nodes.erase(obj->get_id())) {
	multiset<Link*>::iterator i, r;
	
	for (i = m_links.begin(); i != m_links.end();) {
	    if ((*i)->src == obj || (*i)->dest == obj) {
		r = i++;
		undoLink(**r);
		delete *r;
		m_links.erase(r);
	    } else
		++i;
	}

	m_changed = true;
	return true;
    }
    
    return false;
}

void PatcherDynamic::setParamNode(node* obj, int id)
{
    if (id == node::PARAM_POSITION) {
	multiset<Link*>::iterator i, r;
	list<Link*> readd;
    
	for (i = m_links.begin(); i != m_links.end();) {
	    if ((*i)->src == obj || (*i)->dest == obj) {
		r = i++;

		(*r)->dist = (*r)->src->sqr_distance_to(*(*r)->dest);
		if ((*r)->dest == obj)
		    (*r)->dist_to_center = obj->sqr_distance_to_center();

		readd.push_back(*r);
		m_links.erase(r);
		m_changed = true;
	    }
	    else
		++i;
	}

	for (list<Link*>::iterator it = readd.begin(); it != readd.end(); ++it)
	    m_links.insert(*it);
    }
}

void PatcherDynamic::makeLink(Link& l)
{
    //cout << "making link, source: " << l.src->getID() << " dest: "<< l.dest->getID() << endl;

    node* old_src = l.dest->get_in_socket(l.sock_type, l.actual_in_sock).get_source_node();
    if (old_src != NULL) {
	notifyLinkDeleted(PatcherEvent(old_src, l.dest, l.out_sock, l.actual_in_sock, l.sock_type));
	//cout << "undoing link, source: " << old_src->getID() << " dest: "<< l.dest->getID() << endl;
    }
    
    l.dest->connect_in(l.sock_type, l.actual_in_sock, l.src, l.out_sock);
    notifyLinkAdded(PatcherEvent(l.src, l.dest, l.out_sock, l.actual_in_sock, l.sock_type));
}

void PatcherDynamic::undoLink(Link& l)
{
    if (l.actual_in_sock >= 0) {
	if (l.dest->get_in_socket(l.sock_type, l.actual_in_sock).get_source_node() == l.src) {
	    //cout << "undoing link, source: " << l.src->getID() << " dest: "<< l.dest->getID() << endl;
	    l.dest->connect_in(l.sock_type, l.actual_in_sock, NULL, l.out_sock);

	    notifyLinkDeleted(PatcherEvent(l.src, l.dest, l.out_sock, l.actual_in_sock, l.sock_type));
	}
    }
}

bool PatcherDynamic::isLinked(Link& l)
{
    if (l.actual_in_sock >= 0) {
	if (l.dest->get_in_socket(l.sock_type, l.actual_in_sock).get_source_node() == l.src)
	    return true;
    }
    return false;
}

void PatcherDynamic::findInSock(Link &l)
{
    float max_dist = l.dist;
    
    l.actual_in_sock = -1; 

    if (l.in_sock == -1) {
	for (int i = 0; i < l.dest->get_num_input (l.sock_type); i++) {
	    node* obj = l.dest->get_in_socket (l.sock_type, i).get_source_node (); 
	    float dist;
	    
	    if (obj == NULL) {
		l.actual_in_sock = i;
		return;
	    }
	    else if ((dist = obj->sqr_distance_to (*l.dest)) >= max_dist) {
		max_dist = dist;
		l.actual_in_sock = i;
	    }
	}
    } else {
	node* obj = l.dest->get_in_socket (l.sock_type, l.in_sock).get_source_node (); 
	if (obj == NULL || l.src->sqr_distance_to (*l.dest) < obj->sqr_distance_to (*l.dest))
	    l.actual_in_sock = l.in_sock;
    }
}

void PatcherDynamic::update()
{
    if (!m_changed)
	return;
    
    for (map<int, Node>::iterator i = m_nodes.begin(); i != m_nodes.end(); ++i)
	(*i).second.out_used = false;

    for (multiset<Link*>::iterator i = m_links.begin(); i != m_links.end(); ++i) {
	map<int, Node>::iterator n = m_nodes.find((*i)->src->get_id());
	map<int, Node>::iterator n2 = m_nodes.find((*i)->dest->get_id());
	if (n == m_nodes.end()) {
	    WARNING("Object with id " << (*i)->src->get_id () << "not found.");
	}
	
	Node& node_src = (*n).second;
	Node& node_dest = (*n2).second;
	Link& link = **i;

	//cout << "trying, src: " << link.src->getID() << " dest: " << link.dest->getID() << " out: " << node_src.out_used << endl; 

	if (!node_src.out_used &&
	    !(node_dest.out_used == true && node_dest.dest == link.src))
	{
	    if (!isLinked(link)) {
		findInSock(link);
		if (link.actual_in_sock >= 0) {
		    makeLink(link);
		    node_src.out_used = true;
		    node_src.dest = link.dest;
		}
	    } else {
		node_src.out_used = true;
	    }
	} else {
	    undoLink(link);
	    link.actual_in_sock = -1;
	}
    }
    
    //cout << "---" << endl;

    m_changed = false;
}

void PatcherDynamic::clear()
{
    for (multiset<Link*>::iterator i = m_links.begin(); i != m_links.end(); ++i)
	undoLink(**i);
}

} /* namespace psynth */
