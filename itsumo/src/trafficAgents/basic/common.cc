#include "common.hh"
#include "../../basic/utils.hh"

lane::lane(int id, double avgS, double dens, int stopped) 
{
       laneId = id;
       stoppedCars = stopped;
       avgSpeed = avgS;
       density = dens;
}
lane::~lane(){}


node::node(int id, int tlId, int signalP) 
{
       nodeId = id;
	   tlightId = tlId;
       curSignalPlan = signalP;
}
node::node(){}

node::~node(){
	incomingLanes.clear();
	outgoingLanes.clear();
}

simulationState::simulationState(int curIter, node no){
	curIteration = curIter;
	n = no;
}
simulationState::simulationState(){
	curIteration = 0;
	node n(); 
}

simulationState::~simulationState(){}
  
string simulationState::outline()
{
	string ans = "";

	ans += intToString(curIteration);
	ans += " ";

	ans += intToString(n.tlightId);
	ans += " ";

	ans += intToString(n.curSignalPlan);
	ans += " ";

	vector<lane >::iterator itLane;

	//incoming
	int count = 0;
	double avgSpeed = 0;
	double avgDensity = 0;
	double avgStopped = 0;

	for (itLane = n.incomingLanes.begin(); 
		itLane != n.incomingLanes.end(); itLane++) 
	{
		avgSpeed += (*itLane).avgSpeed;
		avgDensity += (*itLane).density;
		avgStopped += (*itLane).stoppedCars;

		count ++;
	}
	
	avgSpeed = avgSpeed / count;
	ans += doubleToString(avgSpeed);
	ans += " ";

	avgDensity = avgDensity / count;
	ans += doubleToString(avgDensity);
	ans += " ";

	avgStopped = avgStopped / count;
	ans += doubleToString(avgStopped);
	
	//outgoing
	count = 0;
	avgSpeed = 0;
	avgDensity = 0;
	avgStopped = 0;

	ans += " ";
	for (itLane = n.outgoingLanes.begin(); 
		itLane != n.outgoingLanes.end(); itLane++) 
	{
		avgSpeed += (*itLane).avgSpeed;
		avgDensity += (*itLane).density;
		avgStopped += (*itLane).stoppedCars;

		count ++;
	}
	
	avgSpeed = avgSpeed / count;
	ans += doubleToString(avgSpeed);
	ans += " ";

	avgDensity = avgDensity / count;
	ans += doubleToString(avgDensity);
	ans += " ";

	avgStopped = avgStopped / count;
	ans += doubleToString(avgStopped);

	return ans;
}


void simulationState::print() 
{
	cout << "Current iteration=" << curIteration << endl;

	cout << "\tNode id=" << n.nodeId << endl;
	cout << "\tParent tlight id=" << n.tlightId << endl;
	cout << "\tSignalPlan=" << n.curSignalPlan << endl;
	vector<lane >::iterator itLane;
	for (itLane = n.incomingLanes.begin(); 
		itLane != n.incomingLanes.end(); itLane++) 
	{
		cout << "\t\tLane id=" << (*itLane).laneId << endl;
		cout << "\t\t\tAvgSpeed=" << (*itLane).avgSpeed << endl;
		cout << "\t\t\tDensity=" << (*itLane).density << endl;
		cout << "\t\t\tStopped cars=" << (*itLane).stoppedCars<< endl;
	}

}
//
//net::net(int nid)
//{
//	id = nid;	
//}


