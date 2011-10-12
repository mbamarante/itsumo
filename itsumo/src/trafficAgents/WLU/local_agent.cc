#include "local_agent.hh"



LocalAgent::LocalAgent(vector<int> tls, string name, bool stat,int memory, int states,double learning_r, double discount_r, int startTime, int type, int plan, int agentid)
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
	
	_simulation_type_ = type;
	_default_plan_ = plan;
	_fixed_agent_id_ = agentid;
	_currentAction_ = 0;
	
	_toFile_="";

	for (unsigned int i = 0; i < tls.size(); i++)
	{	
		_connectMsg_ += intToString(tls[i]);
		_connectMsg_ += ";";
	}         
	
	if(_simulation_type_==1){
		string fileName2 = "avgG_actions.log";		
		try {
      		_outFile_.open(fileName2.c_str());
      		if (!_outFile_.is_open())throw "file not found.";      
   		}
   		catch(char *str){
      		cout << "Error: "<< str << endl;
   		} 
	}
	if(_simulation_type_==2){
		string fileName2 = "q_tables.log";
		try {
      		_outFile_.open(fileName2.c_str(),ios_base::app);
      		if (!_outFile_.is_open())throw "file not found.";      
   		}
   		catch(char *str){
      		cout << "Error: "<< str << endl;
   		} 
   		_inFile_.open("avgG_actions.log");   		
		string temp;
		string token;
  		while(getline(_inFile_, temp)){
  			vector<string> act;
  			std::istringstream iss(temp.c_str());
  			//cout<<"nova linha";   	  			
   			while (getline(iss, token, ' ')){
   				//cout<<token<<",";
   				act.push_back(token);
   		 	}
   		 	//cout<<endl;
     		actions_from_file.push_back(act);
  		}   		   		   				
	}
	if(_simulation_type_==3){
		_inFile_.open("q_tables.log"); 
		int id=-1;
		int act = -1;
		string temp;
		while(getline(_inFile_, temp)){
			std::istringstream iss(temp.c_str()); 
			iss>>id>>act;		
			cout<<"id"<<id<<"act"<<act<<endl;
			learned_actions[id]=act;
		}   		   		   				
	}
	
}

LocalAgent::~LocalAgent()
{
	_outFile_.close();
	_inFile_.close();
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
	_simulationStates_ = updateStates(msg);
	vector<simulationState>::iterator itS;
	vector <TLAgent*>::iterator itT;	

	for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++) {

		for (itS = _simulationStates_.begin(); itS != _simulationStates_.end(); itS++)
		{	
			if ( (*itS).n.tlightId == (*itT)->tlightId ) {
				(*itT)->updateState(*(itS));
				(*itT)->addTotalStoppedCarsOnNetwork(_totalStoppedCars_);
				(*itT)->setTotalInputLanesOnNetwork(_totalInputLanes_);
			}
		}
	}

	*client_socket << "OK.";
}


int LocalAgent::getSavedAction(int act_number,int tlid){
	int resp = 0;
	string tl = intToString(tlid);
	vector <string>::iterator str;
	int local=-1;
	int i=0;	
	vector <string> acs = actions_from_file.at(0);
	for(str = (acs).begin(); str != (acs).end(); str++){		
		if ((*str) == tl ){
			local = i;
			//cout<<"ag"<<tl<<"local "<<i<<endl;
			//cout<<"saved action for "<<tlid <<" at "<<act_number<<": "<<atoi(actions_from_file.at(act_number).at(local).c_str())<<endl;
			break;					
		}
		i++;
	}
	if (local!=-1) resp = atoi(actions_from_file.at(act_number).at(local).c_str());	
	return resp;
}


double LocalAgent::getSavedAvgReward(int act_number){	
	return atof((actions_from_file.at(act_number).at(1)).c_str());// at 1 is the avg.reward	
}

