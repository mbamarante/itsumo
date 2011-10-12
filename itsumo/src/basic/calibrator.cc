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
#include "calibrator.hh"


#include <iostream>
using std::cout;
using std::endl;

#include <algorithm>


using namespace std;


#define SOURCE 222
#define SINK 666


/***********************************************************
  CALIBRATOR
 ************************************************************/





/************************************************************
Function: void Calibrator::setParentNode(Node *parentNode)
Author: Bruno C. da Silva 
Description: this function sets the node to which the 
calibrator belongs

Changelog:
 ************************************************************/

void Calibrator::setParentNode(Node *_parentNode) {
	parentNode = _parentNode;
	parentNode -> addCalibrator(this);

}




/************************************************************
Function: Node* Calibrator::getParentNode()
Author: Bruno C. da Silva 
Description: returns the node to which the 
calibrator belongs

Changelog:
 ************************************************************/

Node* Calibrator::getParentNode() {
	return parentNode;
}




/************************************************************
Function: void Calibrator::setAssociatedLaneSet(LaneSet *_assocLaneSet)
Author: Bruno C. da Silva 
Description: associates a laneset with the
calibrator. This is going to be the 
laneset where cars will be inserted
or removed, depending on the nature
of the calibrator (source or sink)

Changelog:
 ************************************************************/

void Calibrator::setAssociatedLaneSet(LaneSet *_assocLaneSet) {
	associatedLaneSet = _assocLaneSet;

}






/************************************************************
Function: LaneSet* Calibrator::getAssociatedLaneSet()
Author: Bruno C. da Silva 
Description: returns the laneset associated with
this calibrator

Changelog:
 ************************************************************/

LaneSet* Calibrator::getAssociatedLaneSet() {
	return associatedLaneSet;
}





/************************************************************
Function: void Calibrator::setTimeBegin(iteration_t _timeBegin)
Author: Bruno C. da Silva 
Description: this functions sets the moment (in iteration) 
when the calibrator is going to be started.
Before this moment, calls to the 'update' method
have no effect on the object.

Changelog:
 ************************************************************/

void Calibrator::setTimeBegin(iteration_t _timeBegin) {
	timeBegin = _timeBegin;
}




/************************************************************
Function: iteration_t Calibrator::getTimeBegin()
Author: Bruno C. da Silva 
Description: returns the timeBegin (moment, in iterations,
when the calibrator will be started).

Changelog:
 ************************************************************/

iteration_t Calibrator::getTimeBegin() {
	return timeBegin;
}




/************************************************************
Function: int Calibrator::getType()
Author: Bruno C. da Silva 
Description: returns the calibrator's type 
It can be either a SOURCE or a SINK 
(see the defines). The type is used
in order to identify the nature
of the calibrator's derived classes.

Changelog:
 ************************************************************/

int Calibrator::getType() {
	return type;
}




/************************************************************
Function: void Calibrator::setType(int _type)
Author: Bruno C. da Silva 
Description: sets the calibrator's type to
either a SOURCE or SINK.

Changelog:
 ************************************************************/

void Calibrator::setType(int _type) {
	type = _type;

}




/************************************************************
Function: double Calibrator::getActivationProbability()
Author: Bruno C. da Silva 
Description: returns this calibrator's activation probability,
which may be either the probability to
insert or to remove a car.
Changelog:
 ************************************************************/

double Calibrator::getActivationProbability() {
	return activationProbability;
}




/************************************************************
Function: void Calibrator::setActivationProbability(double _probability)
Author: Bruno C. da Silva 
Description: sets the activation probability to this calibrator;
_probability might be either the probability to
insert or to remove a car.
Changelog:
 ************************************************************/

void Calibrator::setActivationProbability(double _probability) {
	activationProbability = _probability;
}



/************************************************************
Function: Calibrator::Calibrator() : TopologyElement()
Author: Bruno C. da Silva 
Description: the constructor to the calibrator class.
Changelog:
 ************************************************************/

