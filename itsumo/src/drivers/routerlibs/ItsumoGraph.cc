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
#include "ItsumoGraph.hh"
#include <cassert>

double ItsumoGraph::invVmax = 0.0;

ItsumoGraph::gNode* ItsumoGraph::getgNode4mapIt(Node * nd)
{
	return mapIt2gNode[nd];
}

void ItsumoGraph::Network2Graph(Network * inw)
{
	invVmax = 1.0/(double)(inw->getParentTopology()->getCarsMaximumSpeed());
	//Get nodes from Itsumo's network
	node_vector_t itsumoNodes = inw->getChildNodes();
	//Get links from Itsumo's network
	laneset_vector_t itsumoLaneSets = inw->getChildLaneSets();

	//Get links from Itsumo's network
	int capacity = inw->getCapacity();
	// double total_distance = 0; //JB::TODO get total euclidian distances for all neighbour nodes in the graph.

	//create a network map (network) using Francisco's simulator structure.
	nodes.resize(itsumoNodes.size());
	links.resize(itsumoLaneSets.size());

	// create nodes from itsumo's nodes
	unsigned long int cnt = 0;
	for (node_vector_t::iterator itNode = itsumoNodes.begin(); itNode != itsumoNodes.end(); ++itNode) {
		nodes[cnt].id = cnt;
		nodes[cnt].itsumoNode = (*itNode);
		nodes[cnt].x = (*itNode)->getXcoord();
		nodes[cnt].y = (*itNode)->getYcoord();
		// nodes[cnt].lat = (*itNode)->getLatCoord();
		// nodes[cnt].lon = (*itNode)->getLonCoord();

		//mapIt2gNode - a map for quick searching from Itsumo ID to gNode.
		mapIt2gNode[(*itNode)] = &(nodes[cnt]);
		//JB::TODO Optimize insertion with a vector, using sort before insert in map
		++cnt;
	} //JB::TODO Optimize using a pointer to nodes[cnt] or an iterator.

	// create links from itsumo's links
	cnt = 0;
	for (laneset_vector_t::iterator itLink = itsumoLaneSets.begin(); itLink != itsumoLaneSets.end(); ++itLink) {
		assert( mapIt2gNode.count( (Node*)((*itLink)->getStartNode()) ) /* > 0 */ );
		assert( mapIt2gNode.count((*itLink)->getEndNode()) /* > 0 */ );

		links[cnt].id = cnt;
		links[cnt].itsumoLaneSet = (*itLink);
		links[cnt].gNodeA = static_cast<gNode*>(mapIt2gNode[(*itLink)->getStartNode()]);
		links[cnt].gNodeZ = static_cast<gNode*>(mapIt2gNode[(*itLink)->getEndNode()]);

		links[cnt].gNodeA->gLinksOut.push_back( &(links[cnt]) );
		links[cnt].gNodeZ->gLinksIn.push_back( &(links[cnt]) );
		links[cnt].current_cost = links[cnt].base_cost = links[cnt].gNodeA->EuclidDist(links[cnt].gNodeZ);

		//JB::TODO add staff here:
		//density = 0
		//num_vehicles = 0
		//capacity = 

		++cnt;

	} //JB::TODO Optimize using a pointer to links[cnt]

	cout << "Network capacity: " << capacity << " vehicles." << endl;
	cout << "Number Of Links = " << cnt << endl;
//	cout << "Total Map Length = " << total_distance << " meters" << endl;
}

void ItsumoGraph::resetCurrentCosts()
{
	vector <ItsumoGraph::gLink>::iterator it;
	for(it = links.begin(); it != links.end(); ++it)
		(*it).current_cost = (*it).base_cost;
	ModifiedLinks.clear();
}

void ItsumoGraph::resetLocalCosts()
{
	vector <ItsumoGraph::gLink>::iterator it;
	for(it = links.begin(); it != links.end(); ++it)
		(*it).local_cost.clear();
}

void ItsumoGraph::updateGraphDensityCosts()
{
	vector <ItsumoGraph::gLink>::iterator it;
	for(it = links.begin(); it != links.end(); ++it)
		updateLinkDensityCosts(&(*it));
}

/*
 * Update graph costs only with local
 * costs, perceived by the driver or
 * received from another drivers (communication)
 *
 * */
