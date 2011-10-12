/*
    Copyright (C) 2005 by Ana Lucia Bazzan (bazzan@inf.ufrgs.br)
                          Bruno Castro da Silva (bcs@inf.ufrgs.br)
                          Diogo Becker de Brum (dbrum@inf.ufrgs.br)
            http://www.inf.ufrgs.br/~mas/traffic/siscot

    This file is part of Itsumo.

    Itsumo is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Itsumo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Itsumo; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/
#include "braess.hh"


void __CLASS__::set_options(vector < pair < string, string> > _options)
{
	for (vector < pair <string,string> >::iterator i = _options.begin(); i != _options.end();i++)
	{
	  if ((*i).first == "deceleration") setDeceleration(atoi((*i).second.c_str()));
	  if ((*i).first == "learn_delta")  delta = atof((*i).second.c_str());
	  if ((*i).first == "learn_prob")   learn_prob = atof((*i).second.c_str());
	  if ((*i).first == "paradox")      paradox = (atoi((*i).second.c_str()) == 1? true: false);
	  if ((*i).first == "prob_oqpd")    probabilidades_das_rotas[OQPD] = atof((*i).second.c_str());
	  if ((*i).first == "prob_oqd")     probabilidades_das_rotas[OQD]  = atof((*i).second.c_str());
	  if ((*i).first == "prob_opd")     probabilidades_das_rotas[OPD]  = atof((*i).second.c_str());
	  if ((*i).first == "fc_oqpd")      if(atof((*i).second.c_str()) == meu_id){
	    floating_car = true;
	    caminho = OQPD;}
	  if ((*i).first == "fc_oqd")       if(atof((*i).second.c_str()) == meu_id){
	    floating_car = true;
	    caminho = OQD;}
	  if ((*i).first == "fc_opd")       if(atof((*i).second.c_str()) == meu_id){
	    floating_car = true;
	    caminho = OPD;}
	}

}



__CLASS__::__CLASS__(string driver_class_name) : Nasch(driver_class_name) {
	srand(time(0));
	
	meu_id = getId();
	caminho = -1;
	// contador de passos deve ser zerado
	// \gambiarra { deve comecar em -1 para mostrar o tempo certo }
	contador[COP]  = -1;
	contador[COQ]  = -1;
	contador[CPD]  = -1;
	contador[CQD]  = -1;
	contador[CQP]  = -1;

	rodada = 0;							
	caminho_percorrido[CXO] = false;

	floating_car = false;
	step_local_interface = -1;

	entra = false;

}

__CLASS__::~__CLASS__() {;}

bool __CLASS__::readyIn(int step) {
	return true;
}

int __CLASS__::where(int step) {
	return XO;
}

void __CLASS__::update(int step) {
	currentStep = step;
	step_local_interface++;

	switch(minha_laneset){
		case XO:
			break;
		case OP:
			contador[COP]++;
			break;
		case OQ:
			contador[COQ]++;
			break;
		case PD:
			contador[CPD]++;
			break;
		case QD:
			contador[CQD]++;
			break;
		case QP:
			contador[CQP]++;
			break;
	}

}

lane_laneset_pair_t __CLASS__::chooseNextLane_Laneset(Car *theCar)
{
	LaneSet * current_laneset = 0;
	int next_laneset_id = 0;
	lane_laneset_pair_t next_lane_laneset; 
	bool needLane = false;
	
	// get current LaneSet ID.
	current_laneset = theCar->getCurrentLane()->getParentLaneSet();
	minha_laneset = current_laneset->getId();

	// get next LaneSet.
	switch(current_laneset->getId()) {
		case XO:
			entra = false; // entrou a primeira vez
			if (caminho_percorrido[CXO] == false){
				// Imprime relatorio da iteracao anterior
				if(rodada) switch(caminho){
					case OPD:
						print	<< "TempoRodada:" 
							<< contador[COP]+contador[CPD]
							<< ":" << contador[COP]
							<< ":" << contador[CPD]
							<< endl;
						break;
					case OQD:
						print 	<< "TempoRodada:"
							<< contador[COQ]+contador[CQD]
							<< ":" << contador[COQ]
							<< ":" << contador[CQD]
							<< endl;
						break;
					case OQPD: 
						print 	<< "TempoRodada:"
							<< contador[COQ]+contador[CQP]+contador[CPD]
							<< ":" << contador[COQ]
							<< ":" << contador[CQP]
							<< ":" <<contador[CPD]
							<< endl;
						break;
					default:
						;
						//print << "LIMBO" << currentStep << ":" 
						//	<< currentLaneSetLocal << ":" << carCurrentStateRef->lane << endl;
				}

				// Escolhe o caminho
				caminho = route(); 
				switch(caminho){
					case OPD: 
						next_laneset_id = OP;
						break;
					case OQD:
						next_laneset_id = OQ;
						break;
					case OQPD: 
						next_laneset_id = OQ;
						break;
					default:
						print << "Caminho desconhecido :" << caminho << endl;
						exit(0);
				}

				// Registra que ja passou por esse link 
				caminho_percorrido[CXO] = true;

				// Limpa os proximos
				caminho_percorrido[COP] = false;
				caminho_percorrido[COQ] = false;
				caminho_percorrido[CQD] = false;
				caminho_percorrido[CQP] = false;
				caminho_percorrido[CPD] = false;
				rodada++;
				// Precisa escolher uma lane
				needLane = true;
			} else {
				;//print << " BUG 1" << endl;
			}
			switch(caminho){
				case OPD:
					next_laneset_id = OP;
					break;
				case OQD:
					next_laneset_id = OQ;
					break;
				case OQPD:
					next_laneset_id = OQ;
					break;
				default:
					print << "Caminho desconhecido :" << caminho << endl;
					exit(0);
			}
			break;

		case OP:			
			if (caminho_percorrido[COP] == false){
				// Registra que ja passou por esse link
				caminho_percorrido[COP] = true;

				// Limpa os proximos
				caminho_percorrido[CPD] = false;

				// Precisa escolher uma lane
				needLane = true;

				// Limpa contador
				contador[COP] = -1;

				// Imprime Caminho
			} else {
				;//print << " BUG 2" << endl;
			}
			// Escolhe o caminho
			next_laneset_id = PD;
			break;

		case OQ:
			if (caminho_percorrido[COQ] == false){
				// Registra que ja passou por esse link 
				caminho_percorrido[COQ] = true;

				// Limpa os proximos
				caminho_percorrido[CQD] = false;
				caminho_percorrido[CQP] = false;

				// Precisa escolher uma lane
				needLane = true;

				// Limpa contador
				contador[COQ] = -1;
			} else {
				;//print << " BUG 3" << endl;
			}
			// Escolhe o caminho
			switch(caminho){
				case OQD: 
					// Imprime Caminho
					//print << "OQD" << endl;
					next_laneset_id = QD;
					break;
				case OQPD:															
					// Imprime Caminho
					//print << "OQPD" << endl;
					next_laneset_id = QP;
					break;
			}
			break;

		case PD:
			if (caminho_percorrido[CPD] == false){
				// Registra que ja passou por esse link 
				caminho_percorrido[CPD] = true;

				// Limpa os proximos
				caminho_percorrido[CXO] = false;

				// Precisa escolher uma lane
				needLane = true;

				// Limpa contador
				contador[CPD] = -1;
			} else {
				;//print << " BUG 4" << endl;
			}

			// Escolhe o caminho
			next_laneset_id = XO;

			break;

		case QD:			
			if (caminho_percorrido[CQD] == false){
				// Registra que ja passou por esse link 
				caminho_percorrido[CQD] = true;

				// Limpa os proximos
				caminho_percorrido[CXO] = false;

				// Precisa escolher uma lane
				needLane = true;

				// Limpa contador
				contador[CQD] = -1;
			} else {
				;//print << " BUG 5" << endl;
			}
			// Escolhe o caminho
			next_laneset_id = XO;
			break;

		case QP:
			if (caminho_percorrido[CQP] == false){
				// Registra que ja passou por esse link 
				caminho_percorrido[CQP] = true;

				// Limpa os proximos
				caminho_percorrido[CPD] = false;

				// Precisa escolher uma lane
				needLane = true;

				// Limpa contador
				contador[CQP] = -1;
			} else {
				;//print << " BUG 6" << endl;
			}
			// Escolhe o caminho
			next_laneset_id = PD;
			break;
		default:
			next_laneset_id = 0 ; //getRandomLaneSet(currentLaneSetLocal);
			break;
	}

	LaneSet *destinationLaneset = static_cast<LaneSet *>(current_laneset->getElementById(next_laneset_id));
  	lane_vector_t possibleLanes = destinationLaneset->getChildLanes();

  	long int randomLane = randomGen.IRandom(0, possibleLanes.size()-1);

  	return make_pair(possibleLanes[randomLane], destinationLaneset);
}

int __CLASS__::route()
{	
	float tempo_ultima;
	float somatorio_inv_tempos;
	float roleta[3];


	if (floating_car) return caminho;	

	if ( ! rodada ) { // primeira rodada
		for (int i=0; i<3; i++)
			tod[i] = media_de_tempos[i] = 1;
	}


	if  (((rand()/(RAND_MAX+1.0)) <= learn_prob) && rodada) {

		// tod = tempo que o motorista "acha" que levou
		if (caminho == OQPD) {
			tempo_ultima = static_cast<float>(contador[COQ]+contador[CQP]+contador[CPD]);
			tod[OQPD] = (media_de_tempos[OQPD] * (1 - delta)) + (tempo_ultima * delta);
		} else {
			if (caminho == OPD) {
				tempo_ultima = static_cast<float>(contador[COP]+contador[CPD]);
				tod[OPD] = (media_de_tempos[OPD] * (1 - delta)) + (tempo_ultima * delta);
			} else {
				tempo_ultima = static_cast<float>(contador[COQ]+contador[CQD]);
				tod[OQD] = (media_de_tempos[OQD] * (1 - delta)) + (tempo_ultima * delta);
			}
		}
		if (paradox)
			somatorio_inv_tempos = 1/tod[OPD] + 1/tod[OQD] + 1/tod[OQPD];
		else
			somatorio_inv_tempos = 1/tod[OPD] + 1/tod[OQD];

		probabilidades_das_rotas[OPD]  = (1/tod[OPD]) / somatorio_inv_tempos;
		probabilidades_das_rotas[OQD]  = (1/tod[OQD]) / somatorio_inv_tempos;
		probabilidades_das_rotas[OQPD] = (1/tod[OQPD])/ somatorio_inv_tempos; 

		// atualiza a media de tempos sem precisar guardar todos os valores anteriores
		if (rodada >=2)
			media_de_tempos[caminho] = (((rodada-1.0)*media_de_tempos[caminho])+tempo_ultima)/(static_cast<float>(rodada));
		else 
			media_de_tempos[caminho] = tempo_ultima;
	}






	if (rodada)
		print << "Probabilidade:" << probabilidades_das_rotas[OQPD] << endl;					
	roleta[OPD] = probabilidades_das_rotas[OPD];
	roleta[OQD] = roleta[OPD] + probabilidades_das_rotas[OQD];
	roleta[OQPD] = roleta[OQD] + probabilidades_das_rotas[OQPD];

	float rota = rand()/(RAND_MAX+1.0);
	if (rota <= roleta[OPD])
		return OPD;
	else {
		if ((rota <= roleta[OQD]) || !paradox)
			return OQD;
		else
			return OQPD;
	}

} /* ! route() */




