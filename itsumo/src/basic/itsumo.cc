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
#include "itsumo.hh"
#include "driver-loader.hh"
#include <iostream>
using std::cout;
using std::endl;

using namespace std;

/** MAIN **/

int main ( int argc, char **argv ) {
	
	if (argc < 2) {
		cout 	<< "Itsumo " << ITSUMO_VERSION <<" \t (compiled on " 
			<< __DATE__ << " "<< __TIME__ 
			<< ")"<< endl
			<< "Usage: " << endl 
			<< "\t" << argv[0]
			<< " <simulation file>"
			<< endl;
		exit(0);
	}
	
	Simulation *s = new Simulation(argv[1]);

	cout << "Start Simulation..." << endl;

	string round = "0";
	string table_name = "";

	if (argc >= 3) {
		round = argv[2];
	}

	if (argc >= 4) {
		table_name = argv[3];
	}

	s->simulate(round, table_name);
	delete s;
}