void ItsumoGraph::updateGraphDensityWithLocalCosts(int driveId, LaneSet *currentLaneSet, int communicate) {
	vector <ItsumoGraph::gLink>::iterator it;
	vector<int> driversId;

	// communication: get all drivers located at current laneset
	if (communicate) {
		driversId = currentLaneSet->getDriversId();
	}

	for(it = links.begin(); it != links.end(); ++it){

		if (communicate) {
			(*it).local_cost[driveId] = queryDriversAtLink(&(*it), &driversId);
		}

		(*it).current_cost = (*it).local_cost[driveId];
	}
}

double ItsumoGraph::queryDriversAtLink(ItsumoGraph::gLink *link, vector<int> *driversId){

	// communication
	vector<int>::iterator d;

	double receivedCost = link->base_cost;
	int countMessages = 0;
	double sumCost = 0;

	if (link->local_cost.size() > 0){

		for (d = (*driversId).begin(); d != (*driversId).end(); ++d){

			double receivedCost = link->local_cost[(*d)];

			if (receivedCost > 0 && receivedCost != link->base_cost) {
				countMessages++;
				sumCost += receivedCost;
			}
		}
	}

	if (countMessages > 0) {
		return sumCost / countMessages;
	} else {
		return link->base_cost;
	}
}

//void ItsumoGraph::updateLocalDensityCosts(gNode * next)
//{
//	vector <ItsumoGraph::gLink *>::iterator it;
//	for(it = next->gLinksOut.begin(); it != next->gLinksOut.end(); ++it)
//		updateLinkDensityCosts(*it);
//}

void ItsumoGraph::updateLocalDensityCosts(gNode * next, int depth)
{
	vector <ItsumoGraph::gLink *>::iterator it;
	for(it = next->gLinksOut.begin(); it != next->gLinksOut.end(); ++it)
	{
		if (depth>0)
			updateLocalDensityCosts((*it)->gNodeZ, depth-1);
		//updateLinkDensityCosts(*it);
		updateLinkDensityCosts(*it);
	}
}

void ItsumoGraph::updateLinkDensityCosts(gLink * lnk)
{
	/* Assuming that:
	 *   MVH = min ( XVMax , (Ncels - Ncars) / Ncars)
	 * Then:
	 *   PenalizedLength = Lenght  // when XVMax < (Ncels - Ncars) / Ncars
	 *   PenalizedLength = Lenght * (Ncels - Ncars) / ( Ncars * XVMax ) //when XVMax > (Ncels - Ncars) / Ncars
	 */

	double Ncels = lnk->itsumoLaneSet->getCapacity();
	double Ncars = lnk->itsumoLaneSet->getNumberOfVehicles();
	double XVmax = lnk->itsumoLaneSet->getMaxVMax();
	lnk->current_cost = lnk->base_cost;
	if ( Ncels - Ncars < XVmax * Ncars)
	{
		lnk->current_cost *= (XVmax * Ncars);
		if(Ncels != Ncars) lnk->current_cost /= (Ncels - Ncars); // avoids div by zero
		ModifiedLinks.insert(lnk->id);
	}

	/* Maicons old penalty if somebody wants to restore it
	 * atualiza o custo do link adicionando ao seu comprimento nominal (base_cost) um custo proporcional
	 * ao volume do tráfego. Ex.: link com 100 metros de comprimento e 80% de ocupação. O novo custo será
	 * 100 m * ( 1 + ( 80 / 100 ) ) = 180 m. */

//	double Ncells = lnk->itsumoLaneSet->getCapacity();	// V (ratio v/c)
//	double Ncars = lnk->itsumoLaneSet->getNumberOfVehicles();	// C (ratio v/c)
//	double ratioVC = Ncars / Ncells;
//
//	lnk->current_cost = lnk->base_cost * (1 + ratioVC);
//	ModifiedLinks.insert(lnk->id);
}

