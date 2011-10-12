/*
    Copyright (C) 2010 by Ana Lucia Bazzan (bazzan@inf.ufrgs.br)
                          Alexander J. Benavides (ajbenavides@inf.ufrgs.br)
                          Tiago Sommer
            http://www.inf.ufrgs.br/maslab/traffic/itsumo/

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

#include "geradorOD.hh"

#define OBLIGATORY 0
#define OPTIONAL 1

#define CHECK(element, mode) (check(element, #element, mode))
#define VALUE(element) (element->FirstChild()->Value())

/*geradorOD::geradorOD(int i, string s, float fx, float fy){
	id=i;name=s;x=fx;y=fy;porcentagem=0;
}*/
geradorOD::geradorOD(int numV){numOdDrivers=numV;id=1;}

geradorOD::~geradorOD() {}

int geradorOD::getNumVehicles(){return numOdDrivers;}

//node getOrigem que retorna apenas o inteiro;
int geradorOD::getSource(){
	int source=0;
	int tam=nodes.size();
	srand(time(0));
	source=rand();
	srand(geraSeed(-source));
	source=rand()%tam;
	return nodes[source];
}

int geradorOD::getSink(int source){
	int sink=0;
	int tam=nodes.size();
	do{
		srand(time(0));
		sink=rand();
		srand(geraSeed(-sink));
		sink=rand()%tam;
	}while(sink==source);
	return nodes[sink];
}

int geradorOD::getSizeOdDrivers(){

	return od_drivers.size();
}

void geradorOD::addOdDriver(int source, int sink){
	od_driver v;
	v.id=id;
	id++;
	v.source=source;
	v.sink=sink;
	od_drivers.push_back(v);
}

matod geradorOD::getRandomOD(){
	int ran=0;
	srand(time(0));
	ran=rand();
	int seed=geraSeed(ran);
	srand(seed);
//	cout << " porc="<<getMatrixPorc()<<endl;
	ran=rand()%getMatrixPorc();
//	cout<<"Rand="<<ran<<endl;
	int sour=0;
	float p=0.0;
//	cout<<"Random="<<ran<<endl;
	for (unsigned int i = 0; i < matrixod.size(); i++) {
//		cout << "porc=" << sources[i].porc+p << endl;
//		cout << "porc=" <<matrixod[i].propor<<endl;
		if(ran<=matrixod[i].propor+p&&matrixod[i].propor!=0){
			sour=i;
			i=matrixod.size();
		}
		p=p+matrixod[i].propor;
//		cout<<"p:"<<p<<endl;
	}
//	cout<<"sour="<<matrixod[sour].x<<" sink="<<matrixod[sour].y<<endl;
//	cout << "src=" << getSourceID(sour) << endl;
	return matrixod[sour];
}

matod geradorOD::getMatrixAt(int i){return matrixod[i];}

void geradorOD::geraODM(string arquivo){
	ofstream file(arquivo.c_str());
	if(!file.is_open()){
		cerr<<"Error!!!";
		exit(1);
	}
	file<<"<od_drivers>"<<endl;
	for(unsigned int i=0;i<od_drivers.size();i++){
		file<<"\t<od_driver>"<<endl;
		file<<"\t\t<od_driver_id> "<<od_drivers[i].id<<" </od_driver_id>"<<endl;
		file<<"\t\t<source> "<<od_drivers[i].source<<" </source>"<<endl;
		file<<"\t\t<sink> "<<od_drivers[i].sink<<" </sink>"<<endl;
		file<<"\t</od_driver>"<<endl;
	}
	file<<"</od_drivers>"<<endl;
	file.close();
}

