/*
    Copyright (C) 2010 by Ana Lucia Bazzan (bazzan@inf.ufrgs.br)
                          Alexander J. Benavides (ajbenavides@inf.ufrgs.br)
                          Maicon de Brito do Amarante (mbamarante@inf.ufrgs.br)
            http://www.inf.ufrgs.br/maslab/traffic/itsumo/

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

#include "routed.hh"

// Static variables initialization:
// graph representation of itsumo's network
ItsumoGraph RoutedDriver::graph = ItsumoGraph();
// initial weight for any ARA* execution
double RoutedDriver::AStarInitialWeight = 2.5;


RoutedDriver::RoutedDriver(string driver_class_name) : Nasch(driver_class_name) 
{
	firstrun = true;

	tolerance = randomGen.IRandom(2,4); // 2 = 100% delayed; 3 = 200% delayed...
	replanCalls = 0;
	replanChanges = 0;
	lastLanesetReplan = 0;

	active = true;	// assume the driver is added to network immediately
	use_local_info = false;	// don't use local info as default

	// if ARA* is selected, the first run will execute with static initial weight
	// the decrement of weight is done before computePath.
	AStarWeight = AStarInitialWeight + 0.5;

	currentLink = currentChoice = NULL;
	ready2go = endOfLane = false;
	stepInserted = stepRemoved = 0;
	theCar = NULL;
	goWait = false;
	iAmBack = 0;
	numberOfTrips = 0;
}

void RoutedDriver::set_source_destination( Node * source, Node * dest )
{
	nSrce = graph.getgNode4mapIt(source);
	nDest = graph.getgNode4mapIt(dest);

	/* to restore when replan
	 * and use many episodes */
	nSrceOriginal = graph.getgNode4mapIt(source);
	nDestOriginal = graph.getgNode4mapIt(dest);
}

void RoutedDriver::setNetwork(Network * n)
{
	graph.Network2Graph(n);
//	graph.gnuplotPrintGraph();
}

void RoutedDriver::useLocalInfo(bool use_li)
{
	use_local_info = use_li;
	if (use_li)
	{
		observed_info.clear();
		loadLocalInfo();
	}
}

//This function calls the right algorithm.
//This should be a function pointer.
//This should be in the config.xml file.
void RoutedDriver::FindShortestPath()
{
//	spDijkstra();
	spAStar();
//	spARAStar(); AStarWeight = 1.1; spARAStar(); // should run until AStarWeight == 1.0;
//	spLPAStar();
//	spADStar(); AStarWeight = 1.1; spADStar(); // should run until AStarWeight == 1.0;
//	cout << "driver - replaner: " <<  replanner << " view: " << viewCost << " comm: " << communicate << endl;
	// avoid deadlock!
}

void RoutedDriver::setPathSize()
{
	pathSize = linksPath.size();
}

/************************************************************
	Function: void RoutedDriver::spBuildRoute()
	Author: Alexander Benavides
	Description: recalls the shortest path found into linksPath.
	Changelog:
		20101212 <maicon.ba> : replanning variables added.
		<xandr> : distance sum optimized, COUTs deleted.
************************************************************/

void RoutedDriver::spBuildRoute()
{
	unsigned long int current_id;
	double distance = 0;
	//////////  BUILD THE ROUTE  //////////
	linksPath.clear();
	current_id = nSrce->id;
	if (current_id != nDest->id)
	{
		linksPath.push_back(parent_lk[current_id]);
		current_id = parent_lk[current_id]->gNodeZ->id;
		while (current_id != nDest->id)
		{
			if ( parent_lk[current_id] == NULL || linksPath.size() > treated.size() )
			{
				linksPath.clear();
				return;
			}
			distance += parent_lk[current_id]->base_cost;
			// last link isnt used
			linksPath.push_back(parent_lk[current_id]);
			current_id = parent_lk[current_id]->gNodeZ->id;
		}
	}

	routeSize = linksPath.size(); // MB: REPLANNING _ used to know how finished the travel is
	routeLength = distance; // MB: REPLLANING _ used to kow many steps the driver needs to complete his travel at free flow traffic
	routeStep = curStep; // MB: REPLLANING _ used to kow route age
} // spBuildRoute

int RoutedDriver::getlinksPathSize(){return linksPath.size();}

