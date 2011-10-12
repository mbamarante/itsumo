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
	// params
	int dcopalg_ = 0;
	int dcopcoor_;
	double dcopalpha_;

	// gamma
	double gammasynch_;
	double gammaunsynch_;

	// tau
	double aa_;
	double an_;
	double na_;
	double nn_;

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

    TiXmlElement *alg; //***
    TiXmlElement *coor; //***
    TiXmlElement *alpha; //***
    TiXmlElement *gamma; //***
    	TiXmlElement *gamma2; //***
    TiXmlElement *tau; //***
    	TiXmlElement *tauup; //***
	TiXmlElement *taudown; //***

    TiXmlElement *tl;
    TiXmlElement *tls;

    for(agent = agents->FirstChildElement("agent");agent; agent = agent->NextSiblingElement("agent"))
    {
        LocalAgent* la;
	string temp;

        alg = agent->FirstChildElement("algorithm");
	temp = VALUE(alg);
        if (temp == "ADOPT")
           dcopalg_ = 1;
        else if (temp == "DPOP")
           dcopalg_ = 2;
        else if (temp == "OPTAPO")
           dcopalg_ = 3;

	cout << "ALG: " << temp << dcopalg_ << endl;

        coor = agent->FirstChildElement("coordneighbors");
	temp = VALUE(coor);
        if ( temp == "yes")
        	dcopcoor_ = 0;	
	else if (temp == "no")
        	dcopcoor_ = 1;

	cout << "COOR: " << temp << dcopcoor_ << endl;

        alpha = agent->FirstChildElement("alpha");
	cout << "ALPHA: " << VALUE(alpha) << endl;
        dcopalpha_ = atof(VALUE(alpha));

	cout << "ALPHA: " << VALUE(alpha) << dcopalpha_ << endl;

        gamma = agent->FirstChildElement("gamma");
        gamma2 = gamma->FirstChildElement("synchronized");
        gammasynch_ = atof(VALUE(gamma2));
        gamma2 = gamma->FirstChildElement("unsynchronized");
        gammaunsynch_ = atof(VALUE(gamma2));

	cout << "GAMMA: " << gammasynch_ << gammaunsynch_ << endl;

        tau = agent->FirstChildElement("tau");
        tauup = tau->FirstChildElement("up_agree");
        taudown = tauup->FirstChildElement("down_agree");
        aa_ = atof(VALUE(taudown));
        taudown = tauup->FirstChildElement("down_not_agree");
        an_ = atof(VALUE(taudown));
        tauup = tau->FirstChildElement("up_not_agree");
        taudown = tauup->FirstChildElement("down_agree");
        na_ = atof(VALUE(taudown));
        taudown = tauup->FirstChildElement("down_not_agree");
        nn_ = atof(VALUE(taudown));

	cout << "TAU: " << aa_ << an_ << na_ << nn_ << endl;

        name = agent->FirstChildElement("name");
        string agentName = VALUE(name);

        stat = agent->FirstChildElement("stat");
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

        la = new LocalAgent(ids, agentName, agentStat,
                     dcopalg_, dcopcoor_, dcopalpha_,
                     gammasynch_, gammaunsynch_,
                     aa_, an_, na_, nn_);

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
