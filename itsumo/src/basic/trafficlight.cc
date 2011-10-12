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
#include "trafficlight.hh"

#include <iostream>


using std::cout;
using std::endl;




// changelog (Bruno): added these 2 methods in order to
// mantain the plan and phase list as private data, but
// at the same time allow other parts of the simulator
// to read from it (example: Network::XMLoutput())
trafficLight_plan_vector_t *TrafficLight::getTrafficLightPlanList()
{
  return &trafficLightPlanList;
}

trafficLight_phase_vector_t *TrafficLight::getTrafficLightPhaseList()
{
  return &trafficLightPhaseList;
}


/************************************************************
	Function: trafficLight_phase_t *getOpenDirectionsCurrentPhase()
	Author: Bruno Silva
	Date: 2003-07-28
	Description: returns the list of open directions for the current phase
	Changelog:
************************************************************/
trafficLight_phase_t *TrafficLight::getOpenDirectionsCurrentPhase()
{
  return &(trafficLightPhaseList[getCurrentPhase()]);
}



/************************************************************
	Function: trafficLight_phase_t *TrafficLight::numberOpenDirectionsCurrentPhase()
	Author: Bruno Silva
	Date: 2003-07-28
	Description: returns the number of open directions for the current phase
	Changelog:
************************************************************/
int TrafficLight::numberOpenDirectionsCurrentPhase()
{
  return trafficLightPhaseList[getCurrentPhase()].size();
}





/************************************************************
	Function: Node *TrafficLight::getParentNode ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-28
	Description: returns parent node
	Changelog:
************************************************************/
Node *TrafficLight::getParentNode () {
  return parentNode;
}

/************************************************************
	Function: void TrafficLight::appendPlan (iteration_t,planId);
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-14
	Description: appends a new plan and sets it as selected
	Changelog: planid included (Denise e Paulo 27/02/04)
************************************************************/
int TrafficLight::appendPlan (iteration_t iter, int planId, string additional_info) {
  trafficLight_plan_t newPlan;
  newPlan.startIter = iter;
  newPlan.planIndex = planId;
  newPlan.additional_info = additional_info;
  int i = trafficLightPlanList.size();
  trafficLightPlanList.push_back(newPlan);
  return i;
}

/************************************************************
	Function: void TrafficLight::setPlanStart (int, iteration_t);
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-14
	Description: sets the initial iteration for this plan
	Changelog:
************************************************************/
void TrafficLight::setPlanStart (int plan, iteration_t iter) {
  if (plan >= static_cast<int>(trafficLightPlanList.size()))
    throw NONEXISTANT_PLAN;
  trafficLightPlanList[plan].startIter = iter;
}

/************************************************************
	Function: iteration_t TrafficLight::getPlanStart (int);
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-14
	Description: gets the initial iteration for this plan
	Changelog:
************************************************************/
iteration_t TrafficLight::getPlanStart (int plan) {
  if (plan >= static_cast<int>(trafficLightPlanList.size()))
    throw NONEXISTANT_PLAN;
  return trafficLightPlanList[plan].startIter;
}

/************************************************************
	Function: iteration_t TrafficLight::getPlanStart (int);
	Author: Denise e Paulo
	Date: 2004 - 02 - 25
	Description: gets the plan index
	Changelog:
************************************************************/
int  TrafficLight::getPlanIndex (int plan) {
	
  for (int i=0; i<static_cast<int>(trafficLightPlanList.size()); i++) {
    if (plan == trafficLightPlanList[i].planIndex) {     
      return i;
    }
  }
  return -1;
}

/************************************************************
	Function: void TrafficLight::appendPlanPhase (int, iteration_t, iteration_t, int);
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-03
	Description: appends a new phase for that plan and returns its index
	Changelog:
************************************************************/
void TrafficLight::appendPlanPhase (int plan, iteration_t start, iteration_t end, int phase) {
  if (plan >= static_cast<int>(trafficLightPlanList.size()))
    throw NONEXISTANT_PLAN;
  if (phase >= static_cast<int>(trafficLightPhaseList.size()))
    throw NONEXISTANT_PHASE;
  trafficLight_plan_phase_t newPhase;
  newPhase.startIter = start;
  newPhase.endIter = end;
  newPhase.iterationInterval = (end>start)?(end-start+1):(start-end+1);
  newPhase.phaseIndex = phase;
  trafficLightPlanList[plan].phases.push_back(newPhase);
}

