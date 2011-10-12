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
#ifndef __HAS_CAR_H
#define __HAS_CAR_H

#include "types.hh"

#include "topologyelement.hh"
#include "lane.hh"


class Car : public TopologyElement {
private:

  // pointer to the lane where the car is
  Lane *pCurrentLane;   // must be consistent with the information of the Lane
  
  // index of the cell where the car is
  dist_t currentCell;   // must be consistent with the information of the Lane
  dist_t lastCell;

  accel_t currentAccel;    // isso vem do driver decision
  speed_t currentSpeed;    // isso vem do driver decision

  Driver *pDriver;     // stuff setted by the constructor
  speed_t maxSpeed;    // stuff setted by the constructor
  accel_t maxAccel;    // stuff setted by the constructor

  void *extraData;

	Car * * active_place;

public:


  static void removeFromActiveCarsList(Car *car);
  static void insertInActiveCarsList(Car *newCar);

  static Car * readFromInactiveCarsList();
  static void insertInInactiveCarsList(Car *newCar);

  // print info on how many inactive/active cars there are
  static string showBuffersStatus();
    //  static void showBuffersStatus();


  decision_t driverDecision;


  Car * getNearbyCar(int which);

  // TODO: these are meant to be private methods. THEY SHOULD NOT BE USED DIRECTLY.. 
  // 'move' should be used instead (Bruno)
  Lane* getCurrentLane ();
  void setCurrentLane (Lane *);
  dist_t getCurrentCell ();
  dist_t getLastCell ();
  void setCurrentCell (dist_t);
  void setLastCell (dist_t);

  speed_t getMaxSpeed ();
  void setMaxSpeed (speed_t);

  accel_t getCurrentAccel();
  void setCurrentAccel(accel_t);
  accel_t getMaxAccel();
  void setMaxAccel(accel_t);


  // When a car is changing lanesets (ex: it reached the end of a
  // street), this functions finds out on what cell of the destination
  // street the car should be put based on its current speed, current
  // cell and the gap in the destination lane (the gap from the start
  // of the lane, that is). Additionally, the current speed may have to
  // be updated, because the car may have to break due to some
  // obstacle on the target lane (ex: another car)  (Bruno)
  dist_t findOutTargetCell();


  // all movements should be done using this method!
  int Move(Lane *, dist_t);

  int insertInLane(Lane *, dist_t, speed_t, accel_t);

  // bureaucratic methods
  Driver* getDriver ();
  void setDriver (Driver *);


  speed_t getCurrentSpeed ();
  void setCurrentSpeed (speed_t);

  void setExtraData (void *);
  void *getExtraData ();



  // this functions sets the driverDecision after asking
  // the driver what he wants to do (currently Nasch is implemented) (Bruno)
  void updateDecision (); // this is gonna set driverDecision


  // this function commits the driver's decision. Check Car.cc
  // for a more compreensive documentation (Bruno)
  void commitDecision ();

  /*
    Constructors and Destructors
  */



  // A new car in NOT inserted automatically into a lane.
  // You must create it and then use insertInLane (to put the car in the 
  // topology), and later on you should only use the 'move' method. (Bruno)
  Car (Driver *, speed_t, accel_t);


  ~Car ();


};

#endif
