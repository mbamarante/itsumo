#include "local_agent.hh"


using namespace std;


LocalAgent::LocalAgent(vector<int> tls, string name, bool stat,int memory, int states, double learning_r, double discount_r)
{
	_name_ = name;
	_stat_ = stat;	
	_memsize_ = memory;
	_states_ = states;
	_actions_ = 0;	
	_connectMsg_ = "a;"; //"s;" if you whant to request for connection to 
						 // receive saturation and flow data
						 // 
						 //"a;" if you want to receive speed and density data
						 // laneAvgSpeed and  laneDensity
	_totalStoppedCars_ = 0;
	_learning_rate_ = learning_r;
	_discount_rate_ = discount_r;	
	_start_time_=0;
	
	//valores conjuntos
	_lastState_		= 0; 
	_lastAction_	= 0; 
	
	
	for (unsigned int i = 0; i < tls.size(); i++)
	{	
		_connectMsg_ += intToString(tls[i]);
		_connectMsg_ += ";";
	}         
	
	_numAgents_ 			= tls.size();
	
}

LocalAgent::~LocalAgent()
{
	vector <TLAgent*>::iterator it;	
	for (it = (_tlights_).begin(); it != (_tlights_).end(); it++) 
		(*it)->~TLAgent();
}
void LocalAgent::initLearning(){
	_num_joint_states_	= pow(_states_, _numAgents_);
    _num_joint_actions_	= pow(_actions_, _numAgents_);
	_mec_ = parseMech( _num_joint_states_ ,_num_joint_actions_);
      if (_mec_ == NULL) {
           printf("Fail creating learning mechanism.\n");
     }
}
/*o numero de estados esta sendo considerado como igual 
 * para todos os agentes!
 */ 
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
		action+=(int)(pow(_actions_,i)*acs[i]);		
	}
	return action;
}
/*o numero de acoes esta sendo considerado como igual 
 * para todos os agentes
 */ 
int LocalAgent::getAgentActionFromJointAction(int agentIndex, int jointAction){
	int digit = 0;
	for (int i=0; i < _numAgents_  - agentIndex - 1; i++) {
		digit = (int)(jointAction / pow(_actions_, _numAgents_ -i-1));
		jointAction -= (int)(digit * pow(_actions_, _numAgents_ -i-1));
	}
	return (int)(jointAction/pow(_actions_, agentIndex));	
}


markov::mdp* LocalAgent::parseMech(int _stateCount, int _actionCount){	
    markov::qlearning *mech = new markov::qlearning;
    if ( ! mech->create( _stateCount,   // number of states
                         _actionCount,  // number of actions
                         _discount_rate_, // discount
                         0.05,             //epsilon-value or "curiosity", used for epsilon-greedy)
                         _learning_rate_  // learning rate
                         ) ) {
        return NULL;
    }
    return (markov::mdp*) mech;
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
      
	int laneId;
	double avgSpeed;
 	double density;
    int stoppedCars;

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
			laneId = atoi(curToken);
		
			curToken = strtok (NULL, ";");
			avgSpeed = atof(curToken);
                
			curToken = strtok (NULL, ";");
			density = atof(curToken);
                
			curToken = strtok (NULL, ";");
			stoppedCars = atoi(curToken);
		
			_totalStoppedCars_ += stoppedCars;
			_totalInputLanes_ ++;

                		
			
			state.n.incomingLanes.push_back(lane(laneId, avgSpeed, density, stoppedCars));
		}
                //outgoing lanes
		curToken = strtok (NULL, ";");
		numberOfLanes = atoi(curToken);
		for (int j=0; j < numberOfLanes; j++) {
			curToken = strtok (NULL, ";");
			laneId = atoi(curToken);

			curToken = strtok (NULL, ";");
			avgSpeed = atof(curToken);

			curToken = strtok (NULL, ";");
			density = atof(curToken);

			curToken = strtok (NULL, ";");
			stoppedCars = atoi(curToken);
		
			
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
	_simulationStates_ = updateStates(msg);
	vector<simulationState>::iterator itS;
	vector <TLAgent*>::iterator itT;	

	for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++) {

		for (itS = _simulationStates_.begin(); itS != _simulationStates_.end(); itS++)
		{	
			if ( (*itS).n.tlightId == (*itT)->tlightId ) {
				(*itT)->updateState(*(itS));
			}
		}
	}

	*client_socket << "OK.";
}
double LocalAgent::getTotalAvgStoped(){
	vector <TLAgent*>::iterator itT;
	double avgstop=0.0 ;
	for (itT = _tlights_.begin(); itT != _tlights_.end(); itT++){			
		//pega a fila media (em relacao aos passos da memoria) de cada TL 
		 avgstop += (*itT)->getAvgLine();
	}
	return avgstop;
}

