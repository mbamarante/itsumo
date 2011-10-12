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
#include "network.hh"
#include <sstream>
#include "types.hh"

double median(vector<double>& nr) {
	double average = 0.0;

	for (int i = 0; i < nr.size();++i)
	average += nr[i];

	return average/nr.size();
}

string Network::getName()
{
  return name;
}


void Network::setName(string _name)
{
  name = _name;
}


/************************************************************
  Method: laneset_vector_t Network::getChildLaneSets()
  Author: Bruno Castro da Silva
  Description: returns a copy of the childLaneSets vector.

  Changelog:
************************************************************/

laneset_vector_t Network::getChildLaneSets()
{
  return static_cast<laneset_vector_t>(childLaneSets);
}


/************************************************************
	Method: int Network::getCapacity()
	Author: Alexander Benavides
	Description: returns the number of cells of the laneset
	Changelog:
************************************************************/

int Network::getCapacity() {
	int rtrn = 0;
	for(laneset_vector_t::iterator it = childLaneSets.begin(); it != childLaneSets.end(); ++it) {
		rtrn += (*it)->getCapacity();
	}
	return rtrn;
}


/************************************************************
  Method: lane_vector_t Network::getChildLanes()
  Author: Bruno Castro da Silva
  Description: returns a copy of the childLanes vector.

  Changelog:
************************************************************/

lane_vector_t Network::getChildLanes() {
  return static_cast<lane_vector_t>(childLanes);

}


/************************************************************
  Method: Node* Network::getNodeByTL(string id)
  Author: Robert Junges
  Description: returns a reference to the node where some
		traffic light is located

  Changelog:
************************************************************/
Node* Network::getNodeByTL(string id)
{
	Node* ans = NULL;

	int i = atoi(id.c_str());

	node_vector_t nds = getChildNodes();
	node_vector_t::iterator itnds;

	//JB::TODO delete, is it for nothing?
	// laneset_vector_t lst = getChildLaneSets();

	for (itnds = nds.begin(); itnds!= nds.end(); itnds++)
	{
		TrafficLight* tl = (*itnds)->getTrafficLight();
		if (tl != NULL)
			if ( tl->getId() == i )
				return *itnds;
	}

	return ans;
}


/************************************************************
  Method: Node* Network::getNodeById(string id)
  Author: Maicon de Brito do Amarante
  Description: returns a reference to the node

  Changelog:
************************************************************/
Node* Network::getNodeById(string id)
{
	Node* ans = NULL;

	int i = atoi(id.c_str());

	node_vector_t nds = getChildNodes();
	node_vector_t::iterator itnds;

	//JB::TODO delete, is it for nothing?
	// laneset_vector_t lst = getChildLaneSets();

	for (itnds = nds.begin(); itnds!= nds.end(); itnds++)
	{
//		TrafficLight* tl = (*itnds)->getTrafficLight();
//		if (tl != NULL)
			//if ( tl->getId() == i )
		if ((*itnds)->getId() == i)
				return *itnds;
	}

	return ans;
}

LaneSet* Network::getLanesetById(int laneset_id)
{
	LaneSet *ans=NULL;

	laneset_vector_t lst = getChildLaneSets();
	laneset_vector_t::iterator itlst;

	for (itlst = lst.begin(); itlst!= lst.end(); itlst++)
	{
		if ((*itlst)->getId() == laneset_id)
		{
			return *itlst;
		}
	}

	return ans;
}

/************************************************************
  Method: void Network::registerSection(string name, Section* _section)
  Author: Bruno Castro da Silva
  Description: adds a section to the correspondent street
        in the streets vector

  Changelog:
************************************************************/

void Network::registerSection(Section* _section) {

  street_vector_t::iterator i;
 // systemLog->putMessage("debug_show_topology_creation", 7,
//			"registrando section #%d na street %d", _section->getId(), _section -> getStreetId());

  // sweep all known streets
  for (i = streets.begin(); i != streets.end(); i++)

    // if the street belongs to this network, then we add _section to the definition of such street
    if ((i->first).first == _section -> getStreetId()) {
      (i->second).push_back(_section);
      break;
    }


  // if the section is malformed (ie: belongs to a street that is not registered in the network),
  // then something wrong ocurred
  if (i == streets.end()) {
   cerr <<" *** ERROR!    Trying to add section (id:"
	<< _section -> getId()
	<<") to street #"
	<<_section -> getStreetId()
  	<< " *** ERROR!    but this street DOES NOT belong to the current network (id:"
	<<_section -> getParentNetwork() -> getId()
	<<")" << endl;
  }

}

/************************************************************
  Method: String Network::process_tlight_update(node_vector_t nodes)
  Author: Bruno Castro da Silva and Robert Junges
  Description: process signal plan changes.

  Changelog: Felipe Boffo, 30/7/2007
  			Denise de Oliveira 29/09/2008 : verifica pelo tamanho da
  			mensagem recebida se o agente esta mandando somete o ID do plano
  			desejado ou se esta mandando o plano completo
************************************************************/
void Network::process_tlight_update(string message)
{
  cout << "NETWORK: Processing traffic light update message....." << endl << message << endl;

  /* Message format
     The message consists of the following fields, separeted by ";"
     x;[trafficLightId;
     signalPlanId;
     numberOfPhases;
     [phaseId;
     phaseStartIter;
     phaseEndIter;
  */

	bool simple = false;

  char p[strlen(message.c_str())+1];
  strncpy(p, message.c_str(), strlen(message.c_str())+1);

  char tok[50];
  char *pont;

  pont = p;

  //start
  getNextVal(&pont, tok);
  cout << "FIRST FIELD [OPERATION TYPE]: " << tok << endl; // ignore the first field (message type)

  //start
  getNextVal(&pont, tok);
  int numberTlights = atoi(tok);

  if (numberTlights!=0){
	  std::string::size_type mark_pos( 0 );
	  int totalmark=0; //total number of the ";" character occurrencies in the message

	  while (mark_pos!=std::string::npos ){
	       mark_pos = message.find(";", mark_pos );
	       if ( mark_pos != std::string::npos ){
				++totalmark;
				++mark_pos; // start next search after this ";"
	       }
	  }

	  if ((numberTlights*2+2) == totalmark ){
	  	simple = true;
	  }


	  for (int tl =0; tl < numberTlights; tl++) {
	    getNextVal(&pont, tok);
	    int tlightId = atoi(tok);

	    TrafficLight * tlight = (TrafficLight *)getElementById(tlightId);
	    cout << "NETWORK: TrafficLight: " << tlightId << " is currently using signal plan: "
	         << tlight->getCurrentPlan() << endl;

	    getNextVal(&pont, tok);
	    int signalPlanId = atoi(tok);

	    // PROBLEMAS? Existe uma grande chance de a causa estar abaixo!
	    // sets the new signalplan and resets the current phase to the first one
	    tlight -> setCurrentPlan (signalPlanId);
	    tlight -> currentPlanPhase = 0;

		if (!simple){
		    //gets a phase list for a specific signal plan
		    trafficLight_plan_vector_t *plans = tlight->getTrafficLightPlanList();

		    int realPlanIndex = tlight->getCurrentPlan();


		    getNextVal(&pont, tok);
		    int numberPhases = atoi(tok);

		    for (int ph=0; ph < numberPhases; ph++)
		      {
		        getNextVal(&pont, tok);
		        int phaseId = atoi(tok);
		        getNextVal(&pont, tok);
		        int start = atoi(tok);
		        getNextVal(&pont, tok);
		        int end = atoi(tok);

		        trafficLight_plan_phase_vector_t::iterator itPhase;
		        for (itPhase=((*plans)[realPlanIndex]).phases.begin(); itPhase != ((*plans)[realPlanIndex]).phases.end(); itPhase++)
		          if ( (*itPhase).phaseIndex ==  phaseId)
		            {
		              (*itPhase).startIter = start;
		              (*itPhase).endIter = end;
		              (*itPhase).iterationInterval = end - start;
		              break;
		            }

		      }

		    tlight -> commitPlanPhase();
		}
	    cout << "NETWORK: TrafficLight: " << tlightId << " has changed the signal plan to: "
	         << tlight->getCurrentPlan() << endl;
  	}

  }
}


/************************************************************
  Method: String Network::tlNodeList()
  Author: Robert Junges
  Description: returns a vector os all nodes with tl

  Changelog:
************************************************************/
node_vector_t Network::tlNodeList()
{
	node_vector_t ans;
	node_vector_t::iterator node;

	node_vector_t childNodes = getChildNodes ();

	for (node = childNodes.begin(); node != childNodes.end(); node++)
	{
    		TrafficLight *tlight = (*node)->getTrafficLight();

		if (tlight != NULL)
			ans. push_back(*node);
	}
	return ans;
}