void RoutedDriver::spDijkstra()
{
	////////////  DECLARATIONS  ////////////
	ItsumoGraph::gNode * current;
	unsigned long int current_id;
	unsigned long int neighbour_id;
	double newg;

	///////////  INITIALIZATIONS  //////////
	g.resize(graph.nodes.size());
	parent_lk.resize(graph.nodes.size(),NULL);
	treated.clear(); treated.resize(graph.nodes.size(), 0);
	OpenHeap.clear();

	_less_f IsLess (*this);

	// evaluate the source and push it in the heap.
	g[nDest->id] = 0;
	parent_lk[nDest->id] = NULL;

	OpenHeap.push_back(nDest->id);
	push_heap(OpenHeap.begin(), OpenHeap.end(), IsLess); // unneeded with one element
	treated[OpenHeap.front()]=1; // Open

	//////////  MAIN LOOP SEARCH  //////////
	while(OpenHeap.front() != nSrce->id)
	{
		// Gets the minimum-cost link from OpenHeap, removes it from the list and closes it
		current = &(graph.nodes [ current_id = OpenHeap.front() ]);
		pop_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
		OpenHeap.pop_back();
		treated [ current_id ] = 2; // Closed


		//////// For Each successor ////////
		for ( vector<ItsumoGraph::gLink*>::iterator it = current->gLinksIn.begin(); it != current->gLinksIn.end(); ++it)
		{
			neighbour_id = (*it)->gNodeA->id;

			// If the neighbour is already closed, doesn't use it
			if ( treated[neighbour_id] == 2) continue; /* else */

			// Calculates the cost to neighbour through current
			newg = g[current_id] + calculateLinkCost(*it);

			if ( treated[neighbour_id] == 1)
			{
				if( newg < g[neighbour_id] )
				{
					// Changes the cost of neighbour and restores the heap structure
					g[neighbour_id] = newg;
					parent_lk[neighbour_id] = (*it);
					make_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
				}
			}
			else /* (treated[neighbour_id] == 0 ) */
			{
				// Opens the neighbour, records its cost and pushes to the OpenHeap
				g[neighbour_id] = newg;
				parent_lk[neighbour_id] = (*it);
				OpenHeap.push_back(neighbour_id);
				push_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
				treated[neighbour_id] = 1;
			}
		}
		// necessary error treatment for unconnected maps
		if(OpenHeap.empty())
		{
			linksPath.clear();
			return;
		}
	}

	spBuildRoute();
} // spDijkstra();

void RoutedDriver::spAStar()
{
	spARAStarInit();
	GeneralizedAStarComputePath(1.0);
} // spAStar();

void RoutedDriver::spARAStar()
{
	// if ARA* is selected, the first run will execute with static initial weight
	// the decrement of weight is done before computePath.
	// if weight is more than 1.0, then it can be reduced

	if (AStarWeight <= 1.0) return; // already optimal path.

	if (AStarWeight > AStarInitialWeight)
		spARAStarInit();
	else
		spARAStarStep();
	GeneralizedAStarComputePath(AStarWeight);
} // spARAStar();

void RoutedDriver::spLPAStar()
{
	if (firstrun)
		GeneralizedInit();
	AStarWeight = 1.0;

	GeneralizedStep();
	GeneralizedComputePath(AStarWeight);
} // spLPAStar();

void RoutedDriver::spADStar()
{
	if (firstrun)
		GeneralizedInit();

	GeneralizedStep();
	GeneralizedComputePath(AStarWeight);
} // spADStar();

void RoutedDriver::spARAStarInit()
{
	///////////  INITIALIZATIONS  //////////
	g.resize(graph.nodes.size());
	h.resize(graph.nodes.size());
	parent_lk.resize(graph.nodes.size(),NULL);
	treated.clear(); treated.resize(graph.nodes.size(), 0);
	OpenHeap.clear();
	InconsSet.clear();
	AStarWeight = AStarInitialWeight;

	// evaluate the source and push it in the heap.
	g[nDest->id] = 0;
	h[nDest->id] = nDest->EuclidDist(nSrce);
	parent_lk[nDest->id] = NULL;
	g[nSrce->id] = numeric_limits<double>::infinity();
	h[nSrce->id] = 0;

	OpenHeap.push_back(nDest->id);
	treated[OpenHeap.front()] = 1; // Open
} // spARAStarInit();

