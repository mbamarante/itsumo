#ifndef MATSIM_H_
#define MATSIM_H_

#include <string>

using namespace std;


typedef pair <int, string> event_t;


// constants for MATSim events 
const event_t ARRIVAL_EVENT = make_pair(0, "arrival");
const event_t LEFT_EVENT = make_pair(2, "left link");
const event_t WAIT_EVENT = make_pair(4, "wait2link");
const event_t ENTERED_EVENT = make_pair(5, "entered link");
const event_t DEPARTURE_EVENT = make_pair(6, "departure");



#endif /*MATSIM_H_*/