/************************************************************
Function: iteration_t TrafficLight::getPlanPhaseStart (int, int);
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-03
	Description: returns the starting iteration for that plan/phase
	Changelog:
************************************************************/
iteration_t TrafficLight::getPlanPhaseStart (int plan, int phase) {
  if (plan >= static_cast<int>(trafficLightPlanList.size()))
    throw NONEXISTANT_PLAN;
  if (phase >= static_cast<int>(trafficLightPlanList[plan].phases.size()))
    throw NONEXISTANT_PLAN_PHASE;
  return trafficLightPlanList[plan].phases[phase].startIter;
}

/************************************************************
Function: iteration_t TrafficLight::getPlanPhaseEnd (int, int);
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-03
	Description: returns the ending iteration for that plan/phase
	Changelog:
************************************************************/
iteration_t TrafficLight::getPlanPhaseEnd (int plan, int phase) {
  if (plan >= static_cast<int>(trafficLightPlanList.size()))
    throw NONEXISTANT_PLAN;
  if (phase >= static_cast<int>(trafficLightPlanList[plan].phases.size()))
    throw NONEXISTANT_PLAN_PHASE;
  return trafficLightPlanList[plan].phases[phase].endIter;
}

/************************************************************
Function: iteration_t TrafficLight::getPlanPhaseLength (int, int);
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-03
	Description: returns the length in iterations for that plan/phase
	Changelog:
************************************************************/
iteration_t TrafficLight::getPlanPhaseLength (int plan, int phase) {
	
  if (plan >= static_cast<int>(trafficLightPlanList.size())) {
	cout << "plan: " << plan << " phase: " << phase << " plano nao existente" << endl;
    throw NONEXISTANT_PLAN;
  }
  if (phase >= static_cast<int>(trafficLightPlanList[plan].phases.size())) {
	cout << "plan: " << plan << " phase: " << phase << " size: " << static_cast<int>(trafficLightPlanList[plan].phases.size());	
    throw NONEXISTANT_PLAN_PHASE;
  }
  return trafficLightPlanList[plan].phases[phase].iterationInterval;

}

/************************************************************
Function: int TrafficLight::getPlanPhaseIndex (int, int);
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-03
	Description: returns the plan index for that plan/phase
	Changelog: 
************************************************************/
int TrafficLight::getPlanPhaseIndex (int plan, int phase) {
  
  if (plan >= static_cast<int>(trafficLightPlanList.size()))
    throw NONEXISTANT_PLAN;
  if (phase >= static_cast<int>(trafficLightPlanList[plan].phases.size()))
    throw NONEXISTANT_PLAN_PHASE;
  return trafficLightPlanList[plan].phases[phase].phaseIndex;
}

/************************************************************
	Function: void TrafficLight::unsetDefaultPlan ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-14
	Description: unsets the default plan
	Changelog:
************************************************************/
void TrafficLight::unsetDefaultPlan () {
  defaultPlanDefined = false;
}

/************************************************************
	Function: void TrafficLight::setDefaultPlan (int);
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-14
	Description: sets the default plan
	Changelog:
************************************************************/
void TrafficLight::setDefaultPlan (int plan) {
  defaultPlanDefined = true;
  defaultPlanIndex = plan;
}

/************************************************************
	Function: void TrafficLight::setAgent(int agent);
	Author: Denise de Oliveira
	Date: 2003-11-30
	Description: sets the ID of the controlling agent, if there's 
		no agent the value is NOAGENT (-1).
	Changelog:
************************************************************/
void TrafficLight::setAgent(int agent) {	
  agentID = agent;
}