/************************************************************
  Method: String Network::tlightAgentData(node_vector_t nodes)
  Author: Bruno Castro da Silva and Robert Junges
  Description: creates a message with state information of a list
            of nodes (eg: for each lane arriving at the node,
            returns the lane density, average speed, etc)

  Changelog:
************************************************************/
string Network::tlightAgentData(node_vector_t nodes, int iteration)
{
	node_vector_t::iterator node;

	string message;
	/* Message format

	The message consists of the following fields, separeted by ";"
		currentIteration
		numberOfNodes;
			[nodeId; trafficLightId; currentPlan;
				totalNumberOfIncomingLanes;
					[laneId; laneDensity;
				 	laneAvgSpeed; laneCarsStopped;]
				totalNumberOfOutgoingLanes;]
					[laneId; laneDensity;
				 	laneAvgSpeed; laneCarsStopped;]]
	*/

	message += "i;";
	message += intToString(iteration) + ";";
	message += intToString(nodes.size()) + ";";

	for (node = nodes.begin(); node != nodes.end(); node++)
	{

		message += intToString((*node)->getId()) + ";";

    		TrafficLight *tlight = (*node)->getTrafficLight();

		message += intToString(tlight->getId()) + ";";

		message += intToString(tlight -> getCurrentPlan()) + ";";

    		section_vector_t sections = (*node) -> getSections();
    		section_vector_t::iterator section;


	    	// calculates total number of incoming lanes (to a node)
		//-------------------------------------------------
		int totalIncomingLanes = 0;
		int totalOutgoingLanes = 0;
		for (section = sections.begin(); section != sections.end(); section++) {
      			laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
      			laneSet_vector_t::iterator laneset;
      			for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {
				if ((*laneset) -> getStartNode() != (*node))
					totalIncomingLanes += (*laneset)->getNumberOfLanes();
				else if ((*laneset) -> getStartNode() == (*node))
					totalOutgoingLanes += (*laneset)->getNumberOfLanes();
			}
		}
		//-------------------------------------------------

		message += intToString(totalIncomingLanes) + ";";

		// for each section
    	for (section = sections.begin(); section != sections.end(); section++) {

  			laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
  			laneSet_vector_t::iterator laneset;

  			// for each laneset
  			for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {
				// only collects data for lanesets arriving at the node
				if ((*laneset) -> getStartNode() != (*node)) {

					// get the lanes' info
					lane_vector_t childLanes = (*laneset)->getChildLanes ();
					lane_vector_t::iterator lane;

					for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {

						message += intToString((*lane)->getId()) + ";";
						message += doubleToString((*lane)->getAvgSpeed()) + ";";
						message += doubleToString((*lane)->getDensity()) + ";";
						//message += intToString((*lane)->getStoppedCars()) + ";";
						message += intToString((*lane)->getQueueSize()) + ";";
						}
				}
			}
		}


		message += intToString(totalOutgoingLanes) + ";";

		// for each section
    		for (section = sections.begin(); section != sections.end(); section++) {

      			laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
      			laneSet_vector_t::iterator laneset;

      			// for each laneset
      			for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {

				// only collects data for lanesets leaving the node
				if ((*laneset) -> getStartNode() == (*node)) {

					// get the lanes' info
					lane_vector_t childLanes = (*laneset)->getChildLanes ();
					lane_vector_t::iterator lane;

					for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {

						message += intToString((*lane)->getId()) + ";";
						message += doubleToString((*lane)->getAvgSpeed()) + ";";
						message += doubleToString((*lane)->getDensity()) + ";";
						//message += intToString((*lane)->getStoppedCars()) + ";";
						message += intToString((*lane)->getQueueSize()) + ";";
}
    				}
  			}
		}
	}

	return message;


}


/************************************************************
  Method: String Network::tlightAgentSatFlowData(node_vector_t nodes)
  Author: Denise de Olvieira
  Description: creates a message with state information of a list
            of nodes (eg: for each lane arriving at the node,
            returns the lane density, average speed, etc)

  Changelog:
************************************************************/
string Network::tlightAgentSatFlowData(node_vector_t nodes, int iteration)
{
	node_vector_t::iterator node;

	string message;
	/* Message format

	The message consists of the following fields, separeted by ";"
		currentIteration
		numberOfNodes;
			[nodeId; trafficLightId; currentPlan;
				totalNumberOfIncomingLanes;
					[laneId; laneDegreeOfSaturation;
				 	laneEntryFlow; laneQueue;]
				totalNumberOfOutgoingLanes;]
					[laneId; laneDegreeOfSaturation;
				 	laneEntryFlow; laneQueue;]]
	*/

	message += "i;";
	message += intToString(iteration) + ";";
	message += intToString(nodes.size()) + ";";

	for (node = nodes.begin(); node != nodes.end(); node++)
	{

		message += intToString((*node)->getId()) + ";";

    		TrafficLight *tlight = (*node)->getTrafficLight();

		message += intToString(tlight->getId()) + ";";

		message += intToString(tlight -> getCurrentPlan()) + ";";

    		section_vector_t sections = (*node) -> getSections();
    		section_vector_t::iterator section;


	    	// calculates total number of incoming lanes (to a node)
		//-------------------------------------------------
		int totalIncomingLanes = 0;
		int totalOutgoingLanes = 0;
		for (section = sections.begin(); section != sections.end(); section++) {
      			laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
      			laneSet_vector_t::iterator laneset;
      			for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {
				if ((*laneset) -> getStartNode() != (*node))
					totalIncomingLanes += (*laneset)->getNumberOfLanes();
				else if ((*laneset) -> getStartNode() == (*node))
					totalOutgoingLanes += (*laneset)->getNumberOfLanes();
			}
		}
		//-------------------------------------------------

		message += intToString(totalIncomingLanes) + ";";

		// for each section
    	for (section = sections.begin(); section != sections.end(); section++) {

  			laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
  			laneSet_vector_t::iterator laneset;

  			// for each laneset
  			for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {
				// only collects data for lanesets arriving at the node
				if ((*laneset) -> getStartNode() != (*node)) {

					// get the lanes' info
					lane_vector_t childLanes = (*laneset)->getChildLanes ();
					lane_vector_t::iterator lane;

					for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {

						message += intToString((*lane)->getId()) + ";";
						message += doubleToString((*lane)->getDegreeOfSaturation()) + ";";
						message += doubleToString((*lane)->getEntryFlow()) + ";";
						message += intToString((*lane)->getQueueSize()) + ";";
						}
				}
			}
		}


		message += intToString(totalOutgoingLanes) + ";";

		// for each section
    		for (section = sections.begin(); section != sections.end(); section++) {

      			laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
      			laneSet_vector_t::iterator laneset;

      			// for each laneset
      			for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {

				// only collects data for lanesets leaving the node
				if ((*laneset) -> getStartNode() == (*node)) {

					// get the lanes' info
					lane_vector_t childLanes = (*laneset)->getChildLanes ();
					lane_vector_t::iterator lane;

					for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {

						message += intToString((*lane)->getId()) + ";";
						message += doubleToString((*lane)->getDegreeOfSaturation()) + ";";
						message += doubleToString((*lane)->getEntryFlow()) + ";";
						message += intToString((*lane)->getQueueSize()) + ";";
}
    				}
  			}
		}
	}

	return message;


}





/************************************************************
  Method: String Network::tlightConfiguration(node_vector_t nodes)
  Author: Bruno Castro da Silva and Robert Junges
  Description: creates a message containing the traffic light configuration
		(eg, number of signalplans, phases, etc) for each trafficlight installed
		in 'nodes'

  Changelog:
************************************************************/
string Network::tlightConfiguration(node_vector_t nodes)
{
	string message;
	/* Message format

	The message consists of the following fields, separeted by ";"
		numberOfTrafficLights
			[trafficLightId;
			 numberOfSignalPlans;
				[signalPlanId;
				 numberOfPhases;
					[phaseId;
					 phaseStartIter;
					 phaseEndIter;
					 numberOfOpenDirections;
						[fromLane;
					       toLane;]]]]
	*/

	message += "c;";
	message += intToString(nodes.size()) + ";";

	node_vector_t::iterator node;
	for (node = nodes.begin(); node != nodes.end(); node++)
	{
		TrafficLight *tlight = (*node)->getTrafficLight();
		message += intToString((*tlight).getId()) + ";";

		cout << "TL: " << intToString((*tlight).getId()) << endl;

		message += intToString(tlight -> getNumberOfPlans()) + ";";

		trafficLight_plan_vector_t *plans = tlight->getTrafficLightPlanList();

		trafficLight_plan_vector_t::iterator itPlan;
		for (itPlan = plans->begin(); itPlan != plans->end(); itPlan++)
		{
			message += intToString((*itPlan).planIndex) + ";";

			trafficLight_plan_phase_vector_t phases = (*itPlan).phases;
			message += intToString(phases.size()) + ";";

			trafficLight_plan_phase_vector_t::iterator itPhase;
			for (itPhase = phases.begin(); itPhase != phases.end(); itPhase++)
			{
				message += intToString((*itPhase).phaseIndex) + ";";
				int startIter = (*itPhase).startIter;
				int endIter = (*itPhase).endIter;

				message += intToString(startIter) + ";";
				message += intToString(endIter) + ";";

				trafficLight_phase_t::iterator direction;
				trafficLight_phase_vector_t *phaselist = tlight->getTrafficLightPhaseList();

				message += intToString((*phaselist)[(*itPhase).phaseIndex].size()) + ";";

				for (direction = (*phaselist)[(*itPhase).phaseIndex].begin();
	     				direction != (*phaselist)[(*itPhase).phaseIndex].end();
	     				direction++) {
						message += intToString(direction->first->getId()) + ";";
						message += intToString(direction->second->getId()) + ";";
				}

			}

		}
	}

	cout << message << endl;
	return message;

}









/************************************************************
  Method: void Network::registerStreet(topology_element_id_t _streetId, string name)
  Author: Bruno Castro da Silva
  Description: adds a new street to the streets vector

  Changelog:
************************************************************/

void Network::registerStreet(topology_element_id_t _streetId, string name) {

  street_vector_t::iterator i;

  // sweep all known streets
  for (i = streets.begin(); i != streets.end(); i++)

    // if the street already belongs to this network, then something wrong ocurred
    if ((i->first).first == _streetId) {
	cerr << "ERRO " << endl;
    break;
    }


  // if the street is not registered yet, then we can add it to the network
  if (i == streets.end()) {
    street_sections_t sections;
    streets.push_back(make_pair(make_pair(_streetId, name), sections));
  }

}


/************************************************************
  Method: void Network::string getStreetById(int id)
  Author: Bruno Castro da Silva
  Description: searches a street name given its id

  Changelog:
************************************************************/

string Network::getStreetNameById(int id)
{

  street_vector_t::iterator i;

  // sweep all known streets
  for (i = streets.begin(); i != streets.end(); i++)

    // if the street already belongs to this network, then something wrong ocurred
    if ((i->first).first == id) {
      return (i->first).second;
    }

  cerr << "ERROR. Street with id " << id << " not found" << endl;

  return "ERR";

}






