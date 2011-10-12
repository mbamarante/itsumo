#include "topologyReader.hh"
#include "markov.hpp"
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <gsl/gsl_statistics.h>
using namespace std;


//TLAgent contem o tratamento de dados fornecidos pelo simulador (info de estado),
//e contem a inteligencia de controle atraves da funcao 'newControlState'.

class TLAgent{	
public:
	TLAgent(int id, unsigned int memorySize, int states, int numActions, bool writeStat, double learning_r, double discount_r);
	TLAgent();
	
	virtual ~TLAgent();


	

	void updateState(simulationState newState);		
	
	static const double MAXSTOPPED = 60.0; //must be defined according to the capacity of the streets
	
	simulationState getCurrentState(); 

	int getCurrentPlan();

	void print();

	

	int tlightId;

	vector<signalPlan> signalPlans;
	vector<TLAgent*> partners;
	nodeSimple* myNode;
	int nodesAtDirections[4];        
	
	int _lastAction_;
	int _lastState_;
	int _newState_;
	int _newAction_;
	double _lastReward_;
	double _newReward_;
	int _actionsNumber_;	

	double getAvgStoppedInLane(int laneId);
	vector<double> avgStoppedCarsWithoutMe;
	double getAvgLine();
	double getAvgStoppedOnNetwork();
	
	int getSimpleState();
	int getSimpleStateExtend();
	int getStateGlobal();
	int getLaneState(int sum);
	int getLaneState(double occup);
	
	//reward functions
	double getGlobalReward();
	double getGlobalRewardWithoutMe();
	double getSimpleReward();
	double getWLUSimpleReward();
	double getExpReward();
	double getExpRewardDualFunc();
	double getDifferenceReward();
	double getComparativeReward();
	double getThreeStepReward();
	
	signalPlan getSignalPlan(int planId);
	int getStoppedInLane(int laneId);
	double getDensityInLane(int laneId);	
	int getGreedyAction(); 
	void addTotalStoppedCarsOnNetwork(int totalSC); 
	void setTotalInputLanesOnNetwork(int totalIL);
	void setNetwork(topologyReader* n);
	string changeString_(signalPlan newSignalPlan);		
	markov::mdp* _mec_;
		
	virtual int getJointState();
	virtual int myState();
	virtual int getState();
	virtual double getReward();
	virtual string newControlState();

	
protected:
	
	unsigned int _memorySize_;
	int _statesNumber_;
	double _learning_rate_;
	double _discount_rate_;

	vector<int> totalStoppedCarsOnNetwork;
	int totalInputLanesOnNetwork;
	
	topologyReader* _network_;
	
	int _planTemp_;

   	vector<simulationState > _states_;
   	
   	ofstream _outFile2_;

	bool _writeStat_;
	virtual void init();
	
	double lastAvgQueue;
	
	double getAvgFlowInLane(int laneId);
	double getAvgSatDegInLane(int laneId);
	double getAvgSpeed();
	double getAvgSatDegree();
	
private:		
	markov::mdp* parseMech( std::string _mechName, int _stateCount, int _actionCount);
	
		

}; 
