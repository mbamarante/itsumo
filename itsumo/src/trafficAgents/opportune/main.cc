#include "local_agent.hh" 
#include "../../socket/ServerSocket.h"
#include "../../tiny/tinyxml.h"	
#include "util.h"
#include <pthread.h>
#include <queue>
#include <vector>

using namespace std;

#define VALUE(element) (element->FirstChild()->Value())

extern "C" void *ThreadStartup(void *);


typedef vector<LocalAgent*> agent_list_t;
typedef vector<pthread_t> thread_list_t;


// reads the agents.xml file for agent definition
agent_list_t parseAgents(char* fileName, topologyReader *n)
{
	agent_list_t ans;
	
	TiXmlElement *agents;

	TiXmlDocument doc( fileName );
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay )
	{
		printf( "Could not load file. Error='%s'.\nExiting.\n", 
				doc.ErrorDesc());
		exit(1);
	}

	agents = doc.FirstChildElement( "agents" );

	TiXmlElement *agent;
	TiXmlElement *name;
	TiXmlElement *stat;
	TiXmlElement *stime;
	TiXmlElement *mem;
	TiXmlElement *stes;
	TiXmlElement *lr;
	TiXmlElement *dr;
	TiXmlElement *err;
	TiXmlElement *rwt;
	TiXmlElement *rwv;

	TiXmlElement *tl;
	TiXmlElement *tls;
	TiXmlElement *comm;
	
	for(agent = agents->FirstChildElement("agent");agent; agent = agent->NextSiblingElement("agent")) 
	{
		TiXmlElement *ty;
		ty = agent->FirstChildElement("type");

		if (static_cast<string>(VALUE(ty))=="OPPORTUNE"){
			LocalAgent* la;

			name = agent->FirstChildElement("name");
			string agentName = VALUE(name);

			int startTime=0;
			stime = agent->FirstChildElement("start_time");
			if(stime!=NULL){
				startTime = atoi(VALUE(stime));
			}

			mem = agent->FirstChildElement("memsize");
			int memSize = atoi(VALUE(mem));
			stes = agent->FirstChildElement("states");
			int states = atoi(VALUE(stes));


			double learning=0.1;
			lr = agent->FirstChildElement("learning_rate");
			if (lr!=NULL){
				learning = atof(VALUE(lr));
			}

			double discount=0.9;
			dr = agent->FirstChildElement("discount_rate");
			if (dr!=NULL){
				discount = atof(VALUE(dr));
			}
			
			double errorTh=0.1;
			err = agent->FirstChildElement("error_threshold");
			if (err!=NULL){
				errorTh = atof(VALUE(err));
			}
			int rwtype=0;
			rwt = agent->FirstChildElement("reward_function");
			if (rwt!=NULL){
				 rwtype= atoi(VALUE(rwt));
			}
			double rwval=0;
			rwv = agent->FirstChildElement("reward_function_parameter");
			if (rwv!=NULL){
				 rwval= atof(VALUE(rwv));
			}
			

			stat = agent->FirstChildElement("log");
			bool agentStat = false;
			if ("on" == static_cast<string> (VALUE(stat)))
				agentStat = true;

			tls = agent->FirstChildElement("trafficlights");

			vector<int> ids;
			if (tls != 0) 
			{	
				for(tl = tls->FirstChildElement("trafficlight"); tl;
				tl = tl->NextSiblingElement("trafficlight"))
				{	
					int id = atoi(VALUE(tl));
					ids.push_back(id);
				}
			}
			
			set<int> commarea;
			comm = agent->FirstChildElement("communication_area");
			if (comm != 0) 
			{	
				for(tl = comm->FirstChildElement("trafficlight_id"); tl;
				tl = tl->NextSiblingElement("trafficlight_id"))
				{						
					commarea.insert(atoi(VALUE(tl)));
				}
			}						
			la = new LocalAgent(ids,rwtype,rwval,agentName, agentStat, memSize, states, learning, discount, commarea, errorTh);
			(la)->setStartTime(startTime);
			(la)->setTopology(n);				
			ans.push_back(la);	
		}	
	}

	return ans;
}
 /* *******************
  * create predator agents
  * 
  *************/
