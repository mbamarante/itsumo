#include "local_agent.hh"


//LocalAgent::LocalAgent(vector<int> tls, string name, bool stat,int memory, int states,double learning_r, double discount_r)
LocalAgent::LocalAgent(vector<int> tls, string name, bool stat,int memory, int states,double learning_r, double discount_r, int startTime, int phase1Dur, int phase2Dur, double tauValue)
{
	_name_ = name;
	_stat_ = stat;
	_memsize_ = memory;
	_states_ = states;
	_connectMsg_ = "a;";
	_totalStoppedCars_ = 0;
	_learning_rate_ = learning_r;
	_discount_rate_ = discount_r;
	_start_time_ = startTime;
	_actions_number_ = 3;//default
	_TAU_= tauValue;
	FASE_1_DURATION = phase1Dur;
	FASE_2_DURATION = phase2Dur;
	


	for (unsigned int i = 0; i < tls.size(); i++)
	{	
		_connectMsg_ += intToString(tls[i]);
		_connectMsg_ += ";";
	}         
}

LocalAgent::~LocalAgent()
{
	_outFile_.close();
	vector <TLAgent*>::iterator it;
	for (it = (_tlights_).begin(); it != (_tlights_).end(); it++) 
		(*it)->~TLAgent();

}

//returns a list os simulation states
vector<simulationState> LocalAgent::updateStates(string rawState) 
{
	/*The message consists of the following fields, separeted by ";"
		currentIteration
		numberOfNodes;
			[nodeId; trafficLightId; currentPlan; 
				totalNumberOfIncomingLanes;
					[laneId;laneAvgSpeed  laneDensity;laneCarsStopped;]
				totalNumberOfOutgoingLanes;]

					[laneId;laneAvgSpeed; laneDensity; laneCarsStopped;]]
	 */

	const char *tmp = rawState.c_str();
	char str[rawState.size()];
	strcpy(str, tmp);
	char *curToken;

	curToken = strtok (str, ";"); 
	curToken = strtok (NULL, ";");    // ignore the first field (message type)

	int curIteration = atoi(curToken);
	_currentItsumoTimeStep_ = curIteration;

	curToken = strtok (NULL, ";");
	int numberOfNodes = atoi(curToken);
	vector<simulationState> ans;

	_totalStoppedCars_ = 0;
	_totalInputLanes_ =0;
	for (int i=0; i < numberOfNodes; i++) 
	{
		   

		curToken = strtok (NULL, ";");
		int nodeId = atoi(curToken);		
		curToken = strtok (NULL, ";");
		int tlightId = atoi(curToken);
		curToken = strtok (NULL, ";");
		int signalPlan = atoi(curToken);

		simulationState state(curIteration, node(nodeId, tlightId, signalPlan));
       
		//incoming lanes
		curToken = strtok (NULL, ";");
		int numberOfLanes = atoi(curToken);
		for (int j=0; j < numberOfLanes; j++) {
			curToken = strtok (NULL, ";");
			int laneId = atoi(curToken);

			curToken = strtok (NULL, ";");
			double avgSpeed = atof(curToken);

			curToken = strtok (NULL, ";");
			double density = atof(curToken);

			curToken = strtok (NULL, ";");
			int stoppedCars = atoi(curToken);

			_totalStoppedCars_ += stoppedCars;
			_totalInputLanes_ ++;

			state.n.incomingLanes.push_back(lane(laneId, avgSpeed, density, stoppedCars));

		}
		//outgoing lanes
		curToken = strtok (NULL, ";");
		numberOfLanes = atoi(curToken);
		for (int j=0; j < numberOfLanes; j++) {
			curToken = strtok (NULL, ";");
			int laneId = atoi(curToken);

			curToken = strtok (NULL, ";");
			double avgSpeed = atof(curToken);

			curToken = strtok (NULL, ";");
			double density = atof(curToken);
			curToken = strtok (NULL, ";");
			int stoppedCars = atoi(curToken);


			state.n.outgoingLanes.push_back(lane(laneId, avgSpeed, density, stoppedCars));		
		}
			
		ans.push_back(state);

	}	
	return ans;
}

void  LocalAgent::setStartTime(int t){
    _start_time_ = t;
}

void LocalAgent::pcsInfoMsg(std::string msg, vector<TLAgent*> &trafficLights, ClientSocket* client_socket){
	//cout << _name_ << "\n: Received new network state\n" << msg << endl;
	//vector<simulationState*> states = updateStates(msg);
	_simulationStates_ = updateStates(msg);
	vector<simulationState>::iterator itS;
	vector <TLAgent*>::iterator itT;	

	for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++) {

		for (itS = _simulationStates_.begin(); itS != _simulationStates_.end(); itS++)
		{			
			if ( (*itS).n.tlightId == (*itT)->tlightId ) {
				(*itT)->updateState((*itS));
				(*itT)->addTotalStoppedCarsOnNetwork(_totalStoppedCars_);
				(*itT)->setTotalInputLanesOnNetwork(_totalInputLanes_);
			}
		}
	}		
	*client_socket << "OK.";
}

