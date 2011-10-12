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
#ifndef _HAS_ROUTED_H
#define _HAS_ROUTED_H

#include <vector>
#include <set>
#include <limits>
//#include <iostream>

#include "nasch.hh"
#include "routerlibs/ItsumoGraph.hh"
#include "../basic/types.hh"
#include "../basic/car.hh"
//#include "../../basic/network.hh"

using namespace std;

struct _less_lpa;
 
class RoutedDriver: public Nasch {
		static ItsumoGraph graph;

		ItsumoGraph::gNode * nSrce;
		ItsumoGraph::gNode * nDest;
		ItsumoGraph::gNode * nSrceOriginal;
		ItsumoGraph::gNode * nDestOriginal;

		ItsumoGraph::gLink * currentLink;
		ItsumoGraph::gLink * currentChoice;
		ItsumoGraph::gRoute  linksPath;

		//	maps the link IDs to pairs of (sum, count) to find the 
		// avg occupation of each visited link
		typedef pair< double, int > OccupationPair;
		typedef map< unsigned long int, OccupationPair > OccupationMap;
		OccupationMap observed_info;

		// activates the recording of densities and their use on weighting the graph
		bool use_local_info;

		bool active;	// wheter or not the current driver is in the network
		bool goWait;
		Car *theCar;	// the car being driven by current driver

		//*** sp algorithms data ***
		vector<double> v;	// cost of this node and predecessors when processed (see Likhachev)
		vector<double> g;	// cost of this node and predecessors
		vector<double> h;	// heuristic cost to goal
		vector<ItsumoGraph::gLink *> parent_lk; // tracked path during search
		// Instead of a closed set, we deal it with flags

		enum Sets { NONE, OPEN, CLOSED, INCONS };
		vector<unsigned char> treated; // 0: Nope; 1: Open; 2: Closed; 3: Inconsistent
		double AStarWeight;	// weight to execute generalized ARA* and AD*, controls accuracy
		static double AStarInitialWeight; // initial weight for any ARA* execution
		bool firstrun;

		// Open List HEAP: simple vector with heap functions: make_heap(), push_heap(), pop_heap()
		vector<unsigned long int> OpenHeap;
		set<unsigned long int> InconsSet; // set of inconsistent states

		bool ready2go;
		bool endOfLane;

		string viewCost; // cost point of view: LOCAL or GLOBAL.
		bool replanner;	// replanner or not
		bool communicate;	// communicate or not

		friend struct _less_f;
		friend struct _less_gwh;
		friend struct _less_lpa;

		int curStep; // MB: REPLANNING _ used to know how delayed the driver is
		int routeSize; // MB: REPLANNING _ used to know how finished the travel is
		double routeLength; // MB: REPLANNING _ used to know many steps the driver needs to complete his travel at free flow traffic
		int routeStep; // MB: REPLANNING _ used to know route age

		void spBuildRoute();
		void spDijkstra();
		void GeneralizedAStarComputePath(double weight = 1.0);
		void GeneralizedComputePath(double weight = 1.0);
		void GeneralizedInit();
		void GeneralizedStep();
		void UpdateSetMembership(const unsigned long int & s, _less_lpa & IsLess);
		void spAStar();
		void spLPAStar();
		void spADStar();
		void spARAStar(); // calls spARAStarInit or ARAStarStep, according to ARAStarWeight
		void spARAStarInit();
		void spARAStarStep();

		int calculateLinkCost(ItsumoGraph::gLink *l);
		int iAmBack;

		bool activateReplanStrategy(LaneSet *currentLaneSet); // call replan strategy
		bool replanDelayStrategy(LaneSet *currentLaneSet); // replan delay strategy
		int routeDiff(ItsumoGraph::gRoute r1, ItsumoGraph::gRoute r2); // for replans stats
		void updateCostsFindShortest();

		void saveLocalInfo();
		void loadLocalInfo();

	public:
		RoutedDriver(string driver_class_name);
		void set_source_destination( Node * source, Node * dest );
		void setNetwork(Network * n);
		void useLocalInfo(bool use_li);

		void FindShortestPath();	//This function calls the right algorithm. This should be a function pointer.
		void setPathSize();
		int getlinksPathSize();     //uded by geradorOD to know if the source sink has a valid route.

		bool readyIn(int step);
		bool readyOut(int step);

		void force2Out(bool out);
		bool back2Wait();
		bool isBack();
		void setBack(bool back);

		void reActivate();
		bool getReplanner();
		bool getGlobal();
		bool getCommunication();

		void force2Wait(bool wait);
		int where(int step);
		void update(int step);
		void removedFromNet();

		void resetGraphCosts();

		int getSourceId();
		int getDestionationId();

		void setViewCost(string view);	// set view cost: global or local
		void setReplan(bool replan);	// set as replanner or not
		void setCommunication(bool comm);	// enable/disable communication

		int getNumberOfTrips();
		double getDistanceTravelled();

		lane_laneset_pair_t chooseNextLane_Laneset(Car *theCar);
		decision_t takeDecision (Car &car);

#ifdef TOCHECK_LDKFKADFKLJADSFKL
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
#endif
};

// objects of this struct compare the f value of two nodes
struct _less_f { 
	RoutedDriver const& sf;
	_less_f(RoutedDriver const& self_):sf(self_){};
	bool operator() ( size_t id1, size_t id2) { return sf.g[id1] > sf.g[id2]; };
};

struct _less_gwh { 
	RoutedDriver const& sf;
	double const& ww;
	_less_gwh(RoutedDriver const& self_,double const& weight_):sf(self_),ww(weight_){};
	bool operator() ( size_t id1, size_t id2) { return sf.g[id1] + ww * sf.h[id1] > sf.g[id2] + ww * sf.h[id2]; };
};

struct _less_lpa { 
	RoutedDriver const& sf;
	double const& ww;
	_less_lpa(RoutedDriver const& self_,double const& weight_):sf(self_),ww(weight_){};
	bool operator() ( size_t id1, size_t id2) {
		double k1, k2, k1a, k2a;
		if ( sf.v[id1] >= sf.g[id1] )
		   	k1 = ( k1a = sf.g[id1] ) + ww * sf.h[id1];
		else
		   	k1 = ( k1a = sf.v[id1] ) + sf.h[id1];

		if ( sf.v[id2] >= sf.g[id2] )
		   	k2 = ( k2a = sf.g[id2] ) + ww * sf.h[id2];
		else
		   	k2 = ( k2a = sf.v[id2] ) + sf.h[id2];

		return ( k1 == k2 ) ? k1a > k2a : k1 > k2;
	};
};

#endif // _HAS_ROUTED_H