ostream& operator<< (ostream &os, ItsumoGraph::gNode &node)
{
	vector <ItsumoGraph::gLink *>::iterator it;
	os << "\tnode id,X,Y,nIn,nOut";
	os << "\t" << node.id;
	os << "\t" << node.x;
	os << "\t" << node.y;
	os << "\t" << node.gLinksIn.size();
	os << "\t" << node.gLinksOut.size();
	os << "\n";
	for(it = node.gLinksIn.begin(); it != node.gLinksIn.end(); ++it)
		os << (*it);
	for(it = node.gLinksOut.begin(); it != node.gLinksOut.end(); ++it)
		os << (*it);
	return os;
}

ostream& operator<< (ostream &os, ItsumoGraph::gLink &link)
{
	os << "\t\tlink A,Z,cost,CC";
	os << "\t" << link.gNodeA->id;
	os << "\t" << link.gNodeZ->id;
	os << "\t" << link.base_cost;
	os << "\t" << link.current_cost;
	os << "\n";
	return os;
}

ostream& operator<< (ostream &os, ItsumoGraph &ig)
{
	os << "\t******** PRINTING ITSUMO GRAPH ********\n";
	os << "\tNodes : " << ig.nodes.size();
	os << "\tLinks : " << ig.links.size();
	os << "\n";
	vector <ItsumoGraph::gNode>::iterator it;
	for(it = ig.nodes.begin(); it != ig.nodes.end(); ++it)
		os << (*it);
	os << "\t******** ITSUMO GRAPH PRINTED! ********\n";
	return os;
}

void ItsumoGraph::gnuplotPrintGraph() {
	static bool gnuplotDirExists = (system ("mkdir -p ../../gnuplot/") >= 0);
	gnuplotPrintLinks();
	gnuplotPrintNodes();
	gnuplotPrintScrpt();
}

void ItsumoGraph::gnuplotPrintLinks() {
	ostringstream text;
	ofstream linksFl("../../gnuplot/links.txt");
	ofstream linksLblFl("../../gnuplot/links_labels.txt");
	ofstream linksBlkFl("../../gnuplot/linksBlk.txt");
	ofstream linksRedFl("../../gnuplot/linksRed.txt");
	ofstream linksGrnFl("../../gnuplot/linksGrn.txt");
	ofstream linksBluFl("../../gnuplot/linksBlu.txt");

	for (vector<gLink>::iterator it = links.begin(); it != links.end(); ++it)
	{
		text.str("");

		text << (*it).gNodeA->x << "\t" << (*it).gNodeA->y << "\t"
			<< (*it).gNodeZ->x - (*it).gNodeA->x << "\t" << (*it).gNodeZ->y - (*it).gNodeA->y << endl ;

		linksFl << text.str();
	
		double dens = (*it).itsumoLaneSet->getDensity();

		if (dens < 0.25) linksBlkFl << text.str();
		else if (dens < 0.50) linksGrnFl << text.str();
		else if (dens < 0.75) linksBluFl << text.str();
		else linksRedFl << text.str();
		
		//CurrentDensity in the middle of the lane line
		linksLblFl << ((*it).gNodeA->x + (*it).gNodeZ->x)/2 << "\t"
			<< ((*it).gNodeA->y + (*it).gNodeZ->y)/2 << "\t"
			<< (*it).gNodeA->itsumoNode->getName() << (*it).gNodeZ->itsumoNode->getName() << "\t"
			<< (*it).current_cost<< endl; // dens << endl;
	}

	linksFl.close();
	linksLblFl.close();
	linksBlkFl.close();
	linksRedFl.close();
	linksGrnFl.close();
	linksBluFl.close();
}

void ItsumoGraph::gnuplotPrintNodes() {
	ofstream nodesFl("../../gnuplot/nodes.txt");

	for (vector<gNode>::iterator it = nodes.begin(); it != nodes.end(); ++it)
		nodesFl << (*it).x << "\t" << (*it).y << "\t" << (*it).itsumoNode->getName() << endl;
	nodesFl.close();
}

void ItsumoGraph::gnuplotPrintScrpt() {
	ofstream plotFl("../../gnuplot/plotmap.gnuplot");

	plotFl << "plot 'links.txt' using 1:2:3:4 with vectors head filled size 0.05,30,1 lw 1.5  lt 1;\n"
		<< "replot 'shortest_path.txt' using 1:2:3:4 with vectors head filled size 0.05,30,1 lw 1.5  lt 2;\n"
		<< "replot 'nodes.txt' u 1:2:3 with labels;\n"
		<< "set terminal png size 4000, 3000;\n set size 1, 1\n"
		<< "set terminal svg;\n set size 1, 1\n"
		<< "set output 'graph.svg';\n"
		<< "replot\n";
	plotFl.close();
}

