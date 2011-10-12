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
#include "lane.hh"


#include <iomanip>    // For setw() and setprecision()
#include <iostream>
using std::cout;
using std::endl;

using namespace std;

// methods to set and return the position of the lane inside the laneset (if it's the first, second, etc);
// the position is measured left->right (1st lane is the leftmost)
void Lane::setPosition(int _pos)
{
	position_in_laneset=_pos;
}

int Lane::getPosition()
{
	return position_in_laneset;
}

//--------------------------------------------------
int Lane::getStoppedCars()
{
	int stoppedCars = 0;

	for (int j = 0; j < getLength(); j++) {
		topology_element_id_t carId = queryCarId(j);
		if (carId != -1) {
			if (queryCarSpeed(j) == 0)
				stoppedCars++;
		}
	}
	return stoppedCars;
}


//--------------------------------------------------
double Lane::getAvgSpeed() {
	int carsInTheLane = 0, totalSpeed = 0;

	// counts the number of cars in the lane and the sum of their speeds
	for (int j = 0; j < getLength(); j++) {
		topology_element_id_t carId = queryCarId(j);
		if (carId != -1) {
			carsInTheLane++;
			totalSpeed += queryCarSpeed(j);
		}
	}

	double averageSpeed;
	if (carsInTheLane != 0)
		averageSpeed = static_cast<double> (totalSpeed) / static_cast<double> (carsInTheLane);
	else
		//if there are no vehicles, the avg speed is the maximum speed
		averageSpeed = getUsedMaxSpeed();

	return averageSpeed;
}

//--------------------------------------------------
/**
 * Returns the average vehicles speeed converted to Km/h.
 *
 * Author: Denise de Oliveira
 */

double Lane::getAvgSpeedKMH(){
	float cellsize = getParentLaneSet()->getStartNode()->getParentNetwork()->getParentTopology()->getCellSize();
	//steps size in seconds
	int stepSize = (int)getParentLaneSet()->getStartNode()->getParentNetwork()->getParentTopology()->getIterationTime();
	return (getAvgSpeed()*cellsize*stepSize*3.6);
}

//--------------------------------------------------
/**
 *
 * Returns the product of the total flow entering the
 * link and link length. Represents the traffic demand
 * on the link. Used to calculate the traffic demand
 * in the network (sum of this value for all links).
 * The value is in Km
 *
 * Author: Denise de Oliveira
 */
double Lane::getDistanceTravelled(){
	float cellsize = getParentLaneSet()->getStartNode()->getParentNetwork()->getParentTopology()->getCellSize();
	return (getEntryFlow()*(getLength()*cellsize)/1000); //value in Km
}

//--------------------------------------------------
/**
 *
 * Returns an approximation of the entry rate of vehicles on the lane
 * (vehicles / step).
 * Calculates using a mobile average of the vehicles inserted in the lane
 * over 300 steps (or 5 minutes, if the step has size 1).
 * This is an aproximation since the entry rate at the sources
 * might change during the simulation time!
 *
 * Author: Denise de Oliveira
 */
double Lane::getEntryRate(){
	unsigned int size = insertedVehicles.size();
	iteration_t newStep = getParentLaneSet()->getStartNode()->getCurrentStep();
	if (currentStep != newStep){
		currentStep = newStep;
		if (size >= 300)
			insertedVehicles.erase(insertedVehicles.begin());
		else
			++size;
		insertedVehicles.push_back(totalInsertedVehicles);
		totalInsertedVehicles = 0;
	}
	double total = accumulate(insertedVehicles.begin(), insertedVehicles.end(), 0);
	//cout<<"total"<< total <<" rate:"<<total/(double)size <<endl;
	return (total/(double)size);
}
/**
 *
 * Returns an approximation of the entry flow of4799 vehicles on the lane
 * (vehicles / hour). Using the entryRate() .
 * This is an aproximation since the entry flow at the sources
 * might change during the simulation time!
 *
 * Author: Denise de Oliveira
 */
double Lane::getEntryFlow(){

	// flow in 1 hour
	return getEntryRate()*(3600.0/stepSize);
}


//-------------------------------------------------
/**
 * Returns the maximum velocity adopted by this lane.
 * Can be different from getMaxSpeed() if the user has not
 * set a specific maximum velocity to the lane, the used velocity
 * is the maximum alowed in teh network (defined by the topology).
 *
 * Author: Denise de Oliveira
 */
