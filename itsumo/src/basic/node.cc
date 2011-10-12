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
#include "node.hh"


#include <iostream>  
using std::cout;
using std::endl;

using namespace std;

void Node::addCalibrator(Calibrator *_newCalib) {
  calibrators.push_back(_newCalib);

}



string Node::getName()
{
  return nodeName;
}


void Node::setName(string _name)
{
  nodeName = _name;
}



/************************************************************
   Function: void Node::signalToSinks()
   Author: Bruno C. da Silva 
   Description: this method should be called whenever
        a car is going through a node, in order to
        send a signal to the sinks that might exist in
        the node. Each sink then removes the car
        with a probability 'activationProbability'
   Changelog:
************************************************************/

int Node::signalToSinks(Car *car) {
  
  int removedCar=0;
  if (calibrators.size()==0) {
    //cerr << " -- KEPT (no sinks on the node!)  -> buffer's status " <<  Car::showBuffersStatus().c_str() << endl;
    return removedCar;
  }

  for (unsigned int i=0; ((i < calibrators.size()) && (!removedCar)); i++) {

    if ((calibrators[i] -> getType()) == SINK) {

      Sink *s = dynamic_cast < Sink * > (calibrators[i]);

      // set the flag if the car was removed
      removedCar = removedCar || (s -> act(car));
    }
  }
  return removedCar;
  
}



/************************************************************
   Function: void Node::update()
   Author: Bruno C. da Silva 
   Description: this function is responsable for updating
          the node's trafficlights (if any), and for updating all 
          calibrators and sensors

   Changelog:
************************************************************/

void Node::update(iteration_t currentIteration) {

  TrafficLight *tlight = getTrafficLight();
  
  currentStep = currentIteration;
  
  if (tlight != NULL) {
    tlight -> update();

//     cout << "tlight #" << tlight->getId() << " com plan " << tlight -> getCurrentPlan();
//     cout << " fase " << tlight -> getCurrentPhase() << " (tem " << tlight -> getNumberOfPlanPhases();
//     cout << " fases). Duracao da fase: ";
//     cout << tlight -> getPlanPhaseLength(tlight -> getCurrentPlan(), tlight -> getCurrentPhase()) << endl;

  }

  for (unsigned int i=0; i< calibrators.size(); i++) {
    if ((calibrators[i] -> getType()) == SOURCE) {
      Source *s = dynamic_cast < Source * > (calibrators[i]);
      s -> update(currentIteration);
    }
    else {
      Sink *s = dynamic_cast < Sink * > (calibrators[i]);
      s -> update(currentIteration);
    }
  }


//  CODIGO PARA IMPRIMIR TODOS PLANOS E TODAS FASES DO SEMAFORO
//
//     trafficLight_plan_vector_t::iterator a;
//     for (a=(tlight->trafficLightPlanList).begin(); a != (tlight->trafficLightPlanList).end(); a++) {
//       cout << "Novo plano (start iter = " << a->startIter << ")" << endl;
      
//       trafficLight_plan_phase_vector_t::iterator b;
//       for (b=(a->phases).begin(); b!=(a->phases).end(); b++) {
// 	cout << "    Nova fase:" << endl;
// 	cout << "        startIter: " << b->startIter << endl;
// 	cout << "        endIter: " << b->endIter << endl;
// 	cout << "        iterationInterval: " << b->iterationInterval << endl;
// 	cout << "        phaseIndex: " << b->phaseIndex << endl;

//       }
//     }

//     trafficLight_phase_vector_t::iterator i;
//     int phaseCount=0;
//     for (i=(tlight->trafficLightPhaseList).begin(); i != (tlight->trafficLightPhaseList).end(); i++) {
//       cout << "Nova fase (phaseIndex = " << phaseCount++ << "):" << endl;

//       trafficLight_phase_t::iterator j;
//       for (j=i->begin(); j != i->end(); j++) {
// 	cout << "    Nova direcao:" << endl;
// 	cout << "        Par: " << (j->first)->getId() << ", " << (j->second)->getId() << endl;
//       }
//     }

 

}