agent_list_t parseHawks(std::string rep){
	agent_list_t ans;
	replace(rep.begin(),rep.end(),',',' '); //replace the separator "," to " ", so the striong can be tokenized	
	
	queue<string> tokens;
	string buf;
	stringstream ss(rep); // Insert the string into a stream	
    while (ss >> buf)
        tokens.push(buf);

	int numberStates = getNextInt(&tokens);    
	int numberActions = (int)sqrt(getNextDouble(&tokens));

	set<int> commarea1, commarea2;
	commarea1.insert(2);
	ans.push_back(new LocalAgent("Predator 1", 1, true, numberStates, numberActions, 0.5, 0.9, commarea1,0.05));
		
	commarea2.insert(1);
	ans.push_back(new LocalAgent("Predator 2", 2, true, numberStates, numberActions, 0.5, 0.9, commarea2,0.05));
		
	return ans;
		
}

void *StartMsgServer(void *t)
{

	vector<ServerSocket*> agent_sockets;
	int timeoutSeconds = 5;
	try
	{
		// Create the socket
		ServerSocket server ( 33000 );
		cout<<"msg server created!!"<<endl;
		server.set_non_blocking(false);
		pthread_yield();							
		while ( true ) {		  		
				fd_set handlers;
				struct timeval timeout;
				int retval;
		
		        // inicializa o conjunto de handlers a serem observados
	         	FD_ZERO(&handlers);
	         	// inclui stdin (fd=0) no conjunto de handlers
	         	FD_SET(0, &handlers);
	         	// inclui socket onde o servidor escuta
	         	FD_SET( server.getHandler(), &handlers);
	
	         	// inicializa estrutura de timeout 
	         	timeout.tv_sec = timeoutSeconds;
	         	timeout.tv_usec = 0;
	
	         	retval = select(server.getHandler()+1, &handlers, NULL, NULL, &timeout);
	
	         	if (retval == -1)
	               		perror("Error in select()!");
	         	else {
	             		if (retval)  {
	                		if (FD_ISSET(0, &handlers)) {                               			
								break;
							}
	                  		else {
	                       			cout << "New connection arrived in MESSAGES SERVER" << endl;
							}
	             		}
	             		else {
							break;
						}
	         	}
	
				ServerSocket* new_sock = new ServerSocket();
		  		server.accept ( *new_sock );
	
		  		try
		  		{
		      		string data;
			  		*new_sock >> data;
					if ((data.substr(0, 1) == "a")||(data.substr(0, 1) == "s")){ //s ou a  = pedido de conexao
						//devolve mensagem de ok					
						agent_sockets.push_back(new_sock);
			  			*new_sock << "ok";
						cout << "Agent registered in MESSAGES SERVER  with success." << endl;
					}
					else{
						*new_sock << "NO";
						cout << "Registration fail!" << endl;
					}
		  		}
		  		catch ( SocketException& ) {}
	
			}		
		}
  	catch ( SocketException& e ){
    	std::cout << "Exception was caught on creating the msg server: " << e.description() << "\nExiting.\n";
    }
    vector<ServerSocket*>::iterator as;
    int sent=0; 
    bool finish = false;
    std::string data, messages;	
	while(!finish){					  			          	
		as=agent_sockets.begin();					
		while(as != agent_sockets.end()){			
			(**as)>>data;					 										  	  
			if (data[0]=='s'){						
				data.erase(data.begin());								
				//if (!data.empty()) cout<<"recebido:"<<data<<endl;		  
				messages+=data;
				sent++;													 			
				(**as)<<"ok";
				//cout<<"msg server received "<<sent<<" messages."<<endl;									
			}
			if (data[0]=='r'){					  	 
				//cout<<"msg server received request"<<endl;										
				(**as) << " "+messages;																						
				if (sent>0) sent--;
				//if (!messages.empty()) cout<<"message to send <"<<messages<<"> "<<sent<<" times."<<endl;					
				if (sent==0){ 
					messages.clear();//all agents received the messages																	
				}					
				
			}						 					  
			if (data=="end"){
				cout<<"MESSAGES SERVER received END message."<<endl;								
				finish = true;										  				 					  	  
			  	agent_sockets.erase(as);
			  	as = agent_sockets.begin();			
			  	continue;  					  					  
			}																			
			if (!finish) as++;	
		}					              		    			  		      
	}
 	pthread_exit((void*) t);
}