speed_t Lane::getUsedMaxSpeed(){
	speed_t maxS = (speed_t)getMaxSpeed();
	if (maxS==0){ //max speed of the lane not set
		 maxS = (speed_t)getParentLaneSet()->getStartNode()->getParentNetwork()->getParentTopology()->getCarsMaximumSpeed();
	}
	return maxS;
}
//--------------------------------------------------
/**
 *  Returns the saturation flow for the current lane in vehicles/ hour of green.
 *  The saturation flow is the maximum amount of queued vehicles that could leave
 *  the lane given 1 hour of green. The used function was calculated specifically for
 *  the used model of movement, obteined by a sigmoidal fit of experimental values:
 *  velocity  number of vehicles in 60s of green:
 *  1			30.24
 *  2			39.74
 *  3			44.88
 *  4			47.63
 *  5			49.1
 *  6			49.88
 *  7			50.3
 *  8			50.52
 *
 * experimental:
 * 1       30
 * 2       40
 * 3       45
 * 4       47
 * 5       49
 * 6       50
 * 7       50
 * 8       51
 *
 *  (using the Origin):
 *  Boltzmann => y = A2 + (A1-A2)/(1 + exp((x-x0)/dx))
 *  A1 initial value (left horizontal asymptote)
 *	A2 final value (right horizontal asymptote)
 *	x0 center (point of inflection)
 *	dx width (the change in X corresponding to the most significant change in Y values)
 *
 *  Author: Denise de Oliveira
 */
int Lane::getSaturationFlow(){ //vehicles / hour of green
	if (saturationFlow == 0){
		//calculates the saturation flow
		double velMax = (double)getUsedMaxSpeed();
		saturationFlow = round(60.0*(50.77 + (-781.45-50.77)/(1 + exp((velMax+4.81)/1.59))));
	}
	return saturationFlow;
}
//--------------------------------------------------
/**
 * Returns the estimated degree of saturation of the lane.
 * Considers the lane given green time and the estimated flow.
 * Same formula used in TRANSYT/10
 * (total flow into the lane x cycle time)/(saturation flow x effective
 * green time for the lane).
 * ATENTION: Returns zero if there is no traffic light at the end of the lane!
 *
 * Author: Denise de Oliveira
 */
double Lane::getDegreeOfSaturation(){
	double degreeOfSaturation = 0.0;

	TrafficLight* tl = getParentLaneSet()->getEndNode()->getTrafficLight();
	if (tl !=NULL){
		int cycleTime = tl->getCycleTimeFromPlan(tl->getCurrentPlan());
		// flow in 1 hour
		double entryFlow = getEntryFlow();
		int splitForLane = tl->getSplitTimeForLaneInPlan(getId(),tl->getCurrentPlan());
		//cout << "CYCLE TIME: "<<cycleTime<<" SPLIT FOR LANE: "<<  splitForLane<<endl;
		//cout<<"entryFlow: "<<entryFlow<<" sat. flow: "<<getSaturationFlow()<< endl;
		degreeOfSaturation = ((double)entryFlow*(double)cycleTime)/((double)getSaturationFlow()*((double)splitForLane));
		//cout<<"degreeOfSaturation "<<degreeOfSaturation<<endl;
		return degreeOfSaturation;
	}
	//returns zero if there is no traffic light at the end of the lane!
	return degreeOfSaturation;
}
//--------------------------------------------------
/**
 * Returns the time (in time steps) needed for
 * crossing the lane considering the lane maximum
 * velocity and length.
 *
 * Author: Denise de Oliveira
 */
double Lane::getCruiseTime(){ //time steps
    double maxS = (double)getUsedMaxSpeed();
	double meanCruiseTime = (double)getLength()/(double)maxS;
	return meanCruiseTime;
}

//--------------------------------------------------
/**
 * Mean time (in time steps) needed for crossing the
 * lane considering the current average velocity
 * and lane length.
 *
 * Returns -1 if all the vehicles are stopped
 *
 * Author: Denise de Oliveira
 */
double Lane::getMeanCruiseTime(){ //time steps
	double speed = getAvgSpeed();
	if (speed == 0)
		return -1;
	else
		return (double)getLength()/speed;
}
//--------------------------------------------------
/**
 * Returns the diference (in seconds or time steps)
 * between the minimum cruise time (with maximum velocity)
 * and the mean cruise time (considering the average
 * velocity of the vehicles in the lane).
 *
 * Returns -1 if all the vehicles are stopped
 *
 * Author: Denise de Oliveira
 */
