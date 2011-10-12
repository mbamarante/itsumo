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
#include "../tiny/tinyxml.h"
#include "parse_network.hh"

#include <string>

#define OBLIGATORY 0
#define OPTIONAL 1

// TrafficLight, Street, Section, LaneSet, Lane, Node

#define CHECK(element, mode) (check(element, #element, mode))
#define VALUE(element) (element->FirstChild()->Value())

bool check(TiXmlElement *element_to_test, string element_name, int condition)
{
  if (element_to_test==0) {
    cout << "Tag '" << element_name << "' expected but not found!" << endl;
    cout << "Fatal error! Leaving." << endl;
    exit(1);
  }
    
  if (element_to_test->FirstChild()==0) {
    if (condition==OBLIGATORY) {
      cout << "Tag '" << element_name << "' must have a value!" << endl;
      cout << "Fatal error! Leaving." << endl;
      exit(1);
    } 
    else
      return false;
  }
  else {
    return true;
  }
}




void parse_turning_probabilities(TiXmlElement *streets, Network *n)
{
  
  TiXmlElement *street;
  TiXmlElement *sections, *section;
  TiXmlElement *lanesets, *laneset, *laneset_id;
  TiXmlElement *turning_probabilities;
  TiXmlElement *direction;
  TiXmlElement *destination_laneset;
  TiXmlElement *probability;
  
  for(street = streets->FirstChildElement("street"); street; street = street->NextSiblingElement("street")) {
    sections = street->FirstChildElement("sections");

    for(section = sections->FirstChildElement("section"); section; section = section->NextSiblingElement("section")) {
      lanesets = section->FirstChildElement("lanesets");
      
      for(laneset = lanesets->FirstChildElement("laneset"); laneset; laneset = laneset->NextSiblingElement("laneset")) {
	
	turning_probabilities = laneset->FirstChildElement("turning_probabilities");
	if (CHECK(turning_probabilities, OPTIONAL)) {
	  laneset_id = laneset->FirstChildElement("laneset_id");
	  CHECK(laneset_id, OBLIGATORY);
	
	  LaneSet *lanesetObject = static_cast<LaneSet *>(n -> getElementById(atoi(VALUE(laneset_id))));
	  assert(lanesetObject);
	

	  for(direction = turning_probabilities->FirstChildElement("direction"); 
	      direction; direction = direction->NextSiblingElement("direction")) {
    
	    destination_laneset = direction->FirstChildElement("destination_laneset");
	    //if (CHECK(destination_laneset, OBLIGATORY))
	    LaneSet *lanesetTo = static_cast<LaneSet *> (n -> getElementById(atoi(VALUE(destination_laneset))));
	  
	    probability = direction->FirstChildElement("probability");
	    //if (CHECK(probability, OBLIGATORY))
	    double prob = atof(VALUE(probability));

	    lanesetObject -> addTurningProb (lanesetTo, prob);    
	  }
	}
	else {
	  //       exit(1);
	}
      }
    }
  }
}



void parse_lanes(TiXmlElement *lanes, Network *n, LaneSet *parentLaneset)
{

  TiXmlElement *lane;
  TiXmlElement *lane_id;
  TiXmlElement *lane_position;
  TiXmlElement *maximum_speed;
  TiXmlElement *deceleration_prob;


  for(lane = lanes->FirstChildElement("lane"); 
      lane; lane = lane->NextSiblingElement("lane")) {

    lane_id = lane->FirstChildElement("lane_id");
    //if (CHECK(lane_id, OBLIGATORY))

    lane_position = lane->FirstChildElement("lane_position");
    //if (CHECK(lane_position, OBLIGATORY))

    maximum_speed = lane->FirstChildElement("maximum_speed");
    //if (CHECK(maximum_speed, OBLIGATORY))

    deceleration_prob = lane->FirstChildElement("deceleration_prob");
    //if (CHECK(deceleration_prob, OBLIGATORY))

    Lane *newLane = new Lane (parentLaneset);
    newLane -> setId (atoi(VALUE(lane_id)));
    newLane -> setMaxSpeed(atoi(VALUE(maximum_speed)));
    newLane -> setPosition(atoi(VALUE(lane_position)));

    newLane -> setDecelerationProbability(atof(VALUE(deceleration_prob)));

    n -> addLane(newLane);

  }
}





