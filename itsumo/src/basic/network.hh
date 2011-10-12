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
#ifndef __HAS_NETWORK_H
#define __HAS_NETWORK_H

#include <sstream>
#include "types.hh"

#include "topologyelement.hh"
#include "topology.hh"
#include "utils.hh"

#include "mysql/mysql.h" // MySQL Include File

class Network : public TopologyElement {
private:

  // the network must know its nodes and sections
  node_vector_t childNodes;
  section_vector_t childSections;

  string name;

  // the Topology that holds this network
  Topology* parentTopology;

  lane_vector_t childLanes;
  laneset_vector_t childLaneSets;
  street_vector_t streets;

public:

  string getName();
  void setName(string _name);

  // SENSORS
  string controlledLanesSensorSummary(int iteration);
  string laneSetDensitiesSensor(int iteration);
  string laneSensor(int iteration);
  string laneSensorSpeedUp(int iteration);
  string laneSetSensor(int iteration);
  string laneSetSensorWithoutBorderNodes(int iteration);
  string sectionSensor(int iteration);
  string visualMicroscopic(int iteration);
  string visualLaneSetDensitiesSensor(int iteration);
  string visualSectionDensitiesSensor(int iteration);
  string visualSectionRelativeDensitiesSensor(int iteration);
  string visualStoppedCarsInSectionSensor(int iteration);
  string stoppedCarsInLaneSetSensor(int iteration);
  string totalStoppedCarsSensor(int iteration);
  string totalStoppedCarsSensorWithoutSources(int iteration);
  string printTopologyForVisualization();

  // changelog (Bruno 26-sept-2003): added two methods for associating a section to a street
  // and a street to a network
  void registerSection(Section* _section);
  void registerStreet(topology_element_id_t _streetId, string name);

  string getStreetNameById(int);

  int numberTrafficLights;

  void printStreets();
  void XMLOutput();

  void addLane (Lane *);
  void addLaneSet (LaneSet *);

  void addSection (Section *);
  void removeSection (Section *);
  bool containsSection (Section *);
  section_vector_t getChildSections ();

  void addNode (Node *);
  void removeNode (Node *);
  bool containsNode (Node *);
  node_vector_t getChildNodes ();

  Node* getNodeByTL(string id);
  Node* getNodeById(string id);
  LaneSet* getLanesetById(int laneset_id);

  node_vector_t tlNodeList();
  string tlightAgentSatFlowData(node_vector_t nodes, int iteration);
  string tlightAgentData(node_vector_t nodes, int iteration);
  string tlightConfiguration(node_vector_t nodes);

  void process_tlight_update(string message);

  lane_vector_t getChildLanes();
  laneset_vector_t getChildLaneSets();
  int getNumberOfStreets();
  int getCapacity();

  void setParentTopology ();
  Topology* getParentTopology ();

  // DCOP log
  copObjects createCopObjects(tlight_agents_t agts, int alg);
  int generateDCOP(tlight_agents_t agts, dcopparams params);
  copObject getCopNodeInfo(Node *mainNode, int alg);

  void makeMap(MYSQL *connect, int odsize, string networkname, string round, int replan, int comm, int view); // mapa de relevo
  void drawMap(); // desenha o mapa

  /*
    Constructors and Destructors
  */
  Network (Topology *);
  ~Network ();
};


#endif
