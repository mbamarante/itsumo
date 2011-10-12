#ifndef OPLEARNER_HH
#define OPLEARNER_HH

#include "../../socket/ClientSocket.h"
#include "../../socket/SocketException.h"
#include "../qlearning/tlagent.hh"
#include "../qlearning/topologyReader.hh"
#include "Message.hh"
#include "DQTable.h"
#include "StringTokenizer.h"

#include <iostream>
#include <fstream>
#include <set>
#include <math.h>


//TLAgent contem o tratamento de dados fornecidos pelo simulador (info de estado),
//e contem a inteligencia de controle atraves da funcao 'newControlState'.

class OpLearner: public TLAgent{	
public:
	OpLearner(bool eps, unsigned int rwtf, double rwtfvalue, int id, int memorySize, int states, int actions, bool writeStat, double learning_r, double discount_r, double max_error);
	OpLearner(bool eps, int id, int memorySize, int states, int actions, bool writeStat, double learning_r, double discount_r, double max_error);
	~OpLearner();
	
	simulationState getCurrentState();
	int getSimpleState(); 
	int getSimpleStateExtend();
	virtual int myState();
	virtual double getReward(unsigned int rewardtype);	
	virtual string newControlState();	
	vector<OpLearner*> mypartners;
	void receiveMessages(ClientSocket* msg_socket);
	string sendMessages();
	void clearMessages();
	void getComposedState();	
	void getComposedState(int state);
	vector<SAComponent*> currentState;
	vector<SAComponent*> currentAction;
	vector<SAComponent*> lastState;
	vector<SAComponent*> lastAction;

	int getCurrentSingleAction();
	unsigned int getSingleStatesNumber();
	unsigned int getSingleActionsNumber();	
	
	double getCuriosity();
	double getLearningRate();
	double getDiscountRate();
	
	void endEpisode();
	
	void setLearning();
	//processa mensagens recebidas
	void pcsIncomingMsg(string msg);
	
	vector<SAComponent*> useReceivedInfoFor(vector<SAComponent*> toUpdate, unsigned int infoType);
	void observeLast();
	void observeNewState();
	void observe();
	void observe(double rw);
	void selectAction();
	
		
	bool callJointAction(vector<SAComponent*> action, double value);
	bool callInfo(vector<SAComponent*> state, unsigned int infotype, double value);		
	string analyzeLastEpisode();
	string analyzeLastEpisodePP();
	string askInfo(unsigned int to,unsigned int infotype);
	void acceptActionOffer(Message* msg);
	void acceptAllBestActionOffers();
	void acceptAllInformationRequests();
	vector<SAComponent*> collectInfo(unsigned int infotype);
	void commitAction();
		
	bool addToComunicationArea(int id);
	void setComunicationArea(set<int> commarea);
	void setNetwork(topologyReader* net);
	
	static const int INVALID_ID;
	static const unsigned int SIMPLE;
	static const unsigned int EXP;
	static const unsigned int COMPARATIVE;
	static const unsigned int THREESTEP;
	
	bool episodic;
	  
	

protected:	
		
	double _curiosity_;
	topologyReader* _network_;
	
private:
	
	vector<Message*> incomingMessages;
	vector<Message*> outgoingMessages;
	
	Message* getBestJointActionOffer();
	vector<Message*> acceptedActionMessages;
	
	set<int> communicationArea;
	int getClosestPartnerNotIn(vector<SAComponent*> sa);


	double getExpReward(double dec);
	double getSimpleReward(int maxstopped);
	double getComparativeReward();
	double getThreeStepReward(double maxavg);
	
	//OpJointQLearning::OpJointQLearning* _learning_;
	DQTable* _learning_;
	
	void init();
	
	
	
	unsigned int sentMessages;
	unsigned int perfomedJointActions;
	unsigned int usedJointStates;
	unsigned int episodes;
	unsigned int actSteps;	
	double _maximum_error_;
	double mySelectedActionValue;
	void constructComposedState();
	bool testConv;
	
	unsigned int _rewardfunction_;
	double _rewardfunction_value_;
	
	
	

}; 

#endif // OPLEARNER_HH