void geradorOD::medirPorcentagens(string f){
	ofstream file(f.c_str());
	vector<od_driver> oddrivers=od_drivers;
	int sourceA=0;
	int sinkA=0;
	int sourceB=0;
	int sinkB=0;
	int cont=0;
	int total=0;
	vector<int> s;
	vector<int> sin;
	vector<int> co;

	for(unsigned int i=0;i<io.size();i++){
		for (int j = 0; j < io.size();j++) {
			sourceA=io[i];sinkA=io[j];
			for(int k=0;k<oddrivers.size();k++){
				sourceB=oddrivers[k].source;
				sinkB=oddrivers[k].sink;
				if(sourceA==sourceB&&sinkA==sinkB){
					cont++;
				}
			}
			s.push_back(sourceA);
			sin.push_back(sinkA);
			co.push_back(cont);
			cont=0;
		}
	}


	for (int i = 0; i < co.size();i++) {
		if(i%(io.size())!=0||i==0)file<<co[i]<<" ";
		else {
			file<<endl;
			file<<co[i]<<" ";
		}
	}

	for(unsigned int i=0;i<co.size();i++){
		total=total+co[i];
	}
	file<<"Total sorteado="<<total<<endl;
}

vector<string> geradorOD::split(string s, string sep){
	vector<string> vecstr;
	string str="str";
	do{
		int i=0;
		i=s.find(sep);
		if(i==-1){
			str=s;
			s="";
		}
		else {str=s.substr(0, s.find(sep));
		//s=s.substr(str.length(), s.find(" "));
		s=s.substr(s.find(sep)+1);}
		vecstr.push_back(str);
	}while(s!="");
	return vecstr;
}

void geradorOD::seeSplit(vector<string> vecs){
	for(unsigned int i=0; i<vecs.size();i++){
		cout<<vecs[i]<<endl;
	}
}

void geradorOD::loadMatrix(string f){
	cout << "open file:" << f << endl;

	ifstream file (f.c_str());
	bool fim=false;
	vector<string> vecs;
	int colunas=-1;
	getline(file, f);
	vecs=split(f, " ");
	vector<double> veci;
//	cout<<vecs.size()<<endl;
	for(unsigned int j=0;j<vecs.size();j++){
//		cout << vecs[j]<<endl;
		veci.push_back(atoi(vecs[j].c_str()));
	}
	setIO(veci);
//	for (int i = 0; i < veci.size(); i++) {
//		cout << i << "=" << veci[i] << endl;
//	}
	vecs.clear();
	matod mod;
	while (!fim) {
		colunas++;
		getline(file, f);
		vecs = split(f, " ");
		if (colunas < vecs.size()) {
			for (unsigned int i = 0; i < vecs.size(); i++) {
				mod.source = io[colunas % io.size()];
				mod.sink = io[i];
				float ftemp = atof(vecs[i].c_str()); //pega o numero decimal
				mod.propor = (ftemp * 100); //converte implicitamente para inteiro (= warning!!!)
				matrixod.push_back(mod);
			}
		}
		if (colunas > vecs.size())
			fim = true;
	}
//	for (int i = 0; i < matrixod.size();i++) {
//		cout<<"x="<<matrixod[i].source<<" y="<<matrixod[i].sink<<" %"<< matrixod[i].propor<<endl;
//	}
}
//calculo utilizado para gerar randons mais "randomizados";
unsigned int geradorOD::geraSeed(int seed){
	{  static unsigned long long a = 2007, b = 4194301, c = 2147483647, z = b;
	    if ( seed < 0 ) {seed = -seed; a = seed;}
	    z = (a + b * z) % c;
	    return z % seed;
	}
}

//novo getNodos para pegar direto da network;
void geradorOD::getNodos(node_vector_t n){

	node_vector_t::iterator it;

	for (it=n.begin();it<n.end();it++){
		nodes.push_back( (*it)->getId() );
	}

//	for (unsigned int i = 0; i < n.size(); i++) {
//		nodes.push_back(n[i]->id);
//	}
}
void geradorOD::setIO(vector<double> v){io=v;}

void geradorOD::seeMultMatrix(){

	for (unsigned int i = 0; i < matrixod.size(); i++) {
		if(i%io.size()==0){cout<<matrixod[i].propor<<" "; cout<<endl;}
		else cout<<matrixod[i].propor<<" ";
	}
}

int geradorOD::getMatrixPorc(){
	int cont=0;
	for (unsigned int i = 0; i < matrixod.size(); i++) {
		cont=cont+matrixod[i].propor;
	}
	return cont;
}