void parse_lanesets(TiXmlElement *lanesets, Network *n, Section *parentSection)
{

  TiXmlElement *laneset;
  TiXmlElement *laneset_id;
  TiXmlElement *laneset_position;
  TiXmlElement *start_node, *end_node;
  TiXmlElement *lanes;

  for(laneset = lanesets->FirstChildElement("laneset"); laneset; laneset = laneset->NextSiblingElement("laneset")) {
    laneset_id = laneset->FirstChildElement("laneset_id");
    //if (CHECK(laneset_id, OBLIGATORY))
    
    laneset_position = laneset->FirstChildElement("laneset_position");
    //if (CHECK(laneset_position, OBLIGATORY))
    
    start_node = laneset->FirstChildElement("start_node");
    //if (CHECK(start_node, OBLIGATORY))
    //  if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\t\t\tstart_node " << VALUE(start_node) << endl;
    
    end_node = laneset->FirstChildElement("end_node");
    //if (CHECK(end_node, OBLIGATORY))
	//cout << "\t\t\t\tend_node " << VALUE(end_node) << endl;
    
    
    Node *startNodeObject = static_cast<Node *>(n -> getElementById(atoi(VALUE(start_node))));
    Node *endNodeObject = static_cast<Node *>(n -> getElementById(atoi(VALUE(end_node))));
    Node *n1, *n2;
    parentSection -> getDelimitingNodes (&n1, &n2);
    
    LaneSet *newLaneSet = new LaneSet (parentSection);
    newLaneSet -> setId(atoi(VALUE(laneset_id)));
    newLaneSet -> setPosition(atoi(VALUE(laneset_position)));
    
    if (! newLaneSet -> setStartNode (startNodeObject)) {
      cout << "Error in parse_lanesets (could not set starting node #" << startNodeObject -> getId();
      cout << " to laneset  #" <<  newLaneSet -> getId() << " in section " << parentSection->getId() << endl;
    }
    if ( ! newLaneSet -> setEndNode (endNodeObject)) {
      cout << "Error in parse_lanesets (could not set ending node #" << startNodeObject -> getId();
      cout << " to laneset  #" <<  newLaneSet -> getId() << " in section " << parentSection->getId() << endl;
    }

    n -> addLaneSet(newLaneSet);

    lanes = laneset->FirstChildElement("lanes");
    if (CHECK(lanes, OBLIGATORY)) {
      //if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\t\t\tlanes" << endl;
      parse_lanes(lanes, n, newLaneSet);
    }


    // sort child lanes
    newLaneSet->sortChildLanes();

  
  }

}