void ItsumoGraph::printLocalCost(int driverId) {
	vector <ItsumoGraph::gLink>::iterator it;

	cout << "local_cost for driver " << driverId << endl;

	for(it = links.begin(); it != links.end(); ++it)
		if (!(*it).local_cost[driverId]) {
			cout << "\t" << (*it).id << " -> " << (*it).base_cost << endl;
		}
		else {
			cout << "\t" << (*it).id << " -> " << (*it).local_cost[driverId] << endl;
		}
}


/**
 * Update local cost
 *
 * Driver update his local link cost information
 * based on his own perception
 *
 * @param driverId driver id
*/
void ItsumoGraph::updateLocalCost(gLink *currentLink, int driverId) {

	updateLocalDensityCosts(currentLink->gNodeZ, 1);	// update current link cost

	if (currentLink->current_cost != currentLink->base_cost) {	// ignore when current cost is the base cost

		currentLink->local_cost[driverId] = currentLink->current_cost;
	}
}


///**
// * EnRoute Communication - V1: ask near drivers links on route costs
// *
// * Driver ask to another drivers located in the
// * same link about link costs on his route.
// *
// * @param *CurrentLaneSet the current laneset where car is located.
// * @param currentRoute	the current driver route
//*/
//void ItsumoGraph::queryCost(LaneSet *currentLaneSet, gRoute currentRoute, int receiverDriverId) {
//
//	// ask for information about links on the receiver route
//	for (gRoute::iterator linkRoute = currentRoute.begin(); linkRoute != currentRoute.end(); linkRoute++) {
//		lane_vector_t ChildLanes = currentLaneSet->getChildLanes(); // get laneset lanes
//
//		// find another cars in the same laneset lane by line
//		for (lane_vector_t::iterator itLane = ChildLanes.begin(); itLane != ChildLanes.end(); ++itLane) {
//
//			int countMessages = 0; // number of received messages
//			double sumCost = 0;	// costs sum
//
//			// locate all cars in this lane
//			for (int i = 0; i < (*itLane)->getLength(); ++i) {
//
//				if ((*itLane)->queryCarPtr(i) != NULL) { // car found
//					double receivedCost; // cost communicated by another driver
//					int driverId = (*itLane)->queryCarPtr(i)->getDriver()->getId(); // car's driver
//
//					if (driverId != receiverDriverId ) {	// avoid auto-communication (from himself)
//						receivedCost = (*linkRoute)->local_cost[driverId]; // cost received during communication
//
//						/*
//						 * ignore when driver don't know the cost,
//						 * or cost is equal the base cost.
//						*/
//						if (receivedCost > 0 && receivedCost != (*linkRoute)->base_cost) {
//
//							countMessages++;
//							sumCost += receivedCost;
////							cout << "sender id: " << driverId
////									<< " | asked laneset: "
////									<< (*linkRoute)->itsumoLaneSet->getId()
////									<< " | cost: " << receivedCost
////									<< endl;
//						}
//					}
//				}
//			}
//
//			// update driver costs based on average costs
//			if (countMessages) {
//
//				(*linkRoute)->local_cost[receiverDriverId] = sumCost / countMessages;
//			}
//
//		} // end lane iterator
//	} // end gRoute iterator
//}

/**
 * EnRoute Communication - V2: ask neighborhood about all costs
 *
 * Driver ask to another drivers located in the
 * same link about link costs on his route.
 *
 * @param *CurrentLaneSet the current laneset where car is located.
 * @param currentRoute	the current driver route
*/
void ItsumoGraph::queryCost(LaneSet *currentLaneSet, gRoute currentRoute, int receiverDriverId) {

//	turning_probabilities_vector_t turnProbs = currentLaneSet->getTurningProbabilities();
//	turning_probabilities_vector_t::iterator itProbs;

	receiveMessages(currentLaneSet, receiverDriverId);

//	// nivel 1
//	for (itProbs = turnProbs.begin(); itProbs != turnProbs.end(); itProbs++) {
//
//		// ask costs for drivers located at a given laneset
//		receiveMessages(itProbs->first, receiverDriverId);
//	}

}