double Lane::getDelayTime(){ //seconds
	double meanCTime =  getMeanCruiseTime();
	if (meanCTime == -1)
		return -1;
	else
		return (meanCTime - getCruiseTime());

}
//--------------------------------------------------
/**
 * Returns queue size: the number of stopped adjacent
 * vehicles from the end of thelane to the beginning
 * (i.e: stopped at a red light),
 * Examples: ("x" = stopped vehicle, "-" = empty cell)
 * 		- - - - - x x x x = queue = 4
 *		- - x - - x x x - = queue = 3
 *		- - x - - x - x - = queue = 1
 *
 * Author: Denise de Oliveira
 */
unsigned int Lane::getQueueSize(){ //number of vehicles
	unsigned int queueSize = 0;
	int laneHead = (int)getLength()-1;
	bool firstStopped = false;
    for (int j = laneHead; j>=0; j--) {
		topology_element_id_t carId = queryCarId(j);
		if (carId!=-1) {
	  		if (queryCarSpeed(j) == 0){
	  			if(!firstStopped){
	  				firstStopped = true;
	  			}
	    		++queueSize;
	  		}
		}
		else if (firstStopped) break;
    }
	return queueSize;
}

/**
 * Returns TRUE if the lane is controllled by a traffic light
 * (if there is a traffic light at its end).
 * FALSE if is not controlled.
 *
 * Author: Denise de Oliveira
 */

bool Lane::isControlled(){
	TrafficLight* tl = getParentLaneSet()->getEndNode()->getTrafficLight();
	if (tl !=NULL){
		return true;
	}
	return false;
}

//--------------------------------------------------

void Lane::setDecelerationProbability(float _decel)
{
  decelerationProbability = _decel;
}

float Lane::getDecelerationProbability()
{
  return decelerationProbability;
}



/************************************************************
   Function: dist_t Lane::getZone(dist_t _position)
   Author: Bruno C. da Silva
   Description: given a position in the lane, returns
       to which lane zone this position belongs (ie:
       initial zone, where only frontal movements are
       allowed, middle zone, where the default driver behaviour
       takes places, etc)

   Changelog:
************************************************************/

