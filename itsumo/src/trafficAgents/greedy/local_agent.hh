#include "tlagent.hh"

#include "../../socket/ClientSocket.h"
#include "../../socket/SocketException.h"


//Agente Local: composto por um determinado numero de semaforos controlados, dado pelo vetor de TLAgent.
//A inteligencia de controle esta em TLAgent.

class LocalAgent
{
public:
  LocalAgent(vector<int> tls, string name, bool select, bool stat, int memSize, int minPhase, float K);
  ~LocalAgent();

  void *run();
  void setStartTime(int t);
  

private:
  //faz o parse da string de info de estado passada pelo simulador e divide entre os semaforos
  //responsaveis
  vector<simulationState> updateStates(string rawState);	

  //processa as mensagens de info de estado
  void pcsInfoMsg(std::string msg, vector<TLAgent*> &trafficLights, 
                  ClientSocket* client_socket);

  //processa os pedidos de mudanca de controle
  void pcsRequestMsg(std::string msg, vector <TLAgent*> &trafficLights, 
                     ClientSocket * client_socket);

  //interpreta a informacao de configuracao dos semaforos e cria os TLAgent
  vector <TLAgent*> parseTLights(std::string tlightConfig);

  vector<TLAgent*> _tlights_; //vetor com os TLAgent
  string _connectMsg_; //string de conexao q deve ser enviada ao simulador
  string _name_;
  bool _select_;
  bool _stat_;
 int _start_time_;

  int _memSize_;
  int _minPhase_;
  float _K_;
}; 