void parse_sections(TiXmlElement *sections, Network *n, topology_element_id_t parentStreetId)
{



  TiXmlElement *section;
  TiXmlElement *section_id;
  TiXmlElement *section_name;
  TiXmlElement *is_preferencial;
  TiXmlElement *delimiting_node_1, *delimiting_node_2;
  TiXmlElement *lanesets;

  for(section = sections->FirstChildElement("section"); section; section = section->NextSiblingElement("section")) {

    section_id = section->FirstChildElement("section_id");
    //if (CHECK(section_id, OBLIGATORY))
    //  if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\t\tsection_id " << VALUE(section_id) << endl;

    section_name = section->FirstChildElement("section_name");
    //if (CHECK(section_name, OBLIGATORY))
      //if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\t\tsection_name " << VALUE(section_name) << endl;

    is_preferencial = section->FirstChildElement("is_preferencial");
    //if (CHECK(is_preferencial, OBLIGATORY))
      //if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\t\tis_preferencial " << VALUE(is_preferencial) << endl;

    delimiting_node_1 = section->FirstChildElement("delimiting_node");
    //if (CHECK(delimiting_node_1, OBLIGATORY))
      //if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\t\tdelimiting_node_1 " << VALUE(delimiting_node_1) << endl;

    delimiting_node_2 = delimiting_node_1->NextSiblingElement("delimiting_node");
    //if (CHECK(delimiting_node_2, OBLIGATORY))
      //if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\t\tdelimiting_node_2 " << VALUE(delimiting_node_2) << endl;


    Node *startNodeObject = static_cast<Node *>(n -> getElementById(atoi(VALUE(delimiting_node_1))));
    Node *endNodeObject = static_cast<Node *>(n -> getElementById(atoi(VALUE(delimiting_node_2))));
    double xStart = startNodeObject->getXcoord();
    double yStart = startNodeObject->getYcoord();
    double xEnd = endNodeObject->getXcoord();
    double yEnd = endNodeObject->getYcoord();

    cellSize_t cellSize= n -> getParentTopology () -> getCellSize();
    float tmp = sqrt(pow((yEnd - yStart), 2.0) + pow((xEnd - xStart), 2.0));
    tmp /= cellSize;
    dist_t sectionLength = static_cast<int>(round(tmp));
 
    Section *newSection = new Section (n, startNodeObject, endNodeObject);
    newSection -> setId(atoi(VALUE(section_id)));
    newSection -> setName(VALUE(section_name));
    newSection -> setStreetId(parentStreetId);
    newSection -> setLength(sectionLength);

    if (!strcmp(VALUE(is_preferencial), "true"))
      newSection -> setPreferencial(true);
    else
      newSection -> setPreferencial(false);

    lanesets = section->FirstChildElement("lanesets");
    if (CHECK(lanesets, OBLIGATORY)) {
      //if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\t\tlanesets" << endl;
      parse_lanesets(lanesets, n, newSection);
    }

    // sort child lanesets
    newSection->sortChildLaneSets();

  }
}



void parse_streets(TiXmlElement *streets, Network *n)
{

  TiXmlElement *street;
  TiXmlElement *street_id, *street_name;
  TiXmlElement *sections;

  for(street = streets->FirstChildElement("street"); street; street = street->NextSiblingElement("street")) {

    street_id = street->FirstChildElement("street_id");
    //if (CHECK(street_id, OBLIGATORY))
    //  if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\tstreet_id " << VALUE(street_id) << endl;
 
    street_name = street->FirstChildElement("street_name");
    //if (CHECK(street_name, OBLIGATORY))
      //if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\tstreet_name " << VALUE(street_name) << endl;

    n -> registerStreet(atoi(VALUE(street_id)), VALUE(street_name));
   
    sections = street->FirstChildElement("sections");
    if (CHECK(sections, OBLIGATORY)) {
      //if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\tsections" << endl;
      parse_sections(sections, n, atoi(VALUE(street_id)));
    }
  }
}




void parse_directions(TiXmlElement *directions, Network *n, TrafficLight *tlight, int phaseHandle)
{

  TiXmlElement *direction;
  TiXmlElement *from_lane, *to_laneset;

  for(direction = directions->FirstChildElement("direction"); direction; 
      direction = direction->NextSiblingElement("direction")) {

    from_lane = direction->FirstChildElement("from_lane");
    //if (CHECK(from_lane, OBLIGATORY))
      //if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\t\t\t\tfrom_lane " << VALUE(from_lane) << endl;

    to_laneset = direction->FirstChildElement("to_laneset");
    //if (CHECK(to_laneset, OBLIGATORY))
      //if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
	//cout << "\t\t\t\t\tto_laneset " << VALUE(to_laneset) << endl;

    Lane *laneFrom = static_cast<Lane *>(n -> getElementById(atoi(VALUE(from_lane))));
    LaneSet *lanesetTo = static_cast<LaneSet *>(n -> getElementById(atoi(VALUE(to_laneset))));

    //if (systemLog->checkAllowedTag("debug_show_topology_creation_from_xml"))
      //cout << "\t\t\t\t\t\tadicionando direcao " << laneFrom -> getId() << " -> " << lanesetTo->getId() << endl;
    tlight -> addDirection (phaseHandle, laneFrom, lanesetTo);

  }

}



