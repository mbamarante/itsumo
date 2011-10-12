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
#ifndef __HAS_IRC_H
#define __HAS_IRC_H

#include <iostream>
#include <algorithm>
#include <functional>

#include "../basic/types.hh"
#include "../basic/driver.hh"
#include "../basic/car.hh"

#define NO_START_TIME -1 // to mark that no time is under mesure.
#define IRC_DRIVER_HIST_SIZE -1 // no defined size, grow at will

class IRC: public Nasch {
	protected:
		iteration_t currentStep;

		double heuristic; ///< default choice.
		topology_element_id_t sourceLaneSet; ///< where route decision begins.
		topology_element_id_t sinkLaneSet; ///< where routes end.
		topology_element_id_t route0Begin; ///< route 0 laneset ID.
		topology_element_id_t route1Begin; ///< route 1 laneset ID.
		bool isRoute0; ///< registry if choosen route was route 0.

		double learningProb; ///< say when to review its knowledge.
		double heuristicProb; ///< to make learning rate possible.

		iteration_t startRouteTime; ///< start route time step.
		iteration_t lastRouteTime; ///< last route travel time.

		iteration_t timesOnRoute0; ///< times on route 0.
		iteration_t timesOnRoute1; ///< times on route 1.

		//std::list<iteration_t> timeRoute0; ///< time experienced on route 0.
		//std::list<iteration_t> timeRoute1; ///< time experienced on route 1.

		std::vector< std::pair<iteration_t, iteration_t> > history; ///< history of route times.

		double historySize; ///< set history size.

		//brief Calculate the heuristic according to the formula specified on class.
		double calculateHeuristic(Car *theCar);


		//brief Return the right route according the heuristic, if it is time to do that
		topology_element_id_t getNextDir(Car *theCar);

		//brief Get vehicles time, on route, and registry it.
		bool checkRouteTime(Car &car);

		//brief Get vehicles time, on route, and registry it.
		bool checkRouteTimeBase(Car &car);

	public:

		lane_laneset_pair_t chooseNextLane_Laneset(Car *theCar);

		decision_t takeDecision (Car &);

		void update(int step);

		int where(int step);

		void set_options(vector < pair < string, string> > _options);

		IRC (string driver_class_name);
		~IRC ();

};


#endif // __HAS_IRC_H
