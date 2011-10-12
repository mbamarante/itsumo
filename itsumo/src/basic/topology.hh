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
#ifndef __HAS_TOPOLOGY_H
#define __HAS_TOPOLOGY_H

#include "types.hh"
#include "utils.hh"                   // define classes for random number generators
#include "node.hh"

class Simulation;

static TRandomMotherOfAll randomGen(getpid());

class Topology : public TopologyElement {
private:
  string topologyName;

  network_vector_t childNetworks;
  //  node_pair_vector_t networkConnections;

  iteration_t timeInDay;

  
  // changelog (Bruno, 26-sept-2003): added more three definitions in the topology

  // size of a cell in Real World (tm)  meters
  cellSize_t cellSize;

  // duration of each iteration in Real World (tm) seconds
  iterationPeriod_t iterationTime;

  speed_t carsMaximumSpeed;

  Simulation *s;

public:

  speed_t getCarsMaximumSpeed();
  void setCarsMaximumSpeed(speed_t _maxSpeed);
   
  cellSize_t getCellSize();
  iterationPeriod_t getIterationTime();

  // changelog (Bruno, 26-sept-2003): added these
  void setCellSize(cellSize_t _cellSize);
  void setIterationTime(iterationPeriod_t _iterationTime);

  void setSensorTimeStep();  
  void setCellSize();
  void setIterationTime();



  string getName ();
  void setName (string);

  network_vector_t getChildNetworks ();

  void addNetwork (Network *);
  bool containsNetwork (Network *);
  int indexOfNetwork (Network *);

  void connectNetworkByNode (Node &, Node &);

  void setTimeInDay (iteration_t);
  iteration_t getTimeInDay ();

  void update ();
  Simulation * getSimulation();
  /*
    Constructors and Destructors
  */
  Topology (Simulation *_s);
  ~Topology ();
};

#endif
