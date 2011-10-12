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
#include "section.hh"

#include <iostream>
using std::cout;
using std::endl;



/************************************************************
   Function: bool ordering_function(const LaneSet_a&, const LaneSet_b& b) 
   Author: Bruno C. da Silva 
   Description: defines an ordering function regarding two lanesets. 
        The comparison is made using the laneset's position inside the section

   Changelog:
************************************************************/
bool child_lanesets_ordering_function(const LaneSet *a, const LaneSet *b)
{ 
  return a->position < b->position;
}
void Section::sortChildLaneSets()
{
  sort(childLaneSets.begin(), 
       childLaneSets.end(), 
       child_lanesets_ordering_function);
}






void Section::setPreferencial(bool _preferencial)
{
  preferencial = _preferencial;
}


bool Section::isPreferencial()
{
  return preferencial;
}




/************************************************************
   Function: void Section::setStreetId(topology_element_id_t _streetId)
   Author: Bruno C. da Silva 
   Description: sets the id of the street to which this
          section belongs (remember that a street is a set
          of sections, and each section if formed by
          lanesets, etc etc)

   Changelog:
************************************************************/

void Section::setStreetId(topology_element_id_t _streetId) {
  parentStreetId = _streetId;
  getParentNetwork() -> registerSection(this);
}






/************************************************************
   Function: topology_element_id_t Section::getStreetId()
   Author: Bruno C. da Silva 
   Description: returns the id of the street to which this
          section belongs (remember that a street is a set
          of sections, and each section if formed by
          lanesets, etc etc)

   Changelog:
************************************************************/

topology_element_id_t Section::getStreetId() {
  return parentStreetId;
}





/************************************************************
   Function: int Section::getNumberOfLaneSets()
   Author: Bruno C. da Silva 
   Description: returns the number of lanesets that belong to 
          this section

   Changelog:
************************************************************/
   
int Section::getNumberOfLaneSets() {
  return childLaneSets.size();
}


/************************************************************
   Function: int Section::getNumberOfLanes()
   Author: Bruno C. da Silva 
   Description: returns the number of lanes that belong to 
          this section's lanesets

   Changelog:
************************************************************/
   
int Section::getNumberOfLanes() {
  int tmp=0;

  laneSet_vector_t::iterator laneset;
  for (laneset = childLaneSets.begin(); laneset != childLaneSets.end(); laneset++)
    tmp += (*laneset)->getNumberOfLanes();

  return tmp;
}




/************************************************************
  Method: string Section::getName ();
  Author: John Kliff Jochens
  Date: Tue Jun 10 16:17:02 BRT 2003
  Description: Returns this Sections's name.

  Changelog: 
************************************************************/
string Section::getName () {
  return name;
}

/************************************************************
  Method: void Section::setName (string);
  Author: John Kliff Jochens
  Date: Tue Jun 10 16:32:52 BRT 2003
  Description: Sets this Section's name;

  Changelog: 
************************************************************/
void Section::setName (string n) {
  name = n;
}

/************************************************************
  Method: dist_t Section::getLength ();
  Author: John Kliff Jochens
  Date: Tue Jun 10 15:02:27 BRT 2003
  Description: Returns this Section's length in dist_t units.

  Changelog: 
************************************************************/
dist_t Section::getLength () {
  return length;
}

/************************************************************
  Method: void Section::setLength (dist_t _l)
  Author: John Kliff Jochens
  Date: Tue Jun 10 15:03:59 BRT 2003
  Description: Sets this Section's (dist_t) length to _l.

  Changelog: 
************************************************************/
void Section::setLength (dist_t _l) {
  length = _l;
}

/************************************************************
  Method: void Section::getDelimitingNodes (Node *, Node *);
  Author: John Kliff Jochens
  Date: Tue Jun 10 15:16:27 BRT 2003
  Description: sets in each of the arguments the delimiting Nodes.

  Changelog: 
************************************************************/
void Section::getDelimitingNodes (Node** n1, 
				  Node** n2) {

  *n1 = nodes [0];
  *n2 = nodes [1];
}

/************************************************************
  Method: void Section::addLaneSet (LaneSet *_ls);
  Author: John Kliff Jochens
  Date: Tue Jun 10 16:41:32 BRT 2003
  Description: Add a LaneSet to the childLaneSets vector.

  Changelog: 
12/6/2003, John:
Changed the type of the iteration index from int to unsigned int to avoid compilation warnings
 
************************************************************/
void Section::addLaneSet (LaneSet *_ls) {
  childLaneSets.push_back (_ls);

}
/************************************************************
  Function: void Section::removeLaneSet (LaneSet *);
  Author: John Kliff Jochens
  Date: Wed Jun 11 17:12:46 BRT 2003
  Description: removes a laneSet from this Section.

  Changelog: 
************************************************************/
void Section::removeLaneSet (LaneSet *) {
}

/************************************************************
  Method: bool Section::containsLaneSet (LaneSet *);
  Author: John Kliff Jochens
  Date: Tue Jun 10 16:36:46 BRT 2003
  Description: If the LaneSet _ls is contained by the 
childLaneSets vector, returns true. Otherwise false.

  Changelog:
12/6/2003, John:
Changed the type of the iteration index from int to unsigned int to avoid compilation warnings
 
************************************************************/
bool Section::containsLaneSet (LaneSet *_ls) {
  for (unsigned int i = 0 ; i < childLaneSets.size () ; i++) 
    if (childLaneSets[i] == _ls)
      return true;

  return false;
}

/************************************************************
  Method: laneSet_vector_t Section::getChildLaneSets ();
  Author: John Kliff Jochens
  Date: Tue Jun 10 16:35:47 BRT 2003
  Description: returns a copy of the childLaneSet vector.

  Changelog: 
************************************************************/
laneSet_vector_t Section::getChildLaneSets () {
  return laneSet_vector_t (childLaneSets);
}

/************************************************************
  Method: Network* Section::getParentNetwork ()
  Author: John Kliff Jochens
  Date: Tue Jun 10 13:18:27 BRT 2003
  Description: Returns a pointer to the parentNetwork Network;

  Changelog: 
************************************************************/
Network* Section::getParentNetwork () {
  return parentNetwork;
}


/************************************************************
  Method: constructor Section::Section (Network*, Node*, Node*);
  Author: John Kliff Jochens
  Date: Tue Jun 10 13:15:53 BRT 2003
  Description: Default constructor

  Changelog: 
************************************************************/
Section::Section (Network * _parent, 
		  Node * _n1, 
		  Node * _n2) : TopologyElement () {

  nodes[0] = _n1;
  nodes[1] = _n2;

  nodes[0] -> addSection (this);
  nodes[1] -> addSection (this);
  
  parentNetwork = _parent;
  parentNetwork -> addSection (this);
}

/************************************************************
  Method: destructor Section::Section ()
  Author: John Kliff Jochens
  Date: Tue Jun 10 13:16:31 BRT 2003
  Description: default destructor

  Changelog: 
************************************************************/
Section::~Section () {

}
