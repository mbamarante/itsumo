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
#ifndef __HAS_FC_H
#define __HAS_FC_H

#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>

#include "../basic/types.hh"
#include "../basic/driver.hh"
#include "../basic/car.hh"



using namespace std;

// the events constants of matsim
const int ARRIVAL_FLAG   = 0;
const int LEFT_FLAG      = 2;
const int ENTERED_FLAG   = 5;
const int DEPARTURE_FLAG = 6;

const string ARRIVAL_DESCRIPTION   = "arrival";
const string LEFT_DESCRIPTION      = "left link";
const string ENTERED_DESCRIPTION   = "entered link";
const string DEPARTURE_DESCRIPTION = "departure";



class FC: public Nasch {
	private:
		int insert_timestep_;
		int insert_laneset_;
		int destination_laneset_;

		bool departure;
		bool arrival;

	public:
		vector < Route > routes;

		lane_laneset_pair_t chooseNextLane_Laneset(Car *theCar);

		void set_routes(vector < Route > _rts,
				int source_laneset, 
				int destination_laneset);

		void set_options(vector < pair < string, string> > _options);

		bool readyIn(int step);
		bool readyOut(int step);
		int where(int step);
		void update(int step);


		void logEvent(int globalTime, 
			      string driverNick, 
			      topology_element_id_t linkId, 
			      const int, 
			      const string);
	
	
		int curStep;

		FC (string driver_class_name);
		~FC ();
};


#endif // __HAS_FC_H