/************************************************************
	Function: bool TrafficLight::checkDefaultPlan ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-14
	Description: returns whether the default plan is set
	Changelog:
************************************************************/
bool TrafficLight::checkDefaultPlan () {
  return defaultPlanDefined;
}

/************************************************************
	Function: int TrafficLight::getDefaultPlan ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-14
	Description: returns the default plan
	Changelog:
************************************************************/
int TrafficLight::getDefaultPlan () {
  if (defaultPlanDefined)
    return defaultPlanIndex;
  else
    throw NONEXISTANT_PLAN;
}

/************************************************************
	Function: int TrafficLight::appendPhase ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-14
	Description: creates a new phase and returns its index
	Changelog:
************************************************************/
int TrafficLight::appendPhase () {
  trafficLight_phase_t newPhase;
  int i = trafficLightPhaseList.size();
  trafficLightPhaseList.push_back(newPhase);
  return i;
}

/************************************************************
	Function: void TrafficLight::addDirection (int, trafficLight_direction_t);
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-23
	Description: adds a new open direction for selected phase
	Changelog:
************************************************************/
void TrafficLight::addDirection (int phase, trafficLight_direction_t direction) {
  if (phase >= static_cast<int>(trafficLightPhaseList.size()))
    throw NONEXISTANT_PHASE;

  // changelog (bruno): mudei de push_front pra push_back, pra poder comparar
  // os xmls gerados a partir do banco e o gerado a partir da leitura de outro
  // xml.. espero que isso nao estrague os semaforos!
  trafficLightPhaseList[phase].push_back(direction);
}

/************************************************************
	Function: bool TrafficLight::checkDirection (int, trafficLight_direction_t);
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-23
	Description: checks if a direction is open for a certain phase
	Changelog:
************************************************************/
bool TrafficLight::checkDirection (int phase, trafficLight_direction_t direction) {

  if (phase >= static_cast<int>(trafficLightPhaseList.size()))
    throw NONEXISTANT_PHASE;
  trafficLight_phase_t::iterator it, itEnd;
  itEnd = trafficLightPhaseList[phase].end();
 

  
  // apparently xml implements a direction as lane->laneset, but omar
  // did laneset->laneset (probably to solve unpredicted jams caused
  // when the user forgets to add some lane->laneset direction to the phase)
  for (it = trafficLightPhaseList[phase].begin(); it != itEnd; it++) {
    if ((it->second == direction.second) &&
	(it->first->getParentLaneSet()->getId() == 
	 (direction.first)->getParentLaneSet()->getId()))
      return true;
  }
  return false;
}

/************************************************************
	Function: void TrafficLight::addDirection (int, Lane*, LaneSet*);
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-23
	Description: adds a new open direction for selected phase
	Changelog: bruno (16-10-2004): now the direction is (lane->laneset),
           so I changed the last parameters of the method
************************************************************/
void TrafficLight::addDirection (int phase, Lane *from, LaneSet *to) {
  trafficLight_direction_t dir(from, to);
  addDirection (phase, dir);
}

/************************************************************
	Function: bool TrafficLight::checkDirection (int, Lane*, LaneSet*);
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-23
	Description: checks if a direction for a certain phase, is open
	Changelog: bruno (16-10-2004): now the direction is (lane->laneset),
           so I changed the last parameters of the method
************************************************************/
bool TrafficLight::checkDirection (int phase, Lane *from, LaneSet *to) {
  trafficLight_direction_t dir(from, to);
  return checkDirection (phase, dir);
}


void TrafficLight::setCurrentPlan (int plan) 
{
	int np = getNumberOfPlans();

	for (int i = 0; i < np; i++)
	{
		if (trafficLightPlanList[i].planIndex == plan)
			currentPlan = i;
	}
}


/************************************************************
	Function: int TrafficLight::getCurrentPlan ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-25
	Description: gets the current plan
	Changelog:
************************************************************/
int TrafficLight::getCurrentPlan () {
  return currentPlan;
}

