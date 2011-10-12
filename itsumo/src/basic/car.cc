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
#include "car.hh"
#include <stdlib.h>
#include <sys/time.h>

#include <iomanip>    // For setw() and setprecision() 


#include <sstream>

#include <iostream>
using std::cout;
using std::endl;

using namespace std;



// list of cars that are being used in the simulation
active_cars_vector_t active_cars;

// list of unsed cars that can be put at any time in the simulation
inactive_cars_queue_t inactive_cars;



/************************************************************
   Function: void Car::setExtraData (void *_extraData)
   Author: Bruno C. da Silva 
   Description: sets the extraData structure, responsable for
          storing additional informations about the car
   Changelog:
************************************************************/
void Car::setExtraData (void *_extraData)
{
  extraData = _extraData;
}



/************************************************************
   Function: void * Car::getExtraData ()
   Author: Bruno C. da Silva 
   Description: returns the extraData structure, responsable for
          storing additional informations about the car
   Changelog:
************************************************************/
void * Car::getExtraData ()
{
  return extraData;
}



/************************************************************
   Function: Car * Car::getNearbyCar(int which)
   Author: Bruno C. da Silva 
   Description: returns a pointer to a nearby car, if it 
       exists. Valid "directions" of the wanted nearby car
       are {FRONT, REAR, LEFT, RIGHT, FRONT_LEFT, FRONT_RIGHT,
       REAR_LEFT, REAR_RIGHT}. If the car (or the lane) does
       not exist the function returns NULL.
   Changelog:
************************************************************/

Car * Car::getNearbyCar(int which)
{

  Lane *leftLane, *rightLane;
  

  switch(which) {

  case FRONT:
    return getCurrentLane()->getNearbyCar(getCurrentCell(), FRONT);
  case REAR:
    return getCurrentLane()->getNearbyCar(getCurrentCell(), REAR);
  case LEFT:
    // there is a left lane
    if ((leftLane=getCurrentLane()->getLeftLane()) != NULL)
      return leftLane->queryCarPtr(getCurrentCell());
    else
      return NULL;
  case RIGHT:
    // there is a right lane
    if ((rightLane=getCurrentLane()->getRightLane()) != NULL)
      return rightLane->queryCarPtr(getCurrentCell());
    else
      return NULL;
  case FRONT_LEFT:
    // there is a left lane
    if ((leftLane=getCurrentLane()->getLeftLane()) != NULL)
      return leftLane->getNearbyCar(getCurrentCell(), FRONT);
    else
      return NULL;
  case FRONT_RIGHT:
    // there is a right lane
    if ((rightLane=getCurrentLane()->getRightLane()) != NULL)
      return rightLane->getNearbyCar(getCurrentCell(), FRONT);
    else
      return NULL;
  case REAR_LEFT:
    // there is a left lane
    if ((leftLane=getCurrentLane()->getLeftLane()) != NULL)
      return leftLane->getNearbyCar(getCurrentCell(), REAR);
    else
      return NULL;
  case REAR_RIGHT:
    // there is a right lane
    if ((rightLane=getCurrentLane()->getRightLane()) != NULL)
      return rightLane->getNearbyCar(getCurrentCell(), REAR);
    else
      return NULL;

  default:
    cout << "Car " << getInternalId() << " trying to get nearby car on invalid direction!" << endl;
    cout << "Valid directions are FRONT, REAR, LEFT, RIGHT, FRONT_LEFT, FRONT_RIGHT, READ_LEFT and REAR_RIGHT";
    cout << endl;
    exit(1);
  }

 
}



/************************************************************
   Function: dist_t Car::findOutTargetCell()
   Author: Bruno C. da Silva 
   Description: When a car is changing lanesets (ex:
          it reached the end of a street), this functions
          finds out on what cell of the destination street
          the car should be put based on its current
          speed, current cell and the gap in the
          destination lane (the gap from the start of the
          lane, that is). Additionally, the current speed
          may have to be updated, because the car may have
          to break due to some obstacle on the target lane
          (ex: another car)

   Changelog:
************************************************************/

