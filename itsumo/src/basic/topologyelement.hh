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
#ifndef __HAS_TOPOLOGY_ELEMENT_H
#define __HAS_TOPOLOGY_ELEMENT_H

#include "types.hh"


extern topology_element_vector_t elements;


class TopologyElement {
private:

  /* 
     this is a vector that'll contain all topology elements;
     each element's id will be set according to its index on the vector
  */

  void setInTopology ();
protected:
  //  Topology *pParentTopology;

  // element's unique identification (in the simulator)
  topology_element_id_t id;

  // element's unique identification (according to the DB)
  topology_element_id_t internalId;


public:

  static topology_element_id_t nextInternalId;

  // gets the object's id according to the DB
  topology_element_id_t getId ();

  // gets the object's id according to the simulator
  topology_element_id_t getInternalId ();

  void setId (topology_element_id_t _id);

  // gets the object's according to the simulator's id
  TopologyElement* getElementByInternalId (topology_element_id_t);
  // gets the object's according to DB's id
  TopologyElement* getElementById (topology_element_id_t);



  /*
    Constructors and Destructors
  */
  TopologyElement ();
  ~TopologyElement ();
};


#endif
