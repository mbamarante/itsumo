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
#ifndef __BRAESS_DRIVER_HH
#define __BRAESS_DRIVER_HH

#include "nasch.hh"
#include <ctime>
#include <cstdlib>
#define __CLASS__ Braess 

#define printp cerr		
#define print printp << meu_id << ":" << currentStep << ":" << rodada << ":" << caminho << ":" 
#define printm printp << ":" 

#define DRIVER_DECEL_RATE 0.1

#define OPD  0
#define OQD  1
#define OQPD 2

#define XO	2111
#define OP 	2211
#define OQ	2311
#define PD	2411
#define QD	2511
#define QP	2611

/* caminhos */
#define CXO	0
#define COP 	1
#define COQ	2
#define CPD	3
#define CQD	4
#define CQP	5


class Braess : public Nasch {
	private:
		int route();

	protected:
		int currentStep;
		bool escolhe_caminho;
		bool entra; /* uilizada no whentoinsert para fazer 
			       o meu_id%300 apenas na primeira vez 
			       q entra na topologia */
		bool paradox;
		bool floating_car;
		bool caminho_percorrido[6];

		int contador[6]; 
		int minha_laneset;
		int rodada;
		int caminho;
		int step_local_interface;

		id_t meu_id;

		float media_de_tempos[3];
		float probabilidades_das_rotas[3];
		float learn_prob;
		float delta;
		float tod[3];

	public:
		Braess(string driver_class_name);
		~Braess();

		bool readyIn(int step);
		int  where  (int step);
		void update (int step);

		lane_laneset_pair_t  chooseNextLane_Laneset(Car *);
                void set_options(vector < pair < string, string> > _options);


};

#endif
