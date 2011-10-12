#include "../basic/tlagent.hh"
#include "../../basic/utils.hh"

#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;


//TLAgent contem o tratamento de dados fornecidos pelo simulador (info de estado),
//e contem a inteligencia de controle atraves da funcao 'newControlState'.

class randomTLAgent: public TLAgent
{
public:
	randomTLAgent(int id, int memorySize, bool writeStat);
	~randomTLAgent();
	virtual string newControlState();
}; 
