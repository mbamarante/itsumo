#include "tlagent.hh" 

#define INVALID_ID -1


TLAgent::TLAgent(int id, unsigned int memorySize, int states, int actions, bool writeStat, double learning_r, double discount_r) 
{
	_planTemp_ = 0;
	_memorySize_ = memorySize;
	_statesNumber_ = states;
	_actionsNumber_ = actions;
	_writeStat_ = writeStat;
	tlightId = id;
	totalInputLanesOnNetwork = 0;
	_learning_rate_ = learning_r;
	_discount_rate_ = discount_r;	
	_lastState_ = 0;
	_newState_  = 0;
	_lastAction_= -1;
	_lastReward_= 0.0;
	_newReward_= 0.0;		
	if(_writeStat_){
		string fileName2 = intToString(tlightId) + ".txt";
		try   
		{
      		_outFile2_.open(fileName2.c_str());
      		if (!_outFile2_.is_open())	throw "file not found.";      
   		}
   		catch(char *str)
   		{
      		cout << str << endl;
   		} 
		
	}
	init();
 }
TLAgent::TLAgent(){
	_planTemp_ = 0;
	_memorySize_ = 1;
	_statesNumber_ = 1;
	_actionsNumber_ = 1;
	_writeStat_ = false;
	tlightId = 0;
	totalInputLanesOnNetwork = 0;
	_learning_rate_ = 0;
	_discount_rate_ = 0;	
	_lastState_ = 0;
	_newState_  = 0;
	_lastAction_= -1;
	_lastReward_= 0.0;
	_newReward_= 0.0;		
	
	init();
}
TLAgent::~TLAgent()
{		
	_outFile2_.close();
}

void TLAgent::init() 
{
	lastAvgQueue= 0.1;
      _mec_ = parseMech("ql", _statesNumber_,_actionsNumber_);
      if (_mec_ == NULL) {
           printf("Fail creating learning mechanism.\n");
     }
}

void  TLAgent::setNetwork(topologyReader* n){
    _network_ = n;
    myNode = (n)->getNode(tlightId);    
}


void TLAgent::addTotalStoppedCarsOnNetwork(int totalSC) 
{	
	if (totalStoppedCarsOnNetwork.size() >= _memorySize_){
		totalStoppedCarsOnNetwork.erase(totalStoppedCarsOnNetwork.begin());	
	}
	totalStoppedCarsOnNetwork.push_back(totalSC);
}

void TLAgent::setTotalInputLanesOnNetwork(int totalIL) 
{	
	totalInputLanesOnNetwork = totalIL;
}


markov::mdp* TLAgent::parseMech( std::string _mechName, int _stateCount, int _actionCount)
{

    // Check Q-Learning.
    if ( _mechName == "ql" ) {
        markov::qlearning *mech = new markov::qlearning;
        if ( ! mech->create( _stateCount,   // number of states
                             _actionCount,  // number of actions
                             _discount_rate_,             // discount
                             0.05,             //epsilon-value or "curiosity", used for epsilon-greedy)
                             _learning_rate_            // learning rate
                             ) ) {
            return NULL;
        }
        return (markov::mdp*) mech;
    }
    return NULL;
}
//------------------------------------------------------------------------------