void ItsumoGraph::receiveMessages(LaneSet *currentLaneSet, int receiverDriverId) {

	lane_vector_t ChildLanes = currentLaneSet->getChildLanes(); // get current laneset lanes

	// iterate lanes to locate cars in the laneset
	for (lane_vector_t::iterator itLane = ChildLanes.begin(); itLane
			!= ChildLanes.end(); ++itLane) {

		// iterate all links in the network
		for (vector<gLink>::iterator link = links.begin(); link != links.end(); ++link) {

			if (link->local_cost.size() == 0) { // if nobody knows the cost...
				break;	// ... go to next.
			}

			int countMessages = 0; // number of received messages
			double sumCost = 0; // costs sum

			// locate all cars in this lane and ask the link cost
			for (int i = 0; i < (*itLane)->getLength(); ++i) {

				if ((*itLane)->queryCarPtr(i) != NULL) { // car found
					double receivedCost; // cost communicated by another driver
					int driverId =
							(*itLane)->queryCarPtr(i)->getDriver()->getId(); // car's driver

					if (driverId != receiverDriverId) { // avoid self-communication (from himself)
						// CONSIDERING SELF KNOWLEDGE


						receivedCost = link->local_cost[driverId]; // cost received during communication

						/*
						 * ignore when driver don't know the cost,
						 * or cost is equal the base cost.
						 */
						if (receivedCost > 0 && receivedCost != link->base_cost) {

							countMessages++;
							sumCost += receivedCost;
							//							cout << "sender id: " << driverId
							//									<< " | asked laneset: "
							//									<< (*linkRoute)->itsumoLaneSet->getId()
							//									<< " | cost: " << receivedCost
							//									<< endl;
						}
					}
				}
			}

			// update driver costs based on average costs
			if (countMessages) {

				link->local_cost[receiverDriverId] = sumCost / countMessages;
			}

		}
	} // end lane iterator
}

#ifdef __I_DID_NOT_UNDERSTAND_THIS_PART

/*********** MAICON'S ORIGINAL CODE ***********/

void GraphItsumo2Net::gnuplotShellScript(char *filesDir, int withsp) {

	std::ofstream outFile;

	char file1[strlen(filesDir) + 18];
	strcpy(file1, filesDir);
	strcat(file1, "/cmds.gnuplot.cmds");

	outFile.open(file1);
	outFile
		<< "plot 'links.gnuplot' using 1:2:3:4 with vectors head filled size 0.05,30,1 lw 1.5  lt 1;\n";

	if (withsp)
		outFile
			<< "replot 'shortest_path.gnuplot' using 1:2:3:4 with vectors head filled size 0.05,30,1 lw 1.5  lt 2;\n";

	outFile << "replot 'points.gnuplot' u 1:2:3 with labels;\n";
	//outFile << "set terminal png size 4000, 3000;\n set size 1, 1\n";
	outFile << "set terminal svg;\n set size 1, 1\n";
	outFile << "set output 'graph.svg';\n";
	outFile << "replot\n";
	outFile.close();

	std::ofstream outFile2;

	char file2[strlen(filesDir) + 16];
	strcpy(file2, filesDir);
	strcat(file2, "/plot.gnuplot.sh");

	outFile2.open(file2);
	outFile2 << "#!/bin/bash\n";
	outFile2 << "gnuplot cmds.gnuplot.cmds\n";
	outFile2.close();

	//exec script
	char cmd[strlen(filesDir) + 21];
	strcpy(cmd, "(cd ");
	strcat(cmd, filesDir);
	strcat(cmd, ";");
	strcat(cmd, " bash ");
	strcat(cmd, "plot.gnuplot.sh)");
	//printf("->%s\n", cmd);
	//system(cmd);
}

