#include "local_agent.hh"
using namespace std;

LocalAgent::LocalAgent(vector<int> tls,int rwfunction,	double rwfunctionvalue, string name, bool stat,int memory, int states, double learning_r, double discount_r, set<int> commArea, double errorTh)
{
	_name_ = name;
	_rw_function_= rwfunction;
	_rw_function_parameter_value_= rwfunctionvalue;
	_stat_ = stat;
	_memsize_ = memory;
	_states_ = states;	
	_connectMsg_ = "a;"; //s to request for connection to receive saturation and flow data
	_totalStoppedCars_ = 0;
	_learning_rate_ = learning_r;
	_discount_rate_ = discount_r;	
	_error_threshold_ = errorTh;	
	_start_time_=0;
		
	_communicationArea_ = commArea;
	 
	for (unsigned int i = 0; i < tls.size(); i++)
	{	
		_connectMsg_ += intToString(tls[i]);
		_connectMsg_ += ";";
	}         
	
}
//predator agents
LocalAgent::LocalAgent(string name, int id, bool stat, int states, int actions, double learning_r, double discount_r, set<int> commArea, double errorTh)
{
	_connectMsg_ = "a;";
	_name_ = name;
	_stat_ = stat;
	_memsize_ = 0;
	_states_ = states;	
	_learning_rate_ = learning_r;
	_discount_rate_ = discount_r;
	_error_threshold_ = errorTh;	
	_communicationArea_ = commArea;
	_tlights_ = parsePredators(_states_,actions, id);
}

LocalAgent::~LocalAgent()
{
	cout << "local"<<endl;
	vector <OpLearner*>::iterator it;	
	for (it = (_tlights_).begin(); it != (_tlights_).end(); it++) 
		(*it)->~OpLearner();
}

void  LocalAgent::endEpisode(){
	vector <OpLearner*>::iterator itT;	
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++)			
		(*itT)->endEpisode();			
}
int LocalAgent::getTimeStep(){
	return _currentItsumoTimeStep_;
}

int LocalAgent::getStartTime(){
	return _start_time_;
}


/*
 * Distance indicates the radius of the tl perception area
 * 
 */
void LocalAgent::addTLpartners(unsigned int distance){	
	vector <OpLearner*>::iterator itT;	
	int ptlID =-1;
	
	//unsigned int dis =-1;
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){
		cout<<"->TL"<<(*itT)->tlightId<<endl;
		for (int i=0;i<4;i++){    	    			      				
			if ( (*itT)->nodesAtDirections[i] != -1){				
				//while (dis < distance){
					ptlID = (_topology_)->getTLIdAtNode((*itT)->nodesAtDirections[i]);					
					if ( (ptlID!=-1)&&(ptlID !=(*itT)->tlightId )){
						if ( (*itT)->addToComunicationArea( ptlID ) ){
							//(*itT)->addToDistance( ptlID,distance );
							cout<<"adicionou!"<< ptlID <<endl;
						}
					//++dis;
					}
				//}					
			}
		}	
	}
}

//returns a list os simulation states
vector<simulationState> LocalAgent::updateStates(string rawState) 
{
	/*The message consists of the following fields, separeted by ";"
		currentIteration
		numberOfNodes;
			[nodeId; trafficLightId; currentPlan; 
				totalNumberOfIncomingLanes;
					[laneId; laneDegreeOfSaturation;laneEntryFlow; laneQueue;]]
				totalNumberOfOutgoingLanes;]							
					[laneId; laneDegreeOfSaturation;laneEntryFlow; laneQueue;]]
					
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

string LocalAgent::collectMessagesFromAgents(){
	std::string reply = "";		
	vector <OpLearner*>::iterator itT;
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){				
			string temp = (*itT)->sendMessages();
			if (temp != ""){
				reply += temp;
			}			
	}		
	return reply;			
}

void LocalAgent::redirectMessages(string messages){			
	if (messages!=" "){
		vector <OpLearner*>::iterator itT;	
		for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){						
			(*itT)->pcsIncomingMsg(messages);							
		}		
	}		
}
/**
 * Reply all information requests sending all the outgoing messages 
 */
void LocalAgent::replyRequests(){
	string msg = "";		
	vector <OpLearner*>::iterator itT;		
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){	
			(*itT)->acceptAllInformationRequests();				
			msg+=(*itT)->sendMessages();							
	}	
	*msg_client_socket << "s"+msg; //send messages
	*msg_client_socket >> msg;
}

void LocalAgent::analyzeLastEpisode(){
	string reply = "";
	vector <OpLearner*>::iterator itT;	
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){						
			reply+=(*itT)->analyzeLastEpisode();													
	}
	//cout<<"send the agents messages to messages server"<<endl;
	*msg_client_socket << "s"+reply;	
	*msg_client_socket >> reply; 
	//cout<<"messages server sent ok!"<<endl;
}