void parse_phases(TiXmlElement *phases, Network *n, TrafficLight *tlight, int planHandle)
{

  TiXmlElement *phase;
  TiXmlElement *phase_id;
  TiXmlElement *iteration_start, *iteration_end;
  TiXmlElement *directions;

  for(phase = phases->FirstChildElement("phase"); phase; phase = phase->NextSiblingElement("phase")) {

    phase_id = phase->FirstChildElement("phase_id");

    iteration_start = phase->FirstChildElement("iteration_start");

    iteration_end = phase->FirstChildElement("iteration_end");

    int phaseHandle = tlight -> appendPhase();

    directions = phase->FirstChildElement("directions");
    if (CHECK(directions, OPTIONAL)) {
      parse_directions(directions, n, tlight, phaseHandle);
    }

    tlight -> appendPlanPhase(planHandle, atoi(VALUE(iteration_start)), atoi(VALUE(iteration_end)), phaseHandle);
    
  }
}



void parse_signalplans(TiXmlElement *signalplans, Network *n, TrafficLight *tlight)
{
  TiXmlElement *signalplan;
  TiXmlElement *signalplan_id;
  TiXmlElement *additional_info;
  TiXmlElement *phases;

  for(signalplan = signalplans->FirstChildElement("signalplan"); signalplan; 
      signalplan = signalplan->NextSiblingElement("signalplan")) {

    signalplan_id = signalplan->FirstChildElement("signalplan_id");

    additional_info = signalplan->FirstChildElement("additional_info");
    string str_additional_info;
    if CHECK(additional_info, OPTIONAL) {
      str_additional_info = VALUE(additional_info);
    } else {
      str_additional_info = "";
    }

    // TODO: wtf eh o 1o parametro?
    int planHandle = tlight -> appendPlan(0, atoi(VALUE(signalplan_id)), str_additional_info);
    
    phases = signalplan->FirstChildElement("phases");
    if (CHECK(phases, OBLIGATORY)) {
      parse_phases(phases, n, tlight, planHandle);
    }
  }

}


void parse_trafficlights(TiXmlElement *trafficlights, Network *n)
{

  TiXmlElement *trafficlight;
  TiXmlElement *traffic_light_id;
  TiXmlElement *located_at_node;
  TiXmlElement *signalplans;

  for(trafficlight = trafficlights->FirstChildElement("traffic_light"); trafficlight; 
      trafficlight = trafficlight->NextSiblingElement("traffic_light")) {

    traffic_light_id = trafficlight->FirstChildElement("traffic_light_id");

    located_at_node = trafficlight->FirstChildElement("located_at_node");


    // TODO: DB handle!
    TrafficLight *tlight;
    tlight = new TrafficLight((static_cast<Node *>(n->getElementById(atoi(VALUE(located_at_node))))));
    tlight -> setId(atoi(VALUE(traffic_light_id)));
    
    // TODO: agent!
    tlight -> setAgent(-1);

    signalplans = trafficlight->FirstChildElement("signalplans");
    if (CHECK(signalplans, OBLIGATORY)) {
      parse_signalplans(signalplans, n, tlight);
    }
    
    tlight -> begin();

  }
}

				    