Calibrator::Calibrator (iteration_t _timeBegin, Node *parentNode, 
		LaneSet *_assocLaneSet) {
	running = 0;

	setParentNode(parentNode);
	setTimeBegin(_timeBegin);
	setAssociatedLaneSet(_assocLaneSet);

	//   setId(parentNode -> getId() * -1);


}





/************************************************************
Function: Calibrator::~Calibrator ()
Author: Bruno C. da Silva 
Description: the destructor to the calibrator class
Changelog:
 ************************************************************/

Calibrator::~Calibrator () {

}














/***********************************************************
  SOURCE
 ************************************************************/


/************************************************************
Function: dist_t Source::bestPlaceOnLane(Lane *lane)
Author: Bruno C. da Silva 
Description: returns the best place to insert the car in the
lane. This place is defined as the cell in the 
middle of the segment in the lane with the greatest 
adjacent sequence of empty cells
Changelog:
 ************************************************************/

dist_t Source::bestPlaceOnLane(Lane *lane)
{

	dist_t length = lane->getLength();
	Car *tmpCar;

	dist_t gapToFirstCar = lane->getFrontalGapFromCell(-1);

	// best place to put the car
	dist_t bestPlace;

	// maximum gap found and the currentGap
	dist_t maxGap, currentGap;
	// and the cell (position) from where the maxGap was measured
	dist_t positionMaxGap;

	// there is no car in the lane, so the best place is the middle cell
	if (gapToFirstCar >= length) {
		//    printf("   --> VAZIA!!! length=%d retorna=%d\n", length, (length/2)-1);
		if (length > 1)
			return (length / 2) - 1;
		else
			return length;

	}

	maxGap = gapToFirstCar;
	positionMaxGap = -1;

	for(int i=0; i<length; i++) {

		// get the car at current position
		tmpCar = lane->queryCarPtr(i);
		// if there is a car, we measure the gap from it
		if (tmpCar != NULL) {
			currentGap = lane->getFrontalGapFromCell(i);
			// checks if the value of the gap indicates that there is no 
			// car until the end of the street and adapts currentGap accordingly
			currentGap = (currentGap >= length ? length - i - 1 : currentGap);

			//       printf("   --> ACHOU um carro %d.. curgap=%d maxgap=%d\n", tmpCar->getInternalId(),
			// 	     currentGap, maxGap);

			// current gap is better than previous, so save it
			if (currentGap > maxGap) {
				positionMaxGap = i;
				maxGap = currentGap;
			}
		}
	}


	// if the lane is full, return -1
	if (maxGap==0)
		return -1;
	else {
		//     printf("    --> retornando.. positionMax=%d maxGap=%d calc=%d\n", positionMaxGap,
		// 	   maxGap, positionMaxGap + ((maxGap + 1) / 2));
		// best place is the middle of segment with greatest sequence of empty cells
		bestPlace = positionMaxGap + ((maxGap + 1)/ 2);
	}

	return bestPlace;
}




/************************************************************
Function: void Source::setSourceType(int _sourceType)
Author: Bruno C. da Silva 
Description: sets the source type (constant prob., variable prob., 
constant flow or variable flow)

Changelog:
 ************************************************************/

void Source::setSourceType(int _sourceType)
{
	sourceType = _sourceType;
}



/************************************************************
Function: int Source::getSourceType()
Author: Bruno C. da Silva 
Description: returns the source type (constant prob., variable prob., 
constant flow or variable flow)

Changelog:
 ************************************************************/

int Source::getSourceType()
{
	return sourceType;
}



/************************************************************
Function: int Source::getConstantFlow()
Author: Bruno C. da Silva 
Description: returns the constant flow of vehicles that this
source should produce
Changelog:
 ************************************************************/

int Source::getConstantFlow()
{
	return constantFlow;
}



/************************************************************
Function: int Source::setVariableProb()
Author: Bruno C. da Silva 
Description: sets the table of variable-probability rules
Changelog:
 ************************************************************/

void Source::setVariableProb(variable_probability_vector_t _variableProbTable)
{  
  variableProbabilityTable = _variableProbTable;   
}


