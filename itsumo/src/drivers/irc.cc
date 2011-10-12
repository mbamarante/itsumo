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
#include "irc.hh"
using namespace std;


IRC::IRC(string driver_class_name) 
	: Nasch(driver_class_name) 
{ 
		// set initial startTime
		startRouteTime = NO_START_TIME;

		// heuristc and learning
		heuristicProb = 0.5;
		
		// time init	
		//timeRoute0.clear();
		//timeRoute1.clear();
		history.clear();

		timesOnRoute0 = 0;
		timesOnRoute1 = 0;
		// set initial time
		//timeRoute0.push_back(10);
		//timeRoute1.push_back(10);
}

IRC::~IRC() 
{
}

void IRC::update(int step)
{ 
	currentStep = step; 
}

int IRC::where(int step)
{ 
	return sourceLaneSet; 
}

void IRC::set_options(vector < pair < string, string> > _options)
{
	for (vector < pair <string,string> >::iterator i = _options.begin(); i != _options.end();i++)
	{
		if ((*i).first == "source_laneset") 
			sourceLaneSet = atoi((*i).second.c_str());
		else
		if ((*i).first == "sink_laneset") 
			sinkLaneSet = atoi((*i).second.c_str());
		else
		if ((*i).first == "route0_begin") 
			route0Begin = atoi((*i).second.c_str());
		else
		if ((*i).first == "route1_begin") 
			route1Begin = atoi((*i).second.c_str());
		else
		if ((*i).first == "init_heuristic") 
			heuristicProb = atof((*i).second.c_str());
		else
		if ((*i).first == "learn_prob") 
			learningProb = atof((*i).second.c_str());
		else
		if ((*i).first == "hist_size") 
			historySize = atof((*i).second.c_str());
	}
}

decision_t IRC::takeDecision (Car &car) 
{
	decision_t decision;

	checkRouteTime(car);

	decision = Nasch::takeDecision(car);

	return decision;
}

lane_laneset_pair_t IRC::chooseNextLane_Laneset(Car *theCar) 
{
	lane_laneset_pair_t ans;

	topology_element_id_t nextDir = -1;

	// get next LaneSet according to heuristics
	nextDir = getNextDir(theCar);

	bool found = 0;
	double dice = randomGen.IRandom(0, 100);;

	// stupid initialization to avoid warnings
	LaneSet *destinationLaneset=NULL;

	// next node is the node in front of us, at the end of the current street
	LaneSet *currentLaneSet = theCar->getCurrentLane()->getParentLaneSet();

	turning_probabilities_vector_t tProbs = currentLaneSet->getTurningProbabilities ();
	turning_probabilities_vector_t::iterator itProbs;

	float curProb = 0;
  	for (itProbs = tProbs.begin(); itProbs != tProbs.end(); itProbs++) 
	{
    		if (itProbs->second == 0.0)
      			continue;
    		curProb += itProbs->second;

    		if (dice <= curProb) 
		{
      			destinationLaneset = itProbs->first;
      			found=1;
     	 		break;
    		}
  	}

  	// problema aqui
  	if (!found) 
	{
    		if (tProbs.size() == 0 )
    		{
			destinationLaneset=currentLaneSet;

    		}
   		else 
		{
      			destinationLaneset = tProbs[tProbs.size()-1].first;
    		}
  	}	

	if (nextDir != -1)
	{
		for (itProbs = tProbs.begin(); itProbs != tProbs.end(); itProbs++) 
		{
			if (itProbs->first->getId() == nextDir) 
			{	
				destinationLaneset = itProbs->first;
				break;
			}
		}
	}

	
	//choose laneset
	lane_vector_t possibleLanes = destinationLaneset->getChildLanes();
	// now we choose at random one target lane (a number between 0
	// and 'possibleLanes' - 1
	long int randomLane = randomGen.IRandom(0, possibleLanes.size()-1);

	ans = make_pair(possibleLanes[randomLane], destinationLaneset);

	return ans;
}