dist_t Lane::getZone(dist_t _position)
{

  speed_t carsMaxSpeed = getParentLaneSet()->getStartNode()->getParentNetwork()->getParentTopology()->getCarsMaximumSpeed();

  dist_t cellsAvailableToZones = getLength();
  dist_t positionEndFirstZone = NON_EXISTANT_ZONE;
  dist_t positionEndSecondZone = NON_EXISTANT_ZONE;;
  dist_t positionEndThirdZone = NON_EXISTANT_ZONE;;
  dist_t positionEndFourthZone = NON_EXISTANT_ZONE;;

  int proportionalThirdZoneSize = 3;

  // priority of zones: 1, 3, 4, 2

  // first zone available
  positionEndFirstZone = carsMaxSpeed-1;
  cellsAvailableToZones -= carsMaxSpeed;

  // third zone available
  if (cellsAvailableToZones >= proportionalThirdZoneSize*carsMaxSpeed) {
    cellsAvailableToZones -= proportionalThirdZoneSize*carsMaxSpeed;
    positionEndThirdZone = getLength()-1;

    // fourth zone available
    if (READAPTATION_ZONE_SIZE != 0) {
      if (cellsAvailableToZones >= READAPTATION_ZONE_SIZE) {
	positionEndThirdZone -= READAPTATION_ZONE_SIZE;
	positionEndFourthZone = getLength()-1;
	cellsAvailableToZones -= READAPTATION_ZONE_SIZE;

	// second zone available
	if (cellsAvailableToZones > 0) {
	  positionEndSecondZone = positionEndThirdZone - proportionalThirdZoneSize*carsMaxSpeed;
	  cellsAvailableToZones = 0;
	}
      }
      else {
	// no full fourth zone, but at least part of it
	if (cellsAvailableToZones > 0) {
	  positionEndFourthZone = getLength()-1;
	  positionEndThirdZone -= cellsAvailableToZones;
	  cellsAvailableToZones = 0;
	}
      }
    }
    else {
      // no fourth zone, but second zone available
      if (cellsAvailableToZones > 0) {
	positionEndSecondZone = positionEndThirdZone - proportionalThirdZoneSize*carsMaxSpeed;
	cellsAvailableToZones = 0;
      }
    }
  }
  else {
    // no full third zone, but at least part of it
    if (cellsAvailableToZones > 0) {
      positionEndThirdZone = getLength()-1;
    }
  }

//   cout << "Lane size: " << getLength() << endl;
//   if (positionEndFirstZone != NON_EXISTANT_ZONE)
//     cout << "   End 1st: " << positionEndFirstZone << endl;
//   if (positionEndSecondZone != NON_EXISTANT_ZONE)
//   cout << "   End 2st: " << positionEndSecondZone << endl;
//   if (positionEndThirdZone != NON_EXISTANT_ZONE)
//   cout << "   End 3st: " << positionEndThirdZone << endl;
//   if (positionEndFourthZone != NON_EXISTANT_ZONE)
//   cout << "   End 4st: " << positionEndFourthZone << endl;

//   for (int i=0; i < size; i++) {
//     cout << "      testando posicao " << i;
  if (positionEndFirstZone != NON_EXISTANT_ZONE)
    if (_position<=positionEndFirstZone) {
      //      cout << "         Zona 1" << endl;
      return INITIAL_ZONE_NO_CHANGES;
    }

  if (positionEndSecondZone != NON_EXISTANT_ZONE) {
    if (_position<=positionEndSecondZone) {
      //       cout << "         Zona 2" << endl;
      return MIDDLE_ZONE_NORMAL_BEHAVIOUR;
    }
  }

  if (positionEndThirdZone != NON_EXISTANT_ZONE) {
    if (_position<=positionEndThirdZone) {
      //       cout << "         Zona 3" << endl;
      return MIDDLE_ZONE_CORRECT_PLACEMENT_ONLY;
    }
  }

  if (positionEndFourthZone != NON_EXISTANT_ZONE) {
    if (_position<=positionEndFourthZone) {
      //       cout << "         Zona 4" << endl;
      return FINAL_ZONE_READAPTATION;
    }
  }

  // what do we do if the above function is bugged??
  cout << "   ERROR on lane::getZone()!!!" << endl;
  cout << "      verifying position #" << _position << " but did not match any zone!" << endl;
  return MIDDLE_ZONE_NORMAL_BEHAVIOUR;

}


/************************************************************
   Function: int getNumberOfVehicles()
   Author: Denise de Oliveira
   Description: returns the number of vehicles in the lane
   Changelog:
************************************************************/

unsigned int Lane::getNumberOfVehicles() {

    unsigned int carsInTheLane=0;

    for (int j = 0; j <  getLength() ; j++) {
      topology_element_id_t carId = queryCarId(j);
      if (carId!=-1)
		++carsInTheLane;
    }
    return carsInTheLane;
}



/************************************************************
   Function: double Lane::getDensity()
   Author: Bruno C. da Silva
   Description: this functions returns the lane density
       (quotient between the number of cars and the number
         of cells)
   Changelog: Changed by Denise (10/03/2009) to use the
   getNumberOfVehicles method
************************************************************/

double Lane::getDensity() {

    // calculates the lane density and the average speed
    double density = (static_cast<double>(getNumberOfVehicles()) / static_cast<double>(getLength()));

    return density;
}



/************************************************************
   Function: void Lane::printLaneStatus(int iteration)
   Author: Bruno C. da Silva
   Description: this functions prints the status of a
          lane. It is used by some methods used to
          print the overall state of the topology.

   Changelog:
************************************************************/

string Lane::printLaneStatus(int iteration) {

    ostringstream cout;

    int carsInTheLane=0, totalSpeed=0;

    cout << iteration << "\t";
    cout << getId();

    // counts the number of cars in the lane and the sum of their speeds
    for (int j = 0; j <  getLength() ; j++) {
      topology_element_id_t carId = queryCarId(j);
      if (carId!=-1) {
	carsInTheLane++;
	totalSpeed += queryCarSpeed(j);
      }
    }

    // calculates the lane density and the average speed
    double density = (static_cast<double>(carsInTheLane) / static_cast<double>(getLength()));
    double averageSpeed;
    if (carsInTheLane!=0)
      averageSpeed = static_cast<double>(totalSpeed) / static_cast<double>(carsInTheLane);
    else
      averageSpeed = 0;

    // prints everything
    cout.setf(ios::showpoint);
    cout.setf(ios::fixed,ios::floatfield);
    cout.precision(2);
    cout.width(4);

    cout << "\t" << density;
    cout << "\t" << averageSpeed << "\t\t";
    for (int j = 0; j <  getLength() ; j++) {
      topology_element_id_t carId = queryCarId(j);
      if (carId==-1)
	cout << "-- ";
      else {
	cout << carId << " ";
// 	cout << carId << "(" << queryCarSpeed(j) << ")" << " ";
      }
    }

    cout << endl;

    return cout.str();
}





