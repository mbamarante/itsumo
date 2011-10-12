/*
 * leNet.h
 *
 *  Created on: 29/03/2010
 *      Author: tiago
 */

#ifndef LENET_H_
#define LENET_H_

#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include "../tiny/tinyxml.h"
using namespace std;

struct ints{
	int node;
	int cont;
};

struct vet{
	vector<int> v;
};

struct tllaneset{
	vet vetor;
	vet lanes;
	int nodeSource;
	int nodeDest;
	int id;
	int sectID;
	int streetID;
};

struct trafficLight{
	vector<tllaneset> laneset;
};

struct trafficRaito{
	vector<trafficLight> traff;
};

struct cruzamento{
	int nodeid;
	vector<int> idruas;
};

struct node{
	int id;
	float x;
	float y;
};

struct anode{
	node source;
	node dest;
};

struct traf{
	int id;
	vector<int> des;
};

struct traffic{
	vector<traf> nodes;
};

//Structs ordenadas hierarquicamente(do mais baixo ao mais alto);
//estruturas para montar uma rua;
struct lane{
	int lane_id;
};

struct laneset{
	int laneset_id;
	vector<lane> lanes;
	vector<int> destinations;
	//position - nao sei se precisa!!!
	//start node;
	int startNode;
	//end node;
	int endNode;
};

struct lanesets{
	vector<laneset> lanesets;
	//vector de laneset
};

struct section{
	int section_id;
	string section_name;
	vector<laneset> lsets;
};

struct street{
	int id;
	string name;
	vector<section> secti;
	traffic traf;
};

//fim das estruturas para a rua!!!

struct simulation{
	vector<node> nodes;
	vector<street> streets;
};
//traffic id 1807!! 1840!! 1815!! 1813!!

class leNet {
private:
	string caminho;
	string name;
	int numTL;
	vector<int> netIds;
	int id;
	int duracaoCiclo;

public:
	leNet();
	virtual ~leNet();
	TiXmlDocument loadFile(string name);
	simulation leRede(TiXmlDocument doc);

	void mostraStreet(vector<street> streets);
	void mostraDirections(vector<street> streets);
	void mostraNos(simulation s);
	int getMaiorId();

	bool findNo(int i, simulation s);
	node getNode(simulation s, int id);
	int getIndice(int i, simulation s);
	void geraTrafficLight(simulation s, TiXmlDocument doc);
	vector<ints> contaNodos(vector<trafficLight> tl);
	string geraTraffic(trafficRaito tl, int id, simulation s);

	bool mesmaStreet(int laneset, int street, int nodeId, int nodeS, trafficRaito tl);
	bool ladosStreet(int laneset, int street, int nodeS, trafficRaito tl);

	bool ehSource(vet pos, simulation s);

	int contaTLRaito(trafficRaito tl, int nodeID);

	void writeFile(TiXmlDocument doc, vector<trafficRaito> tl, vector<int> nos, simulation s);

	string getCaminho(string file);
	void setCaminho(string file);

	string getName();
	void setName(string name);

	int getID();
	void setID();

	int getDuracaoCiclo();
	void setDuracaoCiclo(int i);
};





#endif /* LENET_H_ */