double IRC::calculateHeuristic(Car *theCar) 
{
	double heuristicLocal = heuristicProb; // returning heuristic.
	double meanTimeRoute0Local = 0.0; // mean time for route 0.
	double meanTimeRoute1Local = 0.0; // mean time for route 1.
	

	if ( (timesOnRoute0 > 0) && (timesOnRoute1 > 0) ) 
	{

	std::vector< std::pair<iteration_t, iteration_t> >::iterator routeTimeIterLocal;

	// calculate mean time for routes
	iteration_t route0TotalTime = 0;
	iteration_t route0Times = 0;

	iteration_t route1TotalTime = 0;
	iteration_t route1Times = 0;

	for (routeTimeIterLocal = history.begin(); routeTimeIterLocal != history.end(); routeTimeIterLocal++) 
	{
		if (routeTimeIterLocal->first == 0)
		{
			route0TotalTime += routeTimeIterLocal->second;
			route0Times += 1;
		}
		else
		{
			route1TotalTime += routeTimeIterLocal->second;
			route1Times += 1;
		}
	}
	meanTimeRoute0Local = static_cast<double>(route0TotalTime) / static_cast<double>(route0Times);

	meanTimeRoute1Local = static_cast<double>(route1TotalTime) / static_cast<double>(route1Times);

	// returning heuristic
	heuristicLocal = meanTimeRoute1Local / (meanTimeRoute0Local + meanTimeRoute1Local);

	//log - heuristica = ?

	}

	return heuristicLocal;
}

topology_element_id_t IRC::getNextDir(Car *theCar) 
{
	double heuristicLocal = heuristicProb; // initial heuristic
	double randomSelectionLocal = 0.0; // get value to test against heuristic
	double randomLearningLocal = 0.0; // it is time to learn?
	topology_element_id_t currentLaneSetLocal = -1;

	currentLaneSetLocal = theCar->getCurrentLane()->getParentLaneSet()->getId();
	// It is not time to use heuristic to choose

	if (currentLaneSetLocal != sourceLaneSet) 
	{
		return -1;
	}

	// get random value
	randomSelectionLocal = drand48();
	randomLearningLocal = drand48();
	// get information
	if (randomLearningLocal < learningProb) 
	{
		// get heuristic
		heuristicProb = calculateHeuristic(theCar);
	}

	// check if it will be route 0
	if (randomSelectionLocal <= heuristicLocal) 
	{
		isRoute0 = true;

		cout << "Driver: " << getId() << " - Route: 0 - Bias for route 0: " << heuristicLocal << " - " << timesOnRoute0 << " times on route 0 - " << timesOnRoute1 << " times on route 1" << endl;
		//log = choose route 0
		return route0Begin;
	}
	// or route 1
	isRoute0 = false;
	//log = choose route 1
	
	cout << "Driver: " << getId() << " - Route: 1 - Bias for route 0: " << heuristicLocal << " - " << timesOnRoute0 << " times on route 0 - " << timesOnRoute1 << " times on route 1" << endl;

	return route1Begin;
}


bool IRC::checkRouteTime(Car &car) 
{
	topology_element_id_t currentLaneSetLocal = -1;
	currentLaneSetLocal = car.getCurrentLane()->getParentLaneSet()->getId();

	bool resultLocal = false;

	// check parameters.
	resultLocal = checkRouteTimeBase(car);

	// vehicle leaved the route, stop counting and record the time.
	if ((resultLocal == true) && (currentLaneSetLocal == sinkLaneSet)) 
	{
		if (isRoute0 == true) 
		{
			if( (history.size() >= historySize) && (historySize != -1) )
				history.erase(history.begin());

			timesOnRoute0 += 1;

			history.push_back( make_pair(0, lastRouteTime) );
		} 
		else 
		{
			if( (history.size() >= historySize) && (historySize != -1) )
				history.erase(history.begin());

			timesOnRoute1 += 1;

			history.push_back( make_pair(1, lastRouteTime) );
		}
	}
	return resultLocal;
}


bool IRC::checkRouteTimeBase(Car &car) 
{
	topology_element_id_t currentLaneSetLocal = -1;
	currentLaneSetLocal = car.getCurrentLane()->getParentLaneSet()->getId();
	
	bool resultLocal = false;
	
	int r = 0;
	if (!isRoute0)
		r = 1;

	// if vehicle is in one of the routes, mark start time.
	if (((currentLaneSetLocal != sourceLaneSet) && (currentLaneSetLocal != sinkLaneSet)) && (startRouteTime == (unsigned int)NO_START_TIME)) 
	{
		cout << "Driver: " << getId() << " entered route " << r << " on step: " << currentStep << endl;
		startRouteTime = currentStep;
		resultLocal = true;
	}

	// vehicle leaved the route, stop counting and record the time.
	if ((startRouteTime != (unsigned int)NO_START_TIME) && (currentLaneSetLocal == sinkLaneSet)) 
	{
		cout << "Driver: " << getId() << " left route " << r << " on step: " << currentStep << endl;
		// calculate route time
		lastRouteTime = currentStep - startRouteTime;

		// reset timer
		startRouteTime = NO_START_TIME;
		resultLocal = true;
	}
	return resultLocal;
}