//1
//Aqui devemos declarar cada agente local usado na simulação.
//Um agente local controla um determinado numero de semaforos, cujos ids
//sao passados no construtor do agente.
//Aqui tb devemos declarar o agente global caso ele deva existir na simulacao.
//Ou seja, toda inicialização de agentes deve ser feita aqui neste main.

//2
//Cada agente local eh composto por um vetor de TLAgent. Nesse objeto deve ser
//implementada a inteligencia de controle. Caso se deseje varios tipos de TLAgent
//com diferentes tipos de controle, deve ser criada uma classe derivando de TLAgent
//e em LocalAgent essa nova classe deve ser incluida.

int main ( int argc, char **argv )
{
//
//		vector<SAComponent*> v;
//		vector<SAComponent*>::const_iterator vi;		
//		v.push_back(new SAComponent(340,0));
//		v.push_back(new SAComponent(320,1));
//		v.push_back(new SAComponent(321,2));
//		
//		vector<SAComponent*> v2;		
//		vector<SAComponent*>::const_iterator vi2;		
//		v2.push_back(new SAComponent(340,1));
//		v2.push_back(new SAComponent(321,0));
//		vector<SAComponent*> newValue(v.size()+v2.size());	
//
//		cout<<"321 "<< intToString( getInfoFromOwner( v, 321))<<endl;
//		cout<<"340 "<< intToString( getInfoFromOwner( v, 340))<<endl;
//		cout<<"320 "<< intToString( getInfoFromOwner( v, 320))<<endl;
//		
//		
//		cout<<atoi(argv[1])<<endl;
//		exit(0);
//	
		bool itsumo = false;
		
		agent_list_t::iterator it;
		agent_list_t al;
		ClientSocket* client_socket;

		
		if (argc >2) {//itsumo environment
			if (argc < 3) {
				cout 	<< "start_agents " <<  endl
					<< "Usage: " << endl 
					<< "\t" << argv[0]
					<< " <agents file> <network topology file>"
					<< endl;
				exit(0);
			}
			itsumo = true; 
			
			topologyReader* n = new topologyReader(argv[2]);
			
			al = parseAgents(argv[1], n);
			
				
			thread_list_t tl;

			for (it = al.begin(); it != al.end(); it++){
	      		(*it)->client_socket = (*it)->connectToITSUMO();
	      	}
		}
		else{//Hawk-Dove environment
				client_socket = new ClientSocket( "localhost", 50000 );	
				std::string reply;
				cout <<": I will try to connect to Hawk and Dove server..." << endl;
				*client_socket << "a;";
				*client_socket >> reply;
				cout <<": Hawk and Dove server accepted! \n" << endl;
				std::cout<<": We received this response from the server:\n\"" << reply << "\"\n";
				al = parseHawks(reply);
		}
		
		
		
		int rc;
		
		pthread_t threadServer;	
		rc = pthread_create(&threadServer, NULL,StartMsgServer, NULL);
		if (rc){
        	printf("ERROR; return code from pthread_create() is %d\n", rc);
        	exit(-1);
      	}

		sleep(2);// wait for the messages server start...
		for (it = al.begin(); it != al.end(); it++){
      		(*it)->msg_client_socket = (*it)->connectToMSGServer();
		}
		
		
	
		bool finish = false;
		agent_list_t::iterator it2;
		
		 
		if(itsumo){			
			bool info 	= false;
			bool request= false;
			bool comm 	= true;
			std::string msg="";
			while(!finish){										
				for (it = al.begin(); it != al.end(); it++){//process message received from itsumo
					msg="";			      				
			    	(*(*it)->client_socket)>>msg;
			    	if ( (msg[0] == 'r') && ( (*it)->getTimeStep()< (*it)->getStartTime() ) ){
			    		(*(*it)->client_socket) << "x;0;";
			    		continue;
			    	}
			    	//cout<<(*it)->_name_<<"received "<<msg<<endl;			    	
			    	if (msg[0] == 'i'){ 
			    		(*it)->pcsInfoMsg(msg);
			    		info = true;
			    		request = false;
			    	}
			    	else if (msg[0] == 'r'){
			    		if (info){
			    			request = true;
			    			info 	= false;
			    			comm = true;
			    		}
			    	} 
			    	else if (msg == "end;"){ 
			    		finish = true;
			    		break;
			    	}
			    }
		      	if ((info)&&(comm)){	      	
		      		//inicio avalia o ultimo passo		      																   			
	      			for (it2 = al.begin(); it2 != al.end(); it2++){			      				
	      				(*it2)->analyzeLastEpisode();
	      				//ask for information about the last step
	      			}
	      			for (it2 = al.begin(); it2 != al.end(); it2++){			      				
	      				(*it2)->receiveMessages();
	      				//receive information requests
	      			}
	      			for (it2 = al.begin(); it2 != al.end(); it2++){			      				
	      				(*it2)->replyRequests();
	      				//reply requests
	      			}
	      			for (it2 = al.begin(); it2 != al.end(); it2++){			      				
	      				(*it2)->receiveMessages();
	      				//update last action using received information
	      				(*it2)->observeLast();
	      			}
	      			//fim avalia o ultimo passo	     
	      			comm = false;	      		 						      			      			      								
				}
		      	else if (request){
		      		//inicio avalia o proximo passo
	      			for (it2 = al.begin(); it2 != al.end(); it2++){			      				
	      				(*it2)->analyzeNewState();   						 		      				
	      			}
	      			for (it2 = al.begin(); it2 != al.end(); it2++){			      				
	      				(*it2)->receiveMessages();	
	      				//receive information requests	      				
	      			}
	      			for (it2 = al.begin(); it2 != al.end(); it2++){			      				
	      				(*it2)->replyRequests(); 
	      				//send information , reply requests
	      			}		
	      			for (it2 = al.begin(); it2 != al.end(); it2++){			      				
	      				(*it2)->receiveMessages();	//receive information answers to requests
	      				(*it2)->observeNewState();	// observes the received state with the combined information      				
	      			}      					      	
	      			//fim avalia o proximo passo				      					      						      						      						
      				for (it2 = al.begin(); it2 != al.end(); it2++){	
      					(*it2)->observe(); //update			      						      				
	      				(*it2)->selectAction();	//seleciona uma ação
	      				(*it2)->sendMessages();	      				
	      			}
					for (it2 = al.begin(); it2 != al.end(); it2++){			      				
	      				(*it2)->receiveMessages();	
	      				//receive action requests	      				
	      			}
	      			for (it2 = al.begin(); it2 != al.end(); it2++){
	      				(*it2)->evaluateActionOffers();
	      				(*it2)->sendMessages();	 // send "AGREE/REFUSE_JOINT_ACTION"    			      													      				
	      			}
	      			for (it2 = al.begin(); it2 != al.end(); it2++){			      				
	      				(*it2)->receiveMessages();	// receive "AGREE/REFUSE_JOINT_ACTION"
	      				(*it2)->commitAction();	    
	      				(*it2)->pcsRequestMsg("r;"); //nao precisa da msg porque é so um request "r;"  				
	      			}		        					          						      			      			
		      	}      		
				else if (finish){
					for (it2 = al.begin(); it2 != al.end(); it2++){
						(*(*it2)->client_socket) << "end;";
						cout << (*it2)->_name_ << " finished!" << endl;
					}	
					break;																	
				}
		}
	}//  fim da parte itsumo
	else{ //presa predador
		unsigned int totalEp = atoi(argv[1]); 
		bool started = false;
		ofstream  resultFile;
		resultFile.open("resultados.log");
		//*client_socket << "0 0;";
		unsigned int episode =0;
		unsigned int steps = 1;
		string oldmsg;
		int state[2];
		std::string msg("");
		while(!finish){		
			if (started)			
				*client_socket>>msg;
			else{
				msg = "N-N-N-N -1"; //initial state
				started = true;
			}			
			replaceNTimes(msg,"-", " ",3); //replace the singal "-" 3 times, the states separator 												 
			replaceChars(msg,"N", "25"); //replace the N for "25"
			//cout<<"recebeu "<<msg<<endl;
      
			queue<string> tokens;
			string buf;	
			stringstream ss(msg); // Insert the string into a stream	
			while (ss >> buf)
				tokens.push(buf);
									
			state[0] = 25*getNextInt(&tokens) + getNextInt(&tokens);
			state[1] = 25*getNextInt(&tokens) + getNextInt(&tokens);
			double reward = getNextDouble(&tokens);
			//msg += intToString(rand()%4)+" "+intToString(rand()%4)+";"; /* randomico!! */		     
			if (reward == 10.0){ //pray captured!							
//				cout<<"rw "<<reward<<endl;
//				cout<<"e "<<msg<<endl;
//				cout<<"info ag1: "<<stateInfo[0]<<stateInfo[1]<<"ag2: "<<stateInfo[2]<<stateInfo[3]<<endl;
//				cout<<"estado ag1: "<<state[0]<<"ag2: "<<state[1]<<endl;					
				++episode;
				resultFile<<episode<<" "<<steps<<endl;
				//cout<<"episode "<<episode<<" steps: "<<steps<<endl;
				steps = 0;
			}
			msg="";
			if (episode<totalEp){
				//***************
				 //inicio avalia o ultimo passo			    			
      			for (it = al.begin(); it != al.end(); it++){			      				
      				(*it)->analyzeLastEpisodePP();
      				//ask for information about the last step
      			}
      			for (it = al.begin(); it != al.end(); it++){			      				
      				(*it)->receiveMessages();
      				//receive information requests
      			}
      			for (it = al.begin(); it != al.end(); it++){			      				
      				(*it)->replyRequests();
      				//reply requests
      			}
      			for (it = al.begin(); it != al.end(); it++){			      				
      				(*it)->receiveMessages();
      				//update last action using received information
      				(*it)->observeLast();
      			}
      			//fim avalia o ultimo passo
      			//inicio avalia o proximo passo
  				for (unsigned int i=0; i<2; i++){			      				
      				al[i]->analyzeNewState(state[i]);   
					 		      				
      			}
      			for (it = al.begin(); it != al.end(); it++){			      				
      				(*it)->receiveMessages();	
      				//receive information requests	      				
      			}
      			for (it = al.begin(); it != al.end(); it++){			      				
      				(*it)->replyRequests(); 
      				//send information , reply requests
      			}		
      			for (it = al.begin(); it != al.end(); it++){			      				
      				(*it)->receiveMessages();	//receive information answers to requests
      				(*it)->observeNewState();	// observes the received state with the combined information      				
      			}      					      					      					      						      						      			
      			
  				for (it = al.begin(); it != al.end(); it++){	
  					(*it)->observe(reward);  					
  					if (steps==0) {
  					//cout<<"end episode!"<<endl;
						(*it)->endEpisode();
  					}
  					else{      						      				
      					(*it)->selectAction();	
      					(*it)->sendMessages();
  					}	      				
      			}
      			if (steps!=0) {//end of episode
					for (it = al.begin(); it != al.end(); it++){			      				
	      				(*it)->receiveMessages();	
	      				//receive action requests	      				
	      			}
	      			for (it = al.begin(); it != al.end(); it++){
	      				(*it)->evaluateActionOffers();
	      				(*it)->sendMessages();	 // send "AGREE/REFUSE_JOINT_ACTION"    			      													      				
	      			}
	      			for (it = al.begin(); it != al.end(); it++){			      				
	      				(*it)->receiveMessages();	// receive "AGREE/REFUSE_JOINT_ACTION"
	      				(*it)->commitAction();
	      				msg += intToString( (*it)->getAction() )+" ";/**/		      				
	      			}
					//***************
					msg.resize(msg.size()-1);
					msg +=";";
					*client_socket<< msg;
					//cout<<msg<<endl;
      			}
				else { 
					*client_socket<< "0 0;";
				} 
				++steps;
			}
			else{
				*client_socket<<"CL;";
				*client_socket>>msg;
				cout<<msg<<endl;
				finish = true;
			}
		}	
		resultFile.close();	
	}// fim presa-predador
	
  return 0;
	
}