string Network::printTopologyForVisualization()
{
  ostringstream cout;

  vector < int > sources;
  vector < int > sinks;
  int tlight_id;


  cout << "NETWORK_NAME" << endl;
  cout << getName() << endl;

  cout << "NODES" << endl;

  node_vector_t childNodes = getChildNodes ();
  node_vector_t::iterator node;
  cout << childNodes.size() << endl;
  for (node = childNodes.begin(); node != childNodes.end(); node++) {

    int has_trafficlight = 0;
    int has_source = 0;
    int has_sink = 0;

    tlight_id = -1;
    sources.clear();
    sinks.clear();

    cout << (*node) -> getId() << " " << (*node) -> getXcoord() << " " << (*node) -> getYcoord() << endl;

    cout << (*node)->getName() << endl;

    if ((*node) -> getTrafficLight() != NULL) {
      has_trafficlight = 1;
      tlight_id = (*node) -> getTrafficLight() -> getId();
    }

    for (unsigned int i=0; i < (*node)->calibrators.size(); i++) {
      if (((*node)->calibrators[i] -> getType()) == SOURCE) {
	has_source = 1;
	sources.push_back( ((*node)->calibrators[i])->getId() );
      }
      else {
	has_sink = 1;
	sinks.push_back( ((*node)->calibrators[i])->getId() );
      }
    }


    // #define SOURCE 3
    // #define SOURCE_TRAFFICLIGHT 4
    // #define SINK 5
    // #define SINK_TRAFFICLIGHT 6
    // #define SOURCE_SINK 7
    // #define TRAFFIC_LIGHT 8
	// #define SOURCE_SINK_TRAFFICLIGHT 9


    if (has_trafficlight) {
      if (has_source) {
	if (has_sink) {
	  cout << "9" << endl;   // tlight + source + sink

	  cout << tlight_id << endl;

	  cout << sources.size() << endl;
	  for (unsigned int i=0; i < sources.size(); i++)
	    cout << sources[i] << "\t";
	  cout << endl;

	  cout << sinks.size() << endl;
	  for (unsigned int i=0; i < sinks.size(); i++)
	    cout << sinks[i] << "\t";
	  cout << endl;
	}
	else {
	  cout << "4" << endl;   // tlight + source

	  cout << tlight_id << endl;

	  cout << sources.size() << endl;
	  for (unsigned int i=0; i < sources.size(); i++)
	    cout << sources[i] << "\t";
	  cout << endl;
	}
      }
      else {
	if (has_sink) {
	  cout << "6" << endl;   // tlight + sink

	  cout << tlight_id << endl;

	  cout << sinks.size() << endl;
	  for (unsigned int i=0; i < sinks.size(); i++)
	    cout << sinks[i] << "\t";
	  cout << endl;


	}
	else {
	  cout << "8" << endl;    // tlight

	  cout << tlight_id << endl;

	}
      }
    }
    else {
      if (has_source) {
	if (has_sink) {
	  cout << "7" << endl;    // source + sink

	  cout << sources.size() << endl;
	  for (unsigned int i=0; i < sources.size(); i++)
	    cout << sources[i] << "\t";
	  cout << endl;

	  cout << sinks.size() << endl;
	  for (unsigned int i=0; i < sinks.size(); i++)
	    cout << sinks[i] << "\t";
	  cout << endl;

	}
	else {
	  cout << "3" << endl;    // source

	  cout << sources.size() << endl;
	  for (unsigned int i=0; i < sources.size(); i++)
	    cout << sources[i] << "\t";
	  cout << endl;

	}
      }
      else {
	if (has_sink) {
	  cout << "5" << endl;    // sink

	  cout << sinks.size() << endl;
	  for (unsigned int i=0; i < sinks.size(); i++)
	    cout << sinks[i] << "\t";
	  cout << endl;

	}
	else {
	  cout << "0";    // no objects in nodex
	  cout << endl;
	}
      }
    }

  }



  cout << "SECTIONS" << endl;

  section_vector_t childSections = getChildSections ();
  street_sections_t::iterator section;

  cout << childSections.size() << endl;
  for (section = childSections.begin(); section != childSections.end(); section++) {
    Node *start_node, *end_node;
    (*section) -> getDelimitingNodes(&start_node, &end_node);
    cout << (*section) -> getId() << " " << start_node->getId() << " ";
    cout << end_node->getId() << " " << (*section)->getNumberOfLaneSets();
    cout << " " << (*section)->getLength() << endl;
    cout << (*section)->getName() << endl;
    cout << (*section)->getParentNetwork()->getStreetNameById( (*section)->getStreetId() ) << endl;

    laneset_vector_t childLaneSets = (*section)->getChildLaneSets();
    laneset_vector_t::iterator laneset;
    for (laneset = childLaneSets.begin(); laneset != childLaneSets.end(); laneset++) {

      lane_vector_t childLanes = (*laneset)->getChildLanes();
      lane_vector_t::iterator lane;

      cout << (*laneset)->getId() << " " << (*laneset)->getStartNode()->getId()  << " " << childLanes.size() << endl;
      for (lane = childLanes.begin(); lane != childLanes.end(); lane++)
	cout << (*lane) -> getId() << " ";
      cout << endl;

    }

  }
  return cout.str();
}




/************************************************************
  Method: void Network::printStreets()
  Author: Bruno Castro da Silva
  Description: prints information about all topology's streets,
     including its lanesets, lanes, delimiting nodes, turning
     probabilities, etc

  Changelog:
************************************************************/


void Network::printStreets() {

  street_vector_t::iterator street;
  // sweep all known streets
  for (street = streets.begin(); street != streets.end(); street++) {

    // for each street
    cout << "Street #" << (street->first).first << " (" << (street->first).second << ")" << endl;

    // print the sections
    street_sections_t::iterator section;
    for (section = street->second.begin(); section != street->second.end(); section++) {
      cout << " --- section #" << (*section) -> getId();
      Node *t1, *t2;
      (*section) -> getDelimitingNodes(&t1, &t2);
      cout << " (nodes " << t1 -> getId() << " e " << t2 -> getId() << ")   (length ";
      cout << (*section) -> getLength() << ")" << endl;

      // print the sections' lanesets
      laneSet_vector_t childLaneSets = (*section)->getChildLaneSets ();
      laneSet_vector_t::iterator laneset;

      for (laneset=childLaneSets.begin(); laneset != childLaneSets.end(); laneset++) {
	cout << "     --- laneset #" << (*laneset) -> getId();
	cout << " (startNode #" << (*laneset) -> getStartNode() -> getId();
	cout << " e endNode #" << (*laneset) -> getEndNode() -> getId() << ")" << endl;

	// print the lanesets' turning probabilities
	turning_probabilities_vector_t turnProbs = (*laneset)->getTurningProbabilities ();
	turning_probabilities_vector_t::iterator itProbs;

	for (itProbs = turnProbs.begin(); itProbs != turnProbs.end(); itProbs++) {
	  cout << "         --- " << itProbs->second << "% of turning to laneset ";
	  cout << (itProbs->first)->getId() << endl;
	}


	// print the lanesets' lanes
	lane_vector_t childLanes = (*laneset)->getChildLanes ();
	lane_vector_t::iterator lane;

	for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {
	  cout << "            --- lane #" << (*lane) -> getId();
	  cout << " (maximum speed = " << (*lane) -> getMaxSpeed ();
	  cout << ")" << endl;
	}
      }
    }
  }
}






/************************************************************
  Method: void Network::XMLOutput() {
  Author: Bruno Castro da Silva
  Description: convert the already instantiated network
        to the xml format

  Changelog:
************************************************************/


