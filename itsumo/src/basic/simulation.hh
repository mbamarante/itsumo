/*
    Copyright (C) 2005 by Ana Lucia Bazzan (bazzan@inf.ufrgs.br)
                          Bruno Castro da Silva (bcs@inf.ufrgs.br)
                          Diogo Becker de Brum (dbrum@inf.ufrgs.br)
            http://www.inf.ufrgs.br/~mas/traffic/siscot

    This file is part of Itsumo.

    Itsumo is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Itsumo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Itsumo; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/
#ifndef __HAS_SIMULATION_H
#define __HAS_SIMULATION_H

#define _NONE_ -100 //robert: signal plan changes

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "parse_network.hh"
#include "topology.hh"
#include "network.hh"
#include "../tiny/tinyxml.h"

//#include "../drivers/routerlibs/geradorOD.hh"
//#include "../drivers/routed.hh"

using namespace std;

//JB:: TODO the next defs shall be parameters??
#define STEPS_DETECT_SIMULATION_END 200
#define ACTIVE_CARS_PREBUFFER 1000000
extern active_cars_vector_t active_cars;
extern inactive_cars_queue_t inactive_cars;
extern topology_element_vector_t elements;
class DriverLoader;

class Simulation {
	private:
		tlight_agents_t tlight_agents;

		void load_xml(string);
		string topology_file;
		int numberOfSteps;
		unsigned int numberOfRounds;	// control number of rounds
		unsigned int numberOfEpisodes;	// control number of episodes
		float default_deceleration;
		int sensorInterval;
		int agentInterval;
		string the_sensor;
		vector< pair<string, pair<string,ofstream *> > > vet_sensors;	
		DriverLoader *driver_loader;
		vector<DriverLoader *> vet_drivers;

		void sensorsHeaders(int iterations, int currentEpisode, int numberOfEpisodes, int currentRound, int numberOfRounds);
		void sensorsPrint(int step);
		bool check(TiXmlElement *element_to_test, string element_name, int condition);
		void parse_sensors(TiXmlElement *sensors);
		void parse_drivers(TiXmlElement *);
		void parse_config(TiXmlElement *config);
		void parse_config_file(string file);
		void load_odmatrix(string file, int odsize, Network *n,
				string costs_view, bool replan, bool communication);
		void parse_od_drivers(TiXmlElement *vehicles, int odsize, Network *n,
				string costs_view, bool replan, bool communication);

		void closeSensors();

		node_vector_t stringToNode(string ids, int isdcop);
	public:
		ofstream driverLogger;

		Topology *t;
		Network *n;

		void addTravelTimes(int trvTime, int stepInserted, int stepRemoved, int TravelPredictedSteps, int replanCounter);
		void addReplanStats(int changed);

		float getDeceleration();
		Simulation(string file);
		~Simulation();
		void simulate(string round, string table_name);
		vector<DriverLoader *> * getDriverLoaderList();

		//JB:: implement
//		vector<TtravelTimes> travelTimes;
		//void reopenSensors(int episode);
		//void updateGraphWeights(Car *car);
};


#endif