/************************************************************
   Function: int Node::getNumberOfSections()
   Author: Bruno C. da Silva 
   Description: returns the number of sections connected to 
          this node

   Changelog:
************************************************************/
   
int Node::getNumberOfSections() {
  return sections.size();
}

/************************************************************
  Function: Network* Node::getParentNetwork ();
  Author: John Kliff Jochens
  Date: June 5th, 2003
  Description: Returns the parent Network.

  Changelog: 
************************************************************/
Network* Node::getParentNetwork () {
  return parentNetwork;
}

/************************************************************
  Function: Section* Node::getSections ()
  Author: John Kliff Jochens
  Date: Thu Jun 12 12:24:09 BRT 2003
  Description: Returns a copy of the vector of all the connected sections.

  Changelog: 
************************************************************/
section_vector_t Node::getSections () {
  return section_vector_t (sections);
}

string Node::getSectionsNames (string separator) {

	string rtrn = "";

	section_vector_t::iterator section;

	for (section=sections.begin();section!=sections.end();section++){

		rtrn += (*section)->getParentNetwork()->getStreetNameById( (*section)->getStreetId() )  + separator;

	}

	return rtrn;
}

/************************************************************
  Function: void addSection (Section*);
  Author: John Kliff Jochens
  Date: Thu Jun 12 12:28:23 BRT 2003
  Description: adds a connected section to this node.

  Changelog: 
************************************************************/
void Node::addSection (Section* _s) {
  sections.push_back (_s);
  
}

/************************************************************
  Function: void removeSection (Section *)
  Author: John Kliff Jochens
  Date: Thu Jun 12 12:29:10 BRT 2003
  Description: removes a connected section from this node.

  Changelog: 
************************************************************/
void Node::removeSection (Section* _s) {
  
}

/************************************************************
	Function: void Node::setTrafficLight (TrafficLight *)
	Author: Omar "Gamb?" Balbuena
	Date: 2003-09-05
	Description: sets traffic light pointer
	Changelog: 
************************************************************/
void Node::setTrafficLight (TrafficLight *tl) {
	trafficLight = tl;
	parentNetwork -> numberTrafficLights++;
}

/************************************************************
	Function: TrafficLight *Node::getTrafficLight ()
	Author: Omar "Gamb?" Balbuena
	Date: 2003-09-05
	Description: gets traffic light pointer
	Changelog: 
************************************************************/
TrafficLight *Node::getTrafficLight () {
	return trafficLight;
}

/************************************************************
	Function: bool Node::allowMove (Lane *, LaneSet *)
	Author: Omar "Gamb?" Balbuena
	Date: 2003-09-05
	Description: traffic light alias
	Changelog: bruno (16-10-2004): now the direction is (lane->laneset),
           so I changed the last parameters of the method
************************************************************/
bool Node::allowMove (Lane *from, LaneSet *to) {
  if (trafficLight == NULL)
    return true;

  else return trafficLight->allowCarMove(from, to);
}



float Node::getXcoord() {

  return coordinates.x;
}

float Node::getYcoord() {

  return coordinates.y;
}


//-----------------------------------------------------------
/**
 * Returns the current step (iteration)
 * of the simulation.
 * 
 * Author: Denise de Oliveira
 */

iteration_t Node::getCurrentStep(){
	return currentStep;
}



/************************************************************
  Function: constructor Node::Node ()
  Author: John Kliff Jochens
  Date: June 3rd, 2003
  Description: Default Constructor

  Changelog: 
    Omar 2003-09-05 added traffic light
    Bruno 2004-05-12 added support to the node's coordinates
************************************************************/
Node::Node (Network* _n, float xCoord, float yCoord) : TopologyElement () {
  parentNetwork = _n;
  parentNetwork -> addNode (this);
  
  coordinates.x = xCoord;
  coordinates.y = yCoord;
  
  trafficLight = NULL;
  
  currentStep = 0;
}

/************************************************************
  Function: destructor Node::~Node ();
  Author: John Kliff Jochens
  Date: June 3rd, 2003
  Description: Default destructor

  Changelog: 
************************************************************/
Node::~Node () {
}