void RoutedDriver::spARAStarStep()
{
	AStarWeight -= 0.5; //JB::TODO decrement delta should be a parameter or automatic?
	if (AStarWeight < 1.0) AStarWeight = 1.0;

	// move Inconsistent stages to Open, and clear Closed.
	for(set<unsigned long int>::iterator it = InconsSet.begin(); it != InconsSet.end(); ++it)
	{
		treated [*it] = 1; // Open
		OpenHeap.push_back(*it);
	}
	for(vector<unsigned char>::iterator it = treated.begin(); it != treated.end(); ++it)
	{
		if ( treated [*it] == 2)
			treated [*it] = 0; // non treated
	}
	InconsSet.clear();
	_less_gwh IsLess (*this, AStarWeight);
	make_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
} // spARAStarStep();

void RoutedDriver::GeneralizedAStarComputePath(double weight /* = 1.0 */)
{
	////////////  DECLARATIONS  ////////////
	ItsumoGraph::gNode * current;
	unsigned long int current_id;
	unsigned long int neighbour_id;
	double newg;
	_less_gwh IsLess (*this, weight);


	//////////  MAIN LOOP SEARCH  //////////
//cerr << nSrce->id <<"\t" << OpenHeap.front() << "\t"<< g[ nSrce->id] <<"\t" << g[OpenHeap.front()] << "\t"<< h[ nSrce->id] <<"\t" << h[OpenHeap.front()] << "\t"<< IsLess(nSrce->id, OpenHeap.front()<< endl;
	while(IsLess(nSrce->id, OpenHeap.front()))
	{
		// Gets the min-cost node, then closes and removes it from the OpenHeap
		current = &(graph.nodes [ current_id = OpenHeap.front() ]);
		pop_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
		OpenHeap.pop_back();
		treated [ current_id ] = 2; // Closed

		//////// For Each successor ////////
		for ( vector<ItsumoGraph::gLink*>::iterator it = current->gLinksIn.begin(); it != current->gLinksIn.end(); ++it)
		{
			// Calculates the cost to reach neighbour through current
			neighbour_id = (*it)->gNodeA->id;
			newg = g[current_id] + calculateLinkCost(*it);

			if ( treated[neighbour_id] == 2)
			{
				// Since the neighbour is closed, inserts it into the Inconsistent Set if needed
				if( newg < g[neighbour_id] )
				{
					g[neighbour_id] = newg;
					parent_lk[neighbour_id] = (*it);
					InconsSet.insert(neighbour_id);
				}
			   	continue;
			} /* else */

			if ( treated[neighbour_id] == 1)
			{
				// Changes the g of open neighbours when required
				if( newg < g[neighbour_id] )
				{
					g[neighbour_id] = newg;
					parent_lk[neighbour_id] = (*it);
					make_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
				}
			}
			else /* (treated[neighbour_id] == 0 ) */
			{
				// Opens untreated neighbours, adds them to OpenHeap and assigns their g
				g[neighbour_id] = newg;
				h[neighbour_id] = graph.nodes[neighbour_id].EuclidDist(nSrce);
				parent_lk[neighbour_id] = (*it);
				OpenHeap.push_back(neighbour_id);
				push_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
				treated[neighbour_id] = 1;
			}
		}
		// necessary error treatment for unconnected maps
		if(OpenHeap.empty())
		{
			linksPath.clear();
			return;
		}
	}
	spBuildRoute();
} // GeneralizedAStarComputePath

void RoutedDriver::GeneralizedInit()
{
	firstrun = false;
	///////////  INITIALIZATIONS  //////////
	g.clear(); g.resize(graph.nodes.size(), numeric_limits<double>::infinity());
	v.clear(); v.resize(graph.nodes.size(), numeric_limits<double>::infinity());
	h.clear(); h.resize(graph.nodes.size(), numeric_limits<double>::infinity());
	parent_lk.clear(); parent_lk.resize(graph.nodes.size(),NULL);
	treated.clear(); treated.resize(graph.nodes.size(), NONE);
	OpenHeap.clear();
	InconsSet.clear();
	graph.ModifiedLinks.clear();
	AStarWeight = AStarInitialWeight;

	// evaluate the source and push it in the heap.
	g[nDest->id] = 0;
	h[nDest->id] = nDest->EuclidDist(nSrce);
	parent_lk[nSrce->id] = parent_lk[nDest->id] = NULL;
	v[nSrce->id] = g[nSrce->id] = numeric_limits<double>::infinity();
	h[nSrce->id] = 0;

	OpenHeap.push_back(nDest->id);
	treated[OpenHeap.front()] = OPEN;
} // GeneralizedInit();

