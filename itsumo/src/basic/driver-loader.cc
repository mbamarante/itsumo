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
#include "driver-loader.hh"

#include "laneset.hh"
#include "node.hh"

/** Headers of possible drivers **/

#include "../drivers/nasch.hh"
#include "../drivers/fc.hh"
#include "../drivers/braess.hh"
#include "../drivers/irc.hh"
#include "../drivers/routed.hh"
#include "../drivers/routerlibs/geradorOD.hh"

/*********************************/

DriverLoader::DriverLoader(Network *_n,string name,string nick,int num,vector<pair <string,string> > vet, vector< Route > rts, ofstream *driverLogger)
{
	n = _n;
	amount = num;
	running.clear();
	waiting.clear();

	for (vector< pair <string,string> >::iterator i=vet.begin();i != vet.end(); i++){
		options.push_back(*i);
	}

	cout << "Driver: " << name << "\tn: " << num << endl;

	/** List of possible drivers **/
	if (name == "nasch")
		while(num--) waiting.push_back( static_cast<Driver *>(new Nasch("Nasch")));
	if (name == "braess")
		while(num--) waiting.push_back( static_cast<Driver *>(new Braess("Braess")));
	if (name == "fc")
		while(num--) waiting.push_back( static_cast<Driver *>(new FC("Floating car")));
	if (name == "irc")
		while(num--) waiting.push_back( static_cast<Driver *>(new IRC("IRC")));
	if (name == "Routed")
		while(num--) waiting.push_back( static_cast<Driver *>(new RoutedDriver("Routed")));

	/** end of list ***/

	/******Configura motoristas****/
	for (vector<Driver *>::iterator i=waiting.begin(); i != waiting.end(); i++)
	{
		(*i)->setDeceleration(n->getParentTopology()->getSimulation()->getDeceleration());
		(*i)->setNick(nick);
		(*i)->setLogger(driverLogger);
		(*i)->set_options(vet);


		if (name == "fc")
		{
			// looking for source and destination nodes
			int source_laneset = (rts.begin())->laneset;
			int destination_laneset = (rts.begin())->laneset;

			cout << "DRIVER: " << nick << "   ROUTE: ";
			for (vector<Route>::iterator it = rts.begin(); it!=rts.end(); it++) {
				cout << " " << it->laneset;
				destination_laneset = it->laneset;
			}
			cout << endl;

			static_cast<FC *>(*i)->set_routes(rts,
							  source_laneset,
							  destination_laneset);
		}
		//if (name == "routed")
		//{// Source and Destiny are set in the Simulation::parse_od_drivers() after the call to this function
		//}
	}
}

bool DriverLoader::removeDriver(Driver *d, int step) {
	vector<Driver *>::iterator it;
	Driver *dd;
	for (it=running.begin(); it != running.end(); it++)
	{
		dd = *it;
		if (dd == d) {
			running.erase(it);
			//waiting.push_back(dd);

			//first position - priority to reinserted drivers (anti-deadlock)
			waiting.insert(waiting.end(), dd); // end = prioridade novos; begin = prioridade reinseridos

			d->setStepRemoved(step);	// Travel time statistic
			d->removedFromNet();

			return true;
		}
	}
	return false;
}



DriverLoader::~DriverLoader(){
	vector<Driver *>::iterator it;
	Driver *d;

	it = running.begin();
	while(it != running.end())
	{
		d = (*it);
		it = running.erase(it);
		delete d;
	}

	it = waiting.begin();
	while(it != waiting.end())
	{
		d = (*it);
		it = waiting.erase(it);
		delete d;
	}
}