void Network::XMLOutput()
{

  cout << "<simulation>" << endl;
  cout << "\t<network_id>" << getId() << "</network_id>" << endl;
  cout << "\t<network_name>" << getName() << "</network_name>" << endl;
  cout << "\t<settings>" << endl;
  cout << "\t\t<cell_size_in_meters>" << getParentTopology()->getCellSize() << "</cell_size_in_meters>" << endl;
  cout << "\t\t<iteration_length_in_seconds>" << getParentTopology()->getIterationTime();
  cout << "</iteration_length_in_seconds>" << endl;
  cout << "\t\t<cars_maximum_speed>" << getParentTopology()->getCarsMaximumSpeed() << "</cars_maximum_speed>" << endl;
  cout << "\t</settings>" << endl;

  cout << "\t<nodes>" << endl;
  node_vector_t childNodes = getChildNodes ();
  node_vector_t::iterator c;

  for (c=childNodes.begin(); c != childNodes.end(); c++) {
    cout << "\t\t<node>" << endl;
    cout << "\t\t\t<node_id>" << (*c)->getId() << "</node_id>" << endl;
    cout << "\t\t\t<node_name>" << (*c)->getName() << "</node_name>" << endl;
    cout << "\t\t\t<x_coord>" << (*c)->getXcoord() << "</x_coord>" << endl;
    cout << "\t\t\t<y_coord>" << (*c)->getYcoord() << "</y_coord>" << endl;
    cout << "\t\t</node>" << endl;
  }
  cout << "\t</nodes>" << endl;


  cout << "\t<sources>" << endl;
  for (c=childNodes.begin(); c != childNodes.end(); c++) {
    for (unsigned int i=0; i < ((*c)->calibrators).size(); i++) {
      if ((((*c)->calibrators)[i] -> getType()) == SOURCE) {
		Source *s = dynamic_cast < Source * > ((*c)->calibrators[i]);
		cout << "\t\t<source>" << endl;
		cout << "\t\t\t<source_id>" << s->getId() << "</source_id>" << endl;
		cout << "\t\t\t<first_activation_at>" << s->getTimeBegin() << "</first_activation_at>" << endl;
		cout << "\t\t\t<located_at_node>" << s->getParentNode()->getId() << "</located_at_node>" << endl;
		cout << "\t\t\t<target_laneset>" << s->getAssociatedLaneSet()->getId() << "</target_laneset>" << endl;
		cout << "\t\t\t<source_activation_frequency>" << s->getTimeStep();
		cout << "</source_activation_frequency>" << endl;

		cout << "\t\t\t<source_type>";
		switch (s->getSourceType()) {
			case CONSTANT_PROB:
			  cout << "CONSTANT_PROB</source_type>" << endl;
			  cout << "\t\t\t\t<probability>" << s->getActivationProbability() << "</probability>" << endl;
			  break;
			case VARIABLE_PROB:
			  cout << "VARIABLE_PROB</source_type>" << endl;
			  break;
			case CONSTANT_FLOW:
			  cout << "CONSTANT_FLOW</source_type>" << endl;
			  cout << "\t\t\t\t<flow>" << s->getConstantFlow() << "</flow>" << endl;
			  break;
			case VARIABLE_FLOW:
			  cout << "VARIABLE_FLOW</source_type>" << endl;
			  break;
		}
		cout << "\t\t</source>" << endl;
      }
    }
  }
  cout << "\t</sources>" << endl;


  cout << "\t<sinks>" << endl;
  for (c=childNodes.begin(); c != childNodes.end(); c++) {
    for (unsigned int i=0; i < ((*c)->calibrators).size(); i++) {
      if ((((*c)->calibrators)[i] -> getType()) == SINK) {
		Sink *s = dynamic_cast < Sink * > ((*c)->calibrators[i]);
		cout << "\t\t<sink>" << endl;
		cout << "\t\t\t<sink_id>" << s->getId() << "</sink_id>" << endl;
		cout << "\t\t\t<first_activation_at>" << s->getTimeBegin() << "</first_activation_at>" << endl;
		cout << "\t\t\t<located_at_node>" << s->getParentNode()->getId() << "</located_at_node>" << endl;
		cout << "\t\t\t<source_laneset>" << s->getAssociatedLaneSet()->getId() << "</source_laneset>" << endl;
		cout << "\t\t\t<removal_probability>" << s->getActivationProbability() << "</removal_probability>" << endl;
		cout << "\t\t</sink>" << endl;
      }
    }
  }
  cout << "\t</sinks>" << endl;


  cout << "\t<traffic_lights>" << endl;
  for (c=childNodes.begin(); c != childNodes.end(); c++) {
    TrafficLight *tlight = (*c)->getTrafficLight();
    if (tlight == NULL)
      continue;

    cout << "\t\t<traffic_light>" << endl;
    cout << "\t\t\t<traffic_light_id>" << tlight->getId() << "</traffic_light_id>" << endl;
    cout << "\t\t\t<located_at_node>" << tlight->getParentNode()->getId() << "</located_at_node>" << endl;
    cout << "\t\t\t<signalplans>" << endl;
    trafficLight_plan_vector_t::iterator plan;
    for (plan=(tlight->getTrafficLightPlanList())->begin(); plan != (tlight->getTrafficLightPlanList())->end(); plan++) {
      cout << "\t\t\t\t<signalplan>" << endl;
      cout << "\t\t\t\t\t<signalplan_id>" << plan->planIndex << "</signalplan_id>" << endl;

      cout << "\t\t\t\t\t<phases>" << endl;
      trafficLight_plan_phase_vector_t::iterator phase;
      for (phase=(plan->phases).begin(); phase!=(plan->phases).end(); phase++) {
	cout << "\t\t\t\t\t\t<phase>" << endl;
	cout << "\t\t\t\t\t\t\t<phase_id>" << phase->phaseIndex << "</phase_id>" << endl;
	cout << "\t\t\t\t\t\t\t<iteration_start>" << phase->startIter << "</iteration_start>" << endl;
	cout << "\t\t\t\t\t\t\t<iteration_end>" << phase->startIter << "</iteration_end>" << endl;

	cout << "\t\t\t\t\t\t\t<directions>" << endl;
	trafficLight_phase_t::iterator direction;
	trafficLight_phase_vector_t *phaselist = tlight->getTrafficLightPhaseList();
	for (direction = (*phaselist)[phase->phaseIndex].begin();
	     direction != (*phaselist)[phase->phaseIndex].end();
	     direction++) {
	  cout << "\t\t\t\t\t\t\t\t<direction>" << endl;
	  cout << "\t\t\t\t\t\t\t\t\t<from_lane>" << direction->first->getId();
	  cout << "</from_lane>" << endl;
	  cout << "\t\t\t\t\t\t\t\t\t<to_laneset>" << direction->second->getId();
	  cout << "</to_laneset>" << endl;
	  cout << "\t\t\t\t\t\t\t\t</direction>" << endl;
	}
	cout << "\t\t\t\t\t\t\t</directions>" << endl;
	cout << "\t\t\t\t\t\t</phase>" << endl;
      }
      cout << "\t\t\t\t\t</phases>" << endl;
      cout << "\t\t\t\t</signalplan>" << endl;
    }
    cout << "\t\t\t</signalplans>" << endl;
    cout << "\t\t</traffic_light>" << endl;
  }
  cout << "\t</traffic_lights>" << endl;


  cout << "\t<streets>" << endl;
  street_vector_t::iterator street;
  for (street = streets.begin(); street != streets.end(); street++) {
    cout << "\t\t<street>" << endl;;
    cout << "\t\t\t<street_id>" <<  (street->first).first << "</street_id>" << endl;
    cout << "\t\t\t<street_name>" <<  (street->first).second << "</street_name>" << endl;
    street_sections_t::iterator section;
    cout << "\t\t\t<sections>" << endl;
    for (section = street->second.begin(); section != street->second.end(); section++) {
      cout << "\t\t\t\t<section>" << endl;
      cout << "\t\t\t\t\t<section_id>" << (*section) -> getId() << "</section_id>" << endl;
      cout << "\t\t\t\t\t<section_name>" << (*section) -> getName() << "</section_name>" << endl;
      cout << "\t\t\t\t\t<is_preferencial>";
      cout << (((*section)->isPreferencial()) ? "true" : "false") << "</is_preferencial>" << endl;
      Node *t1, *t2;
      (*section) -> getDelimitingNodes(&t1, &t2);
      cout << "\t\t\t\t\t<delimiting_node>" << t1 -> getId() << "</delimiting_node>" << endl;
      cout << "\t\t\t\t\t<delimiting_node>" << t2 -> getId() << "</delimiting_node>" << endl;
      laneSet_vector_t childLaneSets = (*section)->getChildLaneSets ();
      laneSet_vector_t::iterator laneset;

      cout << "\t\t\t\t\t<lanesets>" << endl;
      for (laneset=childLaneSets.begin(); laneset != childLaneSets.end(); laneset++) {
	cout << "\t\t\t\t\t\t<laneset>" << endl;
	cout << "\t\t\t\t\t\t\t<laneset_id>" << (*laneset) -> getId() << "</laneset_id>" << endl;
	cout << "\t\t\t\t\t\t\t<laneset_position>" << (*laneset) -> getPosition();
	cout << "</laneset_position>" << endl;
	cout << "\t\t\t\t\t\t\t<start_node>" << (*laneset) -> getStartNode() -> getId();
	cout << "</start_node>" << endl;
	cout << "\t\t\t\t\t\t\t<end_node>" << (*laneset) -> getEndNode() -> getId();
	cout << "</end_node>" << endl;

	turning_probabilities_vector_t turnProbs = (*laneset)->getTurningProbabilities ();
	turning_probabilities_vector_t::iterator itProbs;

	cout << "\t\t\t\t\t\t\t<turning_probabilities>" << endl;
	for (itProbs = turnProbs.begin(); itProbs != turnProbs.end(); itProbs++) {
	  cout << "\t\t\t\t\t\t\t<direction>" << endl;
	  cout << "\t\t\t\t\t\t\t\t<destination_laneset>" << (itProbs->first)->getId();
	  cout << "</destination_laneset>" << endl;
	  cout << "\t\t\t\t\t\t\t\t<probability>" << itProbs->second;
	  cout << "</probability>" << endl;
	  cout << "\t\t\t\t\t\t\t</direction>" << endl;
	}
	cout << "\t\t\t\t\t\t\t</turning_probabilities>" << endl;

	lane_vector_t childLanes = (*laneset)->getChildLanes ();
	lane_vector_t::iterator lane;

	cout << "\t\t\t\t\t\t\t<lanes>" << endl;
	for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {
	  cout << "\t\t\t\t\t\t\t\t<lane>" << endl;
	  cout << "\t\t\t\t\t\t\t\t\t<lane_id>" << (*lane) -> getId() << "</lane_id>" << endl;
	  cout << "\t\t\t\t\t\t\t\t\t<lane_position>" << (*lane) -> getPosition();
	  cout << "</lane_position>" << endl;
	  cout << "\t\t\t\t\t\t\t\t\t<maximum_speed>" << (*lane) -> getMaxSpeed ();
	  cout << "</maximum_speed>" << endl;
	  cout << "\t\t\t\t\t\t\t\t\t<deceleration_prob>";
	  cout << (*lane) -> getDecelerationProbability() << "</deceleration_prob>" << endl;
	  cout << "\t\t\t\t\t\t\t\t</lane>" << endl;
	}
	cout << "\t\t\t\t\t\t\t</lanes>" << endl;
	cout << "\t\t\t\t\t\t</laneset>" << endl;
      }
      cout << "\t\t\t\t\t</lsetw(2)anesets>" << endl;
      cout << "\t\t\t\t</section>" << endl;
    }
    cout << "\t\t\t</sections>" << endl;
    cout << "\t\t</street>" << endl;;
  }

  cout << "\t</streets>" << endl;

  cout << "</simulation>" << endl;

}







/************************************************************
  Method: string Network::laneSensorSpeedUp(int iteration)
  Author: Bruno Castro da Silva
  Description: calls the lanesensor to every lane in
      the topology by means of the childLanes vector,
      which contains direct pointers and therefore
      speeds up the printing process (opposed to Network::laneSensor())

  Changelog:
************************************************************/

