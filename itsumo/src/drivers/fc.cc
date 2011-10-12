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
#include "fc.hh"
using namespace std;



lane_laneset_pair_t FC::chooseNextLane_Laneset(Car *theCar) {

  double dice;
  int found=0;
  
  // stupid initialization to avoid warnings
  LaneSet *destinationLaneset=NULL;   

  // next node is the node in front of us, at the end of the current street
  LaneSet *currentLaneSet = theCar->getCurrentLane()->getParentLaneSet();

  // check if it is a departure
  if (currentLaneSet->getId() == insert_laneset_ && !departure && !arrival) {
		departure = true;

		logEvent(curStep, 
			 getNick(), 
			 currentLaneSet->getId(), 
			 DEPARTURE_FLAG,
			 DEPARTURE_DESCRIPTION);
  }

  dice = randomGen.IRandom(0, 100);

  turning_probabilities_vector_t tProbs = currentLaneSet->getTurningProbabilities ();
  turning_probabilities_vector_t::iterator itProbs;

	/*
	logger << "FC " << getNick() << " needs to choose next laneset now." << "\n";
	*/

	if (routes.size() > 0)
	{
		vector < Route >::iterator itRoute;
		vector < Route > tos;

		//seleciona rotas possiveis
		for (itProbs = tProbs.begin(); itProbs != tProbs.end(); itProbs++)
		{
			int id = itProbs->first->getId();
		for (itRoute = routes.begin(); itRoute != routes.end(); itRoute++)  
		{
			if (itRoute->laneset == id)
			{
				destinationLaneset = itProbs->first;
				found=1;
				break;
			}	
  		}
			if (found) break;
		}
	} 
	else if( (routes.size() <= 0) || ( (routes.size()>=0) && (found == 0)) )
	{
		for (itProbs = tProbs.begin(); itProbs != tProbs.end(); itProbs++) 
		{
			if (dice < (itProbs->second)) 
			{
				destinationLaneset = itProbs->first;
				found=1;
				break;
    			}
  		}
	}

	if (found && !arrival) {
		logEvent(curStep, 
			 getNick(), 
			 currentLaneSet->getId(), 
			 LEFT_FLAG,
			 LEFT_DESCRIPTION);

		logEvent(curStep, 
			 getNick(), 
			 destinationLaneset->getId(), 
			 ENTERED_FLAG,
			 ENTERED_DESCRIPTION);

		if (destinationLaneset->getId() == destination_laneset_ && departure) {
			arrival = true;
			logEvent(curStep, 
				 getNick(), 
			         destinationLaneset->getId(), 
				 ARRIVAL_FLAG,
				 ARRIVAL_DESCRIPTION);
		}

		/*
		logger << "FC ";
		logger << getId();
		logger << " is going to laneset "; 
		logger << destinationLaneset->getId();
		logger << " [";
		logger << destinationLaneset->getStartNode()->getName(); 
		logger << " -> ";
		logger << destinationLaneset->getEndNode()->getName(); 
		logger << "]";
		logger << "\n";
		*/
	}

  // problema aqui
  if (!found) {
    if (tProbs.size() == 0 )
    {
#ifdef DEBUG_NASCH
    	cerr << __FILE__ << ":" << __LINE__ << ": "
		<< "Current Laneset(" << currentLaneSet->getId() 
		<< ") doesn't have turning probabilities" << endl;
	exit(1);
#endif
	destinationLaneset=currentLaneSet;	

    }
    else destinationLaneset = tProbs[tProbs.size()-1].first;
  }

  lane_vector_t possibleLanes = destinationLaneset->getChildLanes();
  // now we choose at random one target lane (a number between 0
  // and 'possibleLanes' - 1
  long int randomLane = randomGen.IRandom(0, possibleLanes.size()-1);

#ifdef DEBUG_NASCH
  cout << "--> car " << theCar->getInternalId() << " ";
  cout << theCar->getCurrentLane()->getId() << "->";
  cout << (possibleLanes[randomLane])->getId();
  cout << " (" <<  randomLane << "/" << possibleLanes.size()-1;
  cout << ")" << " in laneset " << destinationLaneset->getId();
  cout << " (dice=" << dice << "%)" << endl;
#endif

  return make_pair(possibleLanes[randomLane], destinationLaneset);

  }


void FC::set_routes(vector < Route > _rts, int source_laneset, int destination_laneset)
{
	routes = _rts;
	insert_laneset_ = source_laneset;
	destination_laneset_ = destination_laneset;	
}


void FC::set_options(vector < pair < string, string> > _options)
{

	for (vector < pair <string,string> >::iterator i = _options.begin(); i != _options.end();i++)
	{
		if ((*i).first == "insert_timestep") 
		{
			insert_timestep_ = atoi((*i).second.c_str());
		}
	}
}

bool FC::readyIn(int step)
{ 
	if (step == insert_timestep_)
	{
		/*
		logger << "FC ";
		logger << getId();
		logger << " is going to be inserted, step = ";
		logger << step;
		logger << "\n";
		*/

		return true; 
	}
	else
		return false;
}

bool FC::readyOut(int step)
{ 
	//return arrival;
	return false;
}

int FC::where(int step)
{ 
	return insert_laneset_; 
}

void FC::update(int step)
{ 
	curStep = step; 
}

FC::FC(string driver_class_name) : Nasch(driver_class_name) 
{ 
	insert_timestep_ = 0;
//	logger.open("drivers.log", ofstream::out | ofstream::app);

	/*
	logger << "FC started with ID = ";
	logger << getId();
	logger << "\n";
	*/

	curStep = 0;
	departure = false;
	arrival = false;
}

FC::~FC()
{ 
//	logger.close(); 
}






void FC::logEvent(int globalTime,
		  string driverNick, 
		  topology_element_id_t linkId, 
		  const int eventFlag, 
	          const string eventDescription)
{
	// T_GBL - Global Simulation time in seconds from midnight 00:00:00
	*logger << globalTime << "\t";

	// DRIVER_ID
	*logger << driverNick << "\t";

	// LEG_ID - Number of trip (Act0-Leg0-Act1-Leg1-... Can be zero, I think)
	*logger << "0\t";

	// LINK_ID - ID of the link the event happened, aka link name from net file.
	*logger << linkId << "\t";

	// FROM_NODE_ID - Good question, never thought about that. Leave it zero.
	*logger << "0\t";

	// EVENT_FLAG
	*logger << eventFlag << "\t";

	// DESCRIPTION
	*logger << eventDescription << "\n";
}

	