/************************************************************
Function: int Source::getVariableProb()
Author: Bruno C. da Silva 
Description: returns the (variable) probability of vehicles being inserted
Changelog:
 ************************************************************/

double Source::getVariableProb()
{

  variable_probability_vector_t::iterator i;

//   for (i = variableProbabilityTable.begin(); i != variableProbabilityTable.end(); i++)
//     cout << "--> (get var_prob)  niter=" << i->first << " prob=" << i->second << endl;

//   printf("(get var prob) source iter = %d\n", source_iter);
//   printf("(get var prob) current behavior = %d\n", current_behavior);

  return variableProbabilityTable[current_behavior].second;
    
}



/************************************************************
Function: int Source::setVariableFlow()
Author: Bruno C. da Silva 
Description: sets the table of variable-flow rules
Changelog:
 ************************************************************/

void Source::setVariableFlow(variable_flow_vector_t _variableFlowTable)
{
  variableFlowTable = _variableFlowTable; 
}



/************************************************************
Function: int Source::getVariableProb()
Author: Bruno C. da Silva 
Description: returns the (variable) probability of vehicles being inserted
Changelog:
 ************************************************************/

int Source::getVariableFlow()
{

  variable_flow_vector_t::iterator i;

//   for (i = variableFlowTable.begin(); i != variableFlowTable.end(); i++)
//     printf("--> (get var_flow) niter=%d flow=%d\n", i->first, i->second);

//   printf("(get var flow) source iter = %d\n", source_iter);
//   printf("(get var flow) current behavior = %d\n", current_behavior);

  return variableFlowTable[current_behavior].second;
    
}




/************************************************************
Function: void Source::setConstantFlow(int _constantFlow)
Author: Bruno C. da Silva 
Description: sets the constant flow of vehicles that this
source should produce
Changelog:
 ************************************************************/

void Source::setConstantFlow(int _constantFlow)
{
	constantFlow = _constantFlow;
}




/************************************************************
Function: iteration_t Source::getTimeStep()
Author: Bruno C. da Silva 
Description: returns the timestep of the Source object.
The time step specifies the number of iterations
between two sucessive moments of acting (by 'acting'
we mean the moment when the source tries to insert 
cars in the simulation)

Changelog:
 ************************************************************/

iteration_t Source::getTimeStep() {
	return timeStep;
}




/************************************************************
Function: void Source::setTimeStep(iteration_t _timeStep)
Author: Bruno C. da Silva 
Description: sets the timestep of the Source object.
The time step specifies the number of iterations
between two sucessive moments of acting (by 'acting'
we mean the moment when the source tries to insert 
cars in the simulation)

Changelog:
 ************************************************************/

void Source::setTimeStep(iteration_t _timeStep) {
	timeStep = _timeStep;

}






/************************************************************
Function: void Source::update(iteration_t currentIteration)
Author: Bruno C. da Silva 
Description: Verifies if the source is running (that is, if
if has already been started) and determines if
it should act or not (by 'acting'
we mean the moment when the source tries to insert 
cars in the simulation)

Changelog:
 ************************************************************/

