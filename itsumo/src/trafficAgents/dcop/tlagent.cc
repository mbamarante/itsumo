#include "tlagent.hh" 
#include "../../basic/utils.hh"


TLAgent::TLAgent(int id, string name, int memorySize, bool writeStat) 
{
	_planTemp_ = 0;
	_memorySize_ = memorySize;
	_writeStat_ = writeStat;
	tlightId = id;
	tlightName = name;

	string fileName = "output/" + intToString(id) + ".txt";
	_outFile_.open(fileName.c_str());
}

TLAgent::~TLAgent()
{	
	_outFile_.close();
}

void TLAgent::updateState(simulationState* newState) 
{
	if (_states_.size() >= _memorySize_)
		_states_.erase(_states_.begin());			

	_states_.push_back(newState);

	newState->print();

	
	if (_writeStat_)
	{
		string temp = newState->outline();
		_outFile_ << temp;
		_outFile_ << "\n";
	}
}


simulationState* TLAgent::getCurrentState() 
{
	if (_states_.size() > 0)
		return _states_[_states_.size()-1];
	else return NULL;
}


int TLAgent::getCurrentPlan()
{
	return signalPlans[getCurrentState()->n->curSignalPlan].signalPlanId;
}


void TLAgent::print() 
{
	cout << "tlight id " << tlightId << endl;
	vector<signalPlan>::iterator sp;
	for (sp = signalPlans.begin(); sp != signalPlans.end(); sp++) {
		cout << "\tsignalplan id " << (*sp).signalPlanId << endl;
		vector<phase>::iterator phase;
		for (phase = (*sp).phases.begin(); phase != (*sp).phases.end(); phase++) {
			cout << "\t\tphase id " << (*phase).phaseId << endl;
			cout << "\t\tstart " << (*phase).start << endl;
			cout << "\t\tend " << (*phase).end << endl;
			vector<direction>::iterator dir;
			for (dir = (*phase).directions.begin(); dir != (*phase).directions.end(); dir++) 
			{
				cout << "\t\t\tfrom lane " << (*dir).fromLane << endl;
				cout << "\t\t\tto laneset " << (*dir).toLaneset << endl;
			}
		}
	}
}


string TLAgent::newControlState(int dcopalg, int coor)
{
	signalPlan changeTo;

	simulationState* state = getCurrentState();

	cout << "new control state" << endl;

	if (coor == 1) {
		if (
		(tlightName == "A1") || (tlightName == "A3") || (tlightName == "A5") ||
		(tlightName == "A7") || (tlightName == "B2") || (tlightName == "B4") ||
		(tlightName == "B6") || (tlightName == "B8") || (tlightName == "C1") ||
		(tlightName == "C3") || (tlightName == "C5") || (tlightName == "C7") ||
		(tlightName == "C9") || (tlightName == "D2") || (tlightName == "D4") ||
		(tlightName == "D6") || (tlightName == "D8") || (tlightName == "E1") ||
		(tlightName == "E3") || (tlightName == "E5") || (tlightName == "E7") ||
		(tlightName == "E9") || (tlightName == "F2") || (tlightName == "F4") ||
		(tlightName == "F6") || (tlightName == "F8") || (tlightName == "G1") ||
		(tlightName == "G3") || (tlightName == "G5") || (tlightName == "G7") ||
		(tlightName == "G9") || (tlightName == "H2") || (tlightName == "H4") ||
		(tlightName == "H6") || (tlightName == "H8") || (tlightName == "I1") ||
		(tlightName == "I3") || (tlightName == "I5") || (tlightName == "I7") ||
		(tlightName == "I9") || (tlightName == "A9")
		) {
			_planTemp_ = 0;
		}
		else {
			_planTemp_ = 1;
		}
		changeTo = signalPlans[_planTemp_];
	} 
	else {
		//process dcop result and fill changeTo
		_planTemp_ = readDCOPResults(dcopalg, state->curIteration);
	}

	changeTo = signalPlans[_planTemp_];

	return changeString_(changeTo);
}
	

string TLAgent::changeString_(signalPlan newSignalPlan) 
{
	bool changeSomething = true;

	string message = "";
		
	/* Message format
	The message consists of the following fields, separeted by ";"
		[trafficLightId;
			 [signalPlanId;
			 	numberOfPhases;
					[phaseId;
				 	phaseStartIter;
				 	phaseEndIter;
	*/


	//values to change for
	if (changeSomething) {

	message += intToString(tlightId) + ";";

	message += intToString(newSignalPlan.signalPlanId) + ";";

	vector<phase> phases = newSignalPlan.phases;

	message += intToString(phases.size()) + ";";

	vector<phase>::iterator itPhase;

	for (itPhase = phases.begin(); itPhase != phases.end(); itPhase++)
	{
		message += intToString((*itPhase).phaseId) + ";";
		int startIter = (*itPhase).start;
		int endIter = (*itPhase).end;

		message += intToString(startIter) + ";";
		message += intToString(endIter) + ";";
	}//for phase
				
		

	}//if

	return message;
}