/************************************************************
	Function: int TrafficLight::getCurrentPhase ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-25
	Description: gets the current phase
	Changelog:
************************************************************/
int TrafficLight::getCurrentPhase () {
  return currentPhase;
}

int TrafficLight::getCurrentPlanPhase () {
	return currentPlanPhase;
}

/************************************************************
	Function: int TrafficLight::getNumberOfPlans ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-25
	Description: gets the number of plans
	Changelog:
************************************************************/
int TrafficLight::getNumberOfPlans () {
  return trafficLightPlanList.size();
}

/************************************************************
	Function: int TrafficLight::getNumberOfPhases ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-25
	Description: gets the number of phases in the current plan
	Changelog:
************************************************************/
int TrafficLight::getNumberOfPhases () {
  return trafficLightPhaseList.size();
}

/************************************************************
	Function: int TrafficLight::getNumberOfPlanPhases ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-03
	Description: gets the number of phases in a certain plan
	Changelog:
************************************************************/
int TrafficLight::getNumberOfPlanPhases () {
  return trafficLightPlanList[currentPlan].phases.size();
}

/************************************************************
	Function: int TrafficLight::getNumberOfPlanPhases (int);
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-03
	Description: gets the number of phases in a certain plan
	Changelog:
************************************************************/
int TrafficLight::getNumberOfPlanPhases (int plan) {
  if (plan >= static_cast<int>(trafficLightPlanList.size()))
    throw NONEXISTANT_PLAN;
  return trafficLightPlanList[plan].phases.size();
}

/************************************************************
	Function: bool TrafficLight::allowCarMove (Lane*, LaneSet*);
	Author: Omar "Gamb� Balbuena
	Date: 2003-07-28
	Description: checks if car can move from lane to lane
	Changelog: bruno (16-10-2004): now the direction is (lane->laneset),
           so I changed the last parameters of the method
************************************************************/
bool TrafficLight::allowCarMove (Lane *from, LaneSet *to) {
  trafficLight_direction_t dir(from, to);
  return checkDirection(currentPhase, dir);
}

/************************************************************
	Function: void TrafficLight::begin ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-04
	Description:
		construction done. begin simulation.
		also, the smallest function in the simulation! :)
	Changelog:
************************************************************/
void TrafficLight::begin () {

  // Bruno and Denise: the default plan is the plan with the
  // smaller id

    int min_id = 32767;
    int min_index = 0;
    int i;
    for (i=0; i<static_cast<int>(trafficLightPlanList.size()); i++) {
      if (trafficLightPlanList[i].planIndex < min_id) {
	min_id = trafficLightPlanList[i].planIndex;
	min_index = i;
      }
    }
    currentPlan = min_index;


  updatePlan();
}

/************************************************************
	Function: void TrafficLight::update ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-04
	Description:
		the method you've been looking for :)
		here's the actual simulation. the semaphore goes to its
		next state, which usually means "subtract one" in a very
		norma-machine fashion. wow. traffic lights are stupid.
	Changelog:
************************************************************/
void TrafficLight::update () {


  if (--beforeNextPlanPhase > 0) {
    // this means the traffic light must remain as-is
    // for some more time.
  } 
  else {
    // next phase! oooh! exciting!
    if (++currentPlanPhase < getNumberOfPlanPhases()) {
      // can move alright.
      commitPlanPhase();
    } 
    else {
      // oh no! i ran out of phases!
      updatePlan();
    }
  }  
  
  
   
}

/************************************************************
	Function: TrafficLight::TrafficLight (Node *);
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-04
	Description: constructor
	Changelog:
************************************************************/

TrafficLight::TrafficLight (Node *parent) :
  // superclass construction
  TopologyElement (),
  // default values
  currentPlan(0),
  currentPhase(0),
  currentPlanPhase(0),
  defaultPlanDefined(false),
  defaultPlanIndex(0),
  beforeNextPlanPhase(0),
  agentID(NOAGENT),
  parentNode(parent)//,

  // gets a copy of the DB connection handle
  //connection(_connection)
  