void Source::update(iteration_t currentIteration) {

        float prob;
	int flow;

	if (!running) {
		if (currentIteration >= getTimeBegin()) {
			running=1;
			current_behavior = 0;
			source_iter=0;
		}
	}

	if (running) {

		// updates the current behavior for a variable-probability source
		if (getSourceType() == VARIABLE_PROB) {
		  if (source_iter == variableProbabilityTable[current_behavior].first) {
		    source_iter = 0;
		    current_behavior = (current_behavior + 1) % variableProbabilityTable.size();
		  }
		}

		// updates the current behavior for a variable-flow source
		if (getSourceType() == VARIABLE_FLOW) {
		  if (source_iter == variableFlowTable[current_behavior].first) {
		    source_iter = 0;
		    current_behavior = (current_behavior + 1) % variableFlowTable.size();
		  }
		}
		
		if ((currentIteration % timeStep)==0) {

			switch (getSourceType()) {
			  //-------------------------------------
				case CONSTANT_PROB:
					if (randomGen.Random() < getActivationProbability())
						act();
					break;

			  //-------------------------------------
				case VARIABLE_PROB:

				  prob = getVariableProb();
// 				  cout << "iter=" << currentIteration << " source_iter=" << source_iter << " var prob=" << prob << endl;
				  if (randomGen.Random() < prob) {
				    //cout << getId() << "on" << endl;
				    act();
				  } //else
				    //cout << getId() << "off" << endl;
				  

				  break;

			  //-------------------------------------
				case CONSTANT_FLOW:
					for (int i=0; i<getConstantFlow(); i++)
						act();
					break;

			  //-------------------------------------
				case VARIABLE_FLOW:

				  flow = getVariableFlow();
//  				  cout << "iter=" << currentIteration << " source_iter=" << source_iter << " var flow=" << flow << endl;
				  for (int i=0; i<flow; i++)
				    act();

				  break;
			  //-------------------------------------
			}

		}
		
		source_iter ++;
	}
}



/************************************************************
Function: void Source::update(iteration_t currentIteration)
Author: Bruno C. da Silva 
Description: this is the method, in the Source class,
that tris to insert cars in the simulation.

Changelog:
 ************************************************************/

void Source::act() {
	static Nasch *nasch = new Nasch("Nasch");
	Car *newCar;

	// tries to pick a car from the inactive cars list; if the list
	// is empty, then we need to create a new car/driver


	if ((newCar = Car::readFromInactiveCarsList()) == NULL) {

		Driver *newDriver = NULL;
		try {
			//cout << "id do source: " << internalId << endl;
			newDriver = static_cast<Driver *> (nasch);
			newDriver->setDeceleration(getParentNode()->getParentNetwork()->
				getParentTopology()->getSimulation()->getDeceleration());

		} catch (std::exception& err) {
			std::cerr << err.what();
			std::cerr << std::endl;
		}
		if (newDriver == NULL) {
			std::cerr << "Could not get a new Driver!";
			std::cerr << std::endl;
		}


		// creates a new car informing {car, maxSpeed, maxAccel}
		newCar= new Car(newDriver, getParentNode()->getParentNetwork()->getParentTopology()->getCarsMaximumSpeed(), 1);  

	}

	else {

		// inserts the car into the simulation!
		// this car has already a noisy driver.
		// the cars used by special drivers aren't
		// kept in the inactivelist, but
		// they are deleted when they reach a sink.
		// see Sink::act
		Car::insertInActiveCarsList(newCar);

	}


	// chooses the lane where the car is going to be inserted
	LaneSet *targetLaneSet = getAssociatedLaneSet();
	lane_vector_t childLanes = targetLaneSet -> getChildLanes ();
	long int randomLane = randomGen.IRandom(0, targetLaneSet -> getNumberOfLanes()-1);
	Lane *targetLane = childLanes[randomLane];

	//   cout << "chamando BESTPLACE" << endl;
	//   fflush(stdout);
	dist_t targetCell;
	targetCell = bestPlaceOnLane(targetLane);
	//   fflush(stdout);

	// is there any free cell on the target lane?
	if (targetCell != -1) {
		//   if ((targetLane -> queryCarId(0)) == -1)   
		//     targetCell=0;

		newCar -> insertInLane(targetLane, targetCell, 
				getParentNode()->getParentNetwork()->getParentTopology()->getCarsMaximumSpeed(), 
				1);  // lane, cell, curSpeed, curAccel

		//       cout << "car " << newCar->getInternalId() << " in " << newCar->getCurrentLane()-> getId();
		//       cout << "/" << newCar -> getCurrentCell() << endl;


		// since the car is entering the simulation, we must ask it's driver
		// for his first decision about where he wants to go (lane-laneset) in the next
		// iteration
		lane_laneset_pair_t next_lane_and_laneset;
		next_lane_and_laneset = newCar -> getDriver() -> chooseNextLane_Laneset(newCar);
		(newCar -> driverDecision).laneInNextLaneSet = next_lane_and_laneset.first;
		(newCar -> driverDecision).nextLaneSet = next_lane_and_laneset.second;
	}

	// all  cells are occupied and therefore the car cannot be inserted 
	else {

		//cout << "FULL - car " << newCar -> getInternalId() << " in lane " << targetLane->getId() << endl;


		// just put the car back into the inactive cars list
		Car::removeFromActiveCarsList(newCar);
		Car::insertInInactiveCarsList(newCar);
	}

}







