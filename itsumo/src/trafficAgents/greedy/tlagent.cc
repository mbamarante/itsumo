#include "tlagent.hh" 
#include "../../basic/utils.hh"


TLAgent::TLAgent(int id, bool select, int memorySize, bool writeStat, int minPhase, float K) 
{
  _select_ = select;
  _memorySize_ = memorySize;
  _writeStat_ = writeStat;
  tlightId = id;

  if (_writeStat_) {
    string fileName = "../../../output/" + intToString(id) + ".log";
    _outFile_.open(fileName.c_str());
//  fileName = "../../../output/avg" + intToString(id) + ".log";
//  _outFileAvg_.open(fileName.c_str());
    fileName = "../../../output/phase" + intToString(id) + ".log";
    _outFilePhase_.open(fileName.c_str());
  }

  _contOut_ = 0;

  _minPhase_ = minPhase;
  _K_ = K;
}

TLAgent::~TLAgent()
{
  if (_writeStat_) {		
     _outFile_.close();
//  _outFileAvg_.close();
     _outFilePhase_.close();
  }
}

void TLAgent::updateState(simulationState newState) 
{
  if (_states_.size() >= _memorySize_)
    _states_.erase(_states_.begin());			

  _states_.push_back(newState);

  //newState->print();
  //print();

  if (_writeStat_) {
    string temp = newState.outline();
    _outFile_ << temp << "\n";
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
      for (dir = (*phase).directions.begin(); dir != (*phase).directions.end(); dir++) { 
        cout << "\t\t\tfrom lane " << (*dir).fromLane << endl;
        cout << "\t\t\tto laneset " << (*dir).toLaneset << endl;
      }
    }
  }
}


int TLAgent::maxMeanStoppedCars()
{
  double avg_density[1000];
  int lanesId[1000];

  for (int i=0; i<1000; i++)
    avg_density[i] = 0;


  int c = 0;
  float max = -1;
  int maxLaneId = -1;

  vector<simulationState>::iterator itState;

  for (itState = _states_.begin(); itState != _states_.end(); itState++) {
    c = 0;

    vector<lane> inclanes = (*itState).n.incomingLanes;
    vector<lane>::iterator itLane;
    for (itLane = inclanes.begin(); itLane != inclanes.end(); itLane++) {
      avg_density[c] += (*itLane).stoppedCars;
      lanesId[c] = (*itLane).laneId;

      if (avg_density[c] > max) {
        max = avg_density[c];
        maxLaneId = lanesId[c];
      }
      c++;
    }
  }

  
  for (int i=0; i<c; i++) 
    avg_density[i] /= _states_.size();


//  if (_writeStat_) {
//    _outFileAvg_ << _contOut_ << "\t";
//    for (int i=0; i<c; i++)
//      _outFileAvg_ << avg_density[i] << "\t";
//    _outFileAvg_ << "\t\t laneId: " << maxLaneId << "\n";
//  } 

  return maxLaneId;
}


double TLAgent::getAvgDensityInLane(int laneId)
{
  vector<simulationState>::iterator itState;

  int c = 0;
  double avg = 0.0;

  for (itState = _states_.begin(); itState != _states_.end(); itState++) {
    vector<lane>::iterator itLane;
    vector<lane> inclanes = (*itState).n.incomingLanes;

    for(itLane = inclanes.begin(); itLane != inclanes.end(); itLane++) {
	if ((*itLane).laneId == laneId) {
 		avg += (*itLane).density;
        	break;
	}
    }

    c++;
  }	

  return avg/c;	
}


double TLAgent::getDensityInLane(int laneId)
{
	vector<lane>::iterator itLane;
	vector<lane> inclanes = getCurrentState().n.incomingLanes;
	
 	for(itLane = inclanes.begin(); itLane != inclanes.end(); itLane++) {
		if ((*itLane).laneId == laneId) {
			return (*itLane).density;
		}
	}
	
	return 0;	
}


