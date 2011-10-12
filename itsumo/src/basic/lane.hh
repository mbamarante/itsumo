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
#ifndef __HAS_LANE_H
#define __HAS_LANE_H
#include <numeric>


#include "types.hh"

#include "topologyelement.hh"
#include "laneset.hh"
#include "car.hh"


class Lane : public TopologyElement {
private:

  LaneSet * parentLaneSet;

  /* vector of pointers to the cars in the lane;
     the pointer associated with the n-th cell can be found at the n-th position 
     of the vector
  */
  car_vector_t cellMap;

  
  Lane * leftLane;
  Lane * rightLane;


  // changelog (Bruno): added this field in order to know how many cells there are
  // In fact, it's a copy of the value contained in the Lane's section
  dist_t cellMapSize;

  float decelerationProbability;

  // maximum speed allowed by this lane
  speed_t vMax;
  
  
  
  vector<int> insertedVehicles;
  int totalInsertedVehicles;
  int saturationFlow;
  iteration_t currentStep;
  double stepSize; 

public:
  

  int position_in_laneset;

  LaneSet * getParentLaneSet ();

  // changelog (Bruno): added functions to return gap relative
  // to some cell
  dist_t getFrontalGapFromCell(dist_t);
  dist_t getRearGapFromCell(dist_t _cell);

  // methods to set and return the position of the lane inside the laneset (if it's the first, second, etc);
  // the position is measured left->right (1st lane is the leftmost)
  void setPosition(int _pos);
  int getPosition();

  void setDecelerationProbability(float _decel);
  float getDecelerationProbability();


  Lane * getLeftLane();
  Lane * getRightLane();

  dist_t getZone(dist_t _position);

  string printLaneStatus(int iteration);

  
  void setLeftLane(Lane * _lLane);
  void setRightLane(Lane * _rRight);

  dist_t getMaxSpeed();


  // changelog (Bruno 26-sept-2003): added a method to return the lane's maximum speed
  void setMaxSpeed(dist_t _maxSpeed);

  dist_t getLength();
  int moveCar(dist_t From, dist_t To); 
  int putCar(Car *, dist_t Where);
  int removeCar(dist_t From);


  // TODO: these functions should return an id, not an int (Bruno)
  int queryCarId(dist_t cell);
  int queryCarSpeed(dist_t cell);
  Car * queryCarPtr(dist_t cell);
  
  unsigned int getNumberOfVehicles();
  double getAvgSpeed();
  double getDensity();
  int getStoppedCars();
  
  //new methods
  bool isControlled();//returns true if there is a traffic light at its end
  double getAvgSpeedKMH(); //speed in Km/h
  double getDistanceTravelled(); // vehicles-Km/hour  
  speed_t getUsedMaxSpeed();  // cell/step 
  int getSaturationFlow(); //vehicles / hour of green
  double getEntryRate(); //vehicles  
  double getEntryFlow(); //vehicles/hour
  double getDegreeOfSaturation(); //percentage
  double getCruiseTime(); //time steps
  double getMeanCruiseTime(); //time steps
  double getDelayTime(); //seconds
  unsigned int getQueueSize(); //number of vehicles
  
  

  // returns a pointer to a nearby car (considering that the reference point is our current cell)
  Car * getNearbyCar(dist_t _currentCell, int which);


  /*
    Constructors and Destructors
  */
  Lane (LaneSet *);
  ~Lane ();
};

#endif
