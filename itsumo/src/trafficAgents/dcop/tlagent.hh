#include "common.hh"

#include <iostream>
#include <fstream>

using namespace std;


//TLAgent contem o tratamento de dados fornecidos pelo simulador (info de estado),
//e contem a inteligencia de controle atraves da funcao 'newControlState'.

class TLAgent 
{
public:
	TLAgent(int id, string name, int memorySize, bool writeStat);
	~TLAgent();

	void updateState(simulationState* newState);	

	simulationState *getCurrentState(); 

	int getCurrentPlan();

	void print();

	string newControlState(int dcopalg, int coor);

	int tlightId;
	string tlightName;

	vector<signalPlan> signalPlans;

	int readDCOPResults(int type, int round);
private:
	
	string changeString_(signalPlan newSignalPlan);
	
	unsigned int _memorySize_;

	int _planTemp_;
	
   	vector<simulationState *> _states_;

	ofstream _outFile_;

	bool _writeStat_;
}; 