void parse_sinks(TiXmlElement *sinks, Network *n)
{
  
  TiXmlElement *sink;
  TiXmlElement *sink_id;
  TiXmlElement *first_activation_at;
  TiXmlElement *located_at_node;
  TiXmlElement *source_laneset;
  TiXmlElement *removal_probability;


  for(sink = sinks->FirstChildElement("sink"); sink; sink = sink->NextSiblingElement("sink")) {

    sink_id = sink->FirstChildElement("sink_id");

    located_at_node = sink->FirstChildElement("located_at_node");

    source_laneset = sink->FirstChildElement("source_laneset");

    removal_probability = sink->FirstChildElement("removal_probability");

    first_activation_at = sink->FirstChildElement("first_activation_at");

    Sink *newSink = NULL;
    try {
      newSink = new Sink(atoi(VALUE(first_activation_at)), 
			 (static_cast<Node *>(n->getElementById(atoi(VALUE(located_at_node))))), 
			 (static_cast<LaneSet *>(n->getElementById(atoi(VALUE(source_laneset))))));
    } catch(std::exception &error) {
      std::cout << error.what() << std::endl;
    }

    newSink->setId(atoi(VALUE(sink_id)));
    newSink -> setActivationProbability(atof(VALUE(removal_probability))); 



  }

}



void parse_sources(TiXmlElement *sources, Network *n)
{
  
  TiXmlElement *source;
  TiXmlElement *source_id;
  TiXmlElement *located_at_node;
  TiXmlElement *first_activation_at;
  TiXmlElement *target_laneset;
  TiXmlElement *source_activation_frequency;
  TiXmlElement *source_type;
  int stype;

  for(source = sources->FirstChildElement("source"); source; source = source->NextSiblingElement("source")) {

    variable_probability_vector_t var_prob_table;
    variable_flow_vector_t var_flow_table;

    source_id = source->FirstChildElement("source_id");

    located_at_node = source->FirstChildElement("located_at_node");

    first_activation_at = source->FirstChildElement("first_activation_at");

    target_laneset = source->FirstChildElement("target_laneset");

    source_activation_frequency = source->FirstChildElement("source_activation_frequency");

    source_type = source->FirstChildElement("source_type");

    if (!strcmp(VALUE(source_type), "CONSTANT_PROB"))
      stype = CONSTANT_PROB;
    else if (!strcmp(VALUE(source_type), "VARIABLE_PROB"))
      stype = VARIABLE_PROB;
    else if (!strcmp(VALUE(source_type), "CONSTANT_FLOW"))
      stype = CONSTANT_FLOW;
    else if (!strcmp(VALUE(source_type), "VARIABLE_FLOW"))
      stype = VARIABLE_FLOW;
    else {
      cout << "  ERROR on parse_source. Source has an invalid type (" << VALUE(source_type) << ")" << endl;
      exit(1);
    }

    Node *tmpN = static_cast<Node *>(n->getElementById(atoi(VALUE(located_at_node))));
    LaneSet *tmpL =  static_cast<LaneSet *>(n->getElementById(atoi(VALUE(target_laneset))));

    Source *newSource = NULL;
    try {
      newSource = new Source(stype,
			     atoi(VALUE(first_activation_at)),
			     tmpN,
			     atoi(VALUE(source_activation_frequency)),
			     tmpL);
    } catch(std::exception &error) {
      std::cout << error.what() << std::endl;
    }

    newSource->setId(atoi(VALUE(source_id)));
     
    switch (stype) {

      //-------------------------------------------------------------------------
    case CONSTANT_PROB:
      TiXmlElement *constant_probability;
      constant_probability = source->FirstChildElement("probability");
      newSource -> setActivationProbability(atof(VALUE(constant_probability)));
      break;


      //-------------------------------------------------------------------------
    case VARIABLE_PROB:
      TiXmlElement *probs;
      TiXmlElement *var_prob;
      TiXmlElement *number_of_iterations_prob;
      TiXmlElement *variable_probability;

      probs = source->FirstChildElement("probabilities");
      
      if (CHECK(probs, OBLIGATORY)) {
	for(var_prob = probs->FirstChildElement("var_prob");
	    var_prob; var_prob = var_prob->NextSiblingElement("var_prob")) {
	  number_of_iterations_prob = var_prob->FirstChildElement("number_of_iterations");
	  variable_probability = var_prob->FirstChildElement("probability");

	  var_prob_table.push_back(make_pair(atoi(VALUE(number_of_iterations_prob)),
					     atof(VALUE(variable_probability))));
	  
	}
	newSource -> setVariableProb(var_prob_table);
      }
      
      break;


      //-------------------------------------------------------------------------
    case CONSTANT_FLOW:
      TiXmlElement *constant_flow;
      constant_flow = source->FirstChildElement("flow");
      newSource -> setConstantFlow(atoi(VALUE(constant_flow)));
      break;


      //-------------------------------------------------------------------------
    case VARIABLE_FLOW:

      TiXmlElement *flows;
      TiXmlElement *var_flow;
      TiXmlElement *number_of_iterations_flow;
      TiXmlElement *flow;

      flows = source->FirstChildElement("flows");

      if (CHECK(flows, OBLIGATORY)) {
	for(var_flow = flows->FirstChildElement("var_flow");
	    var_flow; var_flow = var_flow->NextSiblingElement("var_flow")) {
	  number_of_iterations_flow = var_flow->FirstChildElement("number_of_iterations");
	  flow = var_flow->FirstChildElement("flow");

	  var_flow_table.push_back(make_pair(atoi(VALUE(number_of_iterations_flow)),
					     atoi(VALUE(flow))));
	  
	}
	newSource -> setVariableFlow(var_flow_table);
      }

      break;
      //--------------------------------------------------------------------------
    }
    
  }

}