int LocalAgent::getJointState(vector<int> sts){
	int state=0;
	for(unsigned int i=0;i<sts.size();i++){
		state+=(int)(pow(_states_,i)*sts[i]);		
	}
	return state;
}
/*o numero de acoes esta sendo considerado como igual 
 * para todos os agentes!
 */ 
int LocalAgent::getJointAction(vector<int> acs){
	int action=0;
	for(unsigned int i=0;i<acs.size();i++){ 		
		action+=(int)(pow(_actions_number_,i)*acs[i]);		
	}
	return action;
}
/*o numero de acoes esta sendo considerado como igual 
 * para todos os agentes
 */ 
int LocalAgent::getAgentActionFromJointAction(int agentIndex, int numberofAgents, int jointAction, int actionsNumber){
	int digit = 0;
	for (int i=0; i < numberofAgents - agentIndex - 1; i++) {
		digit = (int)(jointAction / pow(actionsNumber, numberofAgents-i-1));
		jointAction -= (int)(digit * pow(actionsNumber, numberofAgents-i-1));
	}
	return (int)(jointAction/pow(actionsNumber, agentIndex));	
}

void LocalAgent::setPhaseDuration(int fase1duration, int fase2duration){
	 FASE_1_DURATION = fase1duration;
	 FASE_2_DURATION = fase2duration;
	 cout<<"set phase duration!"<<endl;
}

void LocalAgent::setTau(float t){
	 _TAU_= t;
	 cout<<"set tau!"<<endl;
}


void LocalAgent::pcsRequestMsg(std::string msg, vector <TLAgent*> &trafficLights, ClientSocket * client_socket){
    //cout << _name_ << "\n: Received traffic lights update request (" << msg << ")" << endl;

	std::string reply = "";

	reply += "x;"; 
	string message = "";
	int count = 0;
	vector <TLAgent*>::iterator itT;			
	double rwmedio = 0.0;	
	vector<int> jointState;
	vector<int> jointAction;
	bool phase2 = false;
	bool phase3 = false;
	cout<<"Supervisor: "<< _name_<<endl;
	cout<<"  Start at "<<_start_time_<<endl;
	cout<<"  TAU "<<_TAU_<<endl;
	cout<<"  PHASE 1 "<<FASE_1_DURATION<<endl;
	cout<<"  PHASE 2 "<<FASE_2_DURATION<<endl;

	if (_currentItsumoTimeStep_ == 0){
		for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++){			
			(*itT)->newControlState();
		}
		phase2 = true;
		phase3 = true;
	}	

	if (_currentItsumoTimeStep_>= _start_time_){		
		jointState.clear();
		jointAction.clear();

		for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++){			
			// para cada agente, aprende localmente (atualizar tabela Q local)
			string temp = (*itT)->newControlState();
			
			//supervisor
			jointState.push_back((*itT)->_lastState_);								
			jointAction.push_back((*itT)->_lastAction_);
			rwmedio += (*itT)->_newReward_;

			
			if (temp != ""){				
				count++;
				message += temp;
			}
		}
		rwmedio =rwmedio/(double)(count);
		int curJointAct = getJointAction(jointAction);
		int curJointSt = getJointState(jointState);

		// supervisor learns
		if (bestActions[curJointSt] != -1) { 
/*  ana changed (instead of keeping best reward and action, keep average reward)
			if (rwmedio > bestAvgRewards[curJointSt]) {					
				bestAvgRewards[curJointSt] = rwmedio;
				bestActions[curJointSt] = curJointAct;
			}
*/

		    if (rwmedio > (bestAvgRewards[curJointSt] * (1. - _learning_rate_)) + (rwmedio * _learning_rate_)) {
				bestActions[curJointSt] = curJointAct;
			}
		    bestAvgRewards[curJointSt] = (bestAvgRewards[curJointSt] * (1. - _learning_rate_)) + (rwmedio * _learning_rate_);
		} else {
			bestAvgRewards[curJointSt] = rwmedio;
			bestActions[curJointSt] = curJointAct;					
			}
		cout<<"bestAvgRewards["<<curJointSt<<"] ="<<bestAvgRewards[curJointSt]<<endl;
		cout<<"bestActions["<<curJointSt<<"] ="<<bestActions[curJointSt]<<endl;
			


		//2a phase
		if ((bestActions[curJointSt] != -1) && ((_currentItsumoTimeStep_>=FASE_1_DURATION)  &&  (_currentItsumoTimeStep_<=FASE_1_DURATION+FASE_2_DURATION) ) )  { //2a fase		
		    //if (phase2){
		    cout<<"currently (time step "<<_currentItsumoTimeStep_<<"): phase 2"<<endl;
				//	phase2=false;
				//}
			message=""; //recria a message
			int agentIndex=0;
			for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++){								
				//para cada agente pega a acao que o supervisor escolhe
				agentIndex++;
				(*itT)->_newAction_ =	getAgentActionFromJointAction(agentIndex,(trafficLights).size(), bestActions[curJointSt], (*itT)->_actionsNumber_);										
				message+=(*itT)->changeString_((*itT)->getSignalPlan((*itT)->_newAction_+1));
			}

		} 
		//3a phase
		if ((bestActions[curJointSt] != -1) && (_currentItsumoTimeStep_>FASE_1_DURATION+FASE_2_DURATION)){
		    //if(phase3){
		    cout<<"currently (time step "<<_currentItsumoTimeStep_<<"): phase 3"<<endl;
		    //phase3 = false;
		    //}
			message=""; //recria a message
			int agentIndex=0;
			for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++){								
				// para cada agente
				agentIndex++;

				float localQ = (*itT)->_mec_->getGreedyQValue((*itT)->_lastState_);
				cout<<"agent "<<(*itT)->tlightId <<" rw local:"<<localQ<<" sup:"<<bestAvgRewards[curJointSt]<<" sup*"<<1+_TAU_<<" "<<bestAvgRewards[curJointSt]*(1+_TAU_)<<endl;
				if (bestAvgRewards[curJointSt]*(1+_TAU_) > localQ){						
					(*itT)->_newAction_ =  getAgentActionFromJointAction(agentIndex,(trafficLights).size(), bestActions[curJointSt], (*itT)->_actionsNumber_);
					cout<<"sugestao aceita de acao: "<<(*itT)->_newAction_<<endl;
				}																					
				message+=(*itT)->changeString_((*itT)->getSignalPlan((*itT)->_newAction_+1));
			}
		}
	}

	reply += intToString((int)count) + ";";
	reply += message;

	//cout << _name_ << ": Reply to change request: \n" << reply << endl;

	*client_socket << reply;

}

