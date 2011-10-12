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
#ifndef __HAS_NODE_H
#define __HAS_NODE_H

#include "types.hh"

#include "topologyelement.hh"
#include "trafficlight.hh"
#include "calibrator.hh"


class Node : public TopologyElement {
private:
  // pointers to the sections connected to this node
  section_vector_t sections;
  Network *parentNetwork;

  point_t coordinates;
  string nodeName;

  TrafficLight *trafficLight;
  // tells whether or not the node is used to connect sections
  
  iteration_t currentStep; //value set by the the "update" method 
  

public:

  calibrator_vector_t calibrators;
  int getNumberOfSections();
  Network* getParentNetwork ();
  section_vector_t getSections ();
  void addSection (Section*);
  void removeSection (Section*);

  float getXcoord();
  float getYcoord();

  double getLatCoord();
  double getLonCoord();

  string getName();
  void setName(string _name);

  string getSectionsNames (string separator);

  // trafficLight control
  void setTrafficLight (TrafficLight *);
  TrafficLight *getTrafficLight();

  // changelog bruno (16-10-2004): now the direction is (lane->laneset),
  // so I changed the last parameters of the method
  bool allowMove (Lane *, LaneSet *);


  int signalToSinks(Car *car);

  // changelog (Bruno): this function is responsable for updating
  // the node's trafficlights (if any), and for updating all 
  // calibrators and sensors
  void update(iteration_t currentIteration);

  void addCalibrator(Calibrator *_newCalib);
  
  //new method (08/2008)
  iteration_t getCurrentStep();
  
  

  /*
    Constructors and Destructors
   */
  Node (Network*, float xCoord, float yCoord);
  Node (Network*, float xCoord, float yCoord, float lng, float lat);
  ~Node ();

};

#endif