double LocalAgent::getGlobalComparativeReward(){
	double avgQueue = getTotalAvgStoped();
	double relation = 1.0;
	double ntl = (double)_tlights_.size();
	cout<<avgQueue<<endl;
	if (avgQueue>ntl*1.5)
		return -1.0;
	if (avgQueue<ntl*0.25)
		return 1.0;
	else
		return -0.1;
}


void LocalAgent::pcsRequestMsg(std::string msg, vector <TLAgent*> &trafficLights, 
				ClientSocket * client_socket)
{
    //cout << _name_ << "\n: Received traffic lights update request (" << msg << ")" << endl;

	std::string reply = "";
	reply += "x;";
	string message = "";
	int count = 0;
	vector <TLAgent*>::iterator itT;
	vector <int> sts;
	
	if(_currentItsumoTimeStep_>= _start_time_){
	
		double _newReward_ = getGlobalComparativeReward();
		cout<<_newReward_<<endl;
			
		for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++){			
			//pega o estado simples
			sts.push_back( (*itT)->myState() );
		}
		int _newState_ =  getJointState(sts);
	
		//observa conjunto 
		_mec_->observe(_lastState_,_lastAction_,_newState_,_newReward_);
	//cout<<"observa: lastState"<<_lastState_<<" lastAc "<<_lastAction_<<" newState "<<_newState_<<" rw "<<_newReward_<<endl;
		//acao conjunta 
		int _newAction_ =  _mec_->act(_newState_);
		string temp="";
		int agentIndex=0;
		for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++){
			int ac = getAgentActionFromJointAction(agentIndex, _newAction_);
			temp = (*itT)->changeString_((*itT)->signalPlans[ac]);			
			if (temp != ""){
				++count;
				message += temp;
			}
			++agentIndex;
		}
		_lastState_ = _newState_;
		_lastAction_ = _newAction_;		
		
//cout<<"itsumo "<<_currentItsumoTimeStep_<<" start "<< _start_time_<<endl;
	}
	
	reply += intToString((int)count) + ";";	
	reply += message;

//	cout << _name_ << ": Reply to change request: \n" << reply << endl;

	*client_socket << reply;
}


vector<TLAgent*> LocalAgent::parseTLights(string tlightConfig, int memorySize,int states)
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
    cout << tlightConfig << endl;
	
	


	const char *tmp = tlightConfig.c_str();
	char str[tlightConfig.size()];
  	strcpy(str, tmp);

	char *curToken;
	curToken = strtok (str, ";"); 
	curToken = strtok (NULL, ";");    // ignore the first field (message type)

	int numberTlights= atoi(curToken);
	int numberSignalPlans = 0;
	vector<TLAgent*> answer;
	
	for (int tl=0; tl < numberTlights; tl++) {
		curToken = strtok (NULL, ";");

		int tlightId = atoi(curToken);
		
		curToken = strtok (NULL, ";");

		numberSignalPlans = atoi(curToken);
		
		// the number of signal plans is consedered the number of actions
		if(_actions_==0){ 
			_actions_ = numberSignalPlans;
		}
		TLAgent* nTlight = new TLAgent(tlightId, memorySize, states, numberSignalPlans,_stat_,_learning_rate_,_discount_rate_);		        
               	
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
			nTlight->signalPlans.push_back(newPlan);
		}		
		
		nTlight->setNetwork(_topology_);
		
		answer.push_back(nTlight);
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
				//cout<<"node: "<<ns.id<<" posicao"<<currentNode.relativePosition(ns)<<endl;
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

void* LocalAgent::run()
{
	ClientSocket client_socket ( "localhost", 30000 );

	std::string reply;
	cout << "\nI will try to connect to ITSUMO...\n" << endl;
	client_socket << _connectMsg_;
	client_socket >> reply;
	cout << "ITSUMO accepted! \n" << endl;
	
	// receives a message from the simulator containing the configuration
	// of each traffic light (ie, signalplans, phases, etc). Then, one
	// object for each tlight is created
	_tlights_ = parseTLights(reply,_memsize_,_states_);	
	setDirections();
	
	//creates the leraning mechanism
	initLearning();

	
	std::cout << "We received this response from the server:\n\"" << reply << "\"\n";
	bool finish = false;
	ofstream _outFile_;
	_outFile_.open("jal_qtable.dat");
	while (!finish)
	{
		std::string msg;
		client_socket >> msg;

	
		if (msg[0] == 'i') 
			pcsInfoMsg(msg, _tlights_, &client_socket);

		else if (msg[0] == 'r')
			pcsRequestMsg(msg, _tlights_, &client_socket);

		else if (msg == "end;")
		{
			client_socket << "end;";
			_outFile_<<_mec_->printSimpleQTable()<<endl;
			cout << _name_ << " finished!" << endl;
			finish = true;
		}

		else
			cout << "Unknown message!!!" << msg << endl;
         	//cout<<"performance"<< getPerformance() <<endl;

	}
	exit(0);
	
}