void RoutedDriver::GeneralizedStep()
{
	// the graph.ModifiedLinks must contain the gNodeZ nodes of the links with new costs!!!
	ItsumoGraph::gLink* newparent;
	double newg;

	AStarWeight -= 0.5; //JB::TODO decrement delta should be a parameter or automatic?
	if (AStarWeight < 1.0) AStarWeight = 1.0;
	_less_lpa IsLess (*this, AStarWeight);

	// add new Inconsistent stages (modified by links data updates).
	for(set<unsigned long int>::iterator it = graph.ModifiedLinks.begin(); it != graph.ModifiedLinks.end(); ++it)
	{
		unsigned long int nid = graph.links[(*it)].gNodeA->id;
		newg = numeric_limits<double>::infinity();
		newparent = NULL;
		for ( vector<ItsumoGraph::gLink*>::iterator ik = graph.nodes[(nid)].gLinksOut.begin(); ik != graph.nodes[(nid)].gLinksOut.end(); ++ik)
		{
			if ( newg > v[(*ik)->gNodeZ->id] + calculateLinkCost(*ik) )
			{
				newg = v[(*ik)->gNodeZ->id] + calculateLinkCost(*ik);
				newparent = *ik;
			}
		}
		g[(nid)] = newg;
		parent_lk[(nid)] = newparent;
		UpdateSetMembership((nid), IsLess);
	}
	graph.ModifiedLinks.clear();

	// move Inconsistent stages to Open, and clear Closed.
	for(vector<unsigned char>::iterator it = treated.begin(); it != treated.end(); ++it)
	{
		if ( treated [*it] == CLOSED)
			treated [*it] = NONE;
		else if ( treated [*it] == INCONS)
		{
			treated [*it] = OPEN;
			OpenHeap.push_back(*it);
		}
	}

	// Update the heuristic values to the new source
	h[nDest->id] = graph.nodes[nDest->id].EuclidDist(nSrce);
	for(vector<unsigned long int>::iterator it = OpenHeap.begin(); it != OpenHeap.end(); ++it)
		h[(*it)] = graph.nodes[(*it)].EuclidDist(nSrce);

	// Update the priorities for all OPEN states
	make_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
} // GeneralizedStep();

void RoutedDriver::GeneralizedComputePath(double weight /* = 1.0 */)
{
	////////////  DECLARATIONS  ////////////
	ItsumoGraph::gNode * current;
	unsigned long int current_id;
	unsigned long int neighbour_id;
	ItsumoGraph::gLink* newparent;
	double newg;
	_less_lpa IsLess (*this, weight);

	//////////  MAIN LOOP SEARCH  //////////
//cerr << nSrce->id <<"\t" << OpenHeap.front() << "\t"<< g[ nSrce->id] <<"\t" << g[OpenHeap.front()] << "\t"<< h[ nSrce->id] <<"\t" << h[OpenHeap.front()] << "\t"<< IsLess(nSrce->id, OpenHeap.front()<< endl;
	while(IsLess(nSrce->id, OpenHeap.front()) || v[nSrce->id] < g[nSrce->id])
	{
		// Gets the min-cost node, then closes and removes it from the OpenHeap
		current = &(graph.nodes [ current_id = OpenHeap.front() ]);
		pop_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
		OpenHeap.pop_back();

		if ( v[current_id] > g[current_id])
		{
			v[current_id] = g[current_id];
			treated [ current_id ] = CLOSED;

			//////// For Each successor ////////
			for ( vector<ItsumoGraph::gLink*>::iterator it = current->gLinksIn.begin(); it != current->gLinksIn.end(); ++it)
			{
				// Calculates the cost to reach neighbour through current path
				neighbour_id = (*it)->gNodeA->id;
				newg = g[current_id] + calculateLinkCost(*it);
				if ( newg < g[neighbour_id])
				{
					g[neighbour_id] = newg;
					parent_lk[neighbour_id] = (*it);
					UpdateSetMembership(neighbour_id, IsLess);
				}
			}
		}
		else
		{
			// Propagate underconsistency
			v[current_id] = numeric_limits<double>::infinity();
			UpdateSetMembership(current_id, IsLess);

			//////// For Each successor ////////
			for ( vector<ItsumoGraph::gLink*>::iterator it = current->gLinksIn.begin(); it != current->gLinksIn.end(); ++it)
			{
				neighbour_id = (*it)->gNodeA->id;
				if ( parent_lk[neighbour_id] == (*it) )
				{
					newg = numeric_limits<double>::infinity();
					newparent = NULL;
					for ( vector<ItsumoGraph::gLink*>::iterator ik = graph.nodes[neighbour_id].gLinksOut.begin(); ik != graph.nodes[neighbour_id].gLinksOut.end(); ++ik)
					{
						if ( newg > v[(*ik)->gNodeZ->id] + calculateLinkCost(*ik) )
						{
							newg = v[(*ik)->gNodeZ->id] + calculateLinkCost(*ik);
							newparent = *ik;
						}
					}
					g[neighbour_id] = newg;
					parent_lk[neighbour_id] = newparent;
					UpdateSetMembership(neighbour_id, IsLess);
				}
			}
		}
		// necessary error treatment for unconnected maps
		if(OpenHeap.empty())
		{
	GeneralizedInit();
	GeneralizedStep();
		}
	}
	spBuildRoute();
	if (linksPath.size() == 0 && nSrce->id != nDest->id)
	{
		firstrun = true;
		FindShortestPath();
	}
} // GeneralizedComputePath

