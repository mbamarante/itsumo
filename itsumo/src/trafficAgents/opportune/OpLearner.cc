
#include "OpLearner.hh" 
const int OpLearner::INVALID_ID			= -1; 
//reward functions
const unsigned int OpLearner::SIMPLE		= 0;
const unsigned int OpLearner::EXP			= 1;
const unsigned int OpLearner::COMPARATIVE	= 2;
const unsigned int OpLearner::THREESTEP		= 3;

OpLearner::OpLearner(bool eps, unsigned int rwf, double rwtfvalue, int id, int memorySize, int states, int actions, bool writeStat, double learning_r, double discount_r, double max_error)
		:TLAgent(id, memorySize, states, actions, writeStat, learning_r, discount_r)		        
{	
	_curiosity_ = 0.05;	
	_rewardfunction_= rwf;
	_rewardfunction_value_= rwtfvalue;
	episodic = eps;
	_maximum_error_ = max_error;	
	setLearning();
	init();
	
}

OpLearner::OpLearner(bool eps, int id, int memorySize, int states, int actions, bool writeStat, double learning_r, double discount_r, double max_error)
		:TLAgent(id, memorySize, states, actions, writeStat, learning_r, discount_r)		        
{	
	_curiosity_ = 0.05;	
	episodic = eps;
	_maximum_error_ = max_error;	
	setLearning();
	init();
	
}

OpLearner::~OpLearner(){			
	
//	if(_writeStat_){			
//		_outFile2_<<_learning_->print()<<endl;
//		if (sentMessages>0) _outFile2_<<sentMessages<<endl;
//	}
	_outFile2_.close();
	
	mypartners.clear();
	clearMessages();
	
}


void OpLearner::init(){
	sentMessages 			= 0;
	perfomedJointActions	= 0;
	usedJointStates			= 0;
	episodes 				= 0;
	actSteps				= 0;
	mySelectedActionValue 	= 0.0;
	testConv				= true;	
	lastState.push_back( new SAComponent(tlightId, 0));
	lastAction.push_back( new SAComponent(tlightId, 0));	
	_lastAction_	= 0;
	_newAction_		= 0;
	_lastState_		= 0;
	_newState_		= 0;				
	_lastReward_	= 0.0; 
	_newReward_		= 0.0; 	
}

void OpLearner::endEpisode(){
	if(_writeStat_){			
		_outFile2_<<sentMessages<<"\t"<<perfomedJointActions<<"\t"<<usedJointStates<<"\t"<<_learning_->getSize()<<endl;
	}
	_lastAction_	= _lastAction_;
	_newAction_		= 0;
	_lastState_		= _lastState_;
	_newState_		= 0;	 
	_newReward_		= 10.0; 
	++episodes;	
	sentMessages 	= 0;
	_curiosity_=0.999*_curiosity_;	
	_learning_->setEpsilon(_curiosity_);
	
//	if((_writeStat_)&&((episodes%100)==0)){			
//		_outFile2_<<_learning_->print()<<endl;
//		if (sentMessages>0) _outFile2_<<sentMessages<<endl;
//	}
}
simulationState OpLearner::getCurrentState() {
	if (_states_.size() > 0)
		return _states_.back();
	else return simulationState();
}


/**
 * Creates the learning mechanism
 */
void OpLearner::setLearning(){
	_learning_ = new DQTable(tlightId,getSingleStatesNumber(),getSingleActionsNumber(), getCuriosity(), getLearningRate(), getDiscountRate());	
}		

void OpLearner::setComunicationArea(set<int> commarea){
	communicationArea = commarea; 
}


void OpLearner::setNetwork(topologyReader* net){
	_network_ = net;
}

/**
 * Includes the given id in the communiationArea vector.
 * Keeps the vector always sorted based on the id's.
 * Returns false if the elements was already in the communication area.
 */
bool OpLearner::addToComunicationArea(int id){
	pair<set<int>::iterator,bool> ret = communicationArea.insert(id);
	return ret.second; 
}




/* Get the current state for the agent.
 * uses a simplified state representation, resulting in 3 or 5 possible states according to the 
 * average flow or ocupation density for the lanes
 *   equal flow/occupation (+- 20%): 0; 
 *    NS > EW: 1
 *    NS < EW: 2;
 * 
 */