void DriverLoader::update(int step, bool reduceOccupationSize, MYSQL *connect, int odsize, string networkname, string round, string global_odfile, string tabela)
{
	vector<Driver *>::iterator it;
	int howManyBack = 0;
	char *zErrMsg = 0;

//	cout << "running.size()=" << running.size() << endl;
//	cout << "waiting.size()=" << waiting.size() << endl;

	it = running.begin();
	while(it != running.end())
	{
		// inform the step to the special driver
		(*it)->update(step);

		// checks if the driver want to be removed before being caught by a sink
		if ((*it)->readyOut(step)) {
			Driver* driverToRemove = *it;

			// deadlock was detected.
			if ((*it)->back2Wait()){
				(*it)->force2Out(false);	// reset to readyOut() return false (do not finished yet)
				(*it)->force2Wait(false);	// driver is already at waiting list
				(*it)->setBack(true);		// driver have to back from waiting to running list
				//cout << "you must back! " << (*it)->getId()  << endl;
			}

			if (removeDriver(driverToRemove, step)) {
				driverToRemove->logEvent(ARRIVAL_EVENT, 0);

				active_cars_vector_t::iterator car_it;
				active_cars_vector_t remove_list;

//				cout << "driver: " << driverToRemove->getId();
//				cout << " travel_time:" << driverToRemove->getStepRemoved() - driverToRemove->getStepInserted() << endl;

				stringstream query;
//				query << "insert into trvtime (driver_id, trv_time, replanner, global, communication, odsize, network, round, source, destination, simulation) values (";
//				query << driverToRemove->getId() << "," << (driverToRemove->getStepRemoved() - driverToRemove->getStepInserted());
//				query << "," << driverToRemove->getReplanner() << "," << driverToRemove->getGlobal() << ",";
//				query << driverToRemove->getCommunication() << ",";
//				query << odsize << ",\"" << networkname << "\"" << "," << round << ",";
//				query << static_cast<RoutedDriver *>(driverToRemove)->getSourceId() << ",";
//				query << static_cast<RoutedDriver *>(driverToRemove)->getDestionationId() << ",\"";
//				query << global_odfile << "\"" << ")";

				if (driverToRemove->getNumberOfTrips() == 0) {
					// start statistics
					query << "insert into " << tabela << " (driver_id, trvtime, replanner, global, communication, odsize, network, round, source, destination, simulation, distance, num_trips) values (";
					query << driverToRemove->getId() << "," << (driverToRemove->getStepRemoved() - driverToRemove->getStepInserted());
					query << "," << driverToRemove->getReplanner() << "," << driverToRemove->getGlobal() << ",";
					query << driverToRemove->getCommunication() << ",";
					query << odsize << ",\"" << networkname << "\"" << "," << round << ",";
					query << static_cast<RoutedDriver *>(driverToRemove)->getSourceId() << ",";
					query << static_cast<RoutedDriver *>(driverToRemove)->getDestionationId() << ",\"";
					query << global_odfile << "\",";
					query << driverToRemove->getDistanceTravelled() << ",1)";

					// reset defaults do restart travelling
					static_cast<RoutedDriver *>(driverToRemove)->reActivate();

				} else {
					// update statistics
					query << "update " << tabela << " set trvtime=trvtime+" << (driverToRemove->getStepRemoved() - driverToRemove->getStepInserted()) << "," <<
							 "num_trips=num_trips+1," << //driverToRemove->getNumberOfTrips() <<
							 "distance=" << driverToRemove->getDistanceTravelled() <<
							 " where " <<
							 "driver_id=" << driverToRemove->getId() << " and " <<
							 "replanner=" << driverToRemove->getReplanner() << " and " <<
							 "global=" << driverToRemove->getGlobal() << " and " <<
							 "communication=" << driverToRemove->getCommunication() << " and " <<
							 "odsize=" << odsize << " and " <<
							 "network=\"" << networkname << "\" and " <<
							 "round=" << round << " and " <<
							 "simulation=\"" << global_odfile << "\"";

					// reset defaults do restart travelling
					static_cast<RoutedDriver *>(driverToRemove)->reActivate();
				}

				string str(query.str());

//				sqlite3_exec(db, str.c_str(), NULL, 0, &zErrMsg);
				mysql_query(connect, str.c_str());
//				sqlite3_free(zErrMsg);

				for (car_it=active_cars.begin(); car_it != active_cars.end(); car_it++) {
					if ((*car_it)->getDriver() == driverToRemove)
						remove_list.push_back(*car_it);
				}

				for (car_it=remove_list.begin(); car_it != remove_list.end(); car_it++) {
					(*car_it)->getCurrentLane()->removeCar((*car_it)->getCurrentCell());
					Car::removeFromActiveCarsList((*car_it));

				}

			} else it++;

		} else it++;
	}

	it = waiting.begin();
	while (it != waiting.end()) {

		(*it)->update(step);

		int occupationSize = 70;

//		if (reduceOccupationSize) {
//			occupationSize = 50;
//		} else {
//			occupationSize = 70;
//		}
//
		/* avoid deadlock: when laneset for re-insertion is over x% busy then wait  */
		if ((*it)->isBack() && n->getLanesetById((*it)->where(step))->getOcupationLink() > occupationSize){
			// na funcao insertDriver sera tb verificado o link para onde o driver esta indo.
			it++;
			continue;
		}

		//set for the variable, controls the use of linkLimit;
		if ((*it)->readyIn(step) && linkLimit((*it),step) && !(*it)->readyOut(step) ) {
		  // if insertion is ok (ie, there is available space
		  // on the lane), then puts the car in the 'running'
		  // list and removes it from the waiting queue

		  if (insertDriver((*it),step)) {

//				stringstream query;
//				query << "insert into inside (driver_id) values (" << (*it)->getId() << ")";
//				string str(query.str());
//				mysql_query(connect, str.c_str());

			if ((*it)->isBack()){
				(*it)->setBack(false);
				howManyBack++;	// statistical control, if needed!
			}

//			  cout<<"depois if"<<endl;
			running.push_back((*it));
			(*it)->logEvent(DEPARTURE_EVENT, 0);
			(*it)->logEvent(WAIT_EVENT, 0);
			it = waiting.erase(it);
		  } else it++;
		} else it++;

	}

//	cout << "step: " << step << " - back: " << howManyBack << endl;

}