string Network::laneSensorSpeedUp(int iteration) {
  ostringstream cout;
  lane_vector_t::iterator lane;

  for (lane=childLanes.begin(); lane != childLanes.end(); lane++){
    cout << (*lane) -> printLaneStatus(iteration);
  }
  cout << endl;
  return cout.str();

}



/************************************************************
  Method: string Network::controlledLanesSensorSummary(int iteration)
  Author: Denise de Oliveira
  Description: prints a summarized output, contaning information
  of the controlled lanes:
     	- Average Saturation Degree (decimal percentage, ie: 75% = 0.75)
     	- Average Queue Size (vehicles)
     	- Total Queued Vechiles (vehicles)
     	- Average Speed (Km/h)
     	- Average Delay Time (seconds)
     	- Total Distance Travelled (vehicles-Km/hour)
     This function makes a summary *only for controlled lanes* (lanes
     with traffic lights at the end).
     The definition of those values can be found in the Lane class.
  Changelog:
*************************************************************/
string Network::controlledLanesSensorSummary(int iteration) {
  ostringstream cout;
  street_sections_t::iterator section;
  section_vector_t childSections = getChildSections ();

  double averageSaturationDegree=0.0;
  double averageQueueSize=0.0;
  double averageSpeed=0.0;
  double averageDelayTime=0.0;
  double totalDistanceTravelled=0.0;
  int numlanes=0;

  cout << iteration << "\t";

  for (section = childSections.begin(); section != childSections.end(); section++) {
		laneSet_vector_t lanesets = (*section) -> getChildLaneSets();
		laneSet_vector_t::iterator laneset;
		// for each laneset
		for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {

			// get the lanes' info
			lane_vector_t childLanes = (*laneset)->getChildLanes();
			lane_vector_t::iterator lane;

			for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {
				if((*lane)->isControlled()) {
					averageSaturationDegree+=(*lane)->getDegreeOfSaturation(); //percentage
					averageQueueSize+=(*lane)->getQueueSize(); //number of vehicles
					averageSpeed+=(*lane)->getAvgSpeedKMH(); //Km/h
  					averageDelayTime+=(*lane)->getDelayTime(); //seconds
  					totalDistanceTravelled+=(*lane)->getDistanceTravelled(); // vehicles-Km/hour
					numlanes++;
				}
			}
		}
	}
  // print everything
  cout.setf(ios::showpoint);
  cout.setf(ios::fixed,ios::floatfield);
  cout.precision(3);
  if(numlanes>0){
	  cout << averageSaturationDegree/numlanes;
	  cout <<"\t"<<averageQueueSize/numlanes;
	  cout <<"\t"<<averageQueueSize;
	  cout <<"\t"<<averageSpeed/numlanes;
	  cout <<"\t"<<averageDelayTime/numlanes;
	  cout <<"\t"<<totalDistanceTravelled<<endl;
  }
  return cout.str();
}

/************************************************************
  Method: string Network::laneSensor(int iteration)
  Author: Bruno Castro da Silva
  Description: calls the lanesensor to every lane in
      the topology by sweeping all streets, then all
      lanesets, etc. Normally this method should not
      used. See Network::laneSensorSpeedUp()

  Changelog:
************************************************************/

string Network::laneSensor(int iteration) {

  ostringstream cout;
  street_vector_t::iterator street;
  // sweep all known streets
  for (street = streets.begin(); street != streets.end(); street++) {

    // for each street

    // print the sections
    street_sections_t::iterator section;
    for (section = street->second.begin(); section != street->second.end(); section++) {

      // print the sections' lanesets
      laneSet_vector_t childLaneSets = (*section)->getChildLaneSets ();
      laneSet_vector_t::iterator laneset;

      for (laneset=childLaneSets.begin(); laneset != childLaneSets.end(); laneset++) {

	// print the lanesets' lanes
	lane_vector_t childLanes = (*laneset)->getChildLanes ();
	lane_vector_t::iterator lane;

	for (lane=childLanes.begin(); lane != childLanes.end(); lane++)
	  cout << (*lane) -> printLaneStatus(iteration);
      }
    }
  }
  cout << endl;
  return cout.str();
}


/************************************************************
  Method: string Network::laneSetSensor(int iteration)
  Author: Bruno Castro da Silva
  Description: prints a summarized output, contaning
     laneset information such as the laneset density, etc.
     If you need to print the cell occupation map use
     Network::laneSensorSpeedUp()

  Changelog:
************************************************************/

string Network::laneSetSensor(int iteration) {


  ostringstream cout;
  double laneSetDensity=0;

  // typeOfLaneSet is -1 if the laneset if entering the node, 1 otherwise
  int typeOfLaneSet = 0;


  node_vector_t nodes = getChildNodes();
  node_vector_t::iterator node;

  // sweep all nodes
  for (node = nodes.begin(); node != nodes.end(); node++) {

    TrafficLight *tlight = (*node)->getTrafficLight();
    if (tlight == NULL)
      continue;

    section_vector_t sections = (*node) -> getSections();
    section_vector_t::iterator section;

    // for each section
    for (section = sections.begin(); section != sections.end(); section++) {

      laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
      laneSet_vector_t::iterator laneset;

      // for each laneset
      for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {

	if ((*laneset) -> getStartNode() == (*node))
	  typeOfLaneSet = -1;
	else
	  typeOfLaneSet = 1;

	// get the lanes' info
	lane_vector_t childLanes = (*laneset)->getChildLanes ();
	lane_vector_t::iterator lane;

	laneSetDensity = 0;
	for (lane=childLanes.begin(); lane != childLanes.end(); lane++)
	  laneSetDensity += (*lane) -> getDensity();
	laneSetDensity /= childLanes.size();

	cout << iteration << "\t";
	cout << (*node) -> getId();

	// print everything
	cout.setf(ios::showpoint);
	cout.setf(ios::fixed,ios::floatfield);
	cout.precision(2);
	cout.width(4);

	cout << "\t" << (*laneset)->getId();
	cout << "\t" << laneSetDensity;
	cout << "\t" << setw(2) << typeOfLaneSet << endl;

      }
    }
  }
  cout << endl;
  return cout.str();
}


/************************************************************
  Method: string Network::laneSetSensorWithoutBorderNodes(int iteration)
  Author: Bruno Castro da Silva
  Description: prints a summarized output, contaning
     laneset information such as the laneset density, etc.
     If you need to print the cell occupation map use
     Network::laneSensorSpeedUp(). This function ignores the lanesets
     leaving nodes that contain a source
  Changelog:
************************************************************/

string Network::laneSetSensorWithoutBorderNodes(int iteration) {


  ostringstream cout;
  double laneSetDensity=0;

  // typeOfLaneSet is -1 if the laneset if entering the node, 1 otherwise
  int typeOfLaneSet = 0;

  node_vector_t nodes = getChildNodes();
  node_vector_t::iterator node;

  // sweep all nodes
  for (node = nodes.begin(); node != nodes.end(); node++) {

    TrafficLight *tlight = (*node)->getTrafficLight();
    if (tlight == NULL)
      continue;


    section_vector_t sections = (*node) -> getSections();
    section_vector_t::iterator section;

    // for each section
    for (section = sections.begin(); section != sections.end(); section++) {

      laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
      laneSet_vector_t::iterator laneset;

      // for each laneset
      for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {

        // does not print info for this laneset if it connected with a source
        bool hasSource=false;
    	for (unsigned int i=0; i < (*laneset)->getStartNode()->calibrators.size(); i++)
             if (((*laneset)->getStartNode()->calibrators[i] -> getType()) == SOURCE) {
          	cout << "ignorando lanesets que saem de " << (*laneset)->getStartNode()->getId() << " pq ele eh um source" << endl;
	        hasSource=true;
	     }
        if (hasSource)
       	   continue;


	if ((*laneset) -> getStartNode() == (*node))
	  typeOfLaneSet = -1;
	else
	  typeOfLaneSet = 1;

	// get the lanes' info
	lane_vector_t childLanes = (*laneset)->getChildLanes ();
	lane_vector_t::iterator lane;

	laneSetDensity = 0;
	for (lane=childLanes.begin(); lane != childLanes.end(); lane++)
	  laneSetDensity += (*lane) -> getDensity();
	laneSetDensity /= childLanes.size();

	cout << iteration << "\t";
	cout << (*node) -> getId();

	// print everything
	cout.setf(ios::showpoint);
	cout.setf(ios::fixed,ios::floatfield);
	cout.precision(2);
	cout.width(4);

	cout << "\t" << (*laneset)->getId();
	cout << "\t" << laneSetDensity;
	cout << "\t" << setw(2) << typeOfLaneSet << endl;

      }
    }
  }
  cout << endl;
  return cout.str();
}



/************************************************************
  Method: string Network::sectionSensor(int iteration)
  Author: Bruno Castro da Silva
  Description: prints a summarized output contaning
     information per section such as the average section density, etc.

  Changelog:
************************************************************/

string Network::sectionSensor(int iteration) {

  ostringstream cout;
  // prints the section's densities
  street_sections_t::iterator section;
  section_vector_t childSections = getChildSections ();

  cout << iteration << "\t";

  for (section = childSections.begin(); section != childSections.end(); section++) {

    double sectionDensity=0;
    double sectionAvgSpeed=0;
    int carsInTheSection=0;

    laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
    laneSet_vector_t::iterator laneset;

    // for each laneset
    for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {

      // get the lanes' info
      lane_vector_t childLanes = (*laneset)->getChildLanes ();
      lane_vector_t::iterator lane;

      double laneSetDensity = 0;
      for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {
	laneSetDensity += (*lane) -> getDensity();
	// counts the number of cars in the lane and the sum of their speeds
	for (int j = 0; j <  (*lane)->getLength() ; j++) {
	  topology_element_id_t carId = (*lane)->queryCarId(j);
	  if (carId!=-1) {
	    carsInTheSection++;
	    sectionAvgSpeed += (*lane)->queryCarSpeed(j);
	  }
	}
      }
      laneSetDensity /= childLanes.size();
      sectionDensity += laneSetDensity;

    }
    sectionDensity /= lanesets.size();

    if (carsInTheSection==0)
      sectionAvgSpeed=0.0;
    else
      sectionAvgSpeed /= carsInTheSection;

    // print everything
    cout.setf(ios::showpoint);
    cout.setf(ios::fixed,ios::floatfield);
    cout.precision(2);

    cout << sectionDensity << "\t";
    cout << sectionAvgSpeed << "\t";

  }
  cout << endl;
  return cout.str();
}




