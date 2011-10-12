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
agent_list_t parseAgents(char* fileName)
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

	TiXmlElement *tl;
	TiXmlElement *tls;
	
	for(agent = agents->FirstChildElement("agent");agent; agent = agent->NextSiblingElement("agent")) 
	{
		LocalAgent* la;

		name = agent->FirstChildElement("name");
		string agentName = VALUE(name);


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
		
		la = new LocalAgent(ids, agentName, agentStat);

		ans.push_back(la);
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

//todo: cada agente deve ser uma thread.
int main ( int argc, char **argv )
{
	if (argc < 2) {
		cout 	<< "start_agents " <<  endl
			<< "Usage: " << endl 
			<< "\t" << argv[0]
			<< " <agents file>"
			<< endl;
		exit(0);
	}

	agent_list_t al = parseAgents(argv[1]);
	agent_list_t::iterator it;

	thread_list_t tl;
	int rc;

	for (it = al.begin(); it != al.end(); it++)
	{
  		pthread_t thread;

		// #define COMPILE_ERROR
		#ifdef COMPILE_ERROR
		rc = pthread_create(&thread, NULL, LocalAgent::run, NULL);
		#else
		rc = pthread_create(&thread, NULL, ThreadStartup, *it);
		#endif

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