{

  parent->setTrafficLight(this);
}

/************************************************************
	Function: TrafficLight::~TrafficLight ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-04
	Description: destructor
	Changelog:
************************************************************/
TrafficLight::~TrafficLight () {
  parentNode->setTrafficLight(NULL);
}

/************************************************************
	Function: void TrafficLight::updatePlan ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-04
	Description:
		this won't really change the state of the traffic light
		per se, but just "discover" what is the next state
	Changelog: Denise 30/11 
************************************************************/
void TrafficLight::updatePlan () {

//  else {
//     // (1.b) check if there is a plan for that time of the day
//     // (1.c) get default plan
//     int i, bestMatch = defaultPlanDefined? defaultPlanIndex: 0;
//     iteration_t iVal, bestMatchVal = 0, time = 	parentNode->getParentNetwork()->getParentTopology()->getTimeInDay();
//     for (i=0; i<static_cast<int>(trafficLightPlanList.size()); i++) {
//       iVal = trafficLightPlanList[i].startIter;
//       if ((iVal < time) && (iVal > bestMatchVal)) {
// 	bestMatch = i;
// 	bestMatchVal = iVal;
//       }
//     }
//     currentPlan = bestMatch;
//   }


  // (2) apply plan
  currentPlanPhase = 0;
  commitPlanPhase();

}

/************************************************************
	Function: void TrafficLight::commitPlanPhase ();
	Author: Omar "Gamb� Balbuena
	Date: 2003-09-04
	Description:
		plan and phase have been selected, so phase must
		be set accordingly.
	Changelog:
************************************************************/
void TrafficLight::commitPlanPhase () {
  int phase = getPlanPhaseIndex (currentPlan, currentPlanPhase);
  if (phase >= static_cast<int>(trafficLightPhaseList.size()))
    throw NONEXISTANT_PHASE;
  currentPhase = phase;
  beforeNextPlanPhase = getPlanPhaseLength (currentPlan, currentPlanPhase);

}

//------------------------------------------------------------
/**
 * Returns the cycle time for a given plan id. The cycle 
 * time is calculated by the um of phases duration.
 * 
 * Author: Denise de Oliveira
 */
int TrafficLight::getCycleTimeFromPlan(int planId){	
	int cycle = 0;	
	int phasesEnd = trafficLightPlanList[planId].phases.size();
	for (int phaseIndex = 0; phaseIndex < phasesEnd ; phaseIndex++){		
		cycle += (int)trafficLightPlanList[planId].phases[phaseIndex].iterationInterval;		        
	}
	return cycle;
}

//------------------------------------------------------------
/**
 * Returns the split time (greed duration, portion of the cycle time) 
 * for a given lane id and the plan id.
 * 
 * Author: Denise de Oliveira
 */
int TrafficLight::getSplitTimeForLaneInPlan(int laneId, int planId){	
	int split = 0;
	int planIndex = planId;
	int phIndex =0;
	
	trafficLight_phase_t::iterator it, itEnd;
	
	int phasesEnd = trafficLightPlanList[planId].phases.size();
	//for each fase of the plan
	for (int phaseIndex = 0; phaseIndex < phasesEnd ; phaseIndex++){		
		phIndex = phaseIndex;
		// (int)trafficLightPlanList[planId].phases[phaseIndex].phaseIndex;				        
		itEnd = trafficLightPhaseList[phIndex].end();		
		//for each pair of directions of the phase    	
		for (it = trafficLightPhaseList[phIndex].begin(); it != itEnd; it++) {
			//if the open direction is the lane
    		if (it->first->getId() == laneId){ 		
    			//cout<<" ->tl Id: "<<getId()<<" ->planId: "<<planId<<" ->phaseIndex: "<<phIndex<<endl; 								    		
	    		split += (int)getPlanPhaseLength(planIndex, phIndex);
	    		break;//go to the next phase!	    					    			    			    				    	    			 
    		}
		}
	}
	return split;	
}