/************************************************************
   Function: int Lane::queryCarId(dist_t cell)
   Author: Bruno C. da Silva
   Description: returns the id of the car located in
           'cell'. If there is not car in 'cell', the
           function returns -1.

   Changelog:
************************************************************/

int Lane::queryCarId(dist_t cell) {

  // returns -1 if the cell is empty; otherwise, returns
  // the car's id
  return ((cellMap[cell]==NULL) ? -1: (cellMap[cell] -> getInternalId()));

}



/************************************************************
   Function: int Lane::queryCarPtr(dist_t cell)
   Author: Bruno C. da Silva
   Description: returns a pointer to the car located in
           'cell'. If there is not car in 'cell', the
           function returns NULL.

   Changelog:
************************************************************/

Car * Lane::queryCarPtr(dist_t cell) {
	if (cell > (int)cellMap.size())
		return NULL;
	else
		return cellMap[cell];
}








/************************************************************
   Function: int Lane::queryCarSpeed(dist_t cell)
   Author: Bruno C. da Silva
   Description: returns the speed of the car located in
           'cell'. If there is not car in 'cell', the
           function returns -1.

   Changelog:
************************************************************/

int Lane::queryCarSpeed(dist_t cell) {

  // returns -1 if the cell is empty; otherwise, returns
  // the car's speed
	if ((int)cellMap.size() > cell) {
		return ((cellMap[cell]==NULL) ? -1: (cellMap[cell] -> getCurrentSpeed()));
	} else
		return -1;
}







/************************************************************
   Function: Lane * Lane::getLeftLane()
   Author: Bruno C. da Silva
   Description: returns the lane on the left of the current
           lane. It is assumed that the leftmost lane is
           the first to be inserted in a laneset, and the
           rightmost lane is the last. Left-right orientation
           is based on the point of view of someone standing
           on the street and looking to its end (that is, from
           the point of view of someone moving in the same direction
           of the street).

   Changelog:
************************************************************/

Lane * Lane::getLeftLane() {

  // if there is no right lane, this function will return
  // NULL
  return leftLane;

}









/************************************************************
   Function: Lane * Lane::getRightLane()
   Author: Bruno C. da Silva
   Description: returns the lane on the right of the current
           lane. It is assumed that the leftmost lane is
           the first to be inserted in a laneset, and the
           rightmost lane is the last. Left-right orientation
           is based on the point of view of someone standing
           on the street and looking to its end (that is, from
           the point of view of someone moving in the same direction
           of the street).

   Changelog:
************************************************************/

Lane * Lane::getRightLane() {

  // if there is no right lane, this function will return
  // NULL
  return rightLane;
}











/************************************************************
   Function: void Lane::setLeftLane(Lane * _lLeft)
   Author: Bruno C. da Silva
   Description: sets the left lane of the current lane to be
           '_lLeft'. It is assumed that the leftmost lane is
           the first to be inserted in a laneset, and the
           rightmost lane is the last. Left-right orientation
           is based on the point of view of someone standing
           on the street and looking to its end (that is, from
           the point of view of someone moving in the same direction
           of the street).

   Changelog:
************************************************************/

void Lane::setLeftLane(Lane * _lLeft) {
  leftLane = _lLeft;

}






/************************************************************
   Function: void Lane::setRightLane(Lane * _lRight)
   Author: Bruno C. da Silva
   Description: sets the right lane of the current lane to be
           '_lRight'. It is assumed that the leftmost lane is
           the first to be inserted in a laneset, and the
           rightmost lane is the last. Left-right orientation
           is based on the point of view of someone standing
           on the street and looking to its end (that is, from
           the point of view of someone moving in the same direction
           of the street).

   Changelog:
************************************************************/

void Lane::setRightLane(Lane * _lRight) {

  rightLane = _lRight;

}







