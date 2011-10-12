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
#include "laneset.hh"

#include <iostream>
using std::cout;
using std::endl;

using namespace std;

/************************************************************
   Function: bool child_lanes_ordering_function(const LaneSet *a, const LaneSet *b)
   Author: Bruno C. da Silva 
   Description: defines an ordering function regarding two lanes. 
        The comparison is made using the lanes' position inside the laneset

   Changelog:
************************************************************/
bool child_lanes_ordering_function(const Lane *a, const Lane *b)
{ 
  return a->position_in_laneset < b->position_in_laneset;
}
void LaneSet::sortChildLanes()
{
  sort(childLanes.begin(), 
       childLanes.end(), 
       child_lanes_ordering_function);
}




// methods to set and return the position of the laneset inside the section (if it's the first, second, etc);
// the position is measured left->right (1st laneset is the leftmost)
int LaneSet::getPosition()
{
  return position;
}

void LaneSet::setPosition(int _position)
{
  position = _position;
}



/************************************************************
   Function: void LaneSet::addTurningProb(LaneSet *, double)
   Author: Bruno C. da Silva 
   Description: adds a new turning probability to this 
           laneset, that is, creates a new entry that
           describes the probability of some car choosing
           'targetLaneSet' as the destination laneset

   Changelog:
************************************************************/
void LaneSet::addTurningProb(LaneSet *targetLaneSet, double prob) 
{
  turningProbabilities.push_back(make_pair(targetLaneSet, prob));
}

void LaneSet::clearTurningProbabilities(){

	turningProbabilities.clear();

}




/************************************************************
   Function: turning_probabilities_t LaneSet::getTurningProbabilities()
   Author: Bruno C. da Silva 
   Description: returns the turning probabilities vector

   Changelog:
************************************************************/

turning_probabilities_vector_t LaneSet::getTurningProbabilities() 
{
  return turningProbabilities;
}





/************************************************************
   Function: int LaneSet::getNumberOfLanes()
   Author: Bruno C. da Silva 
   Description: returns the number of lanes that belong
           to this laneset

   Changelog:
************************************************************/

int LaneSet::getNumberOfLanes() {
  return childLanes.size();
}





/************************************************************
   Function: int LaneSet::getCapacity()
   Author: Alexander Benavides
   Description: returns the number of cells of the laneset
   Changelog:
************************************************************/

int LaneSet::getCapacity() {
	int rtrn = 0;
	//for(laneset_vector_t::iterator it = childLanes.begin(); it != childLanes.end(); ++it) {
	for(lane_vector_t::iterator it = childLanes.begin(); it != childLanes.end(); ++it){
		rtrn += (*it)->getLength();
	}
	return rtrn;
}

/************************************************************
   Function: int LaneSet::getOcupationLink()
   Author: Tiago Sommer
   Description: returns the percent (int) of the ocupation
   of the link, the density return a double, is more easy work
   with the int value
   Changelog:
************************************************************/
int LaneSet::getOcupationLink(){
	int numVehicles=getNumberOfVehicles()*100;
	return numVehicles/getCapacity();
}


/************************************************************
   Function: int LaneSet::getNumberOfVehicles()
   Author: Alexander Benavides
   Description: returns the number of vehicles in the laneset
   Changelog:
************************************************************/

int LaneSet::getNumberOfVehicles() {

	int rtrn = 0;
	for(lane_vector_t::iterator it = childLanes.begin(); it != childLanes.end(); ++it)
		rtrn += (*it)->getNumberOfVehicles();
	return rtrn;
}






/************************************************************
   Function: double LaneSet::getDensity()
   Author: Alexander Benavides
   Description: this functions returns the lane density
       (quotient between the number of cars and the number
         of cells)
 returns the number of cells of the laneset
   Changelog:
************************************************************/

double LaneSet::getDensity() {

	return (static_cast<double>(getNumberOfVehicles()) / static_cast<double>(getCapacity()));
}






/************************************************************
   Function: dist_t LaneSet::getMaxVMax()
   Author: Alexander Benavides
   Description: returns the maximum laneset speed limit
   Changelog:
************************************************************/

dist_t LaneSet::getMaxVMax() {
	int rtrn = 0;
	for(lane_vector_t::iterator it = childLanes.begin(); it != childLanes.end(); ++it)
		if (rtrn < (*it)->getMaxSpeed())
			rtrn = (*it)->getMaxSpeed();
	return rtrn;
}