/************************************************************
  Method: string Network::stoppedCarsInLaneSetSensor(int iteration)
  Author: Bruno Castro da Silva
  Description: prints  number of stopped cars in laneset and to total
       number of cars in this laneset

  Changelog:
************************************************************/

string Network::stoppedCarsInLaneSetSensor(int iteration) {

  ostringstream cout;
  laneset_vector_t::iterator laneset;
  laneset_vector_t childLaneSets = getChildLaneSets ();

  cout << iteration << "\t";

  for (laneset = childLaneSets.begin(); laneset != childLaneSets.end(); laneset++) {

    int stoppedCarsInTheLaneSet=0;
    int totalCars=0;

    // get the lanes' info
    lane_vector_t childLanes = (*laneset)->getChildLanes ();
    lane_vector_t::iterator lane;

    for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {
      for (int j = 0; j <  (*lane)->getLength() ; j++) {
	topology_element_id_t carId = (*lane)->queryCarId(j);
	if (carId!=-1) {
	  totalCars++;
	  if ((*lane)->queryCarSpeed(j) == 0)
	    stoppedCarsInTheLaneSet++;
	}
      }
    }
    // print everything
    cout << totalCars << "\t" << stoppedCarsInTheLaneSet << "\t";
  }
  cout << endl;
  return cout.str();
}





/************************************************************
  Method: string Network::visualStoppedCarsInSectionSensor(int iteration)
  Author: Bruno Castro da Silva
  Description: prints the relative number of stopped cars in the section

  Changelog:
************************************************************/

string Network::visualStoppedCarsInSectionSensor(int iteration) {

  ostringstream cout;
  street_sections_t::iterator section;
  section_vector_t childSections = getChildSections ();

  cout << iteration << "\t";

  for (section = childSections.begin(); section != childSections.end(); section++) {

    int stoppedCarsInTheSection=0;
    int totalCells=0;
    double densityOfStoppedCars;

    laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
    laneSet_vector_t::iterator laneset;


    // for each laneset
    for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {

      // get the lanes' info
      lane_vector_t childLanes = (*laneset)->getChildLanes ();
      lane_vector_t::iterator lane;

      for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {
	totalCells += (*lane)->getLength();
	for (int j = 0; j <  (*lane)->getLength() ; j++) {
	  topology_element_id_t carId = (*lane)->queryCarId(j);
	  if ((carId!=-1) && ((*lane)->queryCarSpeed(j) == 0))
	    stoppedCarsInTheSection++;
	}
      }
    }

    if (stoppedCarsInTheSection==0)
      densityOfStoppedCars=0.0;
    else
      densityOfStoppedCars = (stoppedCarsInTheSection*1.0) / (totalCells*1.0);

    // print everything
    cout.setf(ios::showpoint);
    cout.setf(ios::fixed,ios::floatfield);
    cout.precision(2);

//     cout << densityOfStoppedCars << "(" << (*section)->getId() << "->" << totalCells << ")" << "\t";
    cout << densityOfStoppedCars << "\t";


  }

  cout << endl;
  return cout.str();
}






/************************************************************
  Method: string Network::totalStoppedCarsSensor(int iteration)
  Author: Bruno Castro da Silva
  Description: prints the total number of stopped cars in the network

  Changelog:
************************************************************/

string Network::totalStoppedCarsSensor(int iteration) {

  ostringstream cout;
  street_sections_t::iterator section;
  section_vector_t childSections = getChildSections ();

  cout << iteration << "\t";

  int stoppedCars=0;

  for (section = childSections.begin(); section != childSections.end(); section++) {

    laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
    laneSet_vector_t::iterator laneset;


    // for each laneset
    for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {


      // get the lanes' info
      lane_vector_t childLanes = (*laneset)->getChildLanes ();
      lane_vector_t::iterator lane;

      for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {
	for (int j = 0; j <  (*lane)->getLength() ; j++) {
	  topology_element_id_t carId = (*lane)->queryCarId(j);
	  if ((carId!=-1) && ((*lane)->queryCarSpeed(j) == 0))
	    stoppedCars++;
	}
      }
    }
  }
  // print everything
  cout << stoppedCars << endl;
  return cout.str();
}




/************************************************************
  Method: string Network::totalStoppedCarsSensorWithoutSources(int iteration)
  Author: Denise de Oliveira
  Description: prints the total number of stopped cars in the network
               ignoring the lanes conected to sources.
  Changelog:
************************************************************/

string Network::totalStoppedCarsSensorWithoutSources(int iteration) {

  ostringstream cout;
  street_sections_t::iterator section;
  section_vector_t childSections = getChildSections ();

  cout << iteration << "\t";

  int stoppedCars=0;
  bool hasSource = false;

  for (section = childSections.begin(); section != childSections.end(); section++) {

		laneSet_vector_t lanesets = (*section) -> getChildLaneSets();
		laneSet_vector_t::iterator laneset;

		// for each laneset
		for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {
			hasSource = false;
			for (unsigned int k=0; k < (*laneset)->getStartNode()->calibrators.size(); k++)
				if (((*laneset)->getStartNode()->calibrators[k] -> getType()) == SOURCE) {
					hasSource=true;
				}

			if (hasSource)
				continue; //go to the next laneset!

			// get the lanes' info
			lane_vector_t childLanes = (*laneset)->getChildLanes();
			lane_vector_t::iterator lane;

			for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {
				for (int j = 0; j < (*lane)->getLength() ; j++) {
					topology_element_id_t carId = (*lane)->queryCarId(j);
					if ((carId!=-1) && ((*lane)->queryCarSpeed(j) == 0))
						stoppedCars++;
				}
			}
		}
	}
  // print everything
  cout << stoppedCars << endl;
  return cout.str();
}


/************************************************************
  Method: string Network::visualSectionDensitiesSensor(int iteration)
  Author: Bruno Castro da Silva
  Description: sensor for visualization of section densities

  Changelog:
************************************************************/


string Network::visualSectionDensitiesSensor(int iteration)
{
  ostringstream cout;
  // prints the section's densities
  street_sections_t::iterator section;
  section_vector_t childSections = getChildSections ();

  cout << iteration << "\t";

  for (section = childSections.begin(); section != childSections.end(); section++) {

    double sectionDensity=0;

    laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
    laneSet_vector_t::iterator laneset;


    // for each laneset
    for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {

      // get the lanes' info
      lane_vector_t childLanes = (*laneset)->getChildLanes ();
      lane_vector_t::iterator lane;

      double laneSetDensity = 0;
      for (lane=childLanes.begin(); lane != childLanes.end(); lane++)
	laneSetDensity += (*lane) -> getDensity();

      laneSetDensity /= childLanes.size();
      sectionDensity += laneSetDensity;

    }

    sectionDensity /= lanesets.size();

    // print everything
    //cout.setf(ios::showpoint);
    //cout.setf(ios::fixed,ios::floatfield);
    //cout.precision(2);

    cout << sectionDensity << "\t";

  }
  cout << endl;
  return cout.str();
}




/************************************************************
  Method: string Network::visualMicroscopic(int iteration)
  Author: Bruno Castro da Silva
  Description: sensor for microscopic visualization

  Changelog:
************************************************************/


string Network::visualMicroscopic(int iteration)
{
  ostringstream cout;

  cout << iteration << endl;

  cout << active_cars.size() << endl;
  for (active_cars_vector_t::iterator i = active_cars.begin(); i != active_cars.end(); i++) {
    cout << (*i)->getInternalId() << " " << (*i)->getCurrentLane()->getId() << " " << (*i)->getCurrentCell();
    cout << " " << (*i)->getCurrentLane()->getLength()  << endl;
    cout << (*i)->getDriver()->driver_class_name << endl;
  }

  // prints the traffic lights status (open directions)
  node_vector_t childNodes = getChildNodes ();
  node_vector_t::iterator c;
  trafficLight_phase_t *openDirections;
  trafficLight_phase_t::iterator direction;

  cout << numberTrafficLights << endl;

  for (c=childNodes.begin(); c != childNodes.end(); c++) {
    TrafficLight *tlight = (*c)->getTrafficLight();

    if (tlight == NULL)
      continue;

    // currentPlanIndex = tlight->getPlanIndex(tlight->getCurrentPlan());
    //currentPhaseIndex = tlight->getPlanPhaseIndex(currentPlanIndex, tlight->getCurrentPhase());


    cout << tlight->getId() << " " << (*c)->getId() << " " << tlight->getCurrentPlan() << " " << tlight->getCurrentPlanPhase() << " ";
    cout << tlight->getPlanPhaseLength(tlight->getCurrentPlan(), tlight->getCurrentPlanPhase()) << " ";
    cout << tlight -> numberOpenDirectionsCurrentPhase() << endl;
    openDirections = tlight -> getOpenDirectionsCurrentPhase();

    for (direction = openDirections->begin(); direction != openDirections->end(); direction++) {
      cout << direction->first->getId() << " " << direction->second->getId() << endl;
    }
  }


  return cout.str();
}




/************************************************************
  Method: string Network::visualLaneSetDensitiesSensor(int iteration)
  Author: Bruno Castro da Silva
  Description: sensor for visualization of lanesets' densities

  Changelog:
************************************************************/


