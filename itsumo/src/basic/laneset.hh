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
#ifndef __HAS_LANESET_H
#define __HAS_LANESET_H

#include "types.hh"

#include "topologyelement.hh"
#include "section.hh"
#include "node.hh"

class LaneSet : public TopologyElement {
private:


  // this laneset contains a list of every possible target-lane and the turning
  // probability associated
  turning_probabilities_vector_t turningProbabilities;

  lane_vector_t childLanes;
  Section * parentSection;
  Node* startNode;
  Node* endNode;


public:


  void sortChildLanes(); // sorts child lanes according to their
                         // order in the section

  int position;

  vector<double> vecDensity;


  // methods to set and return the position of the laneset inside the section (if it's the first, second, etc);
  // the position is measured left->right (1st laneset is the leftmost)
  int getPosition();
  void setPosition(int _position);

  int getNumberOfLanes();
  int getCapacity();
  int getNumberOfVehicles();
  //regra de 3 para descobrir a porcentagem de ocupacao do link!!!
  int getOcupationLink();
  double getDensity();
  dist_t getMaxVMax();
  Section * getParentSection ();
  bool setStartNode (Node *);
  Node * getStartNode ();
  bool setEndNode (Node *);
  Node * getEndNode ();

  turning_probabilities_vector_t getTurningProbabilities();
  void clearTurningProbabilities();

  void addLane (Lane *);
  void addTurningProb(LaneSet *, double);

  lane_vector_t getChildLanes ();

  vector<int> getDriversId();
  /*
    Constructors and Destructors
  */

  LaneSet (Section *);
  ~LaneSet ();
};


#endif

