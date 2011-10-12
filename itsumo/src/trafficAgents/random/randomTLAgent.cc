#include "randomTLAgent.hh" 

randomTLAgent::randomTLAgent(int id, int memorySize, bool writeStat)
		:TLAgent(id, memorySize, writeStat)
{
 	/* initialize random seed */
  	srand ( time(NULL) );
}

randomTLAgent::~randomTLAgent()
{	
	_outFile_.close();
}

string randomTLAgent::newControlState()
{
	signalPlan changeTo;
	
	//choose random plan 	
	_planTemp_ = rand()%(signalPlans.size());

	changeTo = signalPlans[_planTemp_];
	//print iteration and plan to log;
	_outFile_<<intToString(_planTemp_)<<endl;
	
	return changeString_(changeTo);
}
	
