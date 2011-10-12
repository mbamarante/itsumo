#include "common.hh"

#include <iostream>
#include <fstream>

using namespace std;


//TLAgent contem o tratamento de dados fornecidos pelo simulador (info de estado),
//e contem a inteligencia de controle atraves da funcao 'newControlState'.

class TLAgent 
{
public:
	TLAgent(int id, int memorySize, bool writeStat);
	~TLAgent();

	void updateState(simulationState newState);	

	simulationState getCurrentState(); 

	int getCurrentPlan();

	void print();	

	int tlightId;

	vector<signalPlan> signalPlans;
	
	virtual string newControlState();
	
protected:

	string changeString_(signalPlan newSignalPlan);
	
	unsigned int _memorySize_;

	int _planTemp_;
	
   	vector<simulationState> _states_;

	ofstream _outFile_;

	bool _writeStat_;
}; 