void DriverLoader::prepareNewEpisode() {

	vector<Driver *>::iterator it = waiting.begin();
	while (it != waiting.end()) {
		(*it)->reActivate();
		it++;
	}
}

void DriverLoader::prepareNewRound() {

	if (static_cast<RoutedDriver *> (waiting.front())->driver_class_name.compare("Routed") == 0) {
		static_cast<RoutedDriver *> (waiting.front())-> resetGraphCosts();
	}

}

//metodo para limitar a insercao de veiculos que estao na fila
//a fim de tentar evitar o deadlock
bool DriverLoader::linkLimit(Driver *d, int step){
	//Nao utilizar valores menores que 70, atrasa muito a insercao de veiculos na rede.
	//Setar a variavel "insertLimit" para 100 ou 0 para ignorar a ocupacao do link.
	int insertLimit=100; //porcentagem para tirar da fila ou nao!!!
	int linkId=d->where(step);
	int	limitOfTheLink=0; //capacidade do link corrente
	//laneset_vector_t lvt=n->getChildLaneSets();
	if(insertLimit!=100 && insertLimit!=0)
		limitOfTheLink=n->getLanesetById(linkId)->getOcupationLink();
	if(limitOfTheLink<insertLimit || insertLimit==100 || insertLimit==0)return true;
	else return false;
}

/* Baseado em calibrator.cc::act() */
bool DriverLoader::insertDriver(Driver *d,int step) {
	Car *newCar;

	// tries to pick a car from the inactive cars list; if the list
	// is empty, then we need to create a new car/driver

	if ((newCar = Car::readFromInactiveCarsList()) == NULL) {
		if (d == NULL) {
			std::cerr << "Could not get a new Driver!";
			std::cerr << std::endl;
		}
		// creates a new car informing {car, maxSpeed, maxAccel}
		newCar = new Car(d, n->getParentTopology()->getCarsMaximumSpeed(), 1);
	} else {
		// inserts the car into the simulation!
		newCar->setDriver(d);
		Car::insertInActiveCarsList(newCar);
	}

	long int laneSetIndex;
	LaneSet *targetLaneSet;

	int lanesetId = d->where(step);

	if (lanesetId < 0 )
	{
		laneSetIndex = randomGen.IRandom(0,
				n->getChildLaneSets().size()-1);
		targetLaneSet = n->getChildLaneSets()[laneSetIndex];
	}
	else
		targetLaneSet = n->getLanesetById(lanesetId);
	if (targetLaneSet == NULL)
	{
		cerr<< "error in DriverLoader::insertDriver, null targetLaneSet!!!\n";
		exit(1);
	}


	lane_vector_t childLanes = targetLaneSet -> getChildLanes ();
	long int randomLane = randomGen.IRandom(0, targetLaneSet -> getNumberOfLanes()-1);
	Lane *targetLane = childLanes[randomLane];
	dist_t targetCell;
	targetCell = bestPlaceOnLane(targetLane);

	// is there any free cell on the target lane?
	if (targetCell != -1) {

		if(	newCar -> insertInLane(targetLane, targetCell,
				n->getParentTopology()->getCarsMaximumSpeed(),
				1))
		{	// lane, cell, curSpeed, curAccel
			Car::removeFromActiveCarsList(newCar);
			Car::insertInInactiveCarsList(newCar);
			return false;
		}

		lane_laneset_pair_t next_lane_and_laneset;
		next_lane_and_laneset = newCar -> getDriver() -> chooseNextLane_Laneset(newCar);
		(newCar -> driverDecision).laneInNextLaneSet = next_lane_and_laneset.first;
		(newCar -> driverDecision).nextLaneSet = next_lane_and_laneset.second;

		// deadlock avoidance - test 2: looking ahead!
//		if (d->isBack() && next_lane_and_laneset.second->getOcupationLink() > 90) {
//			Car::removeFromActiveCarsList(newCar);
//			Car::insertInInactiveCarsList(newCar);
//			return false;
//		}

		// Travel time statistic
		d->setStepInserted(step);
		return true;
	} else {
		Car::removeFromActiveCarsList(newCar);
		Car::insertInInactiveCarsList(newCar);
		return false;
	}
}
/* This is a COPY of calibrator's func  
 * see calibrator.cc
 ***********************************************************/