dist_t Car::findOutTargetCell() {

  // totalCarDisplacement is the delta x (that is, the overall
  // distance the car would move based on its current speed)
  dist_t totalCarDisplacement = static_cast<dist_t>(getCurrentSpeed());

  // counts how many cells there are to the end of the current lane
  dist_t distanceToEndOfLane = getCurrentLane() -> getLength() - getCurrentCell() - 1;
  
  // expected cell on the target lane where the car would be on the next iteration
  dist_t cellOnTheTargetLaneLane = totalCarDisplacement - distanceToEndOfLane - 1;

  // this is the number of cells (in the target lane) before the first
  // occupied cell
  dist_t gapFromBeginningOfLane = (driverDecision.lane -> getFrontalGapFromCell(-1))-1;

  // if there is a car in the first cell of the target lane, then we cannot leave the
  // current lane and the car must stop
  if (gapFromBeginningOfLane < 0) {


    //cerr << "   =>> CAR STOPPED ON FIRST CELL OF TARGET LANE!!! STOPPING TO AVOID A CRASH!!!" << endl;

    driverDecision.speed = 0;

    // place the car in the farthest cell possible.. 

    // note that the result from getFrontalGapFromCell may be laneLenght+1 if there is no car in front of us. In this case,
    // we must explicitly put the car in the lane's last cell
    dist_t gapAhead = (getCurrentLane() -> getFrontalGapFromCell(getCurrentCell()));
    if (gapAhead >= getCurrentLane() -> getLength())
      // a negative cell number is returned if the car cannot leave the laneset!
      return ((getCurrentLane() -> getLength())-1) * -1;
    else
      // otherwise we put the car in the farthest cell possible
      // (a negative cell number is returned if the car cannot leave the laneset!)
      return gapAhead * -1;

  }


  // if a car tries to enter a lane and its speed is bigger than the length of the lane, we must do 
  // something about it. If we don't, a car could just "fly over" the entire lane
  if (static_cast<dist_t>(getCurrentSpeed()) > driverDecision.lane -> getLength()) {

    //cerr << "   =>> FLYING OVER THE LANE!! This should never have happened." << endl;


    driverDecision.speed = driverDecision.lane -> getMaxSpeed();

    // place the car in the farthest cell possible.. note that the result from getFrontalGapFromCell may be 
    // laneLenght+1 if there is no car in front of us. In this case, we must explicitly put the car in the lane's last cell 
    if (gapFromBeginningOfLane >= driverDecision.lane -> getLength())
      return (driverDecision.lane -> getLength())-1;
    else
      // otherwise we put the car in the calculated farthest cell
      return gapFromBeginningOfLane;
  }


  // if we cannot go as far as we wanted to (because there is a car in front of us in the target lane),...
  if (cellOnTheTargetLaneLane >= gapFromBeginningOfLane) {

    // then the car moves to the last possible cell before crashing into the front car and then hits the
    // breaks in the last moment.. go figure Nasch
    driverDecision.speed=0;
    return gapFromBeginningOfLane;
  }
  else {


    // yes we can go ahead to where we want.. no one is hindering us, so we just move to
    // the target cell
    return cellOnTheTargetLaneLane;
  }


}















/************************************************************
   Function: void Car::updateDecision () 
   Author: Bruno C. da Silva
   Description: the first thing to do in the simulation is to request
   all the cars to update their decisions about what to do in the next
   iteration. While taking this decision, each car requests to its
   driver to take some decision. This decision only concerns what the
   car is going to do in the CURRENT LANE SET (that is, if it is going
   to go to the left lane, or if it is going to increase its speed,
   etc; the decision of where to go when the car reaches the end of
   the street is taken in 'commitDecision', because it depends only on
   the laneProbabilities, and has nothing to do with the behaviour of
   a model such as Nasch).

   Anyway, after everyone have requested the drivers to take some
   decision, all cars should have enough information about what to do
   next (this information of what to do next is stored in a struct
   called 'driverDecision'; each car has its own 'driverDecision'
   struct). After this, the next thing to do is to request all the cars
   to commit their decisions, by means of the function
   'commitDecision'.  This function basically sets the car's position
   on the topology, and its new speed. The car's position is an
   information compound by the car's lane and car's cell; the car's
   cell is calculated in 'commitDecision' according to the data in
   'driverDecision', which was filled in the previous step of the
   algorithm (updateDecision).  Depending on the conditions and
   occupation of the target lane (the lane where the driver wants to
   go), the car's speed might also be changed by the 'commitDecision'
   function.  Notice that the algorithm should be compound by these
   two distinct steps (all cars update their decision, and then all
   cars commit their decision) in order create a simulation that works
   in the same way as a celular automata (ie. a discrete simulation
   where all state change is done simultaneously).
        
   Changelog:
************************************************************/