void TLAgent::updateState(simulationState newState) 
{
	if (_states_.size() >= _memorySize_)
		_states_.erase(_states_.begin());			

	_states_.push_back(newState);
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getSignalPlan
 *  Description:  Returns the signal plan with the given id
 * =====================================================================================
 */
signalPlan TLAgent::getSignalPlan(int planId) 
{
	for (unsigned int i=0; i< signalPlans.size(); i++) {	
		if(signalPlans[i].signalPlanId==planId){
			return(signalPlans[i]);
		}
	}
	struct signalPlan selectedPlan;
	selectedPlan.signalPlanId=-1;
	return selectedPlan;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getGreedyAction 
 *  Description:  Returns the id of the signal plan that gives priority to the lanes with 
 *                more stopped cars.                   
 * =====================================================================================
 */
int TLAgent::getGreedyAction() 
{
	int score    = 0;
	int bestScore= -1;
	int planId   = -1;
	int duration = 0;
	int total    = 0;
	vector<signalPlan>::iterator sp;
	vector<phase>::iterator phase;
	vector<direction>::iterator dir;
	for (sp = signalPlans.begin(); sp != signalPlans.end(); sp++) {
		score = 0;
		for (phase = (*sp).phases.begin(); phase != (*sp).phases.end(); phase++) {
			duration = (*phase).end-(*phase).start;
                        //cout<<"duration "<<duration<<endl;	
			total = 0;
			for (dir = (*phase).directions.begin(); dir != (*phase).directions.end(); dir++) 
			{
//				total+= getLaneState(getStoppedInLane((*dir).fromLane));//using discretization of the lane state
				total+= getLaneState(getDensityInLane((*dir).fromLane));//using discretization of the lane state				                            cout<<"total "<<total<<endl;	
			}
			score+=duration*total;
		}
		cout<<"signal plan "<<(*sp).signalPlanId<<"score:"<< score<<endl;		
		if (score>bestScore){
			bestScore = score;
			planId = (*sp).signalPlanId;
		}		
	}		
	return planId;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getStoppedInLane
 *  Description:  Returns the number of stopped cars in the lane with the given Id
 * =====================================================================================
 */
int TLAgent::getStoppedInLane(int laneId){
	vector<lane>::iterator itLane;
	vector<lane> inclanes =  getCurrentState().n.incomingLanes;
 	for(itLane = inclanes.begin(); itLane!= inclanes.end();itLane++) {
		if ((*itLane).laneId == laneId){
			return (*itLane).stoppedCars;
		}
	}
	return 0;	
}
/* 
 * Author: Denise de Oliveira
 * Method:getAvgFlowInLane(int laneId)
 * Desciption: Returns the average flow for the given lane considering 
 * the agents memory. We are keeping the *Flow* in the density variable 
 * just for compatibility reasons, this is set by the: 
 * LogalAgent::_connectMsg_ = "s;"
 * if LogalAgent::_connectMsg_ = "a;" than this method returns the avg density
 */
 double TLAgent::getAvgFlowInLane(int laneId){	
	vector<lane>::const_iterator itLane;
	vector<lane> inclanes;
	vector<simulationState>::const_iterator itState;
	double flow = 0.0;	
	double counter=0.0;
	for(itState = _states_.begin(); itState!= _states_.end();itState++){		
		inclanes =  (*itState).n.incomingLanes;		
		for(itLane = inclanes.begin(); itLane!= inclanes.end();itLane++) {
			if ((*itLane).laneId == laneId){
				flow+= (*itLane).density;
				++counter;
			}
		}	
 	}	
	return (flow/counter);	
}

/**
 * Author: Denise de Oliveira
 * Method: getAvgSpeed()
 * Description: returns the average speed over 
 * the incoming lanes
 */

double TLAgent::getAvgSpeed(){		
	double sat=0.0;
	double count =0;	
	vector<lane>::iterator itLane;	
	vector<lane> inclanes;
	vector<simulationState>::iterator itState;		
	for(itState = _states_.begin(); itState!= _states_.end();itState++){		
		inclanes =  (*itState).n.incomingLanes;		
		for(itLane = inclanes.begin(); itLane!= inclanes.end();itLane++) {
			sat += (*itLane).avgSpeed;
			++count; 														
		}					
 	}	
    return(sat/count);
}

/**
 * Author: Denise de Oliveira
 * Method: getAvgSatDegree()
 * Description: returns the maximum degree of saturation of the lanes entering the node
 * must be used with the connenction type "s;"
 */
double TLAgent::getAvgSatDegree(){
 	return getAvgSpeed();
}

/* 
 * Author: Denise de Oliveira
 * Method: getAvgSatDegInLane(int laneId)
 * Desciption: Returns the average degree of saturation for the given 
 * lane considering the agents memory. We are keeping the *degreeOfSaturation* 
 * in the avgSpeed variable just for compatibility reasons, this is set by the: 
 * LogalAgent::_connectMsg_ = "s;"
 * if LogalAgent::_connectMsg_ = "a;" than this method returns the avg Speed
 */
double TLAgent::getAvgSatDegInLane(int laneId){	
	vector<lane>::const_iterator itLane;
	vector<lane> inclanes;
	vector<simulationState>::const_iterator itState;
	double degreeOfSaturation = 0.0;	
	double counter=0.0;
	for(itState = _states_.begin(); itState!= _states_.end();itState++){		
		inclanes =  (*itState).n.incomingLanes;		
		for(itLane = inclanes.begin(); itLane!= inclanes.end();itLane++) {
			if ((*itLane).laneId == laneId){
				degreeOfSaturation += (*itLane).avgSpeed;
				++counter;
			}
		}	
 	}	
	return (degreeOfSaturation/counter);	
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getAvgStoppedInLane
 *  Description:  Returns the average number (over the states in memory) 
 * 				  of the stopped cars in the lane with the given Id
 * =====================================================================================
 */
double TLAgent::getAvgStoppedInLane(int laneId){	
	vector<lane>::iterator itLane;
	vector<lane> inclanes;
	vector<simulationState>::iterator itState;
	double stopped = 0.0;	
	double counter=0.0;
	for(itState = _states_.begin(); itState!= _states_.end();itState++){		
		inclanes =  (*itState).n.incomingLanes;		
		for(itLane = inclanes.begin(); itLane!= inclanes.end();itLane++) {
			if ((*itLane).laneId == laneId){
				stopped+= (double)((*itLane).stoppedCars);
				counter++;
			}
		}	
 	}	
	return (stopped/counter);	
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getAvgStoppedOnNetwork
 *  Description:  Returns the average number (over the states in memory) 
 * 				  of the stopped cars in the whole network
 * =====================================================================================
 */
double TLAgent::getAvgStoppedOnNetwork(){	
	vector<int>::iterator itStop;	
	int stopped = 0;
	int total = 0;
	for(itStop = totalStoppedCarsOnNetwork.begin(); itStop!= totalStoppedCarsOnNetwork.end();itStop++) {
			stopped+= (*itStop);					
			total++;					
 	}
	cout<<"avg stopped network"<<stopped/total<<endl;
	return (stopped/total);	
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getDensityInLane
 *  Description:  Returns the occupation density in the lane with the given Id
 * =====================================================================================
 */
double TLAgent::getDensityInLane(int laneId){
	vector<lane>::iterator itLane;
	vector<lane> inclanes =  getCurrentState().n.incomingLanes;
 	for(itLane = inclanes.begin(); itLane!= inclanes.end();itLane++) {
		if ((*itLane).laneId == laneId){
			return (*itLane).density;
		}
	}
	return 0;	
}

/* Get the current state for the agent.
 * uses a simplified state representation, resulting in 3 possible states:
 * 
 */
int TLAgent::getSimpleState(){
      int nodeBegin =0;
      double directions[4]={0.0,0.0,0.0,0.0};       
      int laneId =INVALID_ID;      
      vector<lane>::iterator itLane;
      vector<lane> inclanes =  getCurrentState().n.incomingLanes;        
      if (inclanes.size()>=2){
    	  for(itLane = inclanes.begin(); itLane!= inclanes.end();itLane++){
    		  laneId = (*itLane).laneId;
    		  nodeBegin = (_network_)->getBeginingNodeFromLane(laneId);
    		  if (nodeBegin != INVALID_ID){    		  
	    		  for (int i=0;i<4;i++){    	    			  
	    			  if (nodesAtDirections[i]==nodeBegin){
	    				  //directions[i] += getAvgStoppedInLane(laneId);    				  
	    				  directions[i] += getDensityInLane(laneId);
					 // cout << "directions[" <<i<<"]="<<directions[i]<<endl;
	    			  }
	    		  }
    		  }
    	  }
    	  double sumNS = directions[0]+directions[1];
    	  double sumEW = directions[2]+directions[3];
          //cout<<tlightId<<"-> ns "<<sumNS<<" ew "<<sumEW<<endl;
    	  int result=0;	  
    	  double diff = 0.2;//if the difference is equal or less 20%, 
    	                    //values are considered to be equal
    	
	  if ( (fabs (sumNS - sumEW)) <= (sumNS * diff) ) {
    		  result = 0; 
    	  }
    	  else if (sumNS>sumEW){
    		  result = 1;
    	  }
    	  else if (sumNS<sumEW){
    		  result = 2;
    	  }
    	//  cout<<"state "<<result<<endl;
    	  return result;
      }
                  
      return 0;
       
}
	
/// Get the current state for the agent.(9 states with 2 input lanes)
int TLAgent::getState(){
      int state = 0;         
      int index = 0;
      vector<lane>::iterator itLane;
      vector<lane> inclanes =  getCurrentState().n.incomingLanes;
      for(itLane = inclanes.begin(); itLane!= inclanes.end();itLane++){
    	  state += getLaneState((*itLane).laneId)* int(pow(3,index)); 	     
    	  index++;	    
      }	            
      return state;
       
}
/**
 * 
 * Considera cada direcao como um estado mais o estado onde todos sao iguais
 */
int TLAgent::getSimpleStateExtend(){
	int nodeBegin =0;
	double directions[4]={0.0,0.0,0.0,0.0};       
	int laneId =INVALID_ID;      
	vector<lane>::iterator itLane;
	vector<lane> inclanes =  getCurrentState().n.incomingLanes;              
	for(itLane = inclanes.begin(); itLane!= inclanes.end();itLane++){
		laneId = (*itLane).laneId;
		nodeBegin = (_network_)->getBeginingNodeFromLane(laneId);
		if (nodeBegin != INVALID_ID){    		  
			for (int i=0;i<4;i++){    	    			  
				if (nodesAtDirections[i]==nodeBegin){
					directions[i] += getAvgFlowInLane(laneId);    				  
				}
			}
		}
	}  
	unsigned int directions_size = sizeof(directions)/sizeof(directions[0]);
	double select = *max_element(directions, directions+directions_size) ;
	int result=0;
	double diff = 0.1;//if the difference of all values is equal or less 10%, 
	                    //values are considered to be equal  		
	for (int i=0;i<4;i++){    	    			  
		if (directions[i]==select){
		  result = i;
		  break;    				  
		}
	}
	double mean     	= gsl_stats_mean(directions, 1, 4);
	double deviation	= gsl_stats_sd_m(directions, 1, 4, mean);
	double cv = deviation/mean;    	  
	if (cv < diff )                   
		return 0;
	else 
		return result+1;
       
}


int TLAgent::myState(){
	switch(_statesNumber_){
		case 1:{
			return 0;
			break;
		}
		case 3:{
			return getSimpleState();
			break;
		}
		case 5:{
			return getSimpleStateExtend();
			break;
		}
		case 9:{
			return getState();
			break;
		}
		default:{
			return getSimpleState();
		}
	}
}
/**
 *  Get the current state for the agent considering the states of its partners
 *          0
 * 			1
 * 			2
 * 	0	0	3
 * 	1	0	4
 * 	2	0	5
 * 	0	1	6
 * 	1	1	7
 * 	2	1	8
 * 	0	2	9
 * 	1	2	10
 * 	2	2	11
 * 
***/
int TLAgent::getJointState(){
	int mystate = 0;         
	int index = 0;	
	mystate = getSimpleState();
	//cout<<">single state: "<<mystate<<" number of partners: "<<numpartners<<endl;
	vector<TLAgent*>::iterator tl;
	if (((mystate == 0)&&(_statesNumber_==12))||(_statesNumber_==27)){
		if (_statesNumber_==27){
			index=1;
			for(tl = partners.begin(); tl!= partners.end();tl++){			
				mystate += (*tl)->getSimpleState()*int(pow(3,index));		
				index++;		
			}
		}
		else{
			for(tl = partners.begin(); tl!= partners.end();tl++){			
				mystate += (*tl)->getSimpleState()*int(pow(3,index))+3*index;		
				index++;		
			}		
		}
	}
	cout<<">>joint state: "<<mystate<<endl;
	return mystate;
}


/// Get the current state for the agent.
int TLAgent::getStateGlobal(){
      double state = 0.0;         
      state = (double)getAvgStoppedOnNetwork()/(double)totalInputLanesOnNetwork;
      //cout<<"state "<<state<<endl;
      if (state<=7.0) return 0;
      else if (state <=15.0) return 1;
      else return 2;       
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getLaneState
 *  Description:  Returns the state of the lane [0,1,2] based on its number of stopped 
 *  		  cars.
 * =====================================================================================
 */
int TLAgent::getLaneState(int idLane){

	double sum = getAvgStoppedInLane(idLane);
	int min_length = 5;
    if ( sum <= min_length) { 
         return 0;   // empty   => blue 
    }
	else if ( sum <= 2*min_length) {
            return 1;   // regular => yellow
        }
	return 2;     // full    => red
	
}

int TLAgent::getLaneState(double occup){
    if ( occup <= 0.1) { 
            return 0;   // empty   => blue 
        }
	else if ( occup <= 0.3) {
            return 1;   // regular => yellow
        }
	return 2;     // full    => red
	
}

/*
 * The folowing reward funcion must be adapted in order to have a funcion that
 * is compatible to the number of lanes, the possible states and the optimum 
 * state.
 * rw=1-(2*(lane1+lane2)-ABS(lane1-lane2))/10
 * lane1   lane2    rw
 * 0        0       1.0  
 * 1   		0   	0.9
 * 2   		0   	0.8
 * 0   		1   	0.9
 * 1   		1   	0.6
 * 2   		1   	0.5
 * 0   		2   	0.8
 * 1   		2   	0.5
 * 2   		2   	0.2
 * 
 * 
 */ 
double TLAgent::getReward(){
//	double rw=0.0;
//	int index=0;
//    int incLane[2]; //2 is the number of incoming lanes! 
//	vector<lane*>::iterator itLane;	
//	vector<lane*> inclanes =  getCurrentState()->n->incomingLanes;
//        for(itLane = inclanes.begin(); itLane!= inclanes.end();itLane++) {		   
//			incLane[index] =getLaneState((*itLane)->laneId);
//			//cout<<"lane"<<incLane[index]<<endl;
//			index++;
//	}    
//    rw = 1.0-(2*(incLane[0]+incLane[1])-abs(incLane[0]-incLane[1]))/10.0;
//    //cout<<"get reward = "<<rw<<endl;
//    if (rw<0){rw =0;}
//	return (rw);
//			 
	double avgstop = getAvgLine();
	double dec = 0.04; //larger dec values give a faster rw decay (i.e: dec=0.5, avgstop=5 rw = 0.08)
	if (avgstop<=10){
		return(1.0);
	}
	if(avgstop>=20){
		return(0);
	}
	else{
		return(exp(-(avgstop-10)*dec));			
	}


	
}
double TLAgent::getComparativeReward(){
	double avgQueue = getAvgLine();
 	double relation = 1.0;
    if (avgQueue != 0) {
            if (lastAvgQueue !=0) //so para evitar que de dizvizao por zero na primeira vez!
                    relation = avgQueue/lastAvgQueue;
            //cout <<"relation "<<relation << " avgQueue: "<<avgQueue <<" lastAvgQueue: "<<lastAvgQueue <<endl;
            lastAvgQueue = avgQueue;

            if ( relation > 1.1 ) //aumentou a fila!
                    return -1.0;
            if ( relation < 0.9 ) //diminuiu a fila!
                    return 1.0;
            else                              //nao aumentou nem diminuiu!
                    return -0.01;
    }
    return 0.1; //nao tem fila!
}

double TLAgent::getThreeStepReward(){
double avgQueue = getAvgLine();
	if (avgQueue>1.5)
		return -1.0 ;
	if (avgQueue<0.25)
		return 1.0;
	else
		return -0.1;  
}
double TLAgent::getSimpleReward(){	 
	double rw = (MAXSTOPPED - getAvgLine())/MAXSTOPPED;
	/*double rw = 0.0;
	double avgLine = getAvgLine();
	if (avgLine<=0) rw = 1.0;
	else rw = 1/avgLine;*/	
    cout<<" reward = "<<rw<<endl;
	 return(rw);
}

double TLAgent::getWLUSimpleReward(){	 
	double avgstop = getAvgLine();
	double rw =0.0;
	/*tabela Denise
	if (avgstop<10){
		rw = 1.0;
	}
	else if (avgstop<21){
		rw = 0.5;
	}
	else if (avgstop<31){
		rw = 0.25;
	}
	else{
		rw = 0.1;
	}
	*/
	
	if (avgstop<=1) rw = 1;
	else if (avgstop<=   5) rw = 0.98;
	else if (avgstop<=  10) rw = 0.97;
	else if (avgstop<=  15) rw = 0.92;
	else if (avgstop<=  20) rw = 0.88;
	else if (avgstop<=  25) rw = 0.84;
	else if (avgstop<=  30) rw = 0.83;
	else if (avgstop<=  35) rw = 0.82;
	else if (avgstop<=  40) rw = 0.81;
	else if (avgstop<=  45) rw = 0.80;
	else if (avgstop<=  50) rw = 0.78;
	else if (avgstop<=  55) rw = 0.76;
	else if (avgstop<=  60) rw = 0.73;
	else if (avgstop<=  65) rw = 0.70;
	else if (avgstop<=  70) rw = 0.67;
	else if (avgstop<=  75) rw = 0.63;
	else if (avgstop<=  80) rw = 0.55;
	else if (avgstop<=  85) rw = 0.45;
	else if (avgstop<=  90) rw = 0.30;
	else if (avgstop<=  95) rw = 0.15;
	else 					rw = 0.01;
	//cout<<"> reward = "<<rw<<endl;
	return(rw);
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getExpReward
 *  Description:  Returns the local reaward calculating an exponential function related to 
 *                the average number of stopped vehicles at the input lanes of the node. 
 *                This average is over the memory values of the traffic agent.
 * =====================================================================================
 */
double TLAgent::getExpReward(){
	double rw;
	double avgstop = getAvgLine();
	double dec = 0.04; //larger dec values give a faster rw decay (i.e: dec=0.5, avgstop=5 rw = 0.08)
	
	rw = exp(-((avgstop)*dec));
	
	//cout<<"> reward = "<<rw<<endl;
	return(rw);
}

double TLAgent::getExpRewardDualFunc(){
	double rw;
	double avgstop = getAvgLine();
	double changePt = 10;
	
	// dual function reward
	
	//reward uses function 1
	if (avgstop <= changePt) {
		//rw = (1-(avgstop/100));
		rw = 1;
	}
	
	//reward uses function 2
	else {
		rw = exp( -(avgstop/changePt) ) + (1 - exp(-1));
	}
	
	cout << "> reward = " << rw << " avgstop = " << avgstop << endl;
	return(rw);
}

double TLAgent::getGlobalReward(){
	
	//cout<<"total input lanes"<<totalInputLanesOnNetwork<<endl;
    double rw = (MAXSTOPPED-(getAvgStoppedOnNetwork()/(double)totalInputLanesOnNetwork))/MAXSTOPPED;  
    //cout<<">> global reward = "<<rw<<endl;    
	return (rw);
}

double TLAgent::getGlobalRewardWithoutMe(){	
	double rw=0.0;
	double numLanes = (double)(_states_).at(0).n.incomingLanes.size() ;
	    rw = (MAXSTOPPED-(getAvgStoppedOnNetwork() - getAvgLine()*numLanes)/((double)totalInputLanesOnNetwork - numLanes))/MAXSTOPPED;
	return (rw);
}

double TLAgent::getAvgLine(){		
	double sumAvg=0.0;
	double avg=0.0;
	vector<lane>::iterator itLane;	
	vector<lane> inclanes;
	vector<simulationState>::iterator itState;		
	for(itState = _states_.begin(); itState!= _states_.end();itState++){		
		inclanes =  (*itState).n.incomingLanes;		
		for(itLane = inclanes.begin(); itLane!= inclanes.end();itLane++) {
			sumAvg+= (*itLane).stoppedCars;								
		}			
	//	cout<<"fila media das lanes: "<<sumAvg<<" tl id: "<<(*itState).n.tlightId<<" no step: "<<(*itState).curIteration<<endl;
 	}	
	
	avg = sumAvg/((double)_states_.size()*(double)inclanes.size());
//	cout<<"fila media das lanes: "<<sumAvg<<" numero de estados: "<<_states_.size()<<endl;
	//cout<<"fila media: "<<avg<<endl;
    return(avg);
}


string TLAgent::newControlState()
{

	signalPlan changeTo;
	//updates the RL state and observe
	_lastAction_ = _newAction_;
	_lastState_= _newState_;
	_lastReward_ = _newReward_;
	_newState_ = myState();	
	
	if (_lastState_>-1){
		/* reward functions:
		*   getExpRewardDualFunc();
		* 	getExpReward();
		* 	getGlobalReward();
		*	getGlobalRewardWithoutMe();
		*	getSimpleReward();
		*	getWLUSimpleReward();
		*	getExpReward();
		*	getExpRewardDualFunc();
		*	getReward();
		*	getDifferenceReward();
		* 	getComparativeReward();
		* 
		*/
		
		_newReward_ = getComparativeReward();//getExpReward();		
		cout<<"rw"<<_newReward_<<endl;	
		_mec_->observe(_lastState_,_lastAction_,_newState_,_newReward_);			
		if(_writeStat_){
			
			_outFile2_<<_mec_->printQTable()<<endl;
		}
	}
	_newAction_ =  _mec_->act(_newState_);
	if ((_newAction_>-1)&&(_newAction_<_actionsNumber_)){ 
		//the act method from the mechanism returns actions from 0 to n 	     
		//the _newAction_ value must remain betwwen 0 and n because of the RL mechanism
		//the plans on itsumo start on 1
		changeTo = getSignalPlan(_newAction_+1);	
		if ((changeTo.signalPlanId>-1)&&(changeTo.signalPlanId<=_actionsNumber_)){
			return changeString_(changeTo);
		}	
	}
	return "";
}
	

string TLAgent::changeString_(signalPlan newSignalPlan) 
{
	bool changeSomething = false;
	if(newSignalPlan.signalPlanId>-1){
		changeSomething = true;
	}
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
//		cout<<"tlight id:"<<intToString(tlightId)<<endl;
		message += intToString(newSignalPlan.signalPlanId) + ";";
//		cout<<"sp id:"<<intToString(newSignalPlan.signalPlanId)<<endl;

/*		Commented: just send the plan id, since the agent is not ALTERING the plan itself, just selecting		
  		vector<phase> phases = newSignalPlan.phases;

		message += intToString(phases.size()) + ";";
		//	cout<<"num. phases:"<<intToString(phases.size())<<endl;

		vector<phase>::iterator itPhase;

		for (itPhase = phases.begin(); itPhase != phases.end(); itPhase++)
		{
			message += intToString((*itPhase).phaseId) + ";";
			int startIter = (*itPhase).start;
			int endIter = (*itPhase).end;
			message += intToString(startIter) + ";";
			message += intToString(endIter) + ";";
		}//for phase
*/
	}//if

	return message;
}