/*
 * 
 */
void LocalAgent::addTLpartners(){	
	vector<simulationState>::iterator itS;
	vector <TLAgent*>::iterator itT, itTT;	
	//cout<<"entrou no addTLpartners"<<endl;
	int tlID =-1;
	int ptlID =-1;
	for (itS = (_simulationStates_).begin(); itS != (_simulationStates_).end(); itS++){
		tlID = (*itS).n.tlightId;			
		//cout<<"->TL ID: "<<tlID<<endl;
		vector<lane>::iterator ln;
		for (ln =(*itS).n.incomingLanes.begin();ln !=(*itS).n.incomingLanes.end(); ln++){
			//cout<<"lane id: "<<(*ln)->laneId<<endl;
			ptlID = (_topology_)->getTLightIdAtLane((*ln).laneId);					
			if (ptlID!=-1){
				for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++) {
					if ( tlID == (*itT)->tlightId ) {
						for (itTT = (_tlights_).begin(); itTT !=  (_tlights_).end(); itTT++) {
							if ( (*itTT)->tlightId == ptlID  ) {
								//cout<<"-partner ID: "<<(*itTT)->tlightId<<endl;
								(*itT)->partners.push_back(*itTT);
							}
						}
					}
				}			
			}
		}								
	}

}



vector <TLAgent *> LocalAgent::parseTLights(std::string tlightConfig, int memorySize,int states)
{
	/*The message consists of the following fields, separeted by ";"
		numberOfTrafficLights
			[trafficLightId;
			 numberOfSignalPlans;
				[signalPlanId;
				 numberOfPhases;
					[phaseId;
					 phaseStartIter;
					 phaseEndIter;
					 numberOfOpenDirections;
						[fromLane; 
					       toLaneset;]]]]
	 */
	
	vector <TLAgent *> answer;

	const char *tmp = tlightConfig.c_str();
	
	char str[strlen(tmp)]; 
	
	strcpy(str, tmp);

	char *curToken;
	curToken = strtok (str, ";"); 
	curToken = strtok (NULL, ";");    // ignore the first field (message type)

	int numberTlights= atoi(curToken);
	cout<<"_tlights_: "<<numberTlights<<endl;
	int numberSignalPlans = 0;

	for (int tl=0; tl < numberTlights; tl++) 
	{
		curToken = strtok (NULL, ";");

		int tlightId = atoi(curToken);

		curToken = strtok (NULL, ";");

		numberSignalPlans = atoi(curToken);
		_actions_number_ =  numberSignalPlans;
		cout<<"_actions_number_: "<<_actions_number_<<endl;
		// the number of signal plans is consedered the number of actions
		TLAgent *newTlight = new TLAgent(tlightId, memorySize, states, numberSignalPlans,_stat_, _learning_rate_, _discount_rate_);		        

		for (int sp=0; sp < numberSignalPlans; sp++) 
		{
			struct signalPlan newPlan;

			curToken = strtok (NULL, ";");
			newPlan.signalPlanId = atoi(curToken);
			curToken = strtok (NULL, ";");

			int numberPhases = atoi(curToken);


			for (int ph=0; ph < numberPhases; ph++)
			{
				struct phase newPhase;
				curToken = strtok (NULL, ";");
				newPhase.phaseId = atoi(curToken);
				curToken = strtok (NULL, ";");
				newPhase.start = atoi(curToken);
				curToken = strtok (NULL, ";");
				newPhase.end = atoi(curToken);
				curToken = strtok (NULL, ";");
				int numberDirections = atoi(curToken);

				for (int direction=0; direction < numberDirections; direction++) 
				{
					struct direction newDirection;
					curToken = strtok (NULL, ";");
					newDirection.fromLane = atoi(curToken);	
					curToken = strtok (NULL, ";");
					newDirection.toLaneset = atoi(curToken);
					newPhase.directions.push_back(newDirection);
				}

				newPlan.phases.push_back(newPhase);				
			}
			newTlight->signalPlans.push_back(newPlan);

		}
		newTlight->setNetwork(_topology_);
		answer.push_back(newTlight);
	}			
	return answer;
}