void LocalAgent::analyzeLastEpisodePP(){
	string reply = "";
	vector <OpLearner*>::iterator itT;	
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){						
			reply+=(*itT)->analyzeLastEpisodePP();													
	}
	//cout<<"send the agents messages to messages server"<<endl;
	*msg_client_socket << "s"+reply;	
	*msg_client_socket >> reply; 
	//cout<<"messages server sent ok!"<<endl;
}


void LocalAgent::analyzeNewState(){
	string reply = "";
	vector <OpLearner*>::iterator itT;	
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){
			(*itT)->getComposedState();						
			reply+=(*itT)->sendMessages();													
	}
	*msg_client_socket << "s"+reply;	
	*msg_client_socket >> reply; 
	//cout<<"messages server sent ok!"<<endl;
}

void LocalAgent::analyzeNewState(int state){
	string reply = "";
	vector <OpLearner*>::iterator itT;	
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){
			(*itT)->getComposedState(state);						
			reply+=(*itT)->sendMessages();													
	}
	*msg_client_socket << "s"+reply;	
	*msg_client_socket >> reply; 
	//cout<<"messages server sent ok!"<<endl;
}


void LocalAgent::sendMessages(){
	string reply = "";
	vector <OpLearner*>::iterator itT;	
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){						
			reply+=(*itT)->sendMessages();													
	}
	//cout<<"send the agents messages to messages server"<<endl;
	*msg_client_socket << "s"+reply;	
	*msg_client_socket >> reply; 
	//cout<<"messages server sent ok!"<<endl;
}


void LocalAgent::receiveMessages(){
	string msg = "";			
	*msg_client_socket << "r"; //request messages
	*msg_client_socket >> msg;
	msg.erase(msg.begin());		
	if (!msg.empty()){		
		redirectMessages(msg);				
	}		
}
void LocalAgent::observeLast(){
	vector <OpLearner*>::iterator itT;		
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){						
			(*itT)->observeLast();							
	}
}	

void LocalAgent::observeNewState(){
	vector <OpLearner*>::iterator itT;		
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){						
			(*itT)->observeNewState();							
	}
}	



void LocalAgent::observe(){
	vector <OpLearner*>::iterator itT;		
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){						
			(*itT)->observe();							
	}
}	
void LocalAgent::observe(double rw){
	vector <OpLearner*>::iterator itT;		
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){						
			(*itT)->observe(rw);							
	}
}	

void LocalAgent::evaluateActionOffers(){
	vector <OpLearner*>::iterator itT;		
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){						
			(*itT)->acceptAllBestActionOffers();							
	}
}


void LocalAgent::selectAction(){
	vector <OpLearner*>::iterator itT;		
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){						
			(*itT)->selectAction();							
	}
}	
void LocalAgent::commitAction(){
	vector <OpLearner*>::iterator itT;		
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++){						
			(*itT)->commitAction();							
	}
}	


void LocalAgent::pcsInfoMsg(std::string msg){
	//cout << _name_ << "\n: Received new network state\n"<< msg << endl;
	_simulationStates_ = updateStates(msg);
	vector<simulationState>::iterator itS;
	vector <OpLearner*>::iterator itT;	

	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++) {

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

//string LocalAgent::pcsPPMsg(int state, double rw){
//	cout << _name_ << "\n: Received new env state\n"<< state << endl;
//	
//	vector <OpLearner*>::iterator itT;	
//
//	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++) {
//			(*itT)->updateStateRW(state, rw);
//	}
//
//}

void LocalAgent::pcsRequestMsg(std::string msg)
{
    //cout << _name_ << "\n: Received traffic lights update request (" << msg << ")" << endl;

	std::string reply = "";
	reply += "x;";
	string message = "";
	int count = 0;
	vector <OpLearner*>::iterator itT;
	
//cout<<"itsumo timestep"<<_currentItsumoTimeStep_<<" start "<< _start_time_<<endl;

	if (_currentItsumoTimeStep_>= _start_time_){
		for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++) 
		{	
			string temp = (*itT)->newControlState();
			if (temp != "")
			{
				count ++;
				message += temp;
			}
		}
	}
	
	reply += intToString((int)count) + ";";
	reply += message;

	//cout << _name_ << ": Reply to change request: \n" << reply << endl;

	*client_socket << reply;
}


unsigned int LocalAgent::getAction(){
	vector <OpLearner*>::iterator itT;
	unsigned int ret;
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++) {
			ret = (*itT)->getCurrentSingleAction();
	}
	return ret;
}

