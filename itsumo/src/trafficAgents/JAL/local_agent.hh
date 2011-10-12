#include "markov.hpp"
#include "../qlearning/tlagent.hh"
#include "../../socket/ClientSocket.h"
#include "../../socket/SocketException.h"

using namespace std;

//Agente Local: composto por um determinado numero de semaforos controlados, dado pelo vetor de TLAgent.
//A inteligencia de controle esta em TLAgent.

class LocalAgent{


public:
	LocalAgent(vector<int> tls, string name, bool stat,int memory, int states, double learning_r, double discount_r);
	~LocalAgent();

	void *run();

         
     /// Get the current context.
    int getContext( );
    
    /// Get the name of the current context.
    char* getContextName( int cont ) ; 

    /// Get the number of contexts.
    int getContextCount( );


    /// Get the number of agents.
    int getAgentCount( );


    /// Get the agent's current state in the environment.
    int getState( int _agent );
    
    /// Get number of states of the environment for each context.
    int getStateCount( );

    /// Get the agent's observation state in the environment. The state and the 
    /// observation are the same if the environment is fully observable.
    int getObservation( int _agent );

    /// Get number of observation states.
    int getObservationCount( );

    /// Apply the agent's action to the environment.
    bool act( int _agent, int _action );

    /// Perform the simulation.
    bool simulate( );

    /// Compute the reward for the agent's last action.
    double getReward( int _agent );

    /// Get number of possible actions.
    int getActionCount( );
	
    int setStateCount();
        

    /// Set the current state for the agent. Put it in a random state if -1.
    bool replace( int _agent, int _state=-1 ) ;

    /// Check if the agent is in a terminal state.
    bool testEnd( int _agent ); 

    /// Print the environment in the specified device.
    void print( FILE *_device );

    /// Print the policy.
    void printPolicy( FILE *_device, int *_policy ) ;

    /// Compute the overall performance of the agents in the environment.
    double getPerformance( ); 

    void setStartTime(int t);
    void  setTopology(topologyReader* n);
    
    void  setDirections();
	
	//do JAL:
	int getJointState(vector<int> sts);
	int getJointAction(vector<int> acs);
	int getAgentActionFromJointAction(int agentIndex,int jointAction);
	int _lastState_; //conjunto
	int _lastAction_; //conjunto
	markov::mdp* _mec_;  //conjunto

private:
    int     context;    ///< Context id. The context expresses hidden properties
                        ///< of the environment.
	int _totalStoppedCars_;
    int _totalInputLanes_;
	int _currentItsumoTimeStep_;
	int _start_time_; 

	//faz o parse da string de info de estado passada pelo simulador e divide entre os semaforos
	//responsaveis
	vector<simulationState> updateStates(string rawState);	

	//processa as mensagens de info de estado
	void pcsInfoMsg(std::string msg, vector<TLAgent*> &trafficLights, 
				ClientSocket* client_socket);

	//processa os pedidos de mudanca de controle
	void pcsRequestMsg(std::string msg, vector<TLAgent*> &trafficLights, 
				ClientSocket * client_socket);

	//interpreta a informacao de configuracao dos semaforos e cria os TLAgent
	vector <TLAgent*> parseTLights(std::string tlightConfig, int memorySize, int states);
	
	vector<TLAgent*> _tlights_; //vetor com os TLAgent
	string _connectMsg_; //string de conexao q deve ser enviada ao simulador
	string _name_;
	bool _stat_;
	int _memsize_;
    int _states_; //number of single states
    int _actions_; //number of single actions
    int _num_joint_states_;
    int _num_joint_actions_;
    int _numAgents_;
	double _learning_rate_;
	double _discount_rate_;
	
    vector<simulationState> _simulationStates_;
     
    topologyReader* _topology_;

	//learning conjunto:
	double lastAvgQueue;
	double getGlobalComparativeReward();
	double getTotalAvgStoped();
	markov::mdp* parseMech(int _stateCount, int _actionCount);
	void initLearning();

}; 