void  LocalAgent::setTopology(topologyReader* n){

	_topology_ = n;  
}

void  LocalAgent::setDirections(){
	vector <TLAgent*>::iterator itT;
	vector<lanesetSimple*>::iterator ls;	
	int node = -1;	
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++) {
		tLight* tlight = (_topology_)->getTLight((*itT)->tlightId);			
		node = (tlight)->node;			
		nodeSimple* currentNode = (_topology_)->getNode(node);
		for (ls = (_topology_)->lanesets.begin(); ls !=  (_topology_)->lanesets.end(); ls++) {
			if ((*ls)->nodeEnd == node) {
				nodeSimple* ns = (_topology_)->getNode((*ls)->nodeBegin);					
				(*itT)->nodesAtDirections[currentNode->relativePosition(ns)] = ns->id;
				//cout<<"node: "<<ns->id<<" posicao"<<currentNode->relativePosition(ns)<<endl;
			}
		}		
	}			

}

///// There are no terminal states in the traffic scenario.
bool  LocalAgent::testEnd( int _agent ) { 
	return false; 
}

/// Compute the overall performance of the agents in the environment.
double  LocalAgent::getPerformance( ){
	double sum = _totalStoppedCars_;
	return sum;
} 

void LocalAgent::initActionsRW(int sts){
	for(unsigned int i =0;i< pow(sts,(_tlights_).size());i++){
		bestActions.push_back(-1); 
		bestAvgRewards.push_back(-1.0);
	}		
}
void *LocalAgent::run()
{
	ClientSocket client_socket ( "localhost", 30000 );

	std::string reply;
	cout << "\nI will try to connect to ITSUMO...\n" << endl;
	client_socket << _connectMsg_;
	client_socket >> reply;
	cout << "ITSUMO accepted! \n" << endl;



	// receives a message from the simulator containing the configuration
	// of each traffic light (ie, signalplans, phases, etc). Then, one
	// object for each tlight is d	
	_tlights_ = parseTLights(reply,_memsize_,_states_);

	initActionsRW(_states_);

	setDirections();

	//print ids to state-action log
	vector <TLAgent*>::iterator itT;


	std::cout << "We received this response from the server:\n\"" << reply << "\"\n";

	while (true){
		std::string msg;
		client_socket >> msg;		
		if (msg[0] == 'i'){
			pcsInfoMsg(msg, _tlights_, &client_socket);
		}
		else if (msg[0] == 'r')
			pcsRequestMsg(msg, _tlights_, &client_socket);
		else if (msg == "end;")	{
			client_socket << "end;";
			cout << _name_ << " finished!" << endl;
			break;
		}
		else	
			cout << "Unknown message!!!" << msg << endl;		
		//cout<<"performance"<< getPerformance() <<endl;

	}

}