void Car::updateDecision () {
  decision_t nextDecision;
  // first of all we make a backup copy of the decision already made. This step is necessary because takeDecision
  // overwrites the driverDecision struct while filling the next lane/speed (and then we loose next Laneset!)

  nextDecision.laneInNextLaneSet = driverDecision.laneInNextLaneSet;
  nextDecision.nextLaneSet = driverDecision.nextLaneSet;
 
  // request the car's driver to take some decision
  driverDecision = getDriver()->takeDecision(*this);

  // now we restore the "next decision" information
  driverDecision.laneInNextLaneSet = nextDecision.laneInNextLaneSet;
  driverDecision.nextLaneSet = nextDecision.nextLaneSet;
  
}









/************************************************************
   Function: void Car::commitDecision ()
   Author: Bruno C. da Silva 
   Description: to understand this function we recommend that you
        read the description of 'updateDecision'. After all cars
        have taken their decisions (by means of 'updateDecision'),
        the cars should commit their decisions. This commitment
        is basically the set of steps needed to effectively execute
        the driver's decision, that is, to place the car in the
        correct lane, in the correct cell, and with the correct
        speed.

   Changelog:
************************************************************/
void Car::commitDecision () {

  dist_t targetCell;
  speed_t restrictedSpeed;



  // sets the car to the speed chosen by the driver
  setCurrentSpeed(driverDecision.speed);

  // finds out if the car is going to stay on the same laneset or not
  if (  (static_cast<dist_t>(getCurrentSpeed()))  >=  (getCurrentLane()->getLength() - getCurrentCell()) ) {


    int removed = getCurrentLane() -> getParentLaneSet() -> getEndNode() -> signalToSinks(this);

    if (!removed) {
   
      // since the car is leaving the current laneset, let's use
      // the already made decision  about the next lane/laneset!
      driverDecision.lane = driverDecision.laneInNextLaneSet;


      // asks the semaphore if the the car can go from the current lane to the lane
      // where the driver wants to go (if there is no semaphore, 'allowMove' answers 'true')
      if (! (getCurrentLane() -> getParentLaneSet() -> getEndNode() -> allowMove(getCurrentLane(),
										 driverDecision.nextLaneSet))) {


      /*************************/
      /*    CLOSED SEMAPHORE   */
      /*************************/
	
	// Bruno 1207 DEBUG
// 	cout << "car " << getInternalId() << " vai ter que parar" << endl;
	
	// place the car in the farthest cell possible.. 

	// note that the result from getFrontalGapFromCell may be laneLenght+1 if there is no car in front of us. 
	// In this case, we must explicitly put the car in the lane's last cell


	dist_t gapAhead = (getCurrentLane() -> getFrontalGapFromCell(getCurrentCell()));
	dist_t targetCell;
	if (gapAhead >= getCurrentLane() -> getLength()) {
	  targetCell = (getCurrentLane() -> getLength())-1;
	}
	else
	  // otherwise we put the car in the farthest cell possible
	  targetCell =  getCurrentCell() + gapAhead;

	// Bruno 1207 DEBUG
// 	cout << "movendo pra cell " << targetCell << endl;
	
	Move(getCurrentLane(), targetCell);
	setCurrentSpeed (0);  // stops the car

      }




      /*************************/
      /*     OPEN  SEMAPHORE   */
      /*************************/

      else {
	// we're free to go      

	// now we`re going to calculate the target cell
	dist_t targetCell = findOutTargetCell();

	// if the target cell is negative, it means that the car could not leave the current laneset
	if (targetCell < 0) {
	  // 	  cout << "OPA, NAO CONSEGUIU MUDAR DE LANESET! " << getInternalId() << " queria ir de ";
	  // 	  cout << getCurrentLane() -> getId() << " para " << driverDecision.lane -> getId() << endl;

	  // moves the car to the destination cell, but remains on the current lane
	  Move(getCurrentLane(), targetCell * -1);

	  int freeSpace = 0;

	  freeSpace = driverDecision.nextLaneSet->getCapacity()
					- driverDecision.nextLaneSet->getNumberOfVehicles();

//		// print the lanesets' turning probabilities
//		turning_probabilities_vector_t turnProbs = driverDecision.nextLaneSet->getTurningProbabilities();
//		turning_probabilities_vector_t::iterator itProbs;
//
//		// nivel 1
//		for (itProbs = turnProbs.begin(); itProbs != turnProbs.end(); itProbs++){
//
//			freeSpace += (itProbs->first)->getCapacity() - (itProbs->first)->getNumberOfVehicles();
//		}

//	  if (freeSpace==0){
//
//		  // removing from running and putting back to wait
//		  getDriver()->force2Out(true);			// remove from network
//		  getDriver()->force2Wait(true);		// remove from running and put into waiting list
//	  }

	}
	else {
	  Move(driverDecision.lane, targetCell);
	  // since the car is entering a new laneset, let's ask the driver to inform what is going to be the 
	  // next destination where he wants to go (lane/laneset)
	  lane_laneset_pair_t next_lane_and_laneset;
	  next_lane_and_laneset = getDriver() -> chooseNextLane_Laneset(this);
	  driverDecision.laneInNextLaneSet = next_lane_and_laneset.first;
	  driverDecision.nextLaneSet = next_lane_and_laneset.second;
	}
      } // end of "open semaphore"
    }   // end of 'if (!removed)'
  }



  else {

    // NO LANESET CHANGE NEEDED


    switch (getCurrentLane()->getZone(getCurrentCell())) {
    case 0:  
      // 'NO CHANGES' ZONE!  (moves the car forward on the same lane; speed is restricted according to nasch rules)
      restrictedSpeed = min(getCurrentSpeed(), min(getMaxSpeed(), 
							   min(getCurrentLane()->getMaxSpeed(), 
							       getCurrentLane()->getFrontalGapFromCell(getCurrentCell()))));
      targetCell = static_cast<dist_t>(getCurrentCell() + restrictedSpeed);
      Move(getCurrentLane(), targetCell); 
      break;
      


    case 1:
      // 'NORMAL NASCH MOVIMENTATION' ZONE!
      targetCell = static_cast<dist_t>(getCurrentCell() + getCurrentSpeed());  // by Denise
      Move(driverDecision.lane, targetCell); 
      break;



    case 2:  

      // 'RESTRICTED NASCH MOVIMENTATION' ZONE!

      restrictedSpeed = min(getCurrentSpeed(), min(getMaxSpeed(), 
							   min(getCurrentLane()->getMaxSpeed(), 
							       getCurrentLane()->getFrontalGapFromCell(getCurrentCell()))));
      targetCell = static_cast<dist_t>(getCurrentCell() + restrictedSpeed);
      Move(getCurrentLane(), targetCell); 

      break;


    case 3:  
      // 'READAPTATION' ZONE!  (moves the car forward on the same lane; speed is restricted according to nasch rules)
      restrictedSpeed = min(getCurrentSpeed(), min(getMaxSpeed(), 
							   min(getCurrentLane()->getMaxSpeed(), 
							       getCurrentLane()->getFrontalGapFromCell(getCurrentCell()))));
      targetCell = static_cast<dist_t>(getCurrentCell() + restrictedSpeed);
      Move(getCurrentLane(), targetCell); 
      break;
    }

  }

  
  
}