void RoutedDriver::UpdateSetMembership(const unsigned long int & s, _less_lpa & IsLess)
{
	if ( v[s] != g[s] ) // Inconsistent
	{
		// if not treated or OPEN, insert or update into OPEN
		if (treated[s] == NONE )
		{
			h[s] = graph.nodes[s].EuclidDist(nSrce);
			OpenHeap.push_back(s);
			push_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
			treated[s] = OPEN;
		}
		else if (treated[s] == OPEN )
		{
			make_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
		}
		else // if CLOSED or INCONS, insert into INCONS
		{
			treated[s] = INCONS;
		}
	}
	else // Consistent
	{
		// if OPEN, remove from OPEN
		if (treated[s] == OPEN )
		{
			for (vector<unsigned long int>:: iterator it = OpenHeap.begin(); it != OpenHeap.end(); ++it)
				if (s == *it)
				{
					*it = OpenHeap.back();
					OpenHeap.pop_back();
					make_heap(OpenHeap.begin(), OpenHeap.end(), IsLess);
					break;
				}
			treated[s] = NONE;
		}
		// if INCONS, remove from INCONS
		else if (treated[s] == INCONS )
		{
			treated[s] = CLOSED;
		}
	}
}

int RoutedDriver::calculateLinkCost(ItsumoGraph::gLink *lnk)
{
	double Ncels, Ncars, XVmax, cost;
	OccupationMap::iterator it;
	OccupationPair *link_info;
	
	if(use_local_info) // If should use local information
	{
		// Searches for observed density for the link
		it = observed_info.find(lnk->itsumoLaneSet->getId());

		if(it != observed_info.end()) // If there's some observed density for the link
		{
			link_info = &it->second;
			Ncars = link_info->first / link_info->second;
			Ncels = lnk->itsumoLaneSet->getCapacity();
			XVmax = lnk->itsumoLaneSet->getMaxVMax();

			// If the link should be penalized, calculates its cost
			if ( Ncels - Ncars < XVmax * Ncars)
			{
				cost = lnk->base_cost * (XVmax * Ncars);
				if(Ncels != Ncars) cost /= (Ncels - Ncars); // avoids div by zero
				return cost;
			}
		}

		// If there's no observed density for the link or it shouldn't be 
		// penalized, returns its base cost
		return lnk->base_cost;
	}

	else	// If should only use global information
	{
		return lnk->current_cost;
	}

}

void RoutedDriver::saveLocalInfo()
{
	char filename[256] = "../../output/local_info/vehicle";
	ofstream file;

	// Creates the filename with the nick (id of current vehicle) and opens the file
	strcat(filename, nick.c_str());
	strcat(filename, ".txt");
	file.open(filename);

	// Writes a line for each link observed
	if(file.is_open())
	{
		for(OccupationMap::iterator it = observed_info.begin(); it != observed_info.end(); it++)
		{
			file << it->first << '\t' << it->second.first << '\t' << it->second.second << endl;
		}

		file.close();
	}
	else
	{
		cerr << "Error opening file '" << filename << "' to save local information." << endl;
	}
}