vector <OpLearner *> LocalAgent::parseTLights(std::string tlightConfig, int memorySize,int states)
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
	
	vector <OpLearner *> answer;
	
	tlightConfig.erase(0,2);// ignore the first field (message type)
	replace(tlightConfig.begin(),tlightConfig.end(),';',' '); //replace the separator ";" to " ", so the striong can be tokenized	
	
	queue<string> tokens;
	string buf;
	stringstream ss(tlightConfig); // Insert the string into a stream	
    while (ss >> buf)
        tokens.push(buf);

	int numberSignalPlans = 0;    
	int numberTlights = getNextInt(&tokens);
	
	for (int tl=0; tl < numberTlights; tl++){			
									
		int tlightId = getNextInt(&tokens); 
		// the number of signal plans is consedered the number of actions			
		numberSignalPlans = getNextInt(&tokens);
//		cout<<"num sp"<<numberSignalPlans<<endl;										
		OpLearner* newTlight = new OpLearner(false,_rw_function_,_rw_function_parameter_value_, tlightId, memorySize, states, numberSignalPlans,_stat_,_learning_rate_,_discount_rate_, _error_threshold_);
		newTlight->setComunicationArea(_communicationArea_);
		cout<<"Created tl"<<tlightId<<" par:"<<memorySize<<" "<<states<<" "<<numberSignalPlans<<" "<<_stat_<<" "<<_learning_rate_<<" "<<_discount_rate_<<endl;				        	    		      	
		for (int sp=0; sp < numberSignalPlans; sp++) 
		{
			struct signalPlan newPlan;
											
			newPlan.signalPlanId = getNextInt(&tokens);
			
			int numberPhases	= getNextInt(&tokens);
			
                   			
			for (int ph=0; ph < numberPhases; ph++){ 
				struct phase newPhase;
				newPhase.phaseId = getNextInt(&tokens);				
				newPhase.start = getNextInt(&tokens);				
				newPhase.end = getNextInt(&tokens);
				int numberDirections = getNextInt(&tokens);
				
//				cout<<"newPhase.phaseId"<<newPhase.phaseId<<endl;
//				cout<<"newPhase.start"<<newPhase.start<<endl;
//				cout<<"newPhase.end "<<newPhase.end <<endl;
//				cout<<"numberDirections"<<numberDirections <<endl;
                                				
				for (int direction=0; direction < numberDirections; direction++) {
					struct direction newDirection;
					newDirection.fromLane = getNextInt(&tokens);
					newDirection.toLaneset = getNextInt(&tokens);
					newPhase.directions.push_back(newDirection);
//					cout<<"fromLane:"<<newDirection.fromLane<<endl;			
//					cout<<"toLaneset:"<<newDirection.toLaneset<<endl;
					
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


vector <OpLearner *> LocalAgent::parsePredators(int states, int actions, int id){
	vector <OpLearner *> answer;
	OpLearner* newP = new OpLearner(true,id,_memsize_, states, actions,_stat_,_learning_rate_,_discount_rate_,_error_threshold_);
	newP->setComunicationArea(_communicationArea_);
	cout<<"Created predator:"<<id<<" par:"<<_memsize_<<" "<<states<<" "<<actions<<" "<<_stat_<<" "<<_learning_rate_<<" "<<_discount_rate_<<endl;				        	    		      		
	answer.push_back(newP);		
	return answer;
}

void  LocalAgent::setTopology(topologyReader* n){

	_topology_ = n;  
}

void  LocalAgent::setDirections(){
	vector <OpLearner*>::iterator itT;
	vector<lanesetSimple*>::iterator ls;	
	int node = OpLearner::INVALID_ID;	
	for (itT = (_tlights_).begin(); itT != (_tlights_).end(); itT++) {
		tLight* tlight = (_topology_)->getTLight((*itT)->tlightId);			
		node = (tlight)->node;			
		nodeSimple* currentNode = (_topology_)->getNode(node);
		for (ls = (_topology_)->lanesets.begin(); ls !=  (_topology_)->lanesets.end(); ls++) {
			if ( ((*ls)->nodeEnd == node)&&( (*ls)->nodeEnd != OpLearner::INVALID_ID ) ) {
				nodeSimple* ns = (_topology_)->getNode((*ls)->nodeBegin);					
				(*itT)->nodesAtDirections[currentNode->relativePosition(ns)] = ns->id;			
			}
		}		
	}			

}

ClientSocket* LocalAgent::connectToITSUMO(){
	TLAgent *dummy = new TLAgent();
	ClientSocket* client_socket = new ClientSocket( "localhost", 30000 );	
	std::string reply;
	cout << _name_ <<": I will try to connect to ITSUMO..." << endl;
	*client_socket << _connectMsg_;
	*client_socket >> reply;
	cout << _name_ <<": ITSUMO accepted! \n" << endl;
		
	// receives a message from the simulator containing the configuration
	// of each traffic light (ie, signalplans, phases, etc). Then, one
	// object for each tlight is created
	_tlights_ = parseTLights(reply,_memsize_,_states_);
	setDirections();
	if (_communicationArea_.empty()) {
		addTLpartners(0);
	}
	std::cout<<_name_ <<": We received this response from the server:\n\"" << reply << "\"\n";
	return client_socket;
}


ClientSocket* LocalAgent::connectToMSGServer(){
	std::string replyMsg;	
	ClientSocket* msg_client_socket = new ClientSocket("localhost", 33000);		
	cout << _name_ <<": I will try to connect to MESSAGES SERVER..."<< endl;	
	*msg_client_socket << _connectMsg_;
	*msg_client_socket >> replyMsg;
	cout << _name_ <<": MESSAGES SERVER accepted! \n" << endl;	
	return msg_client_socket; 
}