/************************************************************
   Function: Car::Car(Driver *_pDriver, 
	              speed_t _maxSpeed, 
          	      accel_t _maxAccel)
   Author: Bruno C. da Silva 
   Description: this is the constructor to the Car class.
           The function uses three parameters: a pointer
           to the car's driver, and the car's maximum speed and
           acceleration. The constructor inserts the car in 
           a list containing all the cars involved in the simulation
           (this list is called 'cars').
	   A new car in NOT inserted automatically into a lane.
           You must create it and then use insertInLane (to put the car in the 
           topology), and later on you should only use the 'move' method.

   Changelog:
************************************************************/

Car::Car(Driver *_pDriver, 
	 speed_t _maxSpeed, 
	 accel_t _maxAccel)  : TopologyElement () {

  setDriver(_pDriver);
  setMaxSpeed(_maxSpeed);
  setMaxAccel(_maxAccel);

  insertInActiveCarsList(this);
  // initialize extra_data
  setExtraData(NULL);
  pDriver -> initializeExtraData(this);

  lastCell = 0;

  //  cout << "criou um carro de id: " << getInternalId() << " " << pDriver->getId() << endl;;
}





/************************************************************
   Function: void Car::removeFromActiveCarsList(Car *car)
   Author: Bruno C. da Silva 
   Description: removes 'car' from the list of cars
        that are currently in the simulation
   Changelog:
************************************************************/