void RoutedDriver::loadLocalInfo()
{
	string tmp;
	char filename[256] = "../../output/local_info/vehicle";
	ifstream file;
	OccupationPair *link_info;
	int linkId, sum, count;

	// Creates the filename with the nick (id of current vehicle) and opens the file
	strcat(filename, nick.c_str());
	strcat(filename, ".txt");
	file.open(filename);
	
	// Reads a line for each link observed, or ignores it if file isn't found
	if (file.is_open())
	{
		while(file.good())
		{
			file >> linkId >> sum >> count;

			link_info = &observed_info[linkId];
			link_info->first = sum;
			link_info->second = count;
		}
	}

}

bool RoutedDriver::readyIn(int step)
{
	return true;
}


bool RoutedDriver::readyOut(int step)
{
	//JB:: return true when "close" to destination.
	//ready2go: kills the car in the last lane
	//endOfLane kills it in the end of the lane.
	return ready2go;// && endOfLane;
}


int RoutedDriver::where(int step)
{
	if (currentLink == NULL)
	{
		currentLink = currentChoice = linksPath.front();
		linksPath.pop_front();
	}
	return currentLink->itsumoLaneSet->getId();
}


void RoutedDriver::update(int step)
{
	OccupationPair *link_info;
	LaneSet *cur_laneset;

	// If required, updates local info with current step
	if(use_local_info && active) 
	{

		// Tries to find the associated car, if none was found yet
		if (theCar == NULL) 
		{
			for (vector<Car*>::iterator car_it=active_cars.begin(); car_it != active_cars.end(); car_it++)
			{
				if ((*car_it)->getDriver() == this)
				{
					theCar = (*car_it);
					break;
				}
			}
		}


		// If a car was found, gets the current laneset's state and saves it
		if (theCar)
		{
			cur_laneset = theCar->getCurrentLane()->getParentLaneSet();
			link_info = &observed_info[cur_laneset->getId()];
			link_info->first += cur_laneset->getNumberOfVehicles();
			link_info->second++;
		}
	}

	// Updates the current step
	curStep = step;

	if(ready2go)
	{

		// Tries to find the associated car, if none was found yet 
		if (theCar == NULL)
		{
			for (vector<Car*>::iterator car_it=active_cars.begin(); car_it != active_cars.end(); car_it++)
			{
				if ((*car_it)->getDriver() == this)
				{
					theCar = (*car_it);
					break;
				}
			}
		}
		
		// If a car was found and the end of lane was/is being reached, records it
		if (theCar)
		{
			if (theCar->getCurrentSpeed() + theCar->getCurrentCell() >= theCar->getCurrentLane()->getLength())
			{
				endOfLane = true;
			}
		}
	}

#ifdef DEBUGGING_ROUTED_DRIVER
	for (vector<Car*>::iterator car_it=active_cars.begin(); car_it != active_cars.end(); car_it++)
	{
	//	cout << (*car_it)->getDriver() << " " << this  << endl;
		if ((*car_it)->getDriver() == this)
		{
			theCar = (*car_it);
			break;
		}
	}
	if (theCar){
		//called first for active and waiting drivers.
		// must set states for readyIn and readyOut 

		cout << theCar->getCurrentLane()->getParentLaneSet()->getStartNode()->getName() 
			<< theCar->getCurrentLane()->getParentLaneSet()->getEndNode()->getName();
		//	cout
		//		<< "\t" << (static_cast<dist_t>(theCar->getCurrentSpeed()))  >=  (theCar->getCurrentLane()->getLength() - theCar->getCurrentCell());

		cout << "update    ***"
			<< "\t" << (int)( theCar->getCurrentSpeed())
			<< "\t" << theCar->getCurrentLane()->getLength()
			<< "\t" << theCar->getCurrentCell() << "\n";
	}
	else 
		cout << "update  *** no active car" << endl ;
#endif
}

void RoutedDriver::removedFromNet()
{
	// saves any accumulated local info
	if (use_local_info)
		saveLocalInfo();

	theCar = NULL;
	active = false;
}


