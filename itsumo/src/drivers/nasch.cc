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
#include "nasch.hh"
using namespace std;



/************************************************************
   Function: void Nasch::initializeExtraData(Car *theCar)
   Author: Bruno C. da Silva 
   Description: the car calls this method in order to ask the 
         driver to initialize the *extraData structure (see car.hh)

   Changelog:
************************************************************/

void Nasch::initializeExtraData(Car *theCar) {
//   cout << "inicializando coisas genericas.." << endl;
//   theCar -> setExtraData((void *)2)
//   cout << "valor generico: " << (int)theCar -> getExtraData() << endl;
}



/************************************************************
   Function: void Nasch::deleteExtraData(Car *theCar)
   Author: Bruno C. da Silva 
   Description: deletes the extraData structure

   Changelog:
************************************************************/

void Nasch::deleteExtraData(Car *theCar) {
}




/************************************************************
   Function: lane_laneset_pair_t Nasch::chooseNextLane_Laneset(Car *theCar)
   Author: Bruno C. da Silva 
   Description: this function is responsable for
        choosing a random destination lane and laneset according to 
        the turning probabilities; first the destination
        laneset is chosen, and then a lane is picked
        at random, with equal probabilities

   Changelog:
************************************************************/

lane_laneset_pair_t Nasch::chooseNextLane_Laneset(Car *theCar) {

  float dice;
  int found=0;

  //printf("escolhendo para o carro %d\n", theCar->getInternalId());
  
  // stupid initialization to avoid warnings
  LaneSet *destinationLaneset=NULL;   

  // next node is the node in front of us, at the end of the current street
  LaneSet *currentLaneSet = theCar->getCurrentLane()->getParentLaneSet();

  dice = randomGen.IRandom(0, 100);

  turning_probabilities_vector_t tProbs = currentLaneSet->getTurningProbabilities ();
  turning_probabilities_vector_t::iterator itProbs;

  float curProb = 0;
  for (itProbs = tProbs.begin(); itProbs != tProbs.end(); itProbs++) {
    if (itProbs->second == 0.0)
      continue;
    curProb += itProbs->second;
//     printf("car #%d    dice=%f, v=%f, %d\n", theCar->getInternalId(), dice, curProb, dice <= curProb);
    if (dice <= curProb) {
//       printf("\tcar %d    achou!\n", theCar->getInternalId());
      destinationLaneset = itProbs->first;
      found=1;
      break;
    }
  }

  // detects dead-end streets (0% probability of going to all possible next lanesets)
  // Selects the same laneset (could be anything), because HOPEFULLY
  // it will be removed by a sink in the node at the end of the street
  if (!found) {
//    if (tProbs.size() == 0 ) {

      // 	printf("hum. %d is deadend. Car %d going to same
      // 	laneset and hoping to be removed by a sink at the end
      // 	of the stret\n", 
      // 	       theCar->getCurrentLane()->getParentLaneSet()->getId(),
      // 	       theCar->getInternalId());
      
	destinationLaneset=currentLaneSet;

//    }
    //else {
    //  destinationLaneset = tProbs[tProbs.size()-1].first;
    //  printf("Problem in the turning probs for laneset %d ", theCar->getCurrentLane()->getParentLaneSet()->getId());
    //  printf("    Selecting last possible destination = laneset #%d\n", destinationLaneset -> getId());
    //}

  }

  lane_vector_t possibleLanes = destinationLaneset->getChildLanes();
  // now we choose at random one target lane (a number between 0
  // and 'possibleLanes' - 1
  long int randomLane = randomGen.IRandom(0, possibleLanes.size()-1);

#ifdef DEBUG_NASCH
  cout << "--> car " << theCar->getInternalId() << " ";
  cout << theCar->getCurrentLane()->getId() << "->";
  cout << (possibleLanes[randomLane])->getId();
  cout << " (" <<  randomLane << "/" << possibleLanes.size()-1;
  cout << ")" << " in laneset " << destinationLaneset->getId();
  cout << " (dice=" << dice << "%)" << endl;
#endif

//   printf("\t%d -> %d\n", theCar -> getCurrentLane() -> getParentLaneSet() -> getId(), destinationLaneset->getId());
//   printf("\tlane %d\n", possibleLanes[randomLane]->getId());

  return make_pair(possibleLanes[randomLane], destinationLaneset);

  }