void GraphItsumo2Net::gnuplotPrintLinks(char *file, char *file2) {

	std::ofstream outFile(file);
	std::ofstream outFile2(file2);

	char f1[strlen(file) + 10];
	char f2[strlen(file) + 10];
	char f3[strlen(file) + 10];
	char f4[strlen(file) + 10];

	strcpy(f1, file);
	strcpy(f2, file);
	strcpy(f3, file);
	strcpy(f4, file);

	strcat(f1, ".black");
	strcat(f2, ".red");
	strcat(f3, ".orange");
	strcat(f4, ".green");

	cout << f1 << endl;
	cout << f2 << endl;
	cout << f3 << endl;
	cout << f4 << endl;

	std::ofstream outFileBK(f1);
	std::ofstream outFileRD(f2);
	std::ofstream outFileBL(f3);
	std::ofstream outFileGR(f4);

	//LanesetList lanesets;
	//LanesetList::iterator itL;
	//lanesets = net_->lanesets();

	laneset_vector_t lst = net_->getChildLaneSets();
	laneset_vector_t::iterator itl;

	for (itl = lst.begin(); itl != lst.end(); itl++) {

		//for (itL=lanesets.begin(); itL!=lanesets.end(); itL++)

		st_link in;

		//finding node that will receive the link.
		TmapNode::iterator iter;

		iter = nodeMap_itsumo.find((*itl)->getStartNode()->getId());

		if (iter != nodeMap_itsumo.end()) {

			TmapNode::iterator node_a = nodeMap_itsumo.find(
					(*itl)->getStartNode()->getId());
			TmapNode::iterator node_z = nodeMap_itsumo.find(
					(*itl)->getEndNode()->getId());

			in.node_id_a = node_a->second.node_id; //itL->begin;
			in.node_id_z = node_z->second.node_id; //itL->end;

			outFile << node_a->second.lat;
			outFile << "\t";
			outFile << node_a->second.lon;
			outFile << "\t";
			outFile << node_z->second.lat - node_a->second.lat;
			outFile << "\t";
			outFile << node_z->second.lon - node_a->second.lon;
			outFile << "\n";

			std::ostringstream text;
			text << node_a->second.lat << "\t";
			text << node_a->second.lon << "\t";
			text << node_z->second.lat - node_a->second.lat << "\t";
			text << node_z->second.lon - node_a->second.lon << "\n";

			//if ((*itl)->getLaneSetDensity() < 0.25)	outFileGR << text.str(); else
			//if ((*itl)->getLaneSetDensity() < 0.50)	outFileBL << text.str(); else
			//if ((*itl)->getLaneSetDensity() < 0.75)	outFileRD << text.str(); else
			//outFileBK << text.str();

			//densidade média
			int linkId = LaneSet2Link[(*itl)->getId()];
			if (net_map->links[linkId]->avg_density < 0.25) outFileGR << text.str(); else
				if (net_map->links[linkId]->avg_density < 0.50) outFileBL << text.str(); else
					if (net_map->links[linkId]->avg_density < 0.75) outFileRD << text.str(); else
						outFileBK << text.str();

			//texto será exibido no ponto médio do vetor
			outFile2 << (node_a->second.lat + node_z->second.lat)/2 << "\t";
			outFile2 << (node_a->second.lon + node_z->second.lon)/2 << "\t";
			outFile2 << (*itl)->getLaneSetDensity() << "\n";
		}

	}

	outFile.close();
	outFile2.close();

	outFileBK.close();
	outFileRD.close();
	outFileBL.close();
	outFileGR.close();
}

void GraphItsumo2Net::gnuplotPrintGraph(char *filesDir, int withsp) {

	char file1[strlen(filesDir) + 14];
	char file1_1[strlen(filesDir) + 14];

	strcpy(file1, filesDir);
	strcat(file1, "/links.gnuplot");

	strcpy(file1_1, filesDir);
	strcat(file1_1, "/links.labels.gnuplot");

	gnuplotPrintLinks(file1, file1_1);
	//printf(">%s\n", filesDir);
	//printf(">%s\n", file1);

	/*	LanesetList lanesets;
		LanesetList::iterator itL;
		lanesets = net_->lanesets(); */

	laneset_vector_t lst = net_->getChildLaneSets();
	laneset_vector_t::iterator itl;

	node_vector_t nds = net_->getChildNodes();
	node_vector_t::iterator it;

	char file2[strlen(filesDir) + 15];
	strcpy(file2, filesDir);
	strcat(file2, "/points.gnuplot");

	std::ofstream outFile(file2);

	for (it = nds.begin(); it != nds.end(); it++) {
		//for (itl = lst.begin(); itl != lst.end(); itl++) 
		//for (itL=lanesets.begin(); itL!=lanesets.end(); itL++)

		TmapNode::iterator iter;
		iter = nodeMap_itsumo.find((*it)->getId());

		if (iter != nodeMap_itsumo.end()) {

			//saving gnuplot points
			outFile << iter->second.lat;
			outFile << "\t";
			outFile << iter->second.lon;
			outFile << "\t";
			//outFile << iter->second.node_id;
			outFile << iter->second.itsumo_node_id;
			//outFile << "-";
			//outFile << iter->second.itsumo_node_name;
			outFile << "\n";
			//}
	}
}

outFile.close();

gnuplotShellScript(filesDir, withsp);
}