string Network::visualLaneSetDensitiesSensor(int iteration) {
  ostringstream cout;

  // prints the section's densities
  street_sections_t::iterator section;
  section_vector_t childSections = getChildSections ();

  cout << iteration << "\t";

  for (section = childSections.begin(); section != childSections.end(); section++) {

    laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
    laneSet_vector_t::iterator laneset;

    // for each laneset
    for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {

      // get the lanes' info
      lane_vector_t childLanes = (*laneset)->getChildLanes ();
      lane_vector_t::iterator lane;

      double laneSetDensity = 0;
      for (lane=childLanes.begin(); lane != childLanes.end(); lane++)
	laneSetDensity += (*lane) -> getDensity();

      laneSetDensity /= childLanes.size();
      // and print the laneset density
      cout << laneSetDensity << "\t";

    }

  }

  cout << endl;

  // prints the traffic lights status (open directions)
  node_vector_t childNodes = getChildNodes ();
  node_vector_t::iterator c;
  trafficLight_phase_t *openDirections;
  trafficLight_phase_t::iterator direction;

  cout << numberTrafficLights << endl;

  for (c=childNodes.begin(); c != childNodes.end(); c++) {
    TrafficLight *tlight = (*c)->getTrafficLight();
    if (tlight == NULL)
      continue;

    cout << tlight->getId() << " " << (*c)->getId() << " " << tlight->getCurrentPlan() << " " << tlight->getCurrentPhase() << " ";
    cout << tlight->getPlanPhaseLength( tlight->getCurrentPlan(), tlight->getCurrentPhase()) << " ";
    cout << tlight -> numberOpenDirectionsCurrentPhase() << endl;
    openDirections = tlight -> getOpenDirectionsCurrentPhase();

    for (direction = openDirections->begin(); direction != openDirections->end(); direction++) {
      cout << direction->first->getId() << " " << direction->second->getId() << endl;
    }
  }


  return cout.str();


}





/************************************************************
  Method: string Network::visualSectionRelativeDensitiesSensor
  Author: Bruno Castro da Silva
  Description: prints the output which is going to be used by the visualizator (?)
         (relative densities for each section, ie, max. density=1 if there is a semaphore,
         1/v_max otherwise)

  Changelog:
************************************************************/

string Network::visualSectionRelativeDensitiesSensor(int iteration) {

  ostringstream cout;
  // prints the section's densities
  street_sections_t::iterator section;
  section_vector_t childSections = getChildSections ();

  cout << iteration << "\t";

  for (section = childSections.begin(); section != childSections.end(); section++) {

    double sectionDensity=0;

    laneSet_vector_t lanesets = (*section) -> getChildLaneSets ();
    laneSet_vector_t::iterator laneset;


    // for each laneset
    for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {

      // get the lanes' info
      lane_vector_t childLanes = (*laneset)->getChildLanes ();
      lane_vector_t::iterator lane;

      double laneSetDensity = 0;
      for (lane=childLanes.begin(); lane != childLanes.end(); lane++) {

	double laneDensity = (*lane) -> getDensity();
	// MEGA magic correction factor tabajara (tm)
	laneDensity *= (*lane)->getMaxSpeed();
	if (laneDensity > 1)
	  laneDensity = 1;
	laneSetDensity += laneDensity;
      }

      laneSetDensity /= childLanes.size();
      sectionDensity += laneSetDensity;

    }

    sectionDensity /= lanesets.size();

    // print everything
    cout.setf(ios::showpoint);
    cout.setf(ios::fixed,ios::floatfield);
    cout.precision(2);


    if (sectionDensity > 1) {
      cout << "ERRRRRRRRRRRRRRRRRO!!!!!!!!!!!" << endl;
      cout << sectionDensity << endl;
      //      abort();
      sectionDensity = 1;
    }

    cout << sectionDensity << "\t";

  }
  cout << endl;
  return cout.str();
}






/************************************************************
  Method:
  Author: Bruno Castro da Silva
  Description:

  Changelog:
************************************************************/

string Network::laneSetDensitiesSensor(int iteration) {

  ostringstream cout;
  double laneSetDensity=0;

  laneSet_vector_t lanesets = getChildLaneSets ();
  laneSet_vector_t::iterator laneset;


  cout << iteration << "\t";

  // for each laneset
  for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++) {

    // get the lanes' info
    lane_vector_t childLanes = (*laneset)->getChildLanes ();
    lane_vector_t::iterator lane;

    laneSetDensity = 0;
    for (lane=childLanes.begin(); lane != childLanes.end(); lane++)
      laneSetDensity += (*lane) -> getDensity();
    laneSetDensity /= childLanes.size();

	(*laneset)->vecDensity.push_back(laneSetDensity);

    // print everything
    cout.setf(ios::showpoint);
    cout.setf(ios::fixed,ios::floatfield);
    cout.precision(2);
    cout.width(4);

    cout <<  laneSetDensity << "\t";
  }

  cout << endl;
  return cout.str();
}




/************************************************************
   Function: void Network::addLaneSet (LaneSet *_ls)
   Author: Bruno C. da Silva
   Description: mantains a vector with all lanesets
        belonging to the current network
   Changelog:
************************************************************/

void Network::addLaneSet (LaneSet *_ls) {
  childLaneSets.push_back (_ls);

}



/************************************************************
   Function: void Network::addLane (Lane* _l)
   Author: Bruno C. da Silva
   Description: mantains a vector with all lanes
        belonging to the current network
   Changelog:
************************************************************/

void Network::addLane (Lane* _l) {
  childLanes.push_back (_l);

}



/************************************************************
  Method: void Network::addSection (Section *);
  Author: John Kliff Jochens
  Date: June 3rd, 2003
  Description: Adds a section to the childSections vector.

  Changelog:
12/6/2003, John:
Changed the type of the iteration index from int to unsigned int to avoid compilation warnings

************************************************************/

void Network::addSection (Section* _s) {

  childSections.push_back (_s);

}

/************************************************************
  Method: void Network::removeSection (Section *);
  Author: John Kliff Jochens
  Date: June 3rd, 2003
  Description: removes a section from the childSections vector.

  Changelog:
************************************************************/

void Network::removeSection (Section *) {

}

/************************************************************
  Method: bool Network::containsSection (Section * _s);
  Author: John Kliff Jochens
  Date: June 3rd, 2003
  Description: If the section _s is contained by the vector
childSections returns true. Otherwise, false.

  Changelog:
12/6/2003, John:
Changed the type of the iteration index from int to unsigned int to avoid compilation warnings

************************************************************/

bool Network::containsSection (Section* _s) {

  for (unsigned int i = 0 ; i < childSections.size () ; i++)
    if (childSections[i] == _s)
      return true;

  return false;
}

/************************************************************
  Method: section_vector_t Network::getChildSections ();
  Author: John Kliff Jochens
  Date: June 3rd, 2003
  Description: Returns a copy of the childSections vector.

  Changelog:
************************************************************/

section_vector_t Network::getChildSections () {
  return section_vector_t (childSections);
}

/************************************************************
  Method: void Network::addNode (Node *);
  Author: John Kliff Jochens
  Date: June 3rd, 2003
  Description: Adds a node to the childNodes vector.

  Changelog:
12/6/2003, John:
Changed the type of the iteration index from int to unsigned int to avoid compilation warnings

************************************************************/

void Network::addNode (Node * _n) {
  childNodes.push_back (_n);

}

/************************************************************
  Method: void Network::removeNode (Node *);
  Author: John Kliff Jochens
  Date: June 3rd, 2003
  Description: Removes a node from the childNodes vector.

  Changelog:
************************************************************/

void Network::removeNode (Node *) {

}

/************************************************************
  Method: bool Network::containsNode (Node* _n);
  Author: John Kliff Jochens
  Date: June 3rd, 2003
  Description: If the node _n is contained by the
childNodes vector, returns true. Otherwise false.

  Changelog:
12/6/2003, John:
Changed the type of the iteration index from int to unsigned int to avoid compilation warnings

************************************************************/

bool Network::containsNode (Node* _n) {
  for (unsigned int i = 0 ; i < childNodes.size () ; i++)
    if (childNodes[i] == _n)
      return true;

  return false;
}



/************************************************************
  Method: section_vector_t Network::getChildNodes ();
  Author: John Kliff Jochens
  Date: June 3rd, 2003
  Description: Returns a copy of the childNodes vector.

  Changelog:
************************************************************/

node_vector_t Network::getChildNodes () {
  return node_vector_t (childNodes);
}

/************************************************************
  Method: Topology * Network::getParentTopology ();
  Author: John Kliff Jochens
  Date: June 3rd, 2003
  Description: Returns a pointer to the parentTopology Topology.

  Changelog:
************************************************************/

Topology * Network::getParentTopology () {
  return parentTopology;
}

/************************************************************
  Method: constructor Network::Network (Topology *);
  Author: John Kliff Jochens
  Date: June 2nd, 2003
  Description: Default constructor
Sets the parentTopology for this instance and inserts this instance
into the parentTopology's networks vector.

  Changelog:
************************************************************/

Network::Network (Topology* _t) : TopologyElement () {
  parentTopology = _t;
  parentTopology -> addNetwork (this);
  numberTrafficLights = 0;
}

/************************************************************
  Method: destructor Network::~Network ();
  Author: John Kliff Jochens
  Date: June 2nd, 2003
  Description: Default destructor.

  Changelog:
************************************************************/

Network::~Network () {
}

int Network::getNumberOfStreets(){

	return streets.size();

}


/************************************************************
  Method: copObjects Network::createCopObjects(tlight_agents_t agts, int alg)
  Author: Robert Junges
  Description:

  Changelog:
************************************************************/
copObjects Network::createCopObjects(tlight_agents_t agts, int alg)
{
 copObjects ans;
 tlight_agents_t::iterator agt;

 //for each agent
 for (agt = agts.begin(); agt != agts.end(); agt++)
 {
  node_vector_t::iterator node;
  node_vector_t::iterator innode;
  node_vector_t childNodes = (*agt).second;

  //for each agent's traffic lights (as nodes)
  for (node = childNodes.begin(); node != childNodes.end(); node++)
  {
	// get their information regarding dcop relations
   	copObject temp = getCopNodeInfo(*node, alg);
   	ans.push_back(temp);
  }

 }
  return ans;

}