/************************************************************
   Function: void LaneSet::addLane (Lane *_l)
   Author: Bruno C. da Silva 
   Description: add a lane to the laneset and sets the left/right neighbours
           of this lane. It is assumed that the leftmost lane is 
           the first to be inserted in a laneset, and the 
           rightmost lane is the last. Left-right orientation
           is based on the point of view of someone standing
           on the street and looking to its end (that is, from
           the point of view of someone moving in the same direction
           of the street).

   Changelog:
************************************************************/

void LaneSet::addLane (Lane *_l) {

  int pos = childLanes.size()-1;


  if (pos >=0) {
    childLanes[pos]->setRightLane(_l);
    _l -> setLeftLane(childLanes[pos]);
  }
  else
    _l -> setLeftLane(NULL);


  _l -> setRightLane(NULL);
  childLanes.push_back (_l);

}





/************************************************************
  Method: Section & LaneSet::getParentSection ();
  Author: John Kliff Jochens
  Date: Qua Jun 11 15:23:57 BRT 2003
  Description: Returns a pointer to the parentNetwork

  Changelog: 
************************************************************/
Section * LaneSet::getParentSection () {
  return parentSection;
}

/************************************************************
  Function: 
  Author: John Kliff Jochens
  Date: Qua Jun 11 15:52:09 BRT 2003
  Description: Sets the starting Node of this LaneSet.

  Changelog: 
************************************************************/
bool LaneSet::setStartNode (Node * _sNode) {
  Node *n1, *n2;
  parentSection -> getDelimitingNodes (&n1, &n2);
  if (_sNode == n1 || _sNode == n2) {
    startNode = _sNode;
    return true;
  }
  else 
    return false;
}

/************************************************************
  Method: Node * LaneSet::getStartNode ()
  Author: John Kliff Jochens
  Date: Qua Jun 11 15:25:12 BRT 2003
  Description: Returns the origin node for this LaneSet.
That Node must be a delimiting node for the parent Section.

  Changelog: 
************************************************************/
Node * LaneSet::getStartNode () {
  return startNode;
}

/************************************************************
  Function: void LaneSet::setEndNode (Node *);
  Author: John Kliff Jochens
  Date: Qua Jun 11 15:52:59 BRT 2003
  Description: 

  Changelog: 
************************************************************/
bool LaneSet::setEndNode (Node *_eNode) {
  Node *n1, *n2;
  parentSection -> getDelimitingNodes (&n1, &n2);
  if (_eNode == n1 || _eNode == n2) {
    endNode = _eNode;
    return true;
  }
  else 
    return false;
}

/************************************************************
  Method: Node * LaneSet::getStartNode ()
  Author: John Kliff Jochens
  Date: Qua Jun 11 15:25:12 BRT 2003
  Description: Returns the destiny node for this LaneSet.
That Node must be a delimiting node for the parent Section.

  Changelog: 
************************************************************/
Node * LaneSet::getEndNode () {
  return endNode;
}

/************************************************************
  Method: lane_vector_t LaneSet::getChildLanes ();
  Author: John Kliff Jochens
  Date: Qua Jun 11 15:33:14 BRT 2003
  Description: 

  Changelog: 
************************************************************/
lane_vector_t LaneSet::getChildLanes () {
  return lane_vector_t (childLanes);
}

/************************************************************
  Method: constructor LaneSet::LaneSet (Section *_parentSection, Node *_startingNode);
  Author: John Kliff Jochens
  Date: Qua Jun 11 15:30:33 BRT 2003
  Description: default Constructor. 

  Changelog: 
************************************************************/
LaneSet::LaneSet (Section *_parentSection) : TopologyElement () {
  parentSection = _parentSection;
  parentSection -> addLaneSet (this);
}

/************************************************************
  Method: destructor LaneSet::~LaneSet ();
  Author: John Kliff Jochens
  Date: Qua Jun 11 15:32:47 BRT 2003
  Description: default destructor

  Changelog: 
************************************************************/
LaneSet::~LaneSet () {

}

/************************************************************
   Function: int LaneSet::getNumberOfVehicles()
   Author: Alexander Benavides
   Description: returns the number of vehicles in the laneset
   Changelog:
************************************************************/

vector<int> LaneSet::getDriversId() {

	vector<int> driversId;

	for(lane_vector_t::iterator it = childLanes.begin(); it != childLanes.end(); ++it){

		for (int i = 0; i < (*it)->getLength(); ++i) {

			if ((*it)->queryCarPtr(i) != NULL) { // car found
				driversId.push_back( (*it)->queryCarPtr(i)->getDriver()->getId() ); // car's driver
			}
		}
	}

	return driversId;
}
