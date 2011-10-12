#ifndef LOCAL_AGENT_HH
#define LOCAL_AGENT_HH
#include <algorithm>
#include <queue>

#include "OpLearner.hh"
#include "util.h"

#include "../../socket/ClientSocket.h"
#include "../../socket/ServerSocket.h"
#include "../../socket/SocketException.h"


using namespace std;
//Agente Local: composto por um determinado numero de semaforos controlados, dado pelo vetor de OpLearner.
//A inteligencia de controle esta em OpLearner.

class LocalAgent{
public:
	LocalAgent( vector<int> tls, int rwfunction,	double rwfunctionvalue, string name, bool stat,int memory, int states, double learning_r, double discount_r, set<int> commArea, double errorTh);
	LocalAgent( string name, int id, bool stat, int states, int actions, double learning_r, double discount_r, set<int> commArea, double errorTh);

	~LocalAgent();
	
	string _name_;
	
	void *run();

	void endEpisode();
    void setStartTime(int t);
    void  setTopology(topologyReader* n);
    
    void  setDirections();
	ClientSocket*  connectToITSUMO();
	ClientSocket*  connectToMSGServer();
	
	void analyzeLastEpisode();
	void analyzeLastEpisodePP();
	void analyzeNewState();
	void analyzeNewState(int state);

	void evaluateActionOffers();
	void commitAction();
	
	unsigned int getAction();
	
	void sendMessages();
	
	void receiveMessages();
	
	void replyRequests();

	void observe();
	void observe(double rw);
	void observeNewState();
	void observeLast();
	
	//processa as mensagens de info de estado
	void pcsInfoMsg(std::string msg);

	string pcsPPMsg(int s, double rw);
	
	//processa os pedidos de mudanca de controle
	void pcsRequestMsg(std::string msg);
	
	void selectAction();
	
	ClientSocket* client_socket;	
	ClientSocket* msg_client_socket;
	
	int getTimeStep();
	int getStartTime();
	
	
		
private:
	int _totalStoppedCars_;
    int _totalInputLanes_;
 	int _currentItsumoTimeStep_;
	int _start_time_;
	int _rw_function_;
	double _rw_function_parameter_value_;

	//faz o parse da string de info de estado passada pelo simulador e divide entre os semaforos
	//responsaveis
	vector<simulationState> updateStates(string rawState);	
	
	void redirectMessages(string messages);
	
	
	string collectMessagesFromAgents();
	


	//interpreta a informacao de configuracao dos semaforos e cria os OpLearner
	vector <OpLearner*> parseTLights(std::string tlightConfig, int memorySize, int states);
	
	//cria os agentes predadores
	vector <OpLearner *> parsePredators(int states, int actions, int id);
	
	void addTLpartners(unsigned int distance);
	
	vector<OpLearner*> _tlights_; //vetor com os OpLearner
	string _connectMsg_; //string de conexao q deve ser enviada ao simulador 

	bool _stat_;
	int _memsize_;
    int _states_;
	double _learning_rate_;
	double _discount_rate_;
	double _error_threshold_;
	set<int> _communicationArea_;
	
    vector<simulationState> _simulationStates_;
    topologyReader* _topology_;

};
 
#endif//LOCAL_AGENT_HH
