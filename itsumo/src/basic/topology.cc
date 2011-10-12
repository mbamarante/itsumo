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
#include "topology.hh" 


#include <iostream>
using std::cout;
using std::endl;

Simulation * Topology::getSimulation(){
	return s;
}

speed_t Topology::getCarsMaximumSpeed() {
  return carsMaximumSpeed;
}

void Topology::setCarsMaximumSpeed(speed_t _maxSpeed) {
  carsMaximumSpeed = _maxSpeed;
}



cellSize_t Topology::getCellSize() {
  return cellSize;
}

iterationPeriod_t Topology::getIterationTime() {
  return iterationTime;
}


void Topology::setCellSize(cellSize_t _cellSize) {
  cellSize = _cellSize;
}

void Topology::setIterationTime(iterationPeriod_t _iterationTime) {
  iterationTime = _iterationTime;
}




/**********************************************************
  Method: Topology::getChildNetworks ();
  Author: John Kliff Jochens
  Date: May 30th, 2003
  Description: returns a copy of the childNetworks contained by the Topology

  Changelog: 
5/6/2003, John:
Changed to return a copy, not the actual instance, 
of the childNetworks vector.
***********************************************************/

network_vector_t Topology::getChildNetworks () {
  return network_vector_t (childNetworks); // returns a copy
}

/**********************************************************
  Method: void Topology::addNetwork (Network & _n);
  Author: John Kliff Jochens
  Date: May 30th, 2003
  Description: adds the network _n to the Topology. Connections 
between different networks are done with calls to 
Topology::connectNetworkByNode (Node &, Node &);
  
  
  Changelog:
12/6/2003, John:
Changed the type of the iteration index from int to unsigned int to avoid compilation warnings

***********************************************************/

void Topology::addNetwork (Network* _n) {
  childNetworks.push_back (_n);

}

/**********************************************************
  Method: Topology::containsNetwork (Network &_n);
  Author: John Kliff Jochens
  Date: May 30th, 2003
  Description: returns true if the Topology has the network _n;

  Changelog:
12/6/2003, John:
Changed the type of the iteration index from int to unsigned int to avoid compilation warnings

***********************************************************/

bool Topology::containsNetwork (Network * _n) {
  for (unsigned int i = 0 ; i < childNetworks.size () ; i++) 
    if (childNetworks[i] == _n)
      return true;

  return false;
}

/**********************************************************
  Method: Topology::indexOfNetwork (Network &_n);
  Author: John Kliff Jochens
  Date: May 30th, 2003
  Description: returns the index of the Network _n supplied as 
argument inside the vector.
In case the Network is not present, returns -1;

  Changelog: 
12/6/2003, John:
Changed the type of the iteration index from int to unsigned int to avoid compilation warnings

***********************************************************/

int Topology::indexOfNetwork (Network * _n) {
  for (unsigned int i = 0; i < childNetworks.size () ; i++)
    if (childNetworks[i] == _n)
      return i;
  return -1;
}
/************************************************************
  Method: void Topology::connectNetworkByNode ( Node &, Node &)
  Author: John Kliff Jochens
  Date: 
  Description: 

  Changelog: 
************************************************************/

void Topology::connectNetworkByNode ( Node &,  
				      Node &) {

}

/**********************************************************
  Method: string Topology::getName ();
  Author: John Kliff Jochens
  Date: May 30th, 2003
  Description: Returns this Topology's name

  Changelog:
***********************************************************/

string Topology::getName () {
  return topologyName;
}

/**********************************************************
  Method: void Topology::setName (string s)
  Author: John Kliff Jochens
  Date: May 30th, 2003
  Description: Sets this Topology's name.

  Changelog:
***********************************************************/

void Topology::setName (string s) {
  topologyName = s;
}

/**********************************************************
  Method: void Topology::setTimeInDay (iteration_t);
  Author: Omar "Gambá" Balbuena
  Date: 2003-09-08
  Description: sets the time of the day for that topology
  Changelog:
***********************************************************/
void Topology::setTimeInDay (iteration_t tod) {
	timeInDay = tod;
}

/**********************************************************
  Method: iteration_t Topology::getTimeInDay ();
  Author: Omar "Gambá" Balbuena
  Date: 2003-09-08
  Description: gets the time of the day for that topology
  Changelog:
***********************************************************/
iteration_t Topology::getTimeInDay () {
	return timeInDay;
}

/**********************************************************
  Method: void Topology::update ();
  Author: Omar "Gambá" Balbuena
  Date: 2003-09-08
  Description: updates topology data
               (FIXME: it should do more or be renamed :P)
  Changelog:
***********************************************************/
void Topology::update () {
	timeInDay++;
}

/***  Constructors and Destructors  ***/

/**********************************************************
  Method: contructor Topology::Topology ();
  Author: John Kliff Jochens
  Date: May 31th, 2003
  Description: Default constructor. Calls the superclass' constructor 
and inserts this instance in its elements vector.

  Changelog:
	Omar 2003-09-08 added timeInDay
***********************************************************/

Topology::Topology (Simulation *_s) : TopologyElement (), timeInDay(0) {
s = _s;

}

/**********************************************************
  Method: destk
ructor Topology::~Topology ();
  Author: John Kliff Jochens
  Date: May 31th, 2003
  Description: Default destructor.

  Changelog:
***********************************************************/

Topology::~Topology () {

  //  ~childNetworks ();
  //  ~networkConnections ();
}

/**********************************************************/
