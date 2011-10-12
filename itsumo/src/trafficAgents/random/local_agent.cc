#include "local_agent.hh"
#include "../../basic/utils.hh"

LocalAgent::LocalAgent(vector<int> tls, string name, bool stat)
{
	_name_ = name;
	_stat_ = stat;
	_connectMsg_ = "a;";
	
	for (unsigned int i = 0; i < tls.size(); i++)
	{	
		_connectMsg_ += intToString(tls[i]);
		_connectMsg_ += ";";
	}
}

LocalAgent::~LocalAgent()
{
	vector <randomTLAgent*>::iterator it;	
	for (it = (_tlights_).begin(); it != (_tlights_).end(); it++) 
		(*it)->~randomTLAgent();
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
	
	curToken = strtok (NULL, ";");
	int numberOfNodes = atoi(curToken);
	vector<simulationState> ans;
	
 
 
      
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
		
			
		//state->n = n;				
        ans.push_back(state);
				    
	}
	
	return ans;
}

void LocalAgent::pcsInfoMsg(std::string msg, vector<randomTLAgent*> &trafficLights, 
				ClientSocket* client_socket)
{
	cout << _name_ << "\n: Received new network state\n" << msg << endl;
	vector<simulationState> states = updateStates(msg);
	
	vector<simulationState>::iterator itS;
	vector <randomTLAgent*>::iterator itT;	

	for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++) {

		for (itS = states.begin(); itS != states.end(); itS++)
		{
			if ( (*itS).n.tlightId == (*itT)->tlightId ) {
				(*itT)->updateState((*itS));
			}
		}
	}

	*client_socket << "OK.";
}

void LocalAgent::pcsRequestMsg(std::string msg, vector <randomTLAgent*> &trafficLights, 
				ClientSocket * client_socket)
{
	cout << _name_ << "\n: Received traffic lights update request (" << msg << ")" << endl;

	std::string reply = "";
	reply += "x;";


	string message = "";
	int count = 0;
	vector <randomTLAgent*>::iterator itT;	

	for (itT = (trafficLights).begin(); itT != (trafficLights).end(); itT++) 
	{	
		string temp = (*itT)->newControlState();
		if (temp != "")
		{
			count ++;
			message += temp;
		}
	}

	reply += intToString(count) + ";";
	reply += message;

	cout << _name_ << ": Reply to change request: \n" << reply << endl;

	*client_socket << reply;
}


vector <randomTLAgent *> LocalAgent::parseTLights(std::string tlightConfig, int memorySize)
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
 	
	vector <randomTLAgent *> answer;

	char tok[50];

	//init
	char p[strlen(tlightConfig.c_str())+1];
	strncpy(p, tlightConfig.c_str(), strlen(tlightConfig.c_str())+1);

	char *pont;
	pont = p;

        //start
	getNextVal(&pont, tok);
        cout << "FIRST FIELD [OPERATION TYPE]: " << tok << endl; // ignore the first field (message type)

	getNextVal(&pont, tok);
	int numberTlights= atoi(tok);

	
	for (int tl=0; tl < numberTlights; tl++)
	{
		getNextVal(&pont, tok);
		int tlightId = atoi(tok);

		randomTLAgent *newTlight = new randomTLAgent(tlightId, memorySize, _stat_);

		getNextVal(&pont, tok);
		int numberSignalPlans = atoi(tok);

		for (int sp=0; sp < numberSignalPlans; sp++)
		{
			struct signalPlan newPlan;

			getNextVal(&pont, tok);
			newPlan.signalPlanId = atoi(tok);

			getNextVal(&pont, tok);
			int numberPhases = atoi(tok);

			for (int ph=0; ph < numberPhases; ph++)
			{
				struct phase newPhase;

				getNextVal(&pont, tok);
				newPhase.phaseId = atoi(tok);
				getNextVal(&pont, tok);
				newPhase.start = atoi(tok);
				getNextVal(&pont, tok);
				newPhase.end = atoi(tok);
				getNextVal(&pont, tok);
				int numberDirections = atoi(tok);

				for (int direction=0; direction < numberDirections; direction++)
				{
					struct direction newDirection;

					getNextVal(&pont, tok);
					newDirection.fromLane = atoi(tok);

					getNextVal(&pont, tok);
					newDirection.toLaneset = atoi(tok);
					newPhase.directions.push_back(newDirection);
				}
				newPlan.phases.push_back(newPhase);
			}
			newTlight->signalPlans.push_back(newPlan);
		}
		answer.push_back(newTlight);
	}
	
	return answer;
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
	// object for each tlight is created
	_tlights_ = parseTLights(reply, 10);
	

	std::cout << "We received this response from the server:\n\"" << reply << "\"\n";

	while (true)
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
			cout << _name_ << " finished!" << endl;
			break;
		}

		else
			cout << "Unknown message!!!" << msg << endl;
	}

	return (NULL);
  	
}