/************************************************************
   Function: bool Nasch::isLaneChangeSecure(Lane *_targetLane) 
   Author: Bruno C. da Silva 
   Description: when the driver decides that a lane change
        could be a good move, he asks this funcion weather or not
        this lane change is safe. If it is, the function returns
        true. Otherwise, the intended lane change could cause an
        accident and should be avoided by the driver (on 'takeDecision').

   Changelog:
************************************************************/

bool Nasch::isLaneChangeSecure(Car &myCar, Lane *_targetLane) {

  // we're considering that the driver first moves ahead some cells,
  // according to his car's speed, and THEN he changes lanes. For
  // this reason, the following code checks if there'll be crash
  // based on my car's and the other car's forward displacement. 

  dist_t currentCell = myCar.getCurrentCell();
  speed_t currentSpeed = myCar.getCurrentSpeed();

  dist_t rearGapInTargetLane = _targetLane -> getRearGapFromCell(currentCell);
  dist_t frontalGapInTargetLane = _targetLane -> getFrontalGapFromCell(currentCell);

  
  dist_t otherCarLocation; // this variable describes the cell where the car in _targetLane 
                           // is located:
                           // |0123456789|
                           // |---x------|    ex: y is trying to move to the lane where x is. 
                           // |------y---|    'otherCarLocation' will be set to "4" 
                           //                 (4th cell in the lane)

  speed_t otherCarSpeed;   // in the example above, this variable defines x's speed

  // checks if there is a car behind us in _targetLane
  if (rearGapInTargetLane != (_targetLane -> getLength() + 1))  { 
    otherCarLocation = currentCell - rearGapInTargetLane - 1;
    otherCarSpeed = _targetLane -> queryCarSpeed(otherCarLocation);


    // checks if a lane change would cause a crash with the car behind us, in the other lane
    // (the other car would 'run over' our car if we change to _targetLane)
    if ((otherCarLocation + otherCarSpeed) >= (currentCell + currentSpeed)) {
      // we detected a crash, so the function should return false
      return false;
#ifdef DEBUG_SHOW_DRIVER_CAR_INFO
      cout << "Can't go to lane " << _targetLane -> getId() << " because I would";
      cout << " crash into the car " << _targetLane -> queryCarId(otherCarLocation) << endl;
#endif
    }
    
    else {
      // ok, there'll be no crash caused by the car behind us..
      // now we verify if there is a car in front of us in _targetLane
      if (frontalGapInTargetLane != (_targetLane -> getLength() + 1))  { 
	otherCarLocation = currentCell + frontalGapInTargetLane + 1;
	otherCarSpeed = _targetLane -> queryCarSpeed(otherCarLocation);
	
	// checks if a lane change would cause us to crash into a car that
	// is in front of us, in the other lane
	if ((currentCell + currentSpeed) >= 
	    (otherCarLocation + otherCarSpeed)) {

#ifdef DEBUG_SHOW_DRIVER_CAR_INFO
	  cout << "Can't go to lane " << _targetLane -> getId() << " because I would";
	  cout << " crash into the car " << _targetLane -> queryCarId(otherCarLocation) << endl;
#endif
	  // we detected a crash into the car in front of us
	  return false;
	}
	else
	  // there is a car behind and a car in front of us, but no crash will occur
	  return true;
      }
      else
	// there is a car in behind us (and no crash), but there is no car in front of us
	return true;
    }
  }

  // there is no car behind us in _targetLane, but we must test for a car
  // in front of us too (in the other lane)
  else {

    // let's verify if there is a car in front of us in _targetLane
    if (frontalGapInTargetLane != (_targetLane -> getLength() + 1))  { 
      otherCarLocation = currentCell + frontalGapInTargetLane + 1;
      otherCarSpeed = _targetLane -> queryCarSpeed(otherCarLocation);
      
      // checks if a lane change would cause us to crash into a car that
      // is in front of us, in the other lane
      if ((currentCell + currentSpeed) >= (otherCarLocation + otherCarSpeed)) {
	// the car would crash into the car in front of us
	return false;
	
#ifdef DEBUG_SHOW_DRIVER_CAR_INFO
	cout << "Can't go to lane " << _targetLane -> getId() << " because I would";
	cout << " crash into the car " << _targetLane -> queryCarId(otherCarLocation) << endl;
#endif
      }
      else
	// ok, lane change is secure
	return true;
    }

    else
      // there is neither a car in front of us nor behind, so the lane change is secure
      return true;
  }
  
}






