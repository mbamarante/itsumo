#include "../basic/common.hh"

#include <iostream>
#include <fstream>

using namespace std;


class TLAgent {

public:
  TLAgent(int id, bool select, int memorySize, bool writeStat, int minPhase, float K);
  ~TLAgent();

  void updateState(simulationState newState);	

  simulationState getCurrentState(); 

  int getCurrentPlan();

  void print();

  string newControlState();

  int tlightId;

  vector<signalPlan> signalPlans;

private:
  int maxMeanStoppedCars();
  
  string changeString_(signalPlan newSignalPlan);
  signalPlan getSignalPlan(int);
  int getGreedyAction();
  double getDensityInLane(int);
  double getAvgDensityInLane(int);
	
  unsigned int _memorySize_;
  vector<simulationState > _states_;

  int _minPhase_;
  float _K_;

  bool _select_;
  bool _writeStat_;

  ofstream _outFile_;
 // ofstream _outFileAvg_;
  ofstream _outFilePhase_;
  int _contOut_;
}; 