int TLAgent::readDCOPResults(int type, int round)
{
	int ans = 0;

	cout << "readdcopresult "<< type << "   "<< round << endl;

	if (type == 1)//adopt
	{
		FILE *fp = NULL;
                char buf[1024];
                char *p = NULL;
                char *p2 = NULL;
                int count = 0;

		char fileName[100];
		system("pwd");
		sprintf(fileName, "common/adopt_result_%d.txt", round);

                if ((fp = fopen(fileName, "r")) == NULL) {
                   cout << "Erro ao abrir o arquivo 1:  " << fileName << endl;
                   return 0;
                }

                if (fread(buf, sizeof(char), sizeof(buf), fp) <= 0) {
                   cout << "Erro ao abrir o arquivo 2:  " << fileName << endl;
                   fclose(fp);
                   return 0;
                }

                p = strtok(buf, "\n");

	        while ((strncasecmp(p, "Solution", 8) != 0))
		      p = strtok(NULL, "\n");

	        p = strtok(NULL, "\n");
	        p = strtok(NULL, "\n");

	        // Fazendo "parsing" do conteudo lido do arquivo de configuracao e carregando os valores
                while (p)
                {
                      if (*p != 'a')
                         break;
                      else
                      if (((p2 = strchr(p, (int) '=')) != NULL) && (*(++p2))) {
                                int signalplan = atoi(p2);

                                //mudar signal plan conforme lido
				if (count+901 == tlightId)
					ans = signalplan;
                      }
                      p = strtok(NULL, "\n");
                      count ++;

                }
                fclose(fp);
	}
	else if (type == 2) //dpop
	{
		FILE *fp = NULL;
	        char buf[1024];
	        char *p = NULL;
	        char *p2 = NULL;
                int count = 0;

		char fileName[100];
		sprintf(fileName, "common/dpop_result_%d.txt", round);

	        if ((fp = fopen(fileName, "r")) == NULL) {
		   cout << "Erro ao abrir o arquivo:  " << fileName << endl;
		   return -1;
	        }

	        if (fread(buf, sizeof(char), sizeof(buf), fp) <= 0) {
		   cout << "Erro ao abrir o arquivo:  " << fileName << endl;
		   fclose(fp);
		   return -1;
	        }

	        buf[0] = ' ';

	        p = strtok(buf, " ");

	        // Fazendo "parsing" do conteudo lido do arquivo de configuracao e carregando os valores
	        while (p)
	        {
                      if (*p == ']') {
			 //cout << "ACHEI O FIM DO DPOP, VOU SAIR!!!" << endl;
                         break; }
                      else
		      if (((p2 = strchr(p, (int) '=')) != NULL) && (*(++p2))) {
				int signalplan = atoi(p2);

				char* pval;
				
				p[p2-p-3] = '\0';

				int index = atoi(p);

				//mudar signal plan conforme lido
		                if (count+901 == tlightId)
					ans = signalplan;
		      }
		      p = strtok(NULL, " ");
		      count ++;

	       }
	       fclose(fp);
	}
	else if (type == 3)//optapo
	{
		FILE *fp = NULL;
                char buf[1024];
                char *p = NULL;
                char *p2 = NULL;

		char fileName[100];
		sprintf(fileName, "common/optapo_result_%d.txt", round);

                if ((fp = fopen(fileName, "r")) == NULL) {
                   cout << "Erro ao abrir o arquivo:  " << fileName << endl;
                   return -1;
                }

                if (fread(buf, sizeof(char), sizeof(buf), fp) <= 0) {
                   cout << "Erro ao abrir o arquivo:  " << fileName << endl;
                   fclose(fp);
                   return -1;
                }

                p = strtok(buf, "\n");

                // Fazendo "parsing" do conteudo lido do arquivo de configuracao e carregando os valores
                while (p)
                {
		      int signalplan;
		      int variable;

		      if (*p != 'V') // nao eh uma VARIABLE ou VALUE
                         break;

                      if (((p2 = strchr(p, (int) '=')) != NULL) && (*(++p2))) {
                                variable = atoi(p2) + 901;
                      }
                      p = strtok(NULL, "\n");
		      if (((p2 = strchr(p, (int) '=')) != NULL) && (*(++p2))) {
                                signalplan = atoi(p2);
                      }
                      p = strtok(NULL, "\n");

                      //mudar signal plan conforme lido
		      if (variable == tlightId)
		      	ans = signalplan;		
               }
               fclose(fp);
	}
	
	return ans;
}
