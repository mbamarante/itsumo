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
#ifndef __HAS_DRIVER_LOADER_H
#define __HAS_DRIVER_LOADER_H

#include <iostream>
#include <fstream>

#include "car.hh"
#include "driver.hh"
#include "calibrator.hh"
#include "network.hh"

#include "sqlite3.h"
//#include "my_global.h" // Include this file first to avoid problems
#include "mysql/mysql.h" // MySQL Include File

class DriverLoader {
	private:
		Network *n;
		vector<Driver *> running; // drivers on the network
		vector<Driver *> waiting; // drivers waiting
		int amount; // amount of drivers to insert
		bool insertDriver(Driver *d, int step);
		dist_t bestPlaceOnLane(Lane *l);
		vector < pair <string,string> > options;


	public:
		string printTravelTimesSensor();
		string printAvgTravelTimesSensor();
		DriverLoader(Network *_n,string name, string nick, int num, vector< pair <string,string> > vet, vector< Route > rts, ofstream *driverLogger);
		void update(int step, bool reduceOccupationSize, MYSQL *connect, int odsize, string networkname, string round, string global_odfile, string tabela);
		~DriverLoader();
		bool removeDriver(Driver *,int step);
		vector <Driver *> getWaitingDrivers() { return waiting;};
		bool linkLimit(Driver *d, int step);
		void prepareNewEpisode();
		void prepareNewRound();
};


#endif
