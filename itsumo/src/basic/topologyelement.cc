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
#include "topologyelement.hh" 

#include <iostream>
using std::cout;
using std::endl;


// last used id
topology_element_id_t TopologyElement::nextInternalId = 0;

// changelog (Bruno 27-sept-2003): added this in order to 
// set the object's id according to the id used in the DB
void TopologyElement::setId (topology_element_id_t _id) {

  id = _id;
}


// changelog (Bruno 29-sept-2003): now the object has 2 id's: the id according to the db, and an internal id
topology_element_id_t TopologyElement::getInternalId () {
  return internalId;
}



/**********************************************************
  Method: topology_element_id_t TopologyElement::getId ();
  Author: John Kliff Jochens
  Date: May 31th, 2003
  Description: Returns the ID associated with a instantece of a derived class.

  Changelog: bruno 27-sept-2003: now this function returns the object's id ACCORDING TO THE DB
***********************************************************/

topology_element_id_t TopologyElement::getId () {
  return id;
}



// changelog (bruno 27-sept-2003): given the db's id, this function
// searches on the 'elements' array for the corresponding object,
// and returns it (this funcion is needed since the simulator
// indexes the objects in its own structures using a different
// id than the DB)
TopologyElement* TopologyElement::getElementById (topology_element_id_t _id) {

  //  topology_element_vector_t::iterator i;
  //  for (i=elements.begin(); i != elements.end(); i++)
  for (int i = 0; i < nextInternalId; i++)
    if ((elements[i] -> getId()) == _id)
      return elements[i];

  return NULL;
}



/**********************************************************
  Method: TopologyElement* TopologyElement::getElementById (topology_element_id_t _id)
  Author: John Kliff Jochens
  Date: May 31th, 2003
  Description: Returns a pointer to derived instance which id was given as the argument _id.

  Changelog:
***********************************************************/

TopologyElement* TopologyElement::getElementByInternalId (topology_element_id_t _id) {
  return elements[static_cast<int>(_id)];
}

/**********************************************************
  Method: void TopologyElement::setInTopology (TopologyElement *);
  Author: John Kliff Jochens
  Date: May 31th, 2003
  Description: Inserts this element into the elements vector 
for easy access.
Updates the size of the vector based on nextInternalId, which is always thid + 1;

  Changelog:
john June 2nd, 2003: push_back () did not work properly, so
the vector is going to be resized as needed.
john June 3rd, 2003: changed the way we check and correct the size 
of the vector from some inline code to a macro (thus we use 
somewhere else).

bruno September 3rd, 2003: inserted a typecast in the first comparising
due to some redefinition of types (all id's are integers now - see types.hh)
***********************************************************/

void TopologyElement::setInTopology () {


  if (static_cast<topology_element_id_t>(elements.size()) <= internalId)
    CHECK_VECTOR_SIZE (elements, this);
  else 
    elements[internalId] = this;
  
}


/**********************************************************
  Method: constructor TopologyElement::TopologyElement (bool inc)
  Author: John Kliff Jochens
  Date: May 31th, 2003
  Description: Default constructor. Must always be called as the superclass constructor. 
Updates the static nextInternalId counter and sets this instance's id accordingly.

  Changelog:
***********************************************************/

TopologyElement::TopologyElement () {
  internalId = nextInternalId;
  nextInternalId++;

  setInTopology ();
}

/**********************************************************
  Method: destructor TopologyElement::~TopologyElement ()
  Author: John Kliff Jochens
  Date: May 31th, 2003
  Description: Default Destructor.

  Changelog:
***********************************************************/

TopologyElement::~TopologyElement () {
  
}
