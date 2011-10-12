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
#ifndef __HAS_SECTION_H
#define __HAS_SECTION_H

#include "types.hh"

#include "topologyelement.hh"
#include "network.hh"

class Section : public TopologyElement {
private:

  string name;

  // length is measured in dist_t units (probably cells)
  dist_t length;
  laneSet_vector_t childLaneSets;

  bool preferencial;

  // reference to the nodes where this section is connected
  Node* nodes[2];
  Network* parentNetwork;

  // the id of the street where this section is located
  topology_element_id_t parentStreetId;

public:

  void sortChildLaneSets(); // sorts child lanesets according to their
			    // order in the street


  void setPreferencial(bool _preferencial);
  bool isPreferencial();

  int getNumberOfLaneSets();
  int getNumberOfLanes();

  // changelog (Bruno 26-sept-2003): returns and sets the id
  //of the street where this section is.
  topology_element_id_t getStreetId();
  void setStreetId(topology_element_id_t _streetId);

  string getName ();
  void setName (string);
  dist_t getLength ();
  void setLength (dist_t);

  void getDelimitingNodes (Node **, Node **);
  
  void addLaneSet (LaneSet *);
  void removeLaneSet (LaneSet *);
  bool containsLaneSet (LaneSet *);
  laneSet_vector_t getChildLaneSets ();
  
  Network* getParentNetwork ();

  /*
    Constructors and Destructors
  */


  Section (Network *, Node *, Node *);
  ~Section ();
};

#endif
