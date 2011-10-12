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
#ifndef __TRAFFIC_LIGHTS_H
#define __TRAFFIC_LIGHTS_H

#include "types.hh"

#include "topologyelement.hh"
#include "node.hh"


// FIXME! Weird, weird exceptions...
#define NONEXISTANT_PLAN 12
#define NONEXISTANT_PLAN_PHASE 13
#define NONEXISTANT_PHASE 14

//if there's no agent controlling the TL
#define NOAGENT -1



class TrafficLight : public TopologyElement {
private:
	// TrafficLight is a list of signal plans
	// Each signal plan is a list of phases
	// Each phase is a struct
	// start-iteration, end-iteration, directions-set


	  trafficLight_plan_vector_t trafficLightPlanList;

	// TrafficLight also contains a list of phases
	// which are selected by plans separatedly
    	trafficLight_phase_vector_t trafficLightPhaseList;


	// default plan properties
	bool defaultPlanDefined;
	int defaultPlanIndex;

	// state control properties
	iteration_t beforeNextPlanPhase;

	// indicates the controlling agent ID
	int agentID;

	// node this traffic light belongs to
	Node *parentNode;
	

public:
  // current state properties
  int currentPlan;
  int currentPhase;
  int currentPlanPhase;  

  trafficLight_plan_vector_t *getTrafficLightPlanList();
  trafficLight_phase_vector_t *getTrafficLightPhaseList();


  // returns the list of open directions for the current phase
  trafficLight_phase_t *getOpenDirectionsCurrentPhase();

  // returns the number of open directions for the current phase
  int numberOpenDirectionsCurrentPhase();

	// node is read-only
	Node *getParentNode ();

	// plan management
	int appendPlan (iteration_t,int,string);
    int getPlanIndex (int);
	void setPlanStart (int, iteration_t);
	iteration_t getPlanStart (int);
	// plan phase management
	void appendPlanPhase (int, iteration_t, iteration_t, int);
	iteration_t getPlanPhaseStart (int, int);
	iteration_t getPlanPhaseEnd (int, int);
	iteration_t getPlanPhaseLength (int, int);
	int getPlanPhaseIndex (int, int);

	// default plan control
	void unsetDefaultPlan ();
	void setDefaultPlan (int);
	bool checkDefaultPlan ();
	int getDefaultPlan ();

	// plan phase management
	int appendPhase ();
	// lane pair management (changes affect current phase)
	void addDirection (int, trafficLight_direction_t);
	bool checkDirection (int, trafficLight_direction_t);
	// same thing, different taste
	void addDirection (int, Lane *, LaneSet *);
	bool checkDirection (int, Lane *, LaneSet *);


	void setAgent(int);
	
	// plan and phase control
	void setCurrentPlan (int);
	int getCurrentPlan ();
	int getCurrentPhase ();
	int getCurrentPlanPhase();
	int getNumberOfPlans ();
	int getNumberOfPhases ();
	int getNumberOfPlanPhases ();
	int getNumberOfPlanPhases (int);
	
	//new methods (08/2008)
	int getCycleTimeFromPlan(int planIndex);
	int getSplitTimeForLaneInPlan(int laneId, int planId);

	// allow car movement based on current state
	bool allowCarMove (Lane *, LaneSet *);

	// indicate that the semaphore is totally constructed,
	// thus, set current (initial) states accordingly
	void begin ();

	// update phase status if applicable
	// and check plan status too
	void update ();

	// constructors and destructors
	TrafficLight (Node *); //, PGconn *_connection);
	// a traffic light will only make sense with a node,
	// even though it nearly never uses one.
	~TrafficLight ();

	// it will reset most state variables, choose a plan,
	// and start it from the beginning. it sould be normally be
	// called when a cycle of a plan ends. 

	void commitPlanPhase ();


protected:
	// this won't change the state, but it will check by
	// all available means what plan should take into
	// action right now.
	void updatePlan ();

};

#endif