void parse_nodes(TiXmlElement *nodes, Network *n)
{

  TiXmlElement *node;
  TiXmlElement *node_id;
  TiXmlElement *node_name;
  TiXmlElement *x_coord;
  TiXmlElement *y_coord;

  for(node = nodes->FirstChildElement("node"); node; node = node->NextSiblingElement("node")) {

    node_id = node->FirstChildElement("node_id");

    node_name = node->FirstChildElement("node_name");
    x_coord = node->FirstChildElement("x_coord");

    y_coord = node->FirstChildElement("y_coord");

    Node *newNode = new Node (n, atof(VALUE(x_coord)), atof(VALUE(y_coord)));
    newNode -> setId(atoi(VALUE(node_id)));
    if (CHECK(node_name, OPTIONAL))
      newNode -> setName(VALUE(node_name));
  }

}


void parse_settings(TiXmlElement *settings, Topology *t)
{
  TiXmlElement *cell_size_in_meters;
  TiXmlElement *iteration_length_in_seconds;
  TiXmlElement *cars_maximum_speed;


  cell_size_in_meters = settings->FirstChildElement("cell_size_in_meters");

  iteration_length_in_seconds = settings->FirstChildElement("iteration_length_in_seconds");


  cars_maximum_speed = settings->FirstChildElement("cars_maximum_speed");


  t -> setId(0);
//   t -> setCellSize(atof(VALUE(cell_size_in_meters)));
//   t -> setIterationTime(atoi(VALUE(iteration_length_in_seconds)));
//   t -> setCarsMaximumSpeed(atoi(VALUE(cars_maximum_speed)));

}



void parse_simulation(TiXmlElement *simulation, Topology *t, Network *n)

