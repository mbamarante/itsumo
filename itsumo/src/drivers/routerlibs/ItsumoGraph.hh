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

#ifndef _HAS_GRAPHITSUMO_H
#define _HAS_GRAPHITSUMO_H

#include <cmath>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <iostream>

//#include "../../basic/types.hh"
#include "../../basic/network.hh"
//#include "../../basic/laneset.hh"
//#include "../../basic/node.hh"
//#include "../../basic/network.hh"

using namespace std;

class ItsumoGraph {
	public:
		struct __gNode;

		struct __gLink{
			unsigned long int id;
			LaneSet * itsumoLaneSet;
			__gNode * gNodeA;
			__gNode * gNodeZ;
			//unsigned long int gNode_id_a;
			//unsigned long int gNode_id_z;
			double base_cost;  	// euclidean distance, the lowest possible cost.
			double current_cost;	// cost considered by the agent (basecost + func_cost(currentVehicles+1))
			map<int, double> local_cost;    // cost known by the agent (local/partial knowledge)
		};// gLink;

		struct __gNode{
			unsigned long int id;
			Node * itsumoNode;
			float x;
			float y;
			// float lat;
			// float lon;
			vector<__gLink *> gLinksIn, gLinksOut;

			double EuclidDist(__gNode * gn)
			{
				static double xD, yD;
				xD = gn->x - x;
				yD = gn->y - y;
				return sqrt(xD * xD + yD * yD);
			}
			double MaxXYDist(__gNode * gn)
			{
				static double xD, yD;
				xD = gn->x - x;
				yD = gn->y - y;
				return xD > yD ? xD : yD;
			}
		}; // gNode;
		typedef struct __gNode gNode;
		typedef struct __gLink gLink;

		typedef list < gLink * > gRoute;
		//typedef pair<Node *, gNode *> It2gNode;
		map<Node *, gNode *> mapIt2gNode;
		gNode* getgNode4mapIt(Node * nd);

		vector<gNode> nodes; //array of nodes.
		vector<gLink> links; //array of pointers to links.

		static double invVmax; // inverted maximum velocity, to penalize crowded streets.
		set<unsigned long int> ModifiedLinks; // set of inconsistent states after link updates


		ItsumoGraph() {};

		void Network2Graph(Network * inw);
		void resetCurrentCosts();
		void resetLocalCosts();
		void updateGraphDensityCosts();	// global costs
		void updateGraphDensityWithLocalCosts(int driveId, LaneSet *currentLaneSet, int communicate); // local costs
		void updateLocalDensityCosts(gNode *, int depth);
		void updateLinkDensityCosts(gLink *);


		void gnuplotPrintGraph();
		void gnuplotPrintLinks();
		void gnuplotPrintNodes();
		void gnuplotPrintScrpt();

		friend ostream& operator<< (ostream& os, gNode & m);
		friend ostream& operator<< (ostream& os, gLink & m);
		friend ostream& operator<< (ostream& os, ItsumoGraph & m);

		/* local cost and communication */
		void updateLocalCost(gLink *currentLink, int driverId); // update local cost
		void printLocalCost(int driverId);
		void queryCost(LaneSet *currentLaneSet, gRoute currentRoute, int receiverDriverId);	// driver ask for information
		void receiveMessages(LaneSet *currentLaneSet, int receiverDriverId);
		double queryDriversAtLink(ItsumoGraph::gLink *link, vector<int> *driversId);
};

#ifdef __I_DID_NOT_UNDERSTAND_THIS_PART

		/*********** MAICON'S ORIGINAL CODE ***********/
	private:
		void buildLinks(st_node *mapNodes);
		void map_print_links();
		void map_print_node(st_node *node);
		void map_print_link(st_link *link);

		double EuclideanDistance(double x1, double y1, double x2, double y2);

		void buildGraph();
		void gnuplotPrintLinks(char *file, char *file2);
		void gnuplotShellScript(char *filesDir, int withsp);

		//cartesian node position relative to internal autoincremental node coords
		int getNodeCoordXInternal(int nodePos);
		int getNodeCoordYInternal(int nodePos);

		int getNodeAFromLink(int linkPos);
		int getNodeZFromLink(int linkPos);

		int getItsumoLinkId(int linkPos);
		int getCurrentCostFromLink(int linkPos);

		int getNodeIdfromItsumo(int itsumoNodeId);
		int getNodeIdToItsumo(int nodeId);

		int getNodeId(int nodePos);
		void setCurrGlobalDensity(int LaneSetId, double density);
		void setAvgGlobalDensity(int LaneSetId, double density);
		void setCurrGlobalCost(int LaneSetId, double density);

		//HASH
		string intToStr(int number);
		string strConcat(string str1, string str2);
		string generateKey(int key1, int key2);

		TmapNode* getRealNodeMap();

		int startNodeId, goalNodeId;

		THashMap1 Link2LaneSet;
		THashMap1 LaneSet2Link;

		__gnu_cxx::hash_map<const char*, double, __gnu_cxx::hash<const char*>, eqstr> weights;

		st_map *net_map;
		Network *net_;

	protected:
		TmapNode nodeMap_itsumo; //itsumo
		TmapNode nodeMap_autoinc; //autoincremento
		//TlinkBounds linksBounds;
		//TmapCoordXYtoId mapCoordXYtoId;

	public:
		void initialize();

		/******** GRAPH STATISTICS ***********/
		int getNumberOfNodes();
		int getNumberOfLinks();

		/*** graph costs ***/
		double getLinkCostById(int id);
		void setLinkCostById(int id, double cost);

		/***** NODE INFORMATION *****/
		//cartesian node position relative to itsumo coords
		float getNodeCoordX(int nodeId);
		float getNodeCoordY(int nodeId);
		int getNodeIdByCoord(int x, int y);
		int getNodePosition(int nodeId);
		int getNodeIdFromPosition(int position);

		/****** SHORTEST PATH *******/
		void setStartNode(int nodeId);
		void setGoalNode(int nodeId);
		int getStartNodeId();
		int getGoalNodeId();

		/****** INTERFACE *******/
		//GraphItsumo interface - child class have to implement
		//		virtual void addNode(int id, double x, double y) = 0;
		//		virtual void addEdge(int id, int idNodeA, int idNodeZ, double weight) = 0;
		//		virtual vector<int> getRoute() = 0;

		/******** HASH ***********/
		double getCost(int linkId, int driverId);
		void setCost(int linkId, int driverId, double cost);

		void updateDriverCost(int LaneSetId, int driverId);
		void updateGraphDensity();
		void resetGraphDensity();

		/****** VISUALIZATION *******/
		void gnuplotPrintGraph(char *filesDir, int withsp);
		void map_print_map();
		void map_print_links_weights();
		void map_print_link_weights(st_link *link);

		/***** SOLID(?) INTERFACE *****/
		st_map* getMap();

		GraphItsumo2Net(Network* nh);
		~GraphItsumo2Net();

#endif // __I_DID_NOT_UNDERSTAND_THIS_PART

#endif // _HAS_GRAPHITSUMO_H