int OpLearner::getSimpleState(){
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
	    				  directions[i] += getAvgFlowInLane(laneId);    				  
					  //cout << "directions[" <<i<<"]="<<directions[i]<<endl;
	    			  }
	    		  }
    		  }
    	  }
    	  double sumNS = directions[0]+directions[1];
    	  double sumEW = directions[2]+directions[3];
      //  cout<<tlightId<<"-> ns "<<sumNS<<" ew "<<sumEW<<endl;
    	  int result=0;	  
    	  double diff = 0.1;//if the difference is equal or less 10%, 
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
    	  return result;
      }
                  
      return 0;
       
}
/**
 * 
 * Considera cada direcao como um estado mais o estado onde todos sao iguais
 */
	int OpLearner::getSimpleStateExtend(){
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


int OpLearner::myState(){
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
 * sets the new single state with a calculated state value
 * using myState() method and construcs the composed state 
 */
void OpLearner::getComposedState(){
	_newState_ = myState();
	constructComposedState();
}
/**
 * sets the new single state with re received value and 
 * construcs the composed state 
 */
void OpLearner::getComposedState(int state){
	_newState_ = state;
	constructComposedState();
}
/**
 * Constructs the best composed state based on
 * the new state
 */
void OpLearner::constructComposedState(){
//	cout<<"simple state" << _newState_<<endl;		
	vector<SAComponent*> simState;
	simState.push_back( new SAComponent(tlightId, _newState_) );
	vector<SAComponent*> compState = _learning_->getBestContainsState(simState);	
	if ( !equalSA(compState, simState)){					
		callInfo(compState,Message::REQUEST_CURRENT_STATE_INFO, 0);				
	} 		
	sort(compState.begin(), compState.end(),saCompare);
	currentState = compState;
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getExpReward
 *  Description:  Returns the local reaward calculating an exponential function related to 
 *                the average number of stopped vehicles at the input lanes of the node. 
 *                This average is over the memory values of the traffic agent.
 *                -> larger dec values give a faster rw decay (i.e: dec=0.5, avgstop=5 rw = 0.08)
 * =====================================================================================
 */
double OpLearner::getExpReward(double dec=0.04){
	double rw;
	double avgstop = getAvgLine();
	rw = exp(-((avgstop)*dec));
	
	//cout<<"> reward = "<<rw<<endl;
	return(rw);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getExpReward
 *  Description:  maxstopped is the maximum number of stopped vehicles 
 * =====================================================================================
 */
double OpLearner::getSimpleReward(int maxstopped=60){	 
	return (maxstopped- getAvgLine())/maxstopped;	
}
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getComparativeReward
 *  Description:  reward relative to the last queue 
 * =====================================================================================
 */

double OpLearner::getComparativeReward(){
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
    return 10; //nao tem fila!
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getThreeStepReward()
 *  Description:  reward relative to the avgLine but only with 3 possible 
 * outcomes according to the maximum allowed avg. queue 
 * =====================================================================================
 */
double OpLearner::getThreeStepReward(double maxavg=1.5){
double avgQueue = getAvgLine();
	if (avgQueue>maxavg)
		return -1.0 ;
	if (avgQueue<((maxavg)/6))
		return 1.0;
	else
		return -0.1;  
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getReward
 *  Description:  Returns the local reward acording to the reward function id
 * =====================================================================================
 */
double OpLearner::getReward(unsigned int rewardtype){	 
	switch(rewardtype){
		case OpLearner::SIMPLE:{
			return getSimpleReward();
			break;
		}
		case OpLearner::EXP:{
			return getExpReward();
			break;
		}
		case OpLearner::COMPARATIVE:{
			return getComparativeReward();
			break;
		}
		case OpLearner::THREESTEP:{
			return getThreeStepReward();
			break;
		}
		default:{
			return getSimpleReward();			
		}
	}
}

unsigned int OpLearner::getSingleStatesNumber(){
	return _statesNumber_;
}

unsigned int OpLearner::getSingleActionsNumber(){
	return _actionsNumber_;
}	
	
double OpLearner::getCuriosity(){
	return _curiosity_;
}	

double OpLearner::getLearningRate(){
	return  _learning_rate_;
}


double OpLearner::getDiscountRate(){
	return _discount_rate_;
}

int OpLearner::getClosestPartnerNotIn(vector<SAComponent*> sa){
	SAComponent* comp;
	set<int>::const_iterator p;
	int dif = 1;
	int closest =0;	
	//cout<<"comm area size "<<communicationArea.size()<<endl;
	if(sa.size() < (communicationArea.size()+1) ){		
		for( p= communicationArea.begin(); p!= communicationArea.end(); p++){
			//cout<<" communication :"<< *p<<endl;		
			comp = new SAComponent(*p,0);
			if(!comp->ownerIsIn(sa)){
				return (*p); 
			}			
		}			
	}
	return OpLearner::INVALID_ID;
}
/**
 * Analize the last epiosode and returns tre if the last pair 
 * has an error and a new pair must be created. 
 * Return fa
 */
string OpLearner::analyzeLastEpisode(){

	//individual values:
	_lastAction_	= _newAction_;
	_lastState_		= _newState_;
	//composed values:
	lastAction		= currentAction;
	lastState		= currentState;
				
	_lastReward_	= _newReward_;
	
	int to = INVALID_ID;
	
	
	if (( _learning_->getStateActionError(lastState,lastAction) > _maximum_error_ )||(_newReward_==10.0)){
		//cout<<"erro maior que o máximo no ultimo par!"<<endl;
		//try to increase the state knowledge				
		to = getClosestPartnerNotIn(lastState);		
		unsigned int msgType;
		if( to!= INVALID_ID){
			msgType = Message::REQUEST_LAST_STATE_INFO;
		}
		else{			
			to = getClosestPartnerNotIn(lastAction);						
			if (to!= INVALID_ID){
				msgType = Message::REQUEST_LAST_ACTION_INFO;	
				//cout<<"agente "<< tlightId <<" erro"<<_learning_->getStateActionError(lastState,lastAction)<<endl;			
			}
			else{
				return "";
			}
		}
		//cout<<"to:"<<to<<"msg type "<<msgType<<endl;
		return askInfo((unsigned int)to,msgType);
	}
	return "";
}


/**
 * Analize the last epiosode and returns tre if the last pair 
 * has an error and a new pair must be created. 
 * Return fa
 */
string OpLearner::analyzeLastEpisodePP(){

	//individual values:
	_lastAction_	= _newAction_;
	_lastState_		= _newState_;
	//composed values:
	lastAction		= currentAction;
	lastState		= currentState;
				
	_lastReward_	= _newReward_;
	
	int to = INVALID_ID;
	
	
	
	if ( _learning_->getStateActionError(lastState,lastAction) > _maximum_error_ ){
		//cout<<"erro maior que o máximo no ultimo par!"<<endl;
		
		//try to increase the state knowledge				
		if (_lastState_<625){
			//se o outro agente esta na area de comunicação
			to = getClosestPartnerNotIn(lastState);		
			unsigned int msgType;
			if( to!= INVALID_ID){
				msgType = Message::REQUEST_LAST_STATE_INFO;
			}
			else{			
				to = getClosestPartnerNotIn(lastAction);						
				if (to!= INVALID_ID){
					msgType = Message::REQUEST_LAST_ACTION_INFO;	
					//cout<<"agente "<< tlightId <<" erro"<<_learning_->getStateActionError(lastState,lastAction)<<endl;			
				}
				else{
					return "";
				}
			}
			//cout<<"to:"<<to<<"msg type "<<msgType<<endl;
			return askInfo((unsigned int)to,msgType);
		}
	}
	return "";
}



/**
 * Uses the received information type for a given state or action.
 * Returns the value updated with received iformation or the 
 * value not changed.
 * 
 */
vector<SAComponent*> OpLearner::useReceivedInfoFor(vector<SAComponent*> toUpdate, unsigned int infoType){
	if (!toUpdate.empty()){
		vector<SAComponent*> info = collectInfo(infoType);
		vector<SAComponent*>::iterator i;
		SAComponent* upComponent;
		if (!info.empty()){									
			for(i= info.begin(); i!=info.end(); ++i){ // test if thre is new info about the agents already envolved in the value to update
				upComponent = getComponentFromOwner(toUpdate, (*i)->getOwner());			
				if ( upComponent != NULL ){
//					cout<<"componente "<<(upComponent)->print()<<endl;	
//					cout<<"owner "<<(*i)->getOwner()<<endl;
//					cout<<"info "<<printSAVector(info)<<endl;
//					cout<<"to update "<<printSAVector(toUpdate)<<endl;
					toUpdate = removeComponent(toUpdate, upComponent);
				}				
			}			
			vector<SAComponent*> newValue = vector_unique_union( info, toUpdate );
			return newValue;
		}
	}
	sort(toUpdate.begin(),toUpdate.end(),saCompare);	
	return toUpdate;	
}


void OpLearner::observeLast(){	
	bool change = false;
	vector<SAComponent*> rstate = useReceivedInfoFor(lastState,Message::INFORM_LAST_STATE_INFO);
	if ((!lastState.empty())&&(rstate.size() > lastState.size())){
		lastState = rstate;				
		change = true;
	}
	vector<SAComponent*> ract = useReceivedInfoFor(lastAction,Message::INFORM_LAST_ACTION_INFO);
	if ((!lastAction.empty())&&(ract.size() > lastAction.size())){
		lastAction = ract;
		change = true;
	}	
	sort(lastState.begin(),lastState.end(),saCompare);
	sort(lastAction.begin(),lastAction.end(),saCompare);				
	if ( (change) && !( _learning_->contains(lastState,lastAction) ) ){ 		
		if( !(_learning_->addActionToState(lastState, lastAction, 0.0)) ){
			// state is not in the learning table
			vector<CQValue*> values;																
			values.push_back(new CQValue(lastAction, 0.0));
			_learning_->addState( new stateRecord(lastState,values) );		
		}		
		//cout<<_learning_->print()<<endl;
	}
	clearMessages();	
}

void OpLearner::observeNewState(){	
	vector<SAComponent*> rstate = useReceivedInfoFor(currentState,Message::INFORM_CURRENT_STATE_INFO);
	if ((!currentState.empty())&&(rstate.size() > currentState.size())){
		currentState = rstate;				
	}
	if ( currentState.size()>1) ++usedJointStates;		
	sort(currentState.begin(),currentState.end(),saCompare);	
	clearMessages();
}
	
	
void OpLearner::observe(){			
	//cout<<"updates the RL state and observe"<<endl;	 
	if (!lastState.empty()){		
		_newReward_ = getReward(_rewardfunction_);		
		//cout<<"reward: "<<_newReward_<<endl;
		
		_learning_->observe(lastState,lastAction,currentState,_newReward_);
		
		if ( lastAction.size()>1) ++perfomedJointActions;
		if(!episodic)
			if(_writeStat_){			
			_outFile2_<<sentMessages<<"\t"<<perfomedJointActions<<"\t"<<usedJointStates<<"\t"<<_learning_->getSize()<<endl;
			}			
	}		
}

void OpLearner::observe(double rw){					
	if (!lastState.empty()){		
		_newReward_ = rw;							
		_learning_->observe(lastState,lastAction,currentState,_newReward_);
//		if (rw ==10){
//			cout<<"last "<<printSAVector(lastState)<<"act: "<<printSAVector(lastAction)<<" current "<<printSAVector(currentState)<<endl;
//		}			
	}		
}





/**
 * Call for a joint action
 * returns false in the case the action is individual  
 */
bool OpLearner::callJointAction(vector<SAComponent*> action, double value){
	return callInfo(action, Message::PROPOSE_JOINT_ACTION, value);
}
/**
 * Request state/action information from other agents, 
 * according to the information type (infotype) passed
 * returns false in the case the state/action has only the local view  
 */
bool OpLearner::callInfo(vector<SAComponent*> info, unsigned int infotype, double value){
	if (info.size()>1){		
		for(unsigned int i=1; i<info.size();i++){	
			if (info[i]->getOwner()!= tlightId) //do not send for myself!	
				outgoingMessages.push_back(new Message(info[i]->getOwner(),tlightId,infotype,info[i]->getInformation(),value));
		}		
		return true;
	}
	return false;
}

/**
 * Request state/action information from other agent, 
 * according to the information type (infotype) passed  
 */
string OpLearner::askInfo(unsigned int to, unsigned int infotype){
	Message* msg = new Message(to,tlightId,infotype,0,0);
	return msg->toString();				
}

/** 
 * returns the collect information with the given performative type 
 * given by "infotype", received from other agents
 *   
 */
vector<SAComponent*>  OpLearner::collectInfo(unsigned int infotype){	
	vector<SAComponent*> info;
    vector<Message*>::const_iterator msg;	
	for(msg = incomingMessages.begin(); msg!=incomingMessages.end(); msg++){	
		if ((*msg)->getPerformative()==infotype){
			info.push_back( new SAComponent((*msg)->getSender(), (*msg)->getContent()) );
		}
	}
	sort( info.begin(), info.end(),saCompare );		
	return info;
}
/**
 * Add the reply message to the outgoing list
 */
void OpLearner::acceptActionOffer(Message* msg){		
	outgoingMessages.push_back(new Message((msg)->getSender(),tlightId,Message::AGREE_JOINT_ACTION,(msg)->getContent(),0));
	acceptedActionMessages.push_back(msg); //puts the accepted message in the list;	                               	
}



/**
 * Select the best joint action offer and 
 * accepts all offers for the same action
 * refuse all other offers!
 * 
 */
void OpLearner::acceptAllBestActionOffers(){	
	acceptedActionMessages.clear();
	//select the message with the highest value
	if ( !incomingMessages.empty() ){
		Message* bestmsg = getBestJointActionOffer();		
		if ( bestmsg != *(incomingMessages.end()) ){		
			if (bestmsg->getValue() > mySelectedActionValue){				
				vector<Message*>::const_iterator msg;
				for(msg = incomingMessages.begin(); msg!=incomingMessages.end(); msg++){	
					if ( ((*msg)->getPerformative()== Message::PROPOSE_JOINT_ACTION) && ( (*msg)->getContent() == bestmsg->getContent() ) ) {
							// (*msg)->getSender() 
							acceptActionOffer(*msg);
					}
					else{ // refuse action offer!
						outgoingMessages.push_back(new Message((*msg)->getSender(),tlightId,Message::REFUSE_JOINT_ACTION,(*msg)->getContent(),0));	          
					}
				}
			}
		}
	}
}

/**
 * Accept all information requests
 */
void OpLearner::acceptAllInformationRequests(){	
	vector<Message*>::const_iterator msg;
	for(msg = incomingMessages.begin(); msg!=incomingMessages.end(); msg++){	
		if ( (*msg)->getPerformative()== Message::REQUEST_LAST_STATE_INFO ) {								
			outgoingMessages.push_back(new Message((*msg)->getSender(),tlightId,Message::INFORM_LAST_STATE_INFO,_lastState_,0));
		}
		else if ( (*msg)->getPerformative()== Message::REQUEST_LAST_ACTION_INFO ) {								
			outgoingMessages.push_back(new Message((*msg)->getSender(),tlightId,Message::INFORM_LAST_ACTION_INFO,_lastAction_,0));
		}
		else if ( (*msg)->getPerformative()== Message::REQUEST_CURRENT_STATE_INFO ) {								
			outgoingMessages.push_back(new Message((*msg)->getSender(),tlightId,Message::INFORM_CURRENT_STATE_INFO,_newState_,0));
		}
		else if ( (*msg)->getPerformative()== Message::REQUEST_CURRENT_ACTION_INFO ) {								
			outgoingMessages.push_back(new Message((*msg)->getSender(),tlightId,Message::INFORM_CURRENT_ACTION_INFO,_newAction_,0));
		}
		
	}
}



void OpLearner::selectAction(){	
//	cout<<"estado atual no selectAction: "<<printSAVector(currentState)<<endl;
	++actSteps;

//	if( (testConv)&&( actSteps >( 10*getSingleActionsNumber()*getSingleStatesNumber() ) ) ){
//		if ( _learning_->testConvergence(0.01)){
//			cout<<"Convergiu no passo: "<<actSteps<<endl;
//			testConv = false;
//		}
//	}
	currentAction =_learning_->act(currentState,DQTable::E_GREEDY);
	mySelectedActionValue = _learning_->getValueForPair(currentState,currentAction);
	if ( currentAction.size()>1 ) {
		//negociate action
		incomingMessages.clear();
		callJointAction( currentAction, mySelectedActionValue );	
	}
//	cout<<"current action: "<<printSAVector(currentAction)<<endl;	
}
/**
 * Decides the action that will be performed, based on the joint action answers received
 */
void OpLearner::commitAction(){
	vector<Message*>::const_iterator msg;
	unsigned int acceptedAction;
	unsigned int selectedAction = getInfoFromOwner(currentAction, tlightId); 
	
	vector<SAComponent*> newAction;
	if ( acceptedActionMessages.size() > 0 ){
		acceptedAction = acceptedActionMessages[0]->getContent();
		newAction.push_back(new SAComponent( tlightId, acceptedAction ) );
	}
	//constructs the newAction with the received agreed actions
	for(msg = incomingMessages.begin(); msg!=incomingMessages.end(); msg++){	
		if ( (*msg)->getPerformative()== Message::AGREE_JOINT_ACTION ) {								
			newAction.push_back(new SAComponent( (*msg)->getSender(), (*msg)->getContent() ) );
		}		
	}
	if (newAction.size()>0){
		sort( newAction.begin(), newAction.end(),saCompare ); // sort the created joint action 
		if (! equalSA( currentAction, newAction)){
			//set the current action to the agreed joint action
			currentAction = newAction;
		}	
	}
}

int OpLearner::getCurrentSingleAction(){
	return getInfoFromOwner(currentAction, tlightId); 
}	

string OpLearner::newControlState(){		
	signalPlan changeTo;	
	if (!currentAction.empty()){					
		_newAction_ = getCurrentSingleAction(); //currentAction[0]->getInformation();
	}
	else{
		 cout <<tlightId<< "-- ERROR -- SELECTING RANDOM ACTION"<<endl;
		_newAction_ = rand()%signalPlans.size();
	}
	if ((_newAction_ != INVALID_ID	)&&(_newAction_<_actionsNumber_)){ 
		//the act method from the mechanism returns actions from 0 to n 	     
		changeTo = signalPlans[_newAction_];//_newAction_ is an index
		return changeString_(changeTo);	
	}	
	return "";
}


/**
 * Sends the Messages to the socket 
 */	
string OpLearner::sendMessages(){
	
	vector<Message*>::const_iterator msg;
	string outmsg;
	for(msg = outgoingMessages.begin(); msg!=outgoingMessages.end(); msg++){	
		outmsg+=(*msg)->toString();		
	}	
	sentMessages+=outgoingMessages.size();
	outgoingMessages.clear();
	return outmsg;
}	

/**
 * Puts a message in the incoming messages list.
 */
void OpLearner::receiveMessages(ClientSocket* msg_socket){
	string msg;
	*msg_socket >> msg;
	pcsIncomingMsg(msg);		
}

/**
 * Process received messages in the format 
 * ";_to_,_from_,_type_,_information_,_value_;"
 * Process only messages containing the agent id
 */
void OpLearner::pcsIncomingMsg(string msg){		
	incomingMessages.clear();
	//test if the tlight id is in the messages
	size_t found = msg.find(intToString(tlightId));	
	//if tlight is found, process messages
    if (found!=string::npos){
    	unsigned int to,from,type,info;
		double value;	    	
		StringTokenizer strtok = StringTokenizer(msg,";");	
		while(strtok.hasMoreTokens()){		
			string one_msg = strtok.nextToken();		
			StringTokenizer message = StringTokenizer(one_msg,",");
			if(message.countTokens()==5){		
				while(message.hasMoreTokens()){ 				
			    	to = message.nextIntToken();		    	    	    	    	
					from = message.nextIntToken();
					type = message.nextIntToken();	
					info = message.nextIntToken();
					value = message.nextFloatToken();
					if( to == tlightId ){ //only receive messages adressed to me						
						incomingMessages.push_back(new Message(to,from,type,info,value));
						if (communicationArea.count(from) == 0){
							communicationArea.insert(from);
						}
						 
//						cout<<"received message:"<<to<<","<<from<<","<<type<<","<<info<<","<<value<<endl;
					}
				}	
			}
		}	
    }
}

/**
 * Returns the best Joint Action offer received.
 */
Message* OpLearner::getBestJointActionOffer(){	
	double bestValue = 0;
	Message* bestMsg = *(incomingMessages.end());
	vector<Message*>::const_iterator msg;
	for(msg = incomingMessages.begin(); msg!=incomingMessages.end(); msg++){
		if ((*msg)->getPerformative() == Message::PROPOSE_JOINT_ACTION){		
			if((*msg)->getValue() > bestValue){
				bestValue = (*msg)->getValue();
				bestMsg = *msg;
			}
		} 		
	}
	return bestMsg;
}

/**
* Clears the outgoing and incoming messages lists.
*/
void OpLearner::clearMessages(){
	outgoingMessages.clear();	
	incomingMessages.clear();	
}