void Car::removeFromActiveCarsList(Car *car) {
	if ( car->active_place /* != NULL */ )
	{
		if ( car == (*(car->active_place)) )
		{
			//cout << "\t" << car->active_place << " " << (*(car->active_place)) << " " << active_cars.back()  << " " << active_cars.back()->active_place  << " " << endl;
			(*(car->active_place)) = active_cars.back();
			active_cars.back()->active_place = car->active_place;
			car->active_place = NULL;
			active_cars.pop_back();
			return;
		}
	}
	cout << " Warning, the active_cars list was reallocated, please increment its background value." << endl;
	active_cars_vector_t::iterator it;
	for ( it = active_cars.begin(); it != active_cars.end(); ++it)
	{
		if ((*it) == car)
		{
			(*it) = active_cars.back();
			(*it)->active_place = car->active_place;
			car->active_place = NULL;
			active_cars.pop_back();
			return;
		}
	}
}



/************************************************************
   Function: void Car::insertInActiveCarsList(Car *newCar)
   Author: Bruno C. da Silva 
   Description: inserts 'newCar' in the list of cars
        that are currently in the simulation
   Changelog:
************************************************************/

void Car::insertInActiveCarsList(Car *newCar) {
	active_cars.push_back(newCar);
	newCar->active_place = & active_cars.back();
}



/************************************************************
   Function: Car * Car::readFromInactiveCarsList()
   Author: Bruno C. da Silva 
   Description: returns a car from the list of inactive cars;
       if the list is empty, this method returns NULL
   Changelog:
************************************************************/

Car * Car::readFromInactiveCarsList() {
  
  if (!inactive_cars.empty()) {
    Car *tmpCar = inactive_cars.front();
    inactive_cars.pop();     
    return tmpCar;
  }
  else
    return NULL;
  
}


string Car::showBuffersStatus() {
  char tmp[30];
  snprintf(tmp, 30, "(%d/%d)", (int) inactive_cars.size(), (int) active_cars.size());
  return string(tmp);

}


/************************************************************
   Function: void Car::insertInInactiveCarsList(Car *newCar)
   Author: Bruno C. da Silva 
   Description: puts a car into the inactive cars list
   Changelog:
************************************************************/

void Car::insertInInactiveCarsList(Car *newCar) {
  inactive_cars.push(newCar); 
}




/************************************************************
   Function: Car::~Car()
   Author: Bruno C. da Silva 
   Description: this is the destructor to the Car class.
           Pretty useless so far.

   Changelog:
************************************************************/

Car::~Car() {
	// Caso o motorista tenha sido destruido
	if (getDriver() != NULL) {
		getDriver() -> deleteExtraData(this);
	}
}









/************************************************************
   Function: int Car::Move(Lane * _targetLane, dist_t _targetCell)
   Author: Bruno C. da Silva 
   Description: this is the function that actually implements
         the car movement, after all considerations
         have been done about the driver's decision. This
         function simply receives a target lane and a target
         cell and puts the car there, using the auxiliary
         methods 'putCar' and 'removeCar'.

   Changelog:
************************************************************/

int Car::Move(Lane * _targetLane, 
	      dist_t _targetCell) {

  int retval;

  // we're not changing lanes, only moving forward  
  if (_targetLane == getCurrentLane()) {

    // updates car's position (in the Lane)
    retval = getCurrentLane() -> moveCar(getCurrentCell(), _targetCell);  

    if (retval != -1) {
      // updates car's position (in the Car)
      setCurrentCell(_targetCell);
    }
  }


  else {
    // we're changing lanes and moving forward

    retval=_targetLane -> putCar(this, _targetCell);    // puts the car in the correct Lane
    
    if (retval != -1) {
      getCurrentLane() -> removeCar(getCurrentCell());  // removes the car from the origin Lane
      pDriver->logEvent(LEFT_EVENT, getCurrentLane()->getParentLaneSet()->getId());
      setCurrentLane(_targetLane);                      // updates car's current lane (in the Car)
      setCurrentCell(_targetCell);                      // updates car's position (in the Car)
      pDriver->logEvent(ENTERED_EVENT, getCurrentLane()->getParentLaneSet()->getId());
    }
  }

  
  return 0;
}









/************************************************************
   Function: int Car::insertInLane(Lane * _targetLane, 
		               dist_t _targetCell, 
          		       speed_t _currentSpeed,
		               accel_t _currentAccel)
   Author: Bruno C. da Silva 
   Description: this function implements a 'hard' method
          of placing a car in some lane and cell
          with some current speed and acceleration. It should
          not be directly used except in the few cases when we
          need to 'magically' put a car in some place.
          

   Changelog:
   	2010-11-03: verify: if the car was inserted returns 0,
				returns 1 if the car was not inserted.
************************************************************/

