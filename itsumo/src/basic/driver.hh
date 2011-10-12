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
#ifndef __HAS_DRIVER_H
#define __HAS_DRIVER_H

#include <iostream>
#include <fstream>

#include "types.hh"


class Driver {
	protected:
		string nick;

		driver_id_t id;

		double deceleration_probability;

		//*** Statistical data for travel times ***
		int stepInserted;    // Step number when car was inserted
		int stepRemoved;     // Step number when car was removed
		int forecastSteps;	 // Travel time forecasted at travel start
		int replanCalls;	 // Replans calls counter (changing the route or not)
		int replanChanges;	 // Replans that changes the route
		int tolerance;		 // delay tolerance before replan (100% delayed = 2, 200% delayed = 3...)
		int pathSize;		 // number of route links
		int lastLanesetReplan; // avoid replanning more than once at current laneset
		int numberOfTrips;	// count number of trips
		double distanceTravelled; // distance that driver travelled

	public:
		// a vector with the IDs of all cars being driver
		// by this driver
		driven_cars_vector_t drivenCars;

		ofstream *logger;
		void setLogger(ofstream *logger_);

                string driver_class_name;
		void setDeceleration(float dece);

		void setNick(string _nick);
		string getNick();

		static driver_id_t nextDriverId;

		driver_id_t getId();

		// the car is going to call this method in order to ask the driver for his decision
		virtual decision_t takeDecision (Car &) =0;

		// given that the car is going to leave the current laneset in the next iteration,
		// this method is responsable for informing to what lane-laneset the driver wants
		// to go
		virtual lane_laneset_pair_t chooseNextLane_Laneset(Car *theCar) =0;

		// used so the car can ask the driver to initialize the *extraData structure (see car.hh)
		virtual void initializeExtraData(Car *theCar) =0;
		// invoked when a Car is beeing deleted to clear eventualy used memory.
		virtual void deleteExtraData(Car *theCar) =0;


		/* special driver's functions */
		virtual bool readyIn (int step) =0;
		virtual bool readyOut(int step) =0;
		virtual void force2Out(bool out) {} // defaults to no action
		virtual bool isBack() { return false; } // defaults to no action
		virtual void setBack(bool back) {} // defaults to no action
		virtual bool back2Wait() { return false; }
		virtual void force2Wait(bool wait) {}
		virtual void reActivate() {} // defaults to no action
		virtual bool getReplanner() {} // defaults to no action
		virtual bool getGlobal() {}
		virtual bool getCommunication() {}
		virtual int getNumberOfTrips() {}
		virtual double getDistanceTravelled() {}

		virtual int  where   (int step) =0; 
		virtual void update  (int step) =0;		
		virtual void set_options(vector < pair < string, string> > _options) =0;
		virtual void removedFromNet() {}	// defaults to no action

		virtual void logEvent(event_t, topology_element_id_t) =0;

		//*** Handlers for Statistical data for travel times ***
		int getStepInserted() {return stepInserted;};
		int getStepRemoved() {return stepRemoved;};
		int getReplanCalls() {return replanCalls;};
		int getReplanChanges() {return replanChanges;};
		int getTolerance() {return tolerance;};
		int getPathSize() {return pathSize;};
		void setStepInserted(int stp) {stepInserted = stp;};
		void setStepRemoved(int stp) {stepRemoved = stp;};

		Driver (string);

		virtual ~Driver ();

};


#endif // __HAS_DRIVER_H