/************************************************************
  Method: int generateDCOP(tlight_agents_t agts, dcopparams params)
  Author: Robert Junges
  Description: generates a DCOP file

  Changelog:
************************************************************/
int Network::generateDCOP(tlight_agents_t agts, dcopparams params)
//1=modi_adopt; 2=dpop; 3=optapo;
{
 //constraints
 copConstraints cc;

 double OP = params.gammasynch;
 double OM = params.gammaunsynch;

 double alfa = params.alpha;

 vector <double> mask;

 mask.clear();

 ofstream *file = NULL;

 if ((params.alg == 1)) //ADOPT
 {
	file = new ofstream("../trafficAgents/dcop/common/adopt.txt");
 }
 else if (params.alg == 2) //DPOP
 {
	system ("pwd");
	file = new ofstream("../trafficAgents/dcop/common/dpop.txt");
	*file << agts.size() << "\t" << agts.size() << endl;
 }
 else if (params.alg == 3) //OptAPO
 {
	file = new ofstream("../trafficAgents/dcop/common/optapo.txt");
 }

 copObjects base = createCopObjects(agts, params.alg);
 copObjects::iterator itCop1;
 copObjects::iterator itCop2;

 for (itCop1 = base.begin(); itCop1 != base.end(); itCop1++)
 {
  for (itCop2 = base.begin(); itCop2 != base.end(); itCop2++)
  {
	mask.clear();

	if (params.alg == 2) { //dpop maximizes F, the others minimize
		OP = params.gammaunsynch;
		OM = params.gammasynch;
	}


	if (params.alg == 2) {

	if ((*itCop1).priority == (*itCop2).priority)
	{
		if ((*itCop1).priority == 1)
		{
			mask.push_back(params.nn);//2
			mask.push_back(params.na);//1
			mask.push_back(params.an);//1.5
			mask.push_back(params.aa);//0
		}
		else
		{
			mask.push_back(params.aa);//0
			mask.push_back(params.an);//1.5
			mask.push_back(params.na);//1
			mask.push_back(params.nn);//2
		}
	}
	else
	{
		if ((*itCop1).priority == 1)
		{
			mask.push_back(params.na);//1
			mask.push_back(params.nn);//2
			mask.push_back(params.aa);//0
			mask.push_back(params.an);//1.5
		}
		else
		{
			mask.push_back(params.an);//1.5
			mask.push_back(params.aa);//0
			mask.push_back(params.nn);//2
			mask.push_back(params.na);//1
		}
	}
	} else {
	if ((*itCop1).priority == (*itCop2).priority)
	{
		if ((*itCop1).priority == 1)
		{
			mask.push_back(params.aa);//0
			mask.push_back(params.an);//1.5
			mask.push_back(params.na);//1
			mask.push_back(params.nn);//2
		}
		else
		{
			mask.push_back(params.nn);//2
			mask.push_back(params.na);//1
			mask.push_back(params.an);//1.5
			mask.push_back(params.aa);//0
		}
	}
	else
	{
		if ((*itCop1).priority == 1)
		{
			mask.push_back(params.an);//1.5
			mask.push_back(params.aa);//0
			mask.push_back(params.nn);//2
			mask.push_back(params.na);//1
		}
		else
		{
			mask.push_back(params.na);//1
			mask.push_back(params.nn);//2
			mask.push_back(params.aa);//0
			mask.push_back(params.an);//1.5
		}
	}
	}

   if ( ((*itCop1).lo.first == (*itCop2).node) || ((*itCop1).ns.first == (*itCop2).node) )
   {
       //create constraints
       cc.push_back(make_pair(make_pair(make_pair((*itCop1).node->getTrafficLight()->getId(),1),make_pair((*itCop2).node->getTrafficLight()->getId(),1)), (*itCop1).lo.second*mask[0]*OP*alfa ));
       cc.push_back(make_pair(make_pair(make_pair((*itCop1).node->getTrafficLight()->getId(),1),make_pair((*itCop2).node->getTrafficLight()->getId(),2)), (*itCop1).lo.second*mask[1]*OM*alfa ));
       cc.push_back(make_pair(make_pair(make_pair((*itCop1).node->getTrafficLight()->getId(),2),make_pair((*itCop2).node->getTrafficLight()->getId(),1)), (*itCop1).lo.second*mask[2]*OM*alfa ));
       cc.push_back(make_pair(make_pair(make_pair((*itCop1).node->getTrafficLight()->getId(),2),make_pair((*itCop2).node->getTrafficLight()->getId(),2)), (*itCop1).lo.second*mask[3]*OP*alfa ));
   }

  }

  // write agents and variables to log
  if ( (params.alg == 1) || (params.alg == 3) )
  {
	*file << "AGENT " << (*itCop1).node->getTrafficLight()->getId()-900 << endl;
	*file << "VARIABLE " << (*itCop1).node->getTrafficLight()->getId()-901 << " "
	<< (*itCop1).node->getTrafficLight()->getId()-900 << " 2" << endl;
  }
  else if (params.alg == 2)
  {
	*file << "X" << (*itCop1).node->getTrafficLight()->getId()-901 << ".0(2) 1 2" << endl;
  }

 }

 copConstraints::iterator itCons;

 // write constraints to file
 int count = 0;
 int from = 0;
 int to = 0;
 for (itCons = cc.begin(); itCons != cc.end(); itCons++)
 {
	if ( (params.alg == 1) || (params.alg == 3) )
	{
		if ( ((((*itCons).first).first).first != to) || ((((*itCons).first).second).first != from) )
		{
			to = (((*itCons).first).first).first;
			from = (((*itCons).first).second).first;

			*file << "CONSTRAINT " << to-901 << " " << from-901 << endl;

			count++;
		}
		*file << "F " << (((*itCons).first).first).second-1 << " " << (((*itCons).first).second).second-1 << " " << (int)((*itCons).second*10) << endl;
	}
 	else if (params.alg == 2)
	{
		if ( ((((*itCons).first).first).first != to) || ((((*itCons).first).second).first != from) )
		{
			if (count > 0)
				*file << endl;
			to = (((*itCons).first).first).first;
			from = (((*itCons).first).second).first;

			*file << to-901 << "\t" << to-901 << ".0@-@" << from-901 << ".0" << flush; //dpop

			count++;
		}
		*file << "\t" << (((*itCons).first).first).second << "," << (((*itCons).first).second).second << ":" << (int)((*itCons).second*10) << flush; //dpop
	}
 }

 file->close();
 return 1;
}


/************************************************************
  Method: vector<pair<Node*, double>> Network::getIncomingNodesInfo(Node *mainNode)
  Author: Robert Junges
  Description: get info about incoming lanes for DCOP

  Changelog:
************************************************************/
copObject Network::getCopNodeInfo(Node *mainNode, int alg)
{
 copObject ans;

 ans.node = mainNode;

 node_vector_t::iterator node;
 node_vector_t nodes = getChildNodes ();

 double totalDensity = 0;

 //for each possible node
 for (node = nodes.begin(); node != nodes.end(); node++)
 {
  section_vector_t sections = (*node) -> getSections();
  section_vector_t::iterator section;

  //search through its objects
  for (section = sections.begin(); section != sections.end(); section++)
  {
   laneSet_vector_t lanesets = (*section) -> getChildLaneSets();
   laneSet_vector_t::iterator laneset;
   for (laneset = lanesets.begin(); laneset != lanesets.end(); laneset++)
   {

    //to find out if they are connected with (*mainNode)
    if ( ((*laneset) -> getEndNode() == mainNode) && (*node != mainNode) )
    {
       // get the lanes' info
       lane_vector_t childLanes = (*laneset)->getChildLanes ();
       lane_vector_t::iterator lane;

       double localDensity = 0;

       //then calculate the avg density for each incoming (to *node) laneset
       for (lane=childLanes.begin(); lane != childLanes.end(); lane++)
       {
	localDensity += (*lane)->getDensity();
       }
       localDensity = localDensity / childLanes.size();

       totalDensity += localDensity;

       if ((*node)->getXcoord() == mainNode->getXcoord())
          ans.lo = make_pair(*node, localDensity);
       else
          ans.ns = make_pair(*node, localDensity);

    }
   }
  }
  }

	if (totalDensity > 0)
	{
		ans.lo.second = ans.lo.second / totalDensity;
  		ans.ns.second = ans.ns.second / totalDensity;
	}
	else
	{
		ans.lo.second = 0;
  		ans.ns.second = 0;
	}



  if (ans.lo.second > ans.ns.second)
       ans.priority = 1;
  else if (ans.lo.second < ans.ns.second)
       ans.priority = 2;

  if (alg != 2) {
  	ans.lo.second = 1 - ans.lo.second;
  	ans.ns.second = 1 - ans.ns.second;
  }

  return ans;
}

/*
 * network static map to plot inside MATLAB
 */
void Network::drawMap() {

	laneset_vector_t lst = getChildLaneSets();
	laneset_vector_t::iterator itlst;

	for (itlst = lst.begin(); itlst!= lst.end(); itlst++) {

		cout << "line([" << (*itlst)->getStartNode()->getXcoord() << " " << (*itlst)->getEndNode()->getXcoord() << "]," <<
						"[" << (*itlst)->getStartNode()->getYcoord() << " " << (*itlst)->getEndNode()->getYcoord() << "])" << endl;

		cout << "text(" << (*itlst)->getStartNode()->getXcoord() << "," << (*itlst)->getStartNode()->getYcoord() << ",'" << (*itlst)->getStartNode()->getId() << "')" << endl;
	}
}