int TLAgent::getGreedyAction() 
{
	int planId   = -1;
	int duration = 0;

	double score    = 0;
	double bestScore= -1;
	double total    = 0;
	
	
	vector<signalPlan>::iterator sp;
	vector<phase>::iterator phase;
	vector<direction>::iterator dir;
	
	for (sp = signalPlans.begin(); sp != signalPlans.end(); sp++) {
		
		score = 0;
		
		for (phase = (*sp).phases.begin(); phase != (*sp).phases.end(); phase++) {
			duration = (*phase).end - (*phase).start;
        	
			int c=0;
			total = 0;
			for (dir = (*phase).directions.begin(); dir != (*phase).directions.end(); dir++) {
				c++;
				total += getAvgDensityInLane((*dir).fromLane);	
			}

			score += (total / c) * duration;
		}
				
		if (score>bestScore) {
			bestScore = score;
			planId = (*sp).signalPlanId;
		}		
	}
	
	return planId;
}


signalPlan TLAgent::getSignalPlan(int planId) 
{
	struct signalPlan selectedPlan;	
	
	selectedPlan.signalPlanId = -1;
	
	for (int i=0; i<signalPlans.size(); i++) {	
		if(signalPlans[i].signalPlanId == planId){
			return(signalPlans[i]);
		}
	}
	return selectedPlan;
}


string TLAgent::newControlState()
{
    _contOut_ += _memorySize_;


  if (_select_) {
  	int newPlanId = getGreedyAction();

	if (_writeStat_) {
    		_outFilePhase_ << _contOut_-_memorySize_ << "\t";
		vector<phase>::iterator itPhase;
		for (itPhase = getSignalPlan(newPlanId).phases.begin(); itPhase != getSignalPlan(newPlanId).phases.end(); itPhase++) 
      			_outFilePhase_ << (*itPhase).end - (*itPhase).start + 1 << "\t";
    		_outFilePhase_ << endl;
 	}

	return changeString_(getSignalPlan(newPlanId));
  }

  int maxLaneId = maxMeanStoppedCars();
  
  int modificaId = -1;
  int numPhases = 0;
  int numInterphases = 0;

  int duracoes[1000];
  bool interphase[1000];

  int sum = 0;


  vector<phase>::iterator itPhase;
  for (itPhase = signalPlans[0].phases.begin(); itPhase != signalPlans[0].phases.end(); itPhase++) {
    duracoes[numPhases] = (*itPhase).end - (*itPhase).start + 1;
    sum += duracoes[numPhases];

    if ((*itPhase).directions.empty()) {
	interphase[numPhases] = true;
	numInterphases++;
    } else {
	interphase[numPhases] = false;

        vector<direction>::iterator itDir;
        for (itDir = (*itPhase).directions.begin(); itDir != (*itPhase).directions.end(); itDir++) 
          if ( (*itDir).fromLane == maxLaneId ) 
            modificaId = numPhases;
    }

    numPhases++;
  }	



  if (_writeStat_) {
    _outFilePhase_ << _contOut_-_memorySize_ << "\t";
    for (int i=0; i<numPhases; i++)
      _outFilePhase_ << duracoes[i] << "\t";
    _outFilePhase_ << endl;
  }



  int newSum = 0;
  int plus = (int)floor(duracoes[modificaId] * _K_);
  int minus = (int)floor(plus) / (numPhases - numInterphases);

  for (int i=0; i<numPhases; i++) {
    if (i != modificaId) {
	if (!interphase[i]) {
           duracoes[i] -= minus;
           if (duracoes[i] < _minPhase_)
              duracoes[i] = _minPhase_;
        }
        newSum += duracoes[i];
    }
  }

  duracoes[modificaId] = sum - newSum;



  int i = 0;
  int prox_start = 0;

  for (itPhase = signalPlans[0].phases.begin(); itPhase != signalPlans[0].phases.end(); itPhase++) {
    (*itPhase).start = prox_start;
    (*itPhase).end = prox_start + duracoes[i] - 1;
    prox_start += duracoes[i];
    i++;
  }

  
  return changeString_(signalPlans[0]);
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
