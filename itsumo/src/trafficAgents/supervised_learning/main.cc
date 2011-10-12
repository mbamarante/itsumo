#include "local_agent.hh" 
#include "../../tiny/tinyxml.h"	
#include <pthread.h>

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
	
	TiXmlElement *pha1;
	TiXmlElement *pha2;
	TiXmlElement *ta;
	TiXmlElement *lr;
	TiXmlElement *dr;
	
	TiXmlElement *mem;
	TiXmlElement *stes;
	TiXmlElement *tl;
	TiXmlElement *tls;
	
	for(agent = agents->FirstChildElement("agent");agent; agent = agent->NextSiblingElement("agent")) 
	{
		TiXmlElement *ty;
		ty = agent->FirstChildElement("type");

		if (static_cast<string>(VALUE(ty))=="supervised-learning"){
			LocalAgent* la;
			name = agent->FirstChildElement("name");
			string agentName = VALUE(name);
			
			int startTime=0;
			stime = agent->FirstChildElement("start_time");
			if(stime!=NULL){
				startTime = atoi(VALUE(stime));
			}
			//cout <<"start time "<<startTime<<endl;
			
			mem = agent->FirstChildElement("memsize");
			int memSize = atoi(VALUE(mem));

			stes = agent->FirstChildElement("states");
			int states = atoi(VALUE(stes));

			pha1 = agent->FirstChildElement("first_phase");
			int phase1 = atoi(VALUE(pha1));
			cout <<"phase 1 "<<phase1<<endl;

			pha2 = agent->FirstChildElement("second_phase");
			int phase2 = atoi(VALUE(pha2));
			cout <<"phase 2 "<<phase2<<endl;

			ta = agent->FirstChildElement("tau");
			double tau = atof(VALUE(ta));
			
			
			lr = agent->FirstChildElement("learning_rate");
			double learning = atof(VALUE(lr));
						
			dr = agent->FirstChildElement("discount_rate");
			double discount = atof(VALUE(dr));
			
			stat = agent->FirstChildElement("log");
			bool agentStat = false;
			if ("on" == static_cast<string> (VALUE(stat)))
				agentStat = true;

			tls = agent->FirstChildElement("trafficlights");

			vector<int> ids;
			if (tls != 0) {	
				for(tl = tls->FirstChildElement("trafficlight"); tl;
				tl = tl->NextSiblingElement("trafficlight")){	
					int id = atoi(VALUE(tl));
					ids.push_back(id);
				}
			}

			la = new LocalAgent(ids, agentName, agentStat, memSize, states, learning, discount,startTime,phase1,phase2,tau);			
			(la)->setTopology(n);			
			ans.push_back(la);
		}
	}

	return ans;
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
	
	if (argc < 2) {
		cout 	<< "start_agents " <<  endl
			<< "Usage: " << endl 
			<< "\t" << argv[0]
			<< " <agents file> <network topology file>"
			<< endl;
		exit(0);
	}
	
	topologyReader* n = new topologyReader(argv[2]);

	agent_list_t al = parseAgents(argv[1], n);
	agent_list_t::iterator it;
		
	thread_list_t tl;
	int rc;

	for (it = al.begin(); it != al.end(); it++)
	{
  		pthread_t thread;
		rc = pthread_create(&thread, NULL, ThreadStartup, *it);

		if (rc) 
		{
			printf("Failed to create a thread\n");
			return 1;
		}
		
		tl.push_back(thread);		 
	}	
	thread_list_t::iterator it2;
	for (it2 = tl.begin(); it2 != tl.end(); it2++)
	{
		rc = pthread_join(*it2, NULL);
	
		if (rc) 
		{
			printf("Failed to join to the thread\n");
			return 1;
		}
	}
	return 0;
}


// This function is a helper function. It has normal C linkage, and is
// as the base for newly created ThreadClass objects. It runs the
// run method on the ThreadClass object passed to it (as a void *).
// After the ThreadClass method completes normally (i.e returns),
// we delete the object.
void *ThreadStartup(void *_tgtObject) 
{
  LocalAgent *tgtObject = (LocalAgent *)_tgtObject;
  void *threadResult = tgtObject->run();
  delete tgtObject;
  return threadResult;
}
