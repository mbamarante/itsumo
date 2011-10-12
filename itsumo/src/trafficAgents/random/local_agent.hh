#include "randomTLAgent.hh"

#include "../../socket/ClientSocket.h"
#include "../../socket/SocketException.h"


//Agente Local: composto por um determinado numero de semaforos controlados, dado pelo vetor de TLAgent.
//A inteligencia de controle esta em TLAgent.

class LocalAgent
{
public:
	LocalAgent(vector<int> tls, string name, bool stat);
	~LocalAgent();

	void *run();

protected:
	//faz o parse da string de info de estado passada pelo simulador e divide entre os semaforos
	//responsaveis
	vector<simulationState> updateStates(string rawState);	

private:
	//processa as mensagens de info de estado
	void pcsInfoMsg(std::string msg, vector<randomTLAgent*> &trafficLights, 
				ClientSocket* client_socket);

	//processa os pedidos de mudanca de controle
	void pcsRequestMsg(std::string msg, vector <randomTLAgent*> &trafficLights, 
				ClientSocket * client_socket);

	//interpreta a informacao de configuracao dos semaforos e cria os TLAgent
	vector <randomTLAgent*> parseTLights(std::string tlightConfig, int memorySize);

	vector<randomTLAgent*> _tlights_; //vetor com os TLAgent
	string _connectMsg_; //string de conexao q deve ser enviada ao simulador
	string _name_;
	bool _stat_;
}; 