/************************************************************
   Function: int Lane::moveCar(dist_t From, dist_t To)
   Author: Bruno C. da Silva
   Description: this method is used to move a car inside
          its current lane. 'moveCar' basically swaps the
          {from, to} cell values and does some security
          checking to ensure that we're not trying to
          put one car over the other.

   Changelog:
************************************************************/

int Lane::moveCar(dist_t From, dist_t To) {

  // checks to see if we are really moving to somewhere different
  if (cellMap[From] != cellMap[To]) {

    // checks to ensure that we are not being put over some
    // other car
    if ((cellMap[From]==NULL) || (cellMap[To] != NULL)) {

// #ifdef DEBUG_CAR
//       cout << "(lane) moveCAR ERROR in lane " << getId() << endl;
//       cout << "(lane) moveCAR ERROR -> FromCell = " << From;
//       if (cellMap[From]==NULL)
// 	cout << " (NULL)";
//       else
// 	cout << " (" << cellMap[From]->getInternalId() << ")" << endl;
//       cout << "(lane) moveCAR ERROR -> ToCell = " << To;
//       if (cellMap[To]==NULL)
// 	cout << " (NULL)";
//       else
// 	cout << " (" << cellMap[To]->getInternalId() << ")" << endl;
// #endif

      return -1;
    }

    // if everything's ok, swaps the cell's values
    else {
      cellMap[To] = cellMap[From];
      cellMap[From] = NULL;
      return 0;
    }
  }

  // returns 0 if the To cell is equal to the From cell (nothing
  // to be done)
  else
    return 0;

}


/************************************************************
   Function: int Lane::putCar(Car *_car, dist_t Where)
   Author: Bruno C. da Silva
   Description: this method is used when we need to put
          a car in some specific cell. This is the ONLY
          method that should in order to perform this
          operation. Use this standard method and do
          not change a cell's value manually.

   Changelog:
************************************************************/

int Lane::putCar(Car *_car, dist_t Where) {

	if (cellMap.size() == 0) {
		return -1;
	}

  if (cellMap[Where] != NULL) {
// #ifdef DEBUG_CAR
//     cout << "(lane) putCAR ERROR!!!! debug now! (car " << _car -> getInternalId()  << " to cell ";
//     cout << Where  << " in lane " << getId() << " because is occupied by ";
//     cout << cellMap[Where]->getInternalId() << ")" << endl;
// #endif
    return -1;
  }

  cellMap[Where] = _car;
  totalInsertedVehicles++; // increments the number of inserted vehicles in this lane
  return 0;
}







/************************************************************
   Function: int Lane::removeCar(Car *_car, dist_t Where)
   Author: Bruno C. da Silva
   Description: this method is used when we need to remove
          a car from some specific cell. This is the ONLY
          method that should in order to perform this
          operation. Use this standard method and do
          not change a cell's value manually.

   Changelog:
************************************************************/

int Lane::removeCar(dist_t From) {

  if (cellMap[From] == NULL) {
// #ifdef DEBUG_CAR
//     cout << endl << "(lane) removeCAR ERROR!!!!  removing from cell " << From;
//     cout << " in lane " << getId() << endl;
// #endif
    return -1;
  }
  cellMap[From] = NULL;

  return 0;
}











/************************************************************
   Function: dist_t Lane::getRearGapFromCell(dist_t _cell)
   Author: Bruno C. da Silva
   Description: returns the frontal gap from _cell. The frontal
          gap is the number of empty cells that exists
          after _cell, in the direction of the end
          of the street. If there is no car after _cell,
          this function returns laneLength+1 (more details
          can be found in the documentation in the function's
          body)

   Changelog:
************************************************************/

// this function returns laneLength+1 so the caller can test if the
// returned value is to be interpreted as "there is a car in front of
// me, and it is located 'x' cells from my position" or "there is no
// car at all until the end of my current lane"

// we could return any other value, but laneLength+1 helps us to avoid
// modifications in other functions, such as naschCalculateNewSpeed

dist_t Lane::getFrontalGapFromCell(dist_t _cell) {


  dist_t i, laneLength=getLength();

  for(i=_cell+1; ((i<laneLength) && (cellMap[i]==NULL)); i++);

  if (i==laneLength) // in the case where no car was found until the end of the street, we return laneLength+1
    return (laneLength+1); // this value should be interpreted as "there is no car in front of us, until
                            // the end of the street"
  else
    return i-_cell-1;  // |---C1---C2----|    -> C1's gap is 3, not 4; C2's gap is 15
}







