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
#ifndef __HAS_NASCH_H
#define __HAS_NASCH_H

#include <iostream>
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>

#include "../basic/types.hh"
#include "../basic/driver.hh"
#include "../basic/car.hh"

class Nasch : public Driver {
	private:

	public:

		speed_t naschCalculateNewSpeed(speed_t, speed_t, speed_t, dist_t, float);

		bool isLaneChangeSecure(Car &myCar, Lane *_targetLane);

		lane_laneset_pair_t chooseNextLane_Laneset(Car *theCar);

		// the car is going to call this method in order to ask the driver for his decision
		decision_t takeDecision (Car &);

		// the car calls this method in order to ask the driver to initialize the
		// *extraData structure (see car.hh)
		void initializeExtraData(Car *theCar);
		void deleteExtraData(Car *theCar);

		bool readyIn(int);
		bool readyOut(int);
		int where(int);
		void set_options(vector < pair < string, string> > _options);

		// Nasch does not use these methods
		void update(int step){;}; 
		void logEvent(event_t, topology_element_id_t){};

		Nasch (string driver_class_name);
		~Nasch ();
};


#endif // __HAS_NASCH_H