lane_laneset_pair_t RoutedDriver::chooseNextLane_Laneset(Car *theCar)
{
#ifdef DEBUGGING_ROUTED_DRIVER
	cout << "chooseNLS  "
	   	<< theCar->getCurrentLane()->getParentLaneSet()->getStartNode()->getName() 
		<< theCar->getCurrentLane()->getParentLaneSet()->getEndNode()->getName() << "\t";
		cout
			<< "\t" << (int)( theCar->getCurrentSpeed())
			<< "\t" << theCar->getCurrentLane()->getLength()
			<< "\t" << theCar->getCurrentCell() ;
    cout << " \t";
	cout << linksPath.size() << " "; 
#endif
	LaneSet *currentLaneSet = theCar->getCurrentLane()->getParentLaneSet();
	LaneSet *destinationLaneset = NULL;

	if (currentLaneSet == currentChoice->itsumoLaneSet)
		currentLink = currentChoice;

	if (currentLaneSet != currentLink->itsumoLaneSet)
	{
		cerr  
	   	<< nick << "\t"
		<< currentLink->itsumoLaneSet->getStartNode()->getName() 
		<< currentLink->itsumoLaneSet->getEndNode()->getName() << "\t"
	   	<< currentLaneSet->getStartNode()->getName() 
		<< currentLaneSet->getEndNode()->getName() << "\t"
			<< nSrce->itsumoNode->getName()
			<< nDest->itsumoNode->getName() << "\n";

	   	cerr<< "Warning, car out of route!!!! must reroute?\n";
	}

	/*
	 * update local driver knowledge about link cost
	 */
	if ( viewCost.compare("LOCAL") == 0 ) {	// view cost local
		graph.updateLocalCost(currentLink, this->getId()); // update local cost (own perception)
	}

	/*
	 * communication
	 */
	if (communicate && linksPath.size() % 3 == 0){
		graph.resetCurrentCosts();
		graph.updateGraphDensityWithLocalCosts(this->getId(), currentLaneSet, 1); // use local costs
	}

	// == update travelled distance ==
	// euclidian distance
	Node *n1, *n2;
	currentLaneSet -> getParentSection() -> getDelimitingNodes (&n1, &n2);

	double dist_x = ((*n1).getXcoord() - (*n2).getXcoord());
	double dist_y = ((*n1).getYcoord() - (*n2).getYcoord());

	distanceTravelled += sqrt(pow(dist_x, 2) + pow(dist_y, 2));	// length sections
	// ^^ update travelled distance ^^

	if (replanner && activateReplanStrategy(currentLaneSet) && lastLanesetReplan != currentLaneSet->getId())
	{
//		nSrce = currentLink->gNodeZ;
		lastLanesetReplan = currentLaneSet->getId();
		ItsumoGraph::gRoute r1 = linksPath; // MB: route before replan
		//FindShortestPath();
		updateCostsFindShortest();

		if (linksPath.size() > 0) {
			if (currentLaneSet->getParentSection()->getId() == linksPath.front()->itsumoLaneSet->getParentSection()->getId()) {
				linksPath = r1;
			} else {
				replanChanges += routeDiff(r1, linksPath); // MB: replans stats
			}
		}

	}

	if (currentLaneSet == currentChoice->itsumoLaneSet)
	{
		if ( linksPath.size() /* > 0 */ )
		{
			// Take the next element of the path.
			currentChoice = linksPath.front();
			linksPath.pop_front();
		}
		else
		{
			// mark the car to be deleted
			ready2go = true;
		}
	}
	destinationLaneset = currentChoice->itsumoLaneSet;

	lane_vector_t possibleLanes = destinationLaneset->getChildLanes();
	long int randomLane = randomGen.IRandom(0, possibleLanes.size()-1);

#ifdef DEBUGGING_ROUTED_DRIVER
	cout
	   	<< destinationLaneset->getStartNode()->getName() 
		<< destinationLaneset->getEndNode()->getName() << "\t";
	cout << "Routed" << endl;
#endif
	return make_pair(possibleLanes[randomLane], destinationLaneset);
}

decision_t RoutedDriver::takeDecision (Car &car) {
#ifdef DEBUGGING_ROUTED_DRIVER
	Car * theCar = &car;
	cout << "takeDecision    *"
		<< "\t" << (int)( theCar->getCurrentSpeed())
		<< "\t" << theCar->getCurrentLane()->getLength()
		<< "\t" << theCar->getCurrentCell() << "\n";
#endif
	return Nasch::takeDecision (car);
}  

bool RoutedDriver::activateReplanStrategy(LaneSet *currentLaneSet){

	//replan at every intersection
//	replanCalls++;
//	return true;

	return replanDelayStrategy(currentLaneSet);
}