{

  TiXmlElement *network_name, *network_id;
  TiXmlElement *nodes;
  TiXmlElement *sources;
  TiXmlElement *sinks;
  TiXmlElement *traffic_lights;
  TiXmlElement *streets;

  network_id = simulation->FirstChildElement("network_id");

  network_name = simulation->FirstChildElement("network_name");

  n -> setId(atoi(VALUE(network_id)));
  n -> setName(VALUE(network_name));

//   settings = simulation->FirstChildElement("settings");
//   if (CHECK(settings, OBLIGATORY)) {
//     parse_settings(settings, t); 
//   }

  nodes = simulation->FirstChildElement("nodes");
  if (CHECK(nodes, OBLIGATORY)) {
    parse_nodes(nodes, n);
  }

  streets = simulation->FirstChildElement("streets");
  if (CHECK(streets, OBLIGATORY)) {
    parse_streets(streets, n);
    parse_turning_probabilities(streets, n);
  }

  sources = simulation->FirstChildElement("sources");
  if (CHECK(sources, OPTIONAL)) {
    parse_sources(sources, n);
  }

  sinks = simulation->FirstChildElement("sinks");
  if (CHECK(sinks, OPTIONAL)) {
    parse_sinks(sinks, n);
  }

  traffic_lights = simulation->FirstChildElement("traffic_lights");
  if (CHECK(traffic_lights, OPTIONAL)) {
    parse_trafficlights(traffic_lights, n);
  }

}


void load_data_from_file(string file,Topology *t, Network *n)
{
  TiXmlElement *simulation;
  char homePath[512];
  string hP;
  
  getcwd(homePath, 512);
  hP = homePath;
  //hP.erase(hP.find_last_of("/"));
  hP.erase(hP.find("/src/basic"));
  //cout << hP << endl; exit(0);
  
  if (file.find(hP, 0) == string::npos && file.substr(0,1).find("/") == string::npos) //home path not found && first caracter "/" not found too
	  file = "../../" + file;
  cout << "abrir: " << file << endl;
  // 	TiXmlBase::SetCondenseWhiteSpace( false );

  
  TiXmlDocument doc( file.c_str() );
  bool loadOkay = doc.LoadFile();
  
  if ( !loadOkay )
    {
      printf( "parse_network.c - Could not load file %s. Error='%s'.\nExiting.\n", file.c_str(),
	      doc.ErrorDesc());
      exit(1);
    }

  
  simulation = doc.FirstChildElement( "simulation" );
  //   doc.Print( stdout );

  parse_simulation(simulation, t, n);

  cout << endl;
  cout << "\t== network statistics ==" << endl;
  cout << "\t#streets= " << n->getNumberOfStreets() << endl;
  cout << "\t#sections= " << n->getChildSections().size() << endl;
  cout << "\t#lanesets= " << n->getChildLaneSets().size() << endl;
  cout << "\t#nodes= " << n->getChildNodes().size() << endl;
  cout << "\t#TLs= " << n->numberTrafficLights << endl;

  section_vector_t childSections = n->getChildSections();
  section_vector_t::iterator section;
  double length_sc = 0;			// sections length in meters
  double length_ls = 0;			// lanesets length in meters
  double length_ln = 0;			// lanes length in meters

  for (section=childSections.begin();section != childSections.end(); section++){

	  // == euclidian distance ==
	  Node *n1, *n2;
	  (*section) -> getDelimitingNodes (&n1, &n2);

	  double dist_x = ((*n1).getXcoord() - (*n2).getXcoord());
	  double dist_y = ((*n1).getYcoord() - (*n2).getYcoord());

	  length_sc += sqrt(pow(dist_x, 2) + pow(dist_y, 2));	// length sections
	  length_ls += sqrt(pow(dist_x, 2) + pow(dist_y, 2)) * (*section)->getChildLaneSets().size();	// length lanesets

	  laneSet_vector_t childLaneSet = (*section)->getChildLaneSets();
	  laneSet_vector_t::iterator laneset;

	  int lanes=0;

	  for (laneset=childLaneSet.begin();laneset != childLaneSet.end(); laneset++){
		  lanes += (*laneset)->getNumberOfLanes();
	  }
	  length_ln += sqrt(pow(dist_x, 2) + pow(dist_y, 2)) * lanes;	// length lanesets
	  // ==
  }

  cout << "\t#supported cars (capacity)= " << n->getCapacity() << endl;
  cout << "\t#sections length (meters)= " << length_sc << endl;
  cout << "\t#laneset length (meters)= " << length_ls << endl;
  cout << "\t#lanes length (meters)= " << length_ln << endl;
  cout << endl;
}