/************************************************************
   Function: speed_t Nasch::naschCalculateNewSpeed(speed_t currentSpeed, 
                                            speed_t carMaxSpeed, 
                                            speed_t laneMaxSpeed,
                                            dist_t currentGap,
                                            float lane_decel)
   Author: Bruno C. da Silva 
   Description: this function implements a method to calculate
         the next speed that a car using the Nasch model will
         assume. The speed is a function of the current speed,
         the maximum allowed speed (for the car and for the lane) and the 
         frontal gap.
************************************************************/
speed_t Nasch::naschCalculateNewSpeed(speed_t currentSpeed, speed_t carMaxSpeed, 
				      speed_t laneMaxSpeed, dist_t currentGap, float lane_decel) {

  // Acceleration: v = Min(v+1, car_vmax, lane_vmax, gapAhead)
  // Position: p = p + v

  speed_t next_speed = 0;

  //cout << "Dece: " << deceleration_probability << endl;


  float dice = randomGen.Random();
  float dece;

  // if the user specified a specific deceleration for the lane, that
  // deceleration overrides the network default deceleration
  if (lane_decel != 0.0)
    dece = lane_decel;
  else
    dece = deceleration_probability;

//   printf("dice=%f, decel prob=%f\n", dice, dece);

  if (dice <= dece) {
    next_speed = max(0, currentSpeed-1);
//     cout << " --> random deceleration (" << currentSpeed << "->" << next_speed << ")" << endl;
  }
  else {
//     cout << "    nope" << endl;
    next_speed = min(currentSpeed + 1, min(laneMaxSpeed, min(carMaxSpeed, currentGap)));
  }

  return next_speed;
  
}










/************************************************************
   Function: decision_t Nasch::takeDecision (Car &car)
   Author: Bruno C. da Silva 
   Description: the driver's (Nasch) decision about what movement
           to do is based only on lane changes in the current laneSet,
           trying to maximize the speed. Currently the driver is not 
           considering good lane changes in respect to his goals (ex: reach 
           the nth lane on the other laneset). The only aspects this
           function takes into account is which lane change could
           maximize the car's speed. The driver's decision is returned
           in a decision_t struct (see types.hh)

   Changelog:
************************************************************/

  // ATTENTION: lanesProbabilities is not yet implemented, so our
  // driver (nasch) is going to take his decision based only on
  // lane changes in the current laneSet (trying to maximize the speed)
  // Currently the driver is not considering good lane changes in respect
  // to his goals (ex: reach the nth lane on the other laneset)

  // in the future i have to change takeDecision so the driver can change lanes inside
  // its current laneset based on the probabilities