/************************************************************
Function: Source::Source ()
Author: Bruno C. da Silva 
Description: the constructor to the Source class.
Changelog:
 ************************************************************/

Source::Source (int _sourceType, 
		iteration_t _timeBegin, 
		Node *parentNode, 
		iteration_t _timeStep,
		LaneSet *_assocLaneSet) : Calibrator( _timeBegin, 
			parentNode,
			_assocLaneSet) {

			setSourceType(_sourceType);
			setTimeStep(_timeStep);
			setType(SOURCE);


		}




/************************************************************
Function: Source::~Source ()
Author: Bruno C. da Silva 
Description: the destructor to the Source class
Changelog:
 ************************************************************/

Source::~Source () {
}






/***********************************************************
  SINK
 ************************************************************/




/************************************************************
Function: void Sink::update(iteration_t currentIteration)
Author: Bruno C. da Silva 
Description: Verifies if the sink is running (that is, if
if has already been started); if timeBegin has been
reached, then the sink is activated.

Changelog:
 ************************************************************/

void Sink::update(iteration_t currentIteration) {

	if (!running) {
		if (currentIteration >= getTimeBegin()) {
			running=1;
		}
	}

}




/************************************************************
Function: Sink::Sink ()
Author: Bruno C. da Silva 
Description: the constructor to the Sink class.
Changelog: Denise
 ************************************************************/

Sink::Sink (iteration_t _timeBegin, 
		Node *parentNode,
		LaneSet *_assocLaneSet) : Calibrator(_timeBegin, 
			parentNode,
			_assocLaneSet) { 
			setType(SINK);						 							 	

		}



/************************************************************
Function: void Sink::act(Car *car)
Author: Diogo 
Description: tell the sink to act over some car. By 'act' we mean
that the sink is going to make some tests and remove
the car with some probability associated with the
calibrator
Changelog:
* 12-Mai-2005 - Diogo
	Delete cars used by special drivers.
	Insert in InactiveList those used by noisy drivers.
* 04-Apr-2008 - F. Boffo
        BUG fix: Added the test to check if the sink is running.
	
************************************************************/
int Sink::act(Car *car) {


	// sinks act if the car comes from the right laneset
	if ((car->getCurrentLane()->getParentLaneSet()) == getAssociatedLaneSet()) {
		if (running && randomGen.Random() <= getActivationProbability()) {    
			// tells driver loader to take care of the driver
			// DriverLoader will decide if it is a special driver or not
			vector<DriverLoader *> *lista;
			vector<DriverLoader *>::iterator it;
			lista = getParentNode()->getParentNetwork()->
				getParentTopology()->getSimulation()-> getDriverLoaderList();
			bool is_special=false;
			for(it=lista->begin();it != lista->end(); it++)
			{
				is_special = (*it)->removeDriver(car->getDriver(), -1); //JB:: must fix the driver's step out

			}
			car->getCurrentLane()->removeCar(car->getCurrentCell());
			Car::removeFromActiveCarsList(car); 
/* 			if (! is_special) */
/* 				Car::insertInInactiveCarsList(car);   */
/* 			// if it is a special driver delete the car. */
/* 			else delete car; */
			delete car;
			return 1;
		}
		else return 0;
	}
	// car does not belong to the expected laneset, so the sink does nothing    
	else return 0;
}


/************************************************************
Function: Sink::~Sink ()
Author: Bruno C. da Silva 
Description: the destructor to the Sink class
Changelog:
 ************************************************************/

Sink::~Sink () {
}