/************************************************************
   Function: dist_t Lane::getRearGapFromCell(dist_t _cell)
   Author: Bruno C. da Silva
   Description: returns the rear gap from _cell. The rear
          gap is the number of empty cells that exists
          before _cell, in the direction of the beginning
          of the street. If there is no car before _cell,
          this function returns laneLength+1 (more details
          can be found in the documentation in the function's
          body)

   Changelog:
************************************************************/

// this function returns laneLength+1 so the caller can test if the
// returned value is to be interpreted as "there is a car before
// me, and it is located 'x' cells from my position" or "there is no
// car at all before my position, in the direction of the beginning
// of my current lane"

// we could return any other value, but laneLength+1 helps us to avoid
// modifications in other functions, such as naschCalculateNewSpeed

dist_t Lane::getRearGapFromCell(dist_t _cell) {

  dist_t i, laneLength=getLength();

  for(i=_cell-1; ((i>=0) && (cellMap[i]==NULL)); i--);

  if (i==-1) // in the case where no car was found until the beginning of the street,
             // we return laneLength+1
    return (laneLength+1); // this value should be interpreted as "there is no car behind
                           // us, until the beginning of the street"
  else
    return _cell-i-1;  //  |C2 -- -- -- -- -- C3 -- C1|
                       //  (C1's gap is 1; C2's gap is 10; C3's gap is 5)

}




// AQUI

Car * Lane::getNearbyCar(dist_t _currentCell, int which) {

  dist_t gapToCar, otherCarLocation;

  if (which == FRONT) {
    gapToCar = getFrontalGapFromCell(_currentCell);
    if (gapToCar != getLength() + 1)  {
      otherCarLocation = _currentCell + gapToCar + 1;
      return queryCarPtr(otherCarLocation);
    }
    else
      return NULL;
  }
  else {
    gapToCar = getRearGapFromCell(_currentCell);
    if (gapToCar != getLength() + 1)  {
      otherCarLocation = _currentCell - gapToCar - 1;
      return queryCarPtr(otherCarLocation);
    }
    else
      return NULL;
  }

}








/************************************************************
   Function: void Lane::setMaxSpeed(dist_t _maxSpeed)
   Author: Bruno C. da Silva
   Description: sets the lane's maximum speed (speed limit)
   Changelog:
************************************************************/

void Lane::setMaxSpeed(dist_t _maxSpeed) {
  vMax = _maxSpeed;

}





/************************************************************
   Function: dist_t Lane::getMaxSpeed()
   Author: Bruno C. da Silva
   Description: returns the lane's maximum speed (speed limit)
   Changelog:
************************************************************/

dist_t Lane::getMaxSpeed() {
  return vMax;
}







/************************************************************
   Function: dist_t Lane::getLength() {
   Author: Bruno C. da Silva
   Description: returns the lane's length (in cells)
   Changelog:
************************************************************/

dist_t Lane::getLength() {
  return cellMapSize;
}
/************************************************************
   Function: LaneSet * Lane::getParentLaneSet()
   Author: Bruno C. da Silva
   Description: returns a pointer to the laneset object in which
         the current lane is located
   Changelog:
************************************************************/

LaneSet * Lane::getParentLaneSet() {
  return parentLaneSet;
}





/************************************************************
   Function: Lane::Lane (LaneSet *_parentLaneSet) : TopologyElement ()
   Author: Bruno C. da Silva
   Description: this is the constructor to the Lane class. It basically
         sets: a) the laneset in which this lane is; and b) the cellMapSize
         (which is a private variable that can the read through
         Lane::getLength())

   Changelog:
************************************************************/

Lane::Lane (LaneSet *_parentLaneSet) : TopologyElement () {

	parentLaneSet = _parentLaneSet;
	parentLaneSet -> addLane (this);

	cellMapSize = getParentLaneSet() -> getParentSection() -> getLength();
	//steps size in seconds
	stepSize = (double)getParentLaneSet()->getStartNode()->getParentNetwork()->getParentTopology()->getIterationTime();
	cellMap.resize(cellMapSize, NULL);
	totalInsertedVehicles = 0;
	saturationFlow = 0;

}







/************************************************************
   Function: Lane::~Lane ()
   Author: Bruno C. da Silva
   Description: this is the destructor to the Lane class.
           Pretty useless so far.

   Changelog:
************************************************************/

Lane::~Lane () {

}