decision_t Nasch::takeDecision (Car &car) {



  decision_t decision;
  
  dist_t leftGap, rightGap, frontalGap;
  speed_t leftSpeed=0, rightSpeed=0, frontalSpeed=0;
  Lane *leftLane, *rightLane;

#ifdef DEBUG_SHOW_DRIVER_CAR_INFO
  cout << ">> taking decision << (car " << car.getInternalId() << ")" << endl;
  cout << "currentLane: " << car.getCurrentLane()->getId();
  cout << ", currentCell: " << car.getCurrentCell() << endl;
#endif

  frontalGap = car.getCurrentLane()->getFrontalGapFromCell(car.getCurrentCell());

  // Bruno 1207 DEBUG
//   cout << "\tTah na cell " << car.getCurrentCell() << " e gap eh " << car.getCurrentLane()->getFrontalGapFromCell(car.getCurrentCell()) << endl;

  frontalSpeed = naschCalculateNewSpeed(car.getCurrentSpeed(), car.getMaxSpeed(), 
					car.getCurrentLane()->getMaxSpeed(), frontalGap, 
					car.getCurrentLane()->getDecelerationProbability()
					);

  if (((leftLane=car.getCurrentLane()->getLeftLane()) != NULL)  && (car.getCurrentSpeed() != 0)) {
    // there is a left lane and the car is not stopped
    leftGap = leftLane->getFrontalGapFromCell(car.getCurrentCell());

    leftSpeed = naschCalculateNewSpeed(car.getCurrentSpeed(), car.getMaxSpeed(), 
					leftLane->getMaxSpeed(), leftGap,
					leftLane->getDecelerationProbability()
					);

    // if changing lanes to the left may cause a crash with other car,
    // then we can't do it.. and the car can't change lanes if its speed is zero
    if (! (isLaneChangeSecure(car, car.getCurrentLane()->getLeftLane() )))
      leftSpeed=-1;
  }


  if (((rightLane=car.getCurrentLane()->getRightLane()) != NULL) && (car.getCurrentSpeed() != 0)) {
    // there is a right lane and the car is not stopped
    rightGap = rightLane->getFrontalGapFromCell(car.getCurrentCell());
    rightSpeed = naschCalculateNewSpeed(car.getCurrentSpeed(), car.getMaxSpeed(), 
					rightLane->getMaxSpeed(), rightGap,
					rightLane->getDecelerationProbability()
					);


    // if changing lanes to the right may cause a crash with other car,
    // then we can't do it.. and the car can't change lanes if its speed is zero
    if (! (isLaneChangeSecure(car, car.getCurrentLane()->getRightLane() )))
      rightSpeed=-1;

  }



  if (leftSpeed > frontalSpeed) 
    if (leftSpeed > rightSpeed) {
      decision.speed = leftSpeed;
      decision.lane = leftLane;

#ifdef DEBUG_SHOW_DRIVER_CAR_INFO
      cout << "decision: take the LEFT lane (id = " << decision.lane->getId() << ")" <<  endl;
      cout << "   (speed will change from " << car.getCurrentSpeed();
      cout << " to " << leftSpeed << ")" << endl;
#endif
    }
    else {
      decision.speed = rightSpeed;
      decision.lane = rightLane;

#ifdef DEBUG_SHOW_DRIVER_CAR_INFO
      cout << "decision: take the RIGHT lane (id = " << decision.lane->getId() << ")" <<  endl;
      cout << "   (speed will change from " << car.getCurrentSpeed();
      cout << " to " << rightSpeed << ")" << endl;
#endif
    }
  else
    if (rightSpeed > frontalSpeed) {
      decision.speed = rightSpeed;
      decision.lane = rightLane;

#ifdef DEBUG_SHOW_DRIVER_CAR_INFO
      cout << "decision: take the RIGHT lane (id = " << decision.lane->getId() << ")" <<  endl;
      cout << "   (speed will change from " << car.getCurrentSpeed();
      cout << " to " << rightSpeed << ")" << endl;
#endif
    }
    else {
      decision.speed = frontalSpeed;
      decision.lane = car.getCurrentLane();

#ifdef DEBUG_SHOW_DRIVER_CAR_INFO
      cout << "decision: stay on the CURRENT lane (id = " << decision.lane->getId() << ")" <<  endl;
      cout << "   (speed will change from " << car.getCurrentSpeed();
      cout << " to " << frontalSpeed << ")" << endl;
#endif
    }
  
  // Bruno 1207 DEBUG
//   cout << "\t\tDecision speed " << decision.speed << endl;
//   cout << "\t\tDecision lane " << decision.lane->getId() << endl;
  return decision;


}







/************************************************************
   Function: Nasch::Nasch()
   Author: Bruno C. da Silva 
   Description: this is the constructor to the Nasch class. It just
         sets the driver's id.

   Changelog:
************************************************************/



Nasch::Nasch(string driver_class_name) : Driver(driver_class_name) {

}


void Nasch::set_options(vector < pair < string, string> > _options)
{
	for (vector < pair <string,string> >::iterator i = _options.begin(); i != _options.end();i++)
	{
		//cout << (*i).first << "-->" << (*i).second << endl;
		if ((*i).first == "deceleration") setDeceleration(atoi((*i).second.c_str()));
	}

}


/************************************************************
   Function: Nasch::~Nasch()
   Author: Bruno C. da Silva 
   Description: this is the destructor to the Nasch class.
           Pretty useless so far.

   Changelog:
************************************************************/

Nasch::~Nasch() {
  //  cout << "destruiu um nasch driver de id: "  << endl;;
}


bool Nasch::readyIn (int step){ return true; }
bool Nasch::readyOut(int step){ return false; }
int  Nasch::where   (int step){ return -1; }