void LocalAgent::pcsRequestMsg(std::string msg, vector <TLAgent*> &trafficLights, ClientSocket * client_socket){
    //cout << _name_ << "\n: Received traffic lights update request (" << msg << ")" << endl;

	std::string reply = "";

	reply += "x;"; 
	string message = "";
	int count = 0;
	vector <TLAgent*>::iterator itT;			
	double rwmedio = 0.0;	
	string jointAction;			
	if (_currentItsumoTimeStep_ == 0){		
		if(_simulation_type_==1) _outFile_<<"step rw ";
		for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++){	
			(*itT)->newControlState();//inicializa os agentes				
			if(_simulation_type_==1) _outFile_<<intToString((*itT)->tlightId)<<" ";			
		}
		if(_simulation_type_==1) _outFile_<<endl;
	}
	if (_currentItsumoTimeStep_>= _start_time_){
		_currentAction_++;				
		if(_simulation_type_==1){//1st save actions and run q-learning
			jointAction.clear(); 
			for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++){			
				// para cada agente, aprende localmente (atualizar tabela Q local)
				string temp = (*itT)->newControlState();
				if (temp != ""){
					jointAction+=intToString((*itT)->_lastAction_)+" ";															
					rwmedio += (*itT)->_newReward_;												
					count++;
					message += temp;
				}
			}			
			rwmedio =rwmedio/(double)(count);	
			_outFile_<<intToString(_currentItsumoTimeStep_)<<" "<<doubleToString(rwmedio,3)<<" "<<jointAction<<endl;
		}		
		if(_simulation_type_==2){//run new simulation using saved actions 						    
			message=""; //recria a message									
			string temp = "";
			for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++){
				temp = "";								
				//para cada agente pega a acao ja feita na 1a sim.
				if((*itT)->tlightId==_fixed_agent_id_){//use a pre-defined plan																
					temp=(*itT)->changeString_((*itT)->getSignalPlan(_default_plan_));												
				}
				else{//use plan from saved simulation 													
					temp=(*itT)->changeString_((*itT)->getSignalPlan(getSavedAction(_currentAction_,(*itT)->tlightId)+1));
					
					//reward function
					rwmedio += (*itT)->getWLUSimpleReward();
					//rwmedio += (*itT)->getExpRewardDualFunc();
				}				
				if (temp != ""){				
					message += temp;
					count++;
				}							
			}			
			rwmedio =rwmedio/(double)(count-1);//disconta o agente que nao somou o rw			
			double diff = getSavedAvgReward(_currentAction_)-rwmedio;
			diff = (1.0+diff)/2.0; //mudança de escala, para ficar entre 0 e 1
			//agora tem a diferenca de reward, entao atualiza q table
			_toFile_="";
			for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++){										
				if((*itT)->tlightId==_fixed_agent_id_){
					 //observe(state,action,newState,reward)					 
					(*itT)->_mec_->observe(0,getSavedAction(_currentAction_,(*itT)->tlightId),0,diff);
					_toFile_ = intToString(_fixed_agent_id_)+" "+intToString((*itT)->_mec_->getBestGreedyAction(0))+" "+(*itT)->_mec_->printSimpleQTable(); //0 is the olny state
					cout<<_toFile_<<endl;					
					break;					
				}				
			}			
		}
		if(_simulation_type_==3){//run new simulation using saved q-table 						    
			message=""; //recria a message			
			for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++){								
				//para cada agente pega a acao com o maior valor Q
				cout<<"agente "<<(*itT)->tlightId<<" usando acao: "<<learned_actions[(*itT)->tlightId]+1<<endl;
				message+=(*itT)->changeString_((*itT)->getSignalPlan(learned_actions[(*itT)->tlightId]+1));
				count++;
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
			if(_simulation_type_==2){
				_outFile_<<_toFile_<<endl;
				cout<<">> to file!"<<_toFile_<<endl;
			}
			cout << _name_ << " finished!" << endl;
			break;
		}
		else	
			cout << "Unknown message!!!" << msg << endl;		
		//cout<<"performance"<< getPerformance() <<endl;

	}
	exit(0);
}



