#include "tlagent.hh" 
#include "../../basic/utils.hh"


TLAgent::TLAgent(int id, int memorySize, bool writeStat) 
{
	_planTemp_ = 0;
	_memorySize_ = memorySize;
	_writeStat_ = writeStat;
	tlightId = id;

	string fileName = "output/" + intToString(id) + ".txt";
	_outFile_.open(fileName.c_str());
}

TLAgent::~TLAgent()
{	
	_outFile_.close();
}

void TLAgent::updateState(simulationState newState) 
{
	if (_states_.size() >= _memorySize_)
		_states_.erase(_states_.begin());			

	_states_.push_back(newState);

	//newState->print();

	
	if (_writeStat_)
	{
		string temp = newState.outline();
		_outFile_ << temp;
		_outFile_ << "\n";
	}
}


simulationState TLAgent::getCurrentState() 
{
	if (_states_.size() > 0)
		return _states_[_states_.size()-1];
	else return simulationState();
}


int TLAgent::getCurrentPlan()
{
	return signalPlans[getCurrentState().n.curSignalPlan].signalPlanId;
}


void TLAgent::print() 
{
	cout << "tlight id " << tlightId << endl;
	vector<signalPlan>::iterator sp;
	for (sp = signalPlans.begin(); sp != signalPlans.end(); sp++) {
		cout << "\tsignalplan id " << (*sp).signalPlanId << endl;
		vector<phase>::iterator phase;
		for (phase = (*sp).phases.begin(); phase != (*sp).phases.end(); phase++) {
			cout << "\t\tphase id " << (*phase).phaseId << endl;
			cout << "\t\tstart " << (*phase).start << endl;
			cout << "\t\tend " << (*phase).end << endl;
			vector<direction>::iterator dir;
			for (dir = (*phase).directions.begin(); dir != (*phase).directions.end(); dir++) 
			{
				cout << "\t\t\tfrom lane " << (*dir).fromLane << endl;
				cout << "\t\t\tto laneset " << (*dir).toLaneset << endl;
			}
		}
	}
}


string TLAgent::newControlState()
{
	signalPlan changeTo;

	//process something and fill changeTo
		
	if (_planTemp_ == 3)
		_planTemp_ = 0;

	changeTo = signalPlans[_planTemp_];

	_planTemp_++;	

	return changeString_(changeTo);
}
	

string TLAgent::changeString_(signalPlan newSignalPlan) 
{
	bool changeSomething = true;

	string message = "";
		
	/* Message format
	The message consists of the following fields, separeted by ";"
		[trafficLightId;
			 [signalPlanId;
			 	numberOfPhases;
					[phaseId;
				 	phaseStartIter;
				 	phaseEndIter;
	*/


	//values to change for
	if (changeSomething) {

	message += intToString(tlightId) + ";";

	message += intToString(newSignalPlan.signalPlanId) + ";";

	vector<phase> phases = newSignalPlan.phases;

	message += intToString(phases.size()) + ";";

	vector<phase>::iterator itPhase;

	for (itPhase = phases.begin(); itPhase != phases.end(); itPhase++)
	{
		message += intToString((*itPhase).phaseId) + ";";
		int startIter = (*itPhase).start;
		int endIter = (*itPhase).end;

		message += intToString(startIter) + ";";
		message += intToString(endIter) + ";";
	}//for phase
				
		

	}//if

	return message;
}