int Car::insertInLane(Lane * _targetLane, 
		dist_t _targetCell, 
		speed_t _currentSpeed,
		accel_t _currentAccel) {
	//JB:: Checks if the car was inserted
	if (_targetLane -> putCar(this, _targetCell) == 0) {
		setCurrentLane(_targetLane);
		setCurrentCell(_targetCell);
		setCurrentSpeed(_currentSpeed);
		setCurrentAccel(_currentAccel);
	}
	else
		return 1;
	return 0;
}









/************************************************************
   Function: Lane* Car::getCurrentLane
   Author: Bruno C. da Silva 
   Description: returns a pointer to the current lane
   Changelog:
************************************************************/

Lane* Car::getCurrentLane () {
  return pCurrentLane;
}






/************************************************************
   Function: void Car::setCurrentLane (Lane * _currentLane)
   Author: Bruno C. da Silva 
   Description: sets the current lane to _currentLane
   Changelog:
************************************************************/

void Car::setCurrentLane (Lane * _currentLane) {
  pCurrentLane = _currentLane;
}





/************************************************************
   Function: dist_t Car::getCurrentCell ()
   Author: Bruno C. da Silva 
   Description: returns the current cell
   Changelog:
************************************************************/

dist_t Car::getCurrentCell () {
  return currentCell;
}


void Car::setLastCell (dist_t lcell) {
	lastCell = lcell;
}

dist_t Car::getLastCell () {
	return lastCell;
}




/************************************************************
   Function: void Car::setCurrentCell (dist_t _currentCell)
   Author: Bruno C. da Silva 
   Description: sets the current cell to _currentCell
   Changelog:
************************************************************/

void Car::setCurrentCell (dist_t _currentCell) {
  currentCell = _currentCell;
}





/************************************************************
   Function: Driver* Car::getDriver ()
   Author: Bruno C. da Silva 
   Description: returns a pointer to the car's driver
   Changelog:
************************************************************/

Driver* Car::getDriver () {
  return pDriver;
}





/************************************************************
   Function: void Car::setDriver (Driver * _pDriver)
   Author: Bruno C. da Silva 
   Description: sets the car's driver to _pDriver
   Changelog:
************************************************************/

void Car::setDriver (Driver * _pDriver) {
  pDriver = _pDriver;
}




/************************************************************
   Function: speed_t Car::getMaxSpeed ()
   Author: Bruno C. da Silva 
   Description: returns the car's maximum speed
   Changelog:
************************************************************/

speed_t Car::getMaxSpeed () {
  return maxSpeed;
}





/************************************************************
   Function: void Car::setMaxSpeed (speed_t _maxSpeed)
   Author: Bruno C. da Silva 
   Description: sets the car's maximum speed to _maxSpeed
   Changelog:
************************************************************/

void Car::setMaxSpeed (speed_t _maxSpeed) {

  maxSpeed = _maxSpeed;
}





/************************************************************
   Function: speed_t Car::getCurrentSpeed ()
   Author: Bruno C. da Silva 
   Description: returns the car's current speed
   Changelog:
************************************************************/

speed_t Car::getCurrentSpeed () {
  return currentSpeed;

}






/************************************************************
   Function: speed_t Car::setCurrentSpeed ()
   Author: Bruno C. da Silva 
   Description: sets the car's current speed to _currentSpeed
   Changelog:
************************************************************/

void Car::setCurrentSpeed (speed_t _currentSpeed) {
  currentSpeed = _currentSpeed;
}





/************************************************************
   Function: accel_t Car::getCurrentAccel()
   Author: Bruno C. da Silva 
   Description: returns the car's current acceleration
   Changelog:
************************************************************/

accel_t Car::getCurrentAccel() {
  return currentAccel;

}





/************************************************************
   Function: void Car::setCurrentAccel(accel_t _curAccel)
   Author: Bruno C. da Silva 
   Description: sets the car's current acceleration
   Changelog:
************************************************************/

void Car::setCurrentAccel(accel_t _curAccel) {
  currentAccel = _curAccel;
  
}




/************************************************************
   Function: accel_t Car::getMaxAccel()
   Author: Bruno C. da Silva 
   Description: returns the car's maximum acceleration
   Changelog:
************************************************************/

accel_t Car::getMaxAccel() {
  return maxAccel;
}





/************************************************************
   Function: void Car::setMaxAccel(accel_t _MaxAccel)
   Author: Bruno C. da Silva 
   Description: sets the car's maximum acceleration to _MaxAccel
   Changelog:
************************************************************/

void Car::setMaxAccel(accel_t _MaxAccel) {
  maxAccel = _MaxAccel;
  
}
