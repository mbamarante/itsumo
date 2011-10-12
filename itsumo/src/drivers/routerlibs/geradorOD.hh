/*
    Copyright (C) 2010 by Ana Lucia Bazzan (bazzan@inf.ufrgs.br)
                          Alexander J. Benavides (ajbenavides@inf.ufrgs.br)
                          Tiago Sommer
            http://www.inf.ufrgs.br/maslab/traffic/itsumo/

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

#ifndef _HAS_GERADOROD_H
#define _HAS_GERADOROD_H

#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cmath>
#include <iostream>
//#include "tiny/tinyxml.h"

#include "../../basic/network.hh"
using namespace std;
/*
 * geradorOD.cpp
 *
 *  Created on: Dec 1, 2009
 *      Author: tiago
 */

struct od_driver{
	int id;
	int source;
	int sink;
};

struct matod{
	int source;
	int sink;
	int propor;
};

class geradorOD {


private:
	//variaveis do gerador uniforme
	vector<int> nodes;
	vector<double> io;
	vector<od_driver> od_drivers;
	int id;
	int numOdDrivers;

	//variaveis da matrixOD

	vector<matod> matrixod;

public:

	geradorOD(int numV);
	virtual ~geradorOD();

	//setter e getters
	int getNumVehicles();

	//metodos
	int getSource();
	int getSink(int source);
	int getSizeOdDrivers();
	void addOdDriver(int source, int sink);
	void geraODM(string file);
	unsigned int geraSeed(int seed);
	void getNodos(node_vector_t nodes);

	//metodos da matrixOD

	//setters e getters
	void setIO(vector<double> v);
	void seeMultMatrix();
	int getMatrixPorc();
	matod getMatrixAt(int i);

	//metodos
	matod getRandomOD();
	void loadMatrix(string file);
	vector<string> split(string s, string separator);
	void seeSplit(vector<string> vecs);
	void medirPorcentagens(string file);
};

#endif // _HAS_GERADOROD_H

