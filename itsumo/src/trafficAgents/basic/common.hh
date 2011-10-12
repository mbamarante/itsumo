#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <math.h>
#include <cstdlib>

using namespace std;

class lane{
public:
    int laneId;
    int stoppedCars;
    double avgSpeed;
    double density;
    lane(int id, double avgS, double dens, int stopped);
    virtual ~lane();
};


class node{
public:
    int nodeId;
    int tlightId;	
    int curSignalPlan;    
    vector<lane > incomingLanes;
    vector<lane > outgoingLanes;
    node(int id, int tlId, int signalP);
    node();
    virtual ~node();
};


// state of a traffic light and its incoming lanes, etc
class simulationState 
{
public:
	int curIteration;
	node n;
	
	void print(); 
	
	string outline();
	simulationState(int curIter, node no);
	simulationState();
	virtual ~simulationState();
};


struct direction 
{
	int fromLane;
	int toLaneset;
};

struct phase 
{
	int phaseId;
	int start;
	int end;
	vector<direction> directions;
};


struct signalPlan 
{
	int signalPlanId;
	vector <phase> phases;
};