/**
EnRoute Replanning

@param *theCar the car to be used.
@param *CurrentLaneSet the current laneset where car is located.
@return true = replan!
*/
bool RoutedDriver::replanDelayStrategy(LaneSet *currentLaneSet) {

	// simulation info
	int maxSpeed = currentLaneSet->getParentSection()->getParentNetwork()->getParentTopology()->getCarsMaximumSpeed();
	int cellSize = currentLaneSet->getParentSection()->getParentNetwork()->getParentTopology()->getCellSize();

	// PARAMS
	int delayFactor = tolerance; // PARAM 1: delay factor (x times delayed)
	double minCompleted = 0.3; // PARAM 2: how much the travel was completed before replan
	double maxCompleted = 0.7; // PARAM 3: how much the tavel cant be completed before replan

	// how completed the travel was?
	// 0.1 means 10% completed; 0.9 means 90% completed
	double completed = 1 - ((double) linksPath.size() / (double) routeSize);

	// forecast steps needed to complete the travel
	double predictSteps = routeLength / cellSize / maxSpeed;

	// DELAY STRATEGY (travelled steps > forecast steps at a certain travel point)
	if (curStep - routeStep > // current step - start travelling step = travelled steps
			predictSteps * delayFactor // forecasted steps (at start) * delay factor = when elapsed time is X times grather than forecasted time
			&& completed >= minCompleted // how completed the travel was (at least X percent)
			&& completed < maxCompleted // ignore when the car is at last link
			&& replanCalls == 0 // replan only once
	){
		replanCalls++; // stats about replans
		return true; // do replan
	}
	else
	return false; // no replan

}

/**
routes are different?
	equal = false
	diff = true
*/
int RoutedDriver::routeDiff(ItsumoGraph::gRoute r1, ItsumoGraph::gRoute r2){

#ifdef DEBUGGING_ROUTED_DRIVER
	cout << "R1: \t";
	for (ItsumoGraph::gRoute::iterator it = r1.begin(); it != r1.end(); it++)
	    cout << (*it)->id << "\t";
	cout << endl;

	cout << "R2: \t";
	for (ItsumoGraph::gRoute::iterator it = r2.begin(); it != r2.end(); it++)
	    cout << (*it)->id << "\t";
	cout << endl;
#endif

	return r1 != r2; // when route change returns 1
}

void RoutedDriver::force2Out(bool out){

	ready2go = out;

}

bool RoutedDriver::back2Wait(){

	return goWait;

}

void RoutedDriver::force2Wait(bool wait){

	goWait = wait;

}

bool RoutedDriver::isBack(){

	return iAmBack;

}

void RoutedDriver::setBack(bool back){

	iAmBack = back;

}

void RoutedDriver::reActivate(){

	replanCalls = 0;
	replanChanges = 0;
	lastLanesetReplan = 0;
	active = true;	// assume the driver is added to network immediately
	currentLink = currentChoice = NULL;
	ready2go = endOfLane = false;
	stepInserted = stepRemoved = 0;
	theCar = NULL;
	goWait = false;
	iAmBack = 0;
	numberOfTrips++;
	nSrce = nSrceOriginal;
	nDest = nDestOriginal;
	//FindShortestPath();
	updateCostsFindShortest();
}

void RoutedDriver::setViewCost(string view) {	// view = "LOCAL" or "GLOBAL"

	viewCost = view;
}

void RoutedDriver::setReplan(bool replan) {

	replanner = replan;
}

void RoutedDriver::setCommunication(bool comm) {

	communicate = comm;
}

void RoutedDriver::resetGraphCosts() {

	graph.resetCurrentCosts();
	graph.resetLocalCosts();

}

bool RoutedDriver::getReplanner() {

	return replanner;
}

bool RoutedDriver::getGlobal() {

	return viewCost.compare("GLOBAL") == 0;
}

bool RoutedDriver::getCommunication() {

	return communicate;
}

int RoutedDriver::getSourceId() {

	return nSrce->itsumoNode->getId();
}

int RoutedDriver::getDestionationId() {

	return nDest->itsumoNode->getId();
}

int RoutedDriver::getNumberOfTrips() {

	return numberOfTrips;
}

double RoutedDriver::getDistanceTravelled() {

	return this->distanceTravelled;
}

void RoutedDriver::updateCostsFindShortest() {

	LaneSet *currentLaneSet; // just a bypass
	graph.resetCurrentCosts();

	// updating costs before find shortest path
	if ( viewCost.compare("GLOBAL") == 0 ) {
		graph.updateGraphDensityCosts(); // look all links to the end
	} else if (viewCost.compare("LOCAL") == 0 ) {
		graph.updateGraphDensityWithLocalCosts(this->getId(), currentLaneSet, 0);
	}

	graph.ModifiedLinks.clear();
	FindShortestPath();
}
