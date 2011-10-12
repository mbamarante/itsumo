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
#include "driver.hh"


#include <iostream>
#include <algorithm>
#include <functional>
// using std::cout;
// using std::endl;
using namespace std;


// last used id
int Driver::nextDriverId = 0;


void Driver::setDeceleration(float dece){
	deceleration_probability = dece;
}




/************************************************************
   Function: Driver::Driver()
   Author: Bruno C. da Silva 
   Description: this is the constructor to the Driver class. It just
         sets the driver's id.

   Changelog:
************************************************************/

Driver::Driver(string _driver_class_name) {

  driver_class_name = _driver_class_name;
  id = nextDriverId;
  nextDriverId++;

}





/************************************************************
   Function: Driver::~Driver()
   Author: Bruno C. da Silva 
   Description: this is the destructor to the Driver class.
           Pretty useless so far.

   Changelog:
************************************************************/

Driver::~Driver() {

  //  cout << "destruiu um driver de id: "  << endl;;
}



/************************************************************
   Function: void Driver::setNick (string nick)
   Author: Felipe Boffo
   Description: set the driver nickname (for integration with matsim)
   Changelog:
************************************************************/

void Driver::setNick (string _nick) {
	nick = _nick;	
}



/************************************************************
   Function: string Driver::getNick ()
   Author: Felipe Boffo
   Description: get the driver nickname (for integration with matsim)
   Changelog:
************************************************************/

string Driver::getNick () {
	return nick;	
}



void Driver::setLogger(ofstream *logger_)
{
	logger = logger_;
}


/************************************************************
   Function: driver_id_t Driver::getId ()
   Author: Bruno C. da Silva 
   Description: returns the driver's id
   Changelog:
************************************************************/

driver_id_t Driver::getId () {
  return id;
}