void GraphItsumo2Net::updateGraphDensity(){

	laneset_vector_t lst = net_->getChildLaneSets();
	laneset_vector_t::iterator itl;

	for (itl = lst.begin(); itl != lst.end(); itl++) {

		int LaneSetId = (*itl)->getId();
		double density = net_->getLanesetById(LaneSetId)->getLaneSetDensity();

		//setCurrGlobalDensity(LaneSetId, density);

		if (density > 0){
			setCurrGlobalDensity(LaneSetId, density);
			setAvgGlobalDensity(LaneSetId, density);
			setCurrGlobalCost(LaneSetId, density);
		}
	}
}

void GraphItsumo2Net::resetGraphDensity(){

	laneset_vector_t lst = net_->getChildLaneSets();
	laneset_vector_t::iterator itl;

	for (itl = lst.begin(); itl != lst.end(); itl++) {

		int LaneSetId = (*itl)->getId();
		int linkId = LaneSet2Link[LaneSetId];

		net_map->links[linkId]->curr_density = 0;
		net_map->links[linkId]->avg_density = 0;
		net_map->links[linkId]->current_cost = net_map->links[linkId]->base_cost;
	}
}

/*
 * atualiza o custo do link adicionando ao seu comprimento nominal (base_cost) um custo proporcional
 * ao volume do tráfego. Ex.: link com 100 metros de comprimento e 80% de ocupação. O novo custo será
 * 100 * (1+(80/100)) = 180.
 * */
void GraphItsumo2Net::setCurrGlobalCost(int LaneSetId, double density){

	int linkId = LaneSet2Link[LaneSetId];

	/*
	 * as densidades abaixo relacionadas são atualizadas antes do método "updateCurrGlobalDensity" ser chamado
	 * net_map->links[linkId]->avg_density = densidade média
	 * net_map->links[linkId]->curr_density = densidade corrente
	 */

	//sobrescrevendo o densidade - precisa mudar: ou passa por parâmetro ou não.
	density = net_map->links[linkId]->avg_density;
	//atualiza o custo corrente que será utilizado no
	net_map->links[linkId]->current_cost = net_map->links[linkId]->base_cost * (1 + density ); //density entre 0 e 1
	//cout << "base_cost=" << net_map->links[linkId]->base_cost << "	density=" << density << "	new_cost=" << net_map->links[linkId]->current_cost << endl;
}

/*
 * atualiza a densidade corrente de todos os link (igual a densidade da laneset correspondente ao link)
 * */
void GraphItsumo2Net::setCurrGlobalDensity(int LaneSetId, double density){

	int linkId = LaneSet2Link[LaneSetId];
	//cout << LaneSetId << "=" << linkId << endl;
	net_map->links[linkId]->curr_density = density;
}

/*
 * atualiza a densidade média de todos os link (igual a densidade da laneset correspondente ao link
 * + a densidade do link no step anterior dividido por 2)
 * */
void GraphItsumo2Net::setAvgGlobalDensity(int LaneSetId, double density){

	int linkId = LaneSet2Link[LaneSetId];

	//	cout << "(net_map->links[linkId]->avg_density + density)/2" << " | " <<
	//			net_map->links[linkId]->avg_density << ";" << density << " | linkId=" << linkId << " LaneSetId=" << LaneSetId << endl;

	net_map->links[linkId]->avg_density = (net_map->links[linkId]->avg_density + density)/2;
}

#endif // __I_DID_NOT_UNDERSTAND_THIS_PART