dist_t DriverLoader::bestPlaceOnLane(Lane *lane)
{

	dist_t length = lane->getLength();
	Car *tmpCar;

	dist_t gapToFirstCar = lane->getFrontalGapFromCell(-1);

	// best place to put the car
	dist_t bestPlace;

	// maximum gap found and the currentGap
	dist_t maxGap, currentGap;
	// and the cell (position) from where the maxGap was measured
	dist_t positionMaxGap;

	// there is no car in the lane, so the best place is the middle cell
	if (gapToFirstCar >= length) {
		//    printf("   --> VAZIA!!! length=%d retorna=%d\n", length, (length/2)-1);
		if (length > 1)
			return (length / 2) - 1;
		else
			return length;

	}

	maxGap = gapToFirstCar;
	positionMaxGap = -1;

	for(int i=0; i<length; i++) {

		// get the car at current position
		tmpCar = lane->queryCarPtr(i);
		// if there is a car, we measure the gap from it
		if (tmpCar != NULL) {
			currentGap = lane->getFrontalGapFromCell(i);
			// checks if the value of the gap indicates that there is no
			// car until the end of the street and adapts currentGap accordingly
			currentGap = (currentGap >= length ? length - i - 1 : currentGap);

			//       printf("   --> ACHOU um carro %d.. curgap=%d maxgap=%d\n", tmpCar->getInternalId(),
			// 	     currentGap, maxGap);

			// current gap is better than previous, so save it
			if (currentGap > maxGap) {
				positionMaxGap = i;
				maxGap = currentGap;
			}
		}
	}


	// if the lane is full, return -1
	if (maxGap==0)
		return -1;
	else {
		//     printf("    --> retornando.. positionMax=%d maxGap=%d calc=%d\n", positionMaxGap,
		// 	   maxGap, positionMaxGap + ((maxGap + 1) / 2));
		// best place is the middle of segment with greatest sequence of empty cells
		bestPlace = positionMaxGap + ((maxGap + 1)/ 2);
	}

	return bestPlace;
}


/************************************************************
Function: string DriverLoader::printTravelTimesSensor()
   Author: Alexander Benavides
   Description: Prints the travel times for the special drivers
   Changelog:
************************************************************/
string DriverLoader::printTravelTimesSensor()
{
	ostringstream cout;
	for(vector<Driver *>::iterator it = waiting.begin(); it != waiting.end(); ++it)
		if ( (*it)->getStepRemoved() > 0)
			cout << (*it)->getStepRemoved() - (*it)->getStepInserted() << "\t" << (*it)->getStepInserted() << "\t" << (*it)->getStepRemoved()
				<< "\t" << (*it)->getReplanCalls() << "\t" << (*it)->getReplanChanges() << "\t" << (*it)->getTolerance() << "\t" << (*it)->getPathSize() << endl;
   	return cout.str();
}

/************************************************************
Function: string DriverLoader::printAvgTravelTimesSensor()
   Author: Maicon BA
   Description: Prints the avg travel times for the special drivers
   Changelog:
************************************************************/
string DriverLoader::printAvgTravelTimesSensor()
{
	ostringstream cout;
	unsigned long int trvTimes=0, replanCalls=0, replanChanges=0;
	int lastIn=0, lastOut=0, cnt=0;
	for(vector<Driver *>::iterator it = waiting.begin(); it != waiting.end(); ++it)
		if ( (*it)->getStepRemoved() > 0)
		{
			++cnt;
			trvTimes += (*it)->getStepRemoved()-(*it)->getStepInserted();
			replanCalls += (*it)->getReplanCalls();
			replanChanges += (*it)->getReplanChanges();
			if ((*it)->getStepInserted() > lastIn) lastIn  = (*it)->getStepInserted();
			if ((*it)->getStepRemoved() > lastOut) lastOut = (*it)->getStepRemoved();
		}

	if (cnt != waiting.size() && running.size() /* > 0 */)
		cout << "# there are still vehicles waiting or in the network." << endl << endl;
	else
	{
		cout << "# All vehicles are out of the network." << endl;
		cout << (trvTimes/waiting.size());
		//	cout << "\t" << active_cars.size();
		cout << "\t" << lastIn;
		cout << "\t" << lastOut;
		cout << "\t" << replanCalls;
		cout << "\t" << replanChanges << endl;
	}

   	return cout.str();
}
