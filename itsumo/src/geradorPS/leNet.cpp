/*
 * leNet.cpp
 *
 *  Created on: 29/03/2010
 *      Author: tiago
 */

#include "leNet.h"

#define CHECK(element, mode) (check(element, #element, mode))
#define VALUE(element) (element->FirstChild()->Value())

leNet::leNet() {
	// TODO Auto-generated constructor stub
	netIds.push_back(0);
}

leNet::~leNet() {
	// TODO Auto-generated destructor stub
}

void leNet::setID(){id=getMaiorId();}

int leNet::getID(){
	id++;
	return id;
}

void leNet::setDuracaoCiclo(int i){duracaoCiclo=i;}

int leNet::getDuracaoCiclo(){return duracaoCiclo;}

TiXmlDocument leNet::loadFile(string name){
	TiXmlDocument doc(name.c_str());
	bool loadOkay = doc.LoadFile();
	if(loadOkay){
		return doc;
	}else return NULL;
}

bool leNet::mesmaStreet(int laneset, int street, int nodeId, int nodeS, trafficRaito tl){
	bool b=false;
	bool aux=false;
	for(unsigned int i=0;i<tl.traff.size();i++){
		for(unsigned int j=0;j<tl.traff[i].laneset.size();j++){
//			int lanesetid=tl.traff[i].laneset[j].id;
			if(laneset==tl.traff[i].laneset[j].id){
				aux=true;
				if(nodeS!=tl.traff[i].laneset[j].nodeDest)b=true;
			}
		}
	}
	if(!aux)b=true;
	return b;
}


int leNet::contaTLRaito(trafficRaito tl, int nodeID){
	int cont=0;
	for(unsigned int i=0;i<tl.traff.size();i++){
		int no=tl.traff[i].laneset[0].nodeDest;
		if(no==nodeID)cont++;
		else if(tl.traff[i].laneset.size()>1){
			no=tl.traff[i].laneset[1].nodeDest;
			if(no==nodeID)cont++;
		}
	}
	if(cont==0)return 1;
	else return cont;
}

simulation leNet::leRede(TiXmlDocument doc){
	TiXmlElement *xml_simulation=doc.FirstChildElement("simulation");
	TiXmlElement *nos=xml_simulation->FirstChildElement("nodes");
	TiXmlElement *sources=nos->NextSiblingElement("sources");
	TiXmlElement *streetsX=xml_simulation->FirstChildElement("streets");
	TiXmlElement *source;
	vector<int> src;
	for(source=sources->FirstChildElement("source");source;source=source->NextSiblingElement("source")){
		TiXmlElement *source_id=source->FirstChildElement("source_id");
		TiXmlElement *located=source->FirstChildElement("located_at_node");
		src.push_back(atoi(VALUE(located)));
		netIds.push_back(atoi(VALUE(source_id)));
	}
	TiXmlElement *sinks=nos->NextSiblingElement("sinks");
	TiXmlElement *sink;
	for(sink=sinks->FirstChildElement("sink");sink;sink=sink->NextSiblingElement("sink")){
		TiXmlElement *sink_id=sink->FirstChildElement("sink_id");
		netIds.push_back(atoi(VALUE(sink_id)));
	}
	TiXmlElement *xml_node;
	TiXmlElement *streetX;
	vector<int> destination;
	vector<street> streets;
	vector<node> nodes;
	street str;
	int numN=0;
	for(xml_node=nos->FirstChildElement("node");xml_node;xml_node=xml_node->NextSiblingElement("node")){
		node n;
		numN++;
		TiXmlElement *node_id=xml_node->FirstChildElement("node_id");
		bool boo=false;
		for(unsigned int a=0;a<src.size();a++){
			if(atoi(VALUE(node_id))==src[a])boo=true;
		}
		if(!boo){
			n.id=atoi(VALUE(node_id));
			TiXmlElement *x=xml_node->FirstChildElement("x_coord");
			n.x=atof(VALUE(x));
			TiXmlElement *y=xml_node->FirstChildElement("y_coord");
			n.y=atof(VALUE(y));
			nodes.push_back(n);
			netIds.push_back(atoi(VALUE(node_id)));
		}
	}
	int numS=0;
	for(streetX=streetsX->FirstChildElement("street");streetX;streetX=streetX->NextSiblingElement("street")){
		numS++;
		TiXmlElement *street_id=streetX->FirstChildElement("street_id");
		netIds.push_back(atoi(VALUE(street_id)));
		TiXmlElement *street_name=streetX->FirstChildElement("street_name");
		str.id=atoi(VALUE(street_id));
		str.name=VALUE(street_name);
		TiXmlElement *sections=streetX->FirstChildElement("sections");
		TiXmlElement *sectionX;
		section sec;
		//cout<<"street_id:"<<VALUE(street_id)<<" street_name:"<<VALUE(street_name)<<endl;
		for(sectionX=sections->FirstChildElement("section");sectionX;sectionX=sectionX->NextSiblingElement("section")){
			vector<laneset> lsets;
			TiXmlElement *section_id=sectionX->FirstChildElement("section_id");
			netIds.push_back(atoi(VALUE(section_id)));
			sec.section_id=atoi(VALUE(section_id));
			TiXmlElement *lanesetsX=sectionX->FirstChildElement("lanesets");
			TiXmlElement *directions;
			TiXmlElement *lanesetX;
			for(lanesetX=lanesetsX->FirstChildElement("laneset");lanesetX;lanesetX=lanesetX->NextSiblingElement("laneset")){
				laneset lset;
				destination.clear();
				//bool b=false;
				TiXmlElement *turning_probabilities=lanesetX->FirstChildElement("turning_probabilities");
				TiXmlElement *laneset_id=lanesetX->FirstChildElement("laneset_id");
				netIds.push_back(atoi(VALUE(laneset_id)));
				TiXmlElement *start_node=lanesetX->FirstChildElement("start_node");
				TiXmlElement *end_node=lanesetX->FirstChildElement("end_node");
				int end=atoi(VALUE(end_node));
				int start=atoi(VALUE(start_node));
				lset.laneset_id=atoi(VALUE(laneset_id));
				lset.endNode=end;
				lset.startNode=start;
				TiXmlElement *lanes;
				for (directions=turning_probabilities->FirstChildElement("direction"); directions; directions=directions->NextSiblingElement("direction")) {
					float tur=atof(VALUE(directions->FirstChildElement("probability")));
					if(tur!=0)destination.push_back(atoi(VALUE(directions->FirstChildElement("destination_laneset"))));
				}
				lset.destinations=destination;
				for(lanes=lanesetX->FirstChildElement("lanes");lanes;lanes=lanes->NextSiblingElement("lanes")){
					TiXmlElement *laneX;
					for(laneX=lanes->FirstChildElement("lane");laneX;laneX=laneX->NextSiblingElement("lane")){
						lane l;
						TiXmlElement *lane_id=laneX->FirstChildElement("lane_id");
						netIds.push_back(atoi(VALUE(lane_id)));
						l.lane_id=atoi(VALUE(lane_id));
						lset.lanes.push_back(l);
					}
					lsets.push_back(lset);
				}
				sec.lsets=lsets;
			}
			str.secti.push_back(sec);
			sec.lsets.clear();
			//str.traf=tra;
		}
		streets.push_back(str);
		str.secti.clear();
	}
	simulation s;
	s.nodes=nodes;
	s.streets=streets;
	setID();
	cout<<"Numero de Streets: "<<numS<<endl;
	cout<<"Numero de Nodos: "<<numN<<endl;
	return s;
}

int leNet::getMaiorId(){
	int max=netIds[0];
	for (unsigned int i=1;i<netIds.size();i++){
		if(max<netIds[i])max=netIds[i];
	}
	return max;
}

bool leNet::findNo(int i, simulation s){
	bool b=false;
	for(unsigned int a=0;a<s.nodes.size();a++){
		if(i==s.nodes[a].id){
			b=true;
			a=s.nodes.size();
		}
	}
	return b;
}

bool leNet::ehSource(vet pos, simulation s){
	if(findNo(pos.v[0], s)&&findNo(pos.v[1], s))return false;
	else return true;
}

void leNet::setName(string na){
	string str=na;
	str=str.substr(str.find_last_of("/")+1);
	str=str.substr(0,str.find("."));
	name=str;
}

string leNet::getName(){
	string str=name+"WTL.xml";
	return str;
}

string leNet::getCaminho(string file){
	string str=caminho;
	str=str.substr(0,str.find_last_of("/"));
	str=str+"/"+file;
	return str;
}

void leNet::setCaminho(string file){
	caminho=file;
}

void leNet::writeFile(TiXmlDocument doc, vector<trafficRaito> tl, vector<int> id, simulation s){
	ofstream file(getCaminho(getName()).c_str());
	ifstream fileOri(getCaminho(name+".xml").c_str());
	string strs="as";
	bool b1=false;
	numTL=0;
	do{
		if(strs.find("<streets")!=-1 && b1)file<<strs<<endl;
		getline(fileOri,strs);
		if(strs.find("<traffic_lights>")==-1){
			file<<strs<<endl;
		}
		else {
			file<<"\t<traffic_lights>"<<endl;
			for(unsigned int i=0;i<tl.size();i++){
				file<<geraTraffic(tl[i], id[i], s);
			}
			file<<"\t</traffic_lights>"<<endl;
			int intei=0;
			do{
				getline(fileOri,strs);
				if((strs.find("traffic_lights>")!=-1)){
					intei=1;
				}else if((strs.find("<streets")!=-1)){
					b1=true;
					intei=1;
				}
			}while(intei==0);
		}
	}while(strs!="");
	cout<<"numTL: "<<numTL<<endl;
	/*file<<"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"<<endl;
	TiXmlElement *xml_simulation=doc.FirstChildElement("simulation");
	file<<"<simulation>"<<endl;
	TiXmlElement *aux=xml_simulation->FirstChildElement("network_id");
	file<<"\t<network_id> "<<atoi(VALUE(aux))<<" </network_id>"<<endl;
	aux=xml_simulation->FirstChildElement("network_name");
	file<<"\t<network_name> "<<atoi(VALUE(aux))<< " </network_name>"<<endl;
	TiXmlElement *bounds=xml_simulation->FirstChildElement("bounds");
	if(bounds!=NULL){
		file<<"\t<bounds>"<<endl;
		TiXmlElement *util=bounds->FirstChildElement("latmax");
		file<<"\t\t<latmax> "<<VALUE(util)<<" </latmax>"<<endl;
		util=bounds->FirstChildElement("lngmax");
		file<<"\t\t<lngmax> "<<VALUE(util)<<" </lngmax>"<<endl;
		util=bounds->FirstChildElement("latmin");
		file<<"\t\t<latmin> "<<VALUE(util)<<" </latmin>"<<endl;
		util=bounds->FirstChildElement("lngmin");
		file<<"\t\t<lngmin> "<<VALUE(util)<<" </lngmin>"<<endl;
		file<<"\t</bounds>"<<endl;
	}
	TiXmlElement *nos=xml_simulation->FirstChildElement("nodes");
	TiXmlElement *no;
	TiXmlElement *sources=nos->NextSiblingElement("sources");
	TiXmlElement *streetsX=xml_simulation->FirstChildElement("streets");
	TiXmlElement *streetx;
	TiXmlElement *source;
	TiXmlElement *sinks=sources->NextSiblingElement("sinks");
	TiXmlElement *sink;
	file<<"\t<nodes>"<<endl;
	for(no=nos->FirstChildElement("node");no;no=no->NextSiblingElement("node")){
		int id=atoi(VALUE(no->FirstChildElement("node_id")));
		const char *name=(VALUE(no->FirstChildElement("node_name")));
		float xcoor=atof(VALUE(no->FirstChildElement("x_coord")));
		float ycoor=atof(VALUE(no->FirstChildElement("y_coord")));
		file<<"\t\t<node>"<<endl;
		file<<"\t\t\t<node_id> "<<id<<" </node_id>"<<endl;
		file<<"\t\t\t<node_name> "<<name<<" </node_name>"<<endl;
		file<<"\t\t\t<x_coord> "<<xcoor<<" </x_coord>"<<endl;
		file<<"\t\t\t<y_coord> "<<ycoor<<" </y_coord>"<<endl;
		if(no->FirstChildElement("lat")!=NULL){
			//float lat=atof(VALUE(no->FirstChildElement("lat")));
			file<<"\t\t\t<lat> "<<VALUE(no->FirstChildElement("lat"))<<" </lat>"<<endl;
		}
		if(no->FirstChildElement("lng")!=NULL){
			//float lng=atof(VALUE(no->FirstChildElement("lng")));
			file<<"\t\t\t<lng> "<<VALUE(no->FirstChildElement("lng"))<<" </lng>"<<endl;
		}
		file<<"\t\t</node>"<<endl;
	}
	file<<"\t</nodes>"<<endl;
	file<<"\t<sources>"<<endl;
	for(source=sources->FirstChildElement("source");source;source=source->NextSiblingElement("source")){
		int id=atoi(VALUE(source->FirstChildElement("source_id")));
		int act=atoi(VALUE(source->FirstChildElement("first_activation_at")));
		int node=atoi(VALUE(source->FirstChildElement("located_at_node")));
		int targ=atoi(VALUE(source->FirstChildElement("target_laneset")));
		int fre=atoi(VALUE(source->FirstChildElement("source_activation_frequency")));
		const char* type=VALUE(source->FirstChildElement("source_type"));
		file<<"\t\t<source>"<<endl;
		file<<"\t\t\t<source_id> "<<id<<" </source_id>"<<endl;
		file<<"\t\t\t<first_activation_at> "<<act<<" </first_activation_at>"<<endl;
		file<<"\t\t\t<located_at_node> "<<node<<" </located_at_node>"<<endl;
		file<<"\t\t\t<target_laneset> "<<targ<<" </target_laneset>"<<endl;
		file<<"\t\t\t<source_activation_frequency> "<<fre<<" </source_activation_frequency>"<<endl;
		file<<"\t\t\t<source_type> "<<type<<" </source_type>"<<endl;
		int flow;
		float prob;
		if(strcmp(type, "CONSTANT_PROB")==0){
			prob=atof(VALUE(source->FirstChildElement("probability")));
			file<<"\t\t\t\t<probability>"<<prob<<" </probability>"<<endl;
		}else if(strcmp(type,"CONSTANT_FLOW")==0){
			flow=atof(VALUE(source->FirstChildElement("flow")));
			file<<"\t\t\t\t<flow>"<<prob<<" </flow>"<<endl;
		}
		else if(strcmp(type, "VARIABLE_PROB")==0){
			TiXmlElement *probs=source->FirstChildElement("probabilities");
			TiXmlElement *var_prob;
			file<<"\t\t\t<probabilities>"<<endl;
			for (var_prob=probs->FirstChildElement("var_prob");var_prob;var_prob=var_prob->NextSiblingElement("var_prob")){
				int num_inte=atoi(VALUE(var_prob->FirstChildElement("number_of_iterations")));
				float probability=atof(VALUE(var_prob->FirstChildElement("probability")));
				file<<"\t\t\t\t<var_prob>"<<endl;
				file<<"\t\t\t\t\t<number_of_iterations> "<<num_inte<<" </number_of_iterations>"<<endl;
				file<<"\t\t\t\t\t<probability> "<<probability<<" </probability>"<<endl;
				file<<"\t\t\t\t</var_prob>"<<endl;
			}
			file<<"\t\t\t</probabilities>"<<endl;
		}
		file<<"\t\t</source>"<<endl;
	}
	file<<"\t</sources>"<<endl;
	file<<"\t<sinks>"<<endl;
	for(sink=sinks->FirstChildElement("sink");sink;sink=sink->NextSiblingElement("sink")){
		int id=atoi(VALUE(sink->FirstChildElement("sink_id")));
		int act=atoi(VALUE(sink->FirstChildElement("first_activation_at")));
		int node=atoi(VALUE(sink->FirstChildElement("located_at_node")));
		int sola=atoi(VALUE(sink->FirstChildElement("source_laneset")));
		float prob=atof(VALUE(sink->FirstChildElement("removal_probability")));
		file<<"\t\t<sink>"<<endl;
		file<<"\t\t\t<sink_id> "<<id<<" </sink_id>"<<endl;
		file<<"\t\t\t<first_activation_at> "<<act<<" </first_activation_at>"<<endl;
		file<<"\t\t\t<located_at_node> "<<node<<" </located_at_node>"<<endl;
		file<<"\t\t\t<source_laneset> "<<sola<<" </source_laneset>"<<endl;
		file<<"\t\t\t<removal_probability>"<<prob<<" </removal_probability>"<<endl;
		file<<"\t\t</sink>"<<endl;
	}
	file<<"\t</sinks>"<<endl;
	file<<"\t<traffic_lights>"<<endl;
	for(unsigned int i=0;i<tl.size();i++){
		file<<geraTraffic(tl[i], id[i], s);
	}
	file<<"\t</traffic_lights>"<<endl;
	file<<"\t<streets>"<<endl;
	for(streetx=streetsX->FirstChildElement("street");streetx;streetx=streetx->NextSiblingElement("street")){
		int id=atoi(VALUE(streetx->FirstChildElement("street_id")));
		string st=VALUE(streetx->FirstChildElement("street_name"));
		file<<"\t\t<street>"<<endl;
		file<<"\t\t\t<street_id> "<<id<<" </street_id>"<<endl;
		file<<"\t\t\t<street_name> "<<st<<"</street_name>"<<endl;
		TiXmlElement *sections=streetx->FirstChildElement("sections");
		TiXmlElement *section;
		file<<"\t\t\t<sections>"<<endl;
		for (section=sections->FirstChildElement("section");section;section=section->NextSiblingElement("section")){
			file<<"\t\t\t\t<section>"<<endl;
			int sec_id=atoi(VALUE(section->FirstChildElement("section_id")));
			file<<"\t\t\t\t\t<section_id> "<<sec_id<<" </section_id>"<<endl;
			string sec_name=VALUE(section->FirstChildElement("section_name"));
			int in=sec_name.find("<");
			if(in!=-1){
				string sec_name1=sec_name.substr(0,sec_name.find_last_of("<"));
				sec_name=sec_name1+"&lt;-&gt; "+sec_name.substr(sec_name.find_last_of(">")+1);
			}
			file<<"\t\t\t\t\t<section_name> "<<sec_name<<" </section_name>"<<endl;
			string is_pref=VALUE(section->FirstChildElement("is_preferencial"));
			file<<"\t\t\t\t\t<is_preferencial> "<<is_pref<<" </is_preferencial>"<<endl;
			TiXmlElement *delimit=section->FirstChildElement("delimiting_node");
			int deli1=atoi(VALUE(delimit));
			file<<"\t\t\t\t\t<delimiting_node> "<<deli1<<" </delimiting_node>"<<endl;
			int deli2=atoi(VALUE(delimit->NextSiblingElement("delimiting_node")));
			file<<"\t\t\t\t\t<delimiting_node> "<<deli2<<" </delimiting_node>"<<endl;
			TiXmlElement *lanesets=delimit->NextSiblingElement("lanesets");
			TiXmlElement *laneset;
			file<<"\t\t\t\t\t<lanesets>"<<endl;
			for (laneset=lanesets->FirstChildElement("laneset");laneset;laneset=laneset->NextSiblingElement("laneset")){
				int laneset_id=atoi(VALUE(laneset->FirstChildElement("laneset_id")));
				int laneset_position=atoi(VALUE(laneset->FirstChildElement("laneset_position")));
				int start_node=atoi(VALUE(laneset->FirstChildElement("start_node")));
				int end_node=atoi(VALUE(laneset->FirstChildElement("end_node")));
				TiXmlElement *turn=laneset->FirstChildElement("turning_probabilities");
				TiXmlElement *direction;
				file<<"\t\t\t\t\t\t<laneset>"<<endl;
				file<<"\t\t\t\t\t\t\t<laneset_id> "<<laneset_id<<" </laneset_id>"<<endl;
				file<<"\t\t\t\t\t\t\t<laneset_position> "<<laneset_position<<" </laneset_position>"<<endl;
				file<<"\t\t\t\t\t\t\t<start_node> "<<start_node<<" </start_node>"<<endl;
				file<<"\t\t\t\t\t\t\t<end_node> "<<end_node<<" </end_node>"<<endl;
				file<<"\t\t\t\t\t\t\t<turning_probabilities>"<<endl;
				for (direction=turn->FirstChildElement("direction");direction;direction=direction->NextSiblingElement("direction")){
					int dest=atoi(VALUE(direction->FirstChildElement("destination_laneset")));
					float prob=atof(VALUE(direction->FirstChildElement("probability")));
					file<<"\t\t\t\t\t\t\t\t<direction>"<<endl;
					file<<"\t\t\t\t\t\t\t\t\t<destination_laneset> "<<dest<<" </destination_laneset>"<<endl;
					file<<"\t\t\t\t\t\t\t\t\t<probability> "<<prob<<" </probability>"<<endl;
					file<<"\t\t\t\t\t\t\t\t</direction>"<<endl;
				}
				file<<"\t\t\t\t\t\t\t</turning_probabilities>"<<endl;
				TiXmlElement *lanes=laneset->FirstChildElement("lanes");
				TiXmlElement *lane;
				file<<"\t\t\t\t\t\t\t<lanes>"<<endl;
				for (lane=lanes->FirstChildElement("lane");lane;lane=lane->NextSiblingElement("lane")){
					int lane_id=atoi(VALUE(lane->FirstChildElement("lane_id")));
					int lane_position=atoi(VALUE(lane->FirstChildElement("lane_position")));
					int max_speed=atoi(VALUE(lane->FirstChildElement("maximum_speed")));
					float decel=atof(VALUE(lane->FirstChildElement("deceleration_prob")));
					file<<"\t\t\t\t\t\t\t\t<lane>"<<endl;
					file<<"\t\t\t\t\t\t\t\t\t<lane_id> "<<lane_id<<" </lane_id>"<<endl;
					file<<"\t\t\t\t\t\t\t\t\t<lane_position> "<<lane_position<<" </lane_position>"<<endl;
					file<<"\t\t\t\t\t\t\t\t\t<maximum_speed> "<<max_speed<<" </maximum_speed>"<<endl;
					file<<"\t\t\t\t\t\t\t\t\t<deceleration_prob> "<<decel<<" </deceleration_prob>"<<endl;
					file<<"\t\t\t\t\t\t\t\t</lane>"<<endl;
				}
				file<<"\t\t\t\t\t\t\t</lanes>"<<endl;
				file<<"\t\t\t\t\t\t</laneset>"<<endl;
			}
			file<<"\t\t\t\t\t</lanesets>"<<endl;
			file<<"\t\t\t\t</section>"<<endl;
		}
		file<<"\t\t\t</sections>"<<endl;
		file<<"\t\t</street>"<<endl;
	}
	file<<"\t</streets>"<<endl;
	file<<"</simulation>"<<endl;*/
}

void leNet::geraTrafficLight(simulation s, TiXmlDocument doc){
	//cout<<s.streets.size()<<endl;
	vector<int> tlp1;
	vector<int> tlp2;
	vector<int> n;
	vector<cruzamento> cruz;

	vector<vet> vec;
	vet pos;
	for(unsigned int t=0;t<s.streets.size();t++){
		//file<<"street: "<<s.streets[t].id<<endl;
		for(unsigned int tes=0;tes<s.streets[t].secti.size();tes++){
			for(unsigned int tes1=0;tes1<s.streets[t].secti[tes].lsets.size();tes1++){
			//	file<<"\tnodeS: "<<s.streets[t].secti[tes].lsets[tes1].startNode<<endl;
				//file<<"\tnodeE: "<<s.streets[t].secti[tes].lsets[tes1].endNode<<endl;
				/*if(pos.v.size()==0){
					pos.v.push_back(s.streets[t].secti[tes].lsets[tes1].startNode);
					pos.v.push_back(s.streets[t].secti[tes].lsets[tes1].endNode);
					vec.push_back(pos);
				}else{*/
				bool difS=false;
				bool difE=false;
				for(unsigned int ver=0;ver<pos.v.size();ver++){
					int start=s.streets[t].secti[tes].lsets[tes1].startNode;
					int end=s.streets[t].secti[tes].lsets[tes1].endNode;
					if(start==pos.v[ver])difS=true;
					if(end==pos.v[ver])difE=true;
				}
				if(!difS)pos.v.push_back(s.streets[t].secti[tes].lsets[tes1].startNode);
				if(!difE)pos.v.push_back(s.streets[t].secti[tes].lsets[tes1].endNode);
					//}
			}
		}
		//if(!ehSource(pos, s))
			vec.push_back(pos);
		pos.v.clear();
	}

	cruzamento cruza;
	vector<int> nos;
	for(unsigned int a=0;a<vec.size();a++){
		for(unsigned int b=0;b<vec[a].v.size();b++){
			int no=vec[a].v[b];
			for(unsigned int c=a+1;c<vec.size();c++){
				for(unsigned int d=0;d<vec[c].v.size();d++){
					if(no==vec[c].v[d]){
						//file<<"no: "<<no<<" ruas: "<<a<<" "<<c<<endl;
						cruza.nodeid=no;
						cruza.idruas.push_back(a);
						cruza.idruas.push_back(c);
						cruz.push_back(cruza);
						cruza.idruas.clear();
						bool tem=false;
						for(unsigned int axa=0;axa<s.nodes.size();axa++){
							//xint comp=s.nodes[axa].id;
							if(no==s.nodes[axa].id){
								tem=true;
								axa=s.nodes.size();
							}
						}
						if(tem)nos.push_back(no);
					}
				}
			}
		}
	}
	/*cout<<"numC: "<<cruz.size()<<endl;
	for(unsigned int c=0;c<cruz.size();c++){
		cout<<"Node: "<<cruz[c].nodeid<<endl;
		for (unsigned d = 0; d < cruz[c].idruas.size(); d++) {
			cout<<"idrua: "<<s.streets[cruz[c].idruas[d]].id<<" street position: "<<cruz[c].idruas[d]<<endl;
		}
	}*/

	//monta trafficlight(struct)
	//traffic light tem dois ints source, dest, e dois vector lanesets e lanes;

	vector<trafficLight> trafficlight;
	trafficLight tl;
	for(unsigned int a=0;a<cruz.size();a++){
		int no=cruz[a].nodeid;
		//cout<<"tam : "<<cruz.size()<<endl;
		//cout<<"cruzamento: "<<a<<endl;
		for(unsigned int ruas=0;ruas<s.streets.size();ruas++){
			//cout<<"num: "<<cruz[a].idruas[ruas]<<endl;
			bool b=false;
			for(unsigned int sections=0;sections<s.streets[ruas].secti.size();sections++){
				vet v;
				tllaneset set;
				for(unsigned int lset=0;lset<s.streets[ruas].secti[sections].lsets.size();lset++){
					b=false;
					int nodeini;
					int nodefim;
					int lanesetID;
					int sectionID;
					int streetID;
					for(unsigned int dest=0;dest<s.streets[ruas].secti[sections].lsets[lset].destinations.size();dest++){
						int i=s.streets[ruas].secti[sections].lsets[lset].destinations[dest];
						nodeini=s.streets[ruas].secti[sections].lsets[lset].startNode;
						nodefim=s.streets[ruas].secti[sections].lsets[lset].endNode;
						lanesetID=s.streets[ruas].secti[sections].lsets[lset].laneset_id;
						sectionID=s.streets[ruas].secti[sections].section_id;
						streetID=s.streets[ruas].id;
						if(no==nodeini||no==nodefim){
							v.v.push_back(i);
							b=true;
						}
					}
					set.nodeDest=nodefim;
					set.nodeSource=nodeini;
					set.id=lanesetID;
					set.vetor=v;
					set.sectID=sectionID;
					set.streetID=streetID;
					if(b)for(unsigned int lan=0;lan<s.streets[ruas].secti[sections].lsets[lset].lanes.size();lan++){
						//tl.lanes.push_back(s.streets[cruz[a].idruas[ruas]].secti[sections].lsets[lset].lanes[lan].lane_id);
						set.lanes.v.push_back(s.streets[ruas].secti[sections].lsets[lset].lanes[lan].lane_id);
					}
					if(b)tl.laneset.push_back(set);
					v.v.clear();
					set.vetor.v.clear();
					set.lanes.v.clear();
				}
			if(b)trafficlight.push_back(tl);
			tl.laneset.clear();
			}
		}
	}
	//cout<<"num tls: "<<trafficlight.size()<<endl;
	//cout<<"S:"<<trafficlight[1].laneset[0].nodeSource<<endl;
	//cout<<"D:"<<trafficlight[1].laneset[0].nodeDest<<endl;
	//cout<<"lanesets:"<<endl;
	/*for(unsigned int uns=0;uns<trafficlight[1].laneset[0].vetor.v.size();uns++){
		cout<<""<<trafficlight[1].laneset[0].vetor.v[uns]<<endl;
	}
	//cout<<"lanes:"<<endl;
	for(unsigned int la=0;la<trafficlight[1].laneset[0].lanes.v.size();la++){
		cout<<" "<<trafficlight[1].laneset[0].lanes.v[la]<<endl;
	}*/

//		for (unsigned int i = 0; i < trafficlight.size(); i++) {
//			for (unsigned int j = 0; j < trafficlight[i].laneset.size(); j++) {
//				cout<<trafficlight[i].laneset[j].id<<endl;
//			}
//		}


	vector<trafficLight> tra;
	vector<trafficRaito> tr;
	trafficRaito raito;

	for(unsigned int a=0;a<nos.size();a++){
		int no=nos[a];
		//cout<<"no "<<no<<endl;
		for(unsigned int b=0;b<trafficlight.size();b++){
			int nodeS=trafficlight[b].laneset[0].nodeSource;
			int nodeD=trafficlight[b].laneset[0].nodeDest;
			if(no==nodeS||no==nodeD){
				tra.push_back(trafficlight[b]);
				//cout<<"nodeS: "<<trafficlight[b].laneset[0].nodeSource<<endl;
				//cout<<"nodeD: "<<trafficlight[b].laneset[0].nodeDest<<endl;
			}
		}
		if(tra.size()!=0)raito.traff=tra;
		if(raito.traff.size()!=0)tr.push_back(raito);
		tra.clear();
	}

	/*for(unsigned int i=0;i<tr.size();i++){
		for(unsigned int j=0;j<tr[i].traff.size();j++){
			for(unsigned int k=0;k<tr[i].traff[j].laneset.size();k++){
				cout<<"i: "<<i<<" j "<<j<<endl;
				cout<<"s: "<<tr[i].traff[j].laneset[k].nodeSource<<endl;
				cout<<"d: "<<tr[i].traff[j].laneset[k].nodeDest<<endl;
			}
		}
	}*/
	//cout<<"tam: "<<tr.size()<<endl;
	//cout<<"Tam: "<<nos.size()<<endl;

	for(unsigned int i=0;i<nos.size();i++){
		int no=nos[i];
		for(unsigned int j=i+1;j<nos.size();j++){
			if(nos[j]==no)nos[j]=0;
		}
	}
//	for (unsigned int i = 0; i < nos.size(); i++) {
//		cout<<nos[i]<<endl;
//	}

	/*vector<trafficRaito> tr2;
	for(unsigned int i=0;i<nos.size();i++){
		if(nos[i]!=0)tr2.push_back(tr[i]);
	}

	for(unsigned int i=0;i<nos.size();i++){
		if(nos[i]==2)cout<<"aki:"<<i<<endl;
	}*/
	writeFile(doc, tr, nos, s);
	string na=getCaminho(getName());
	cout<<"Arquivo: "<<na.substr(na.find_last_of("/")+1)<<" criado com sucesso no diretorio: "<<na.substr(0,na.find_last_of("/")+1)<<endl;
}

string leNet::geraTraffic(trafficRaito tl, int nodeId, simulation s){
	stringstream str;
	stringstream straux;

	vector<tllaneset> horizontais;
	vector<tllaneset> verticais;
	trafficRaito traRa;
	trafficLight traL;
	vector<tllaneset> vlt;

//	for (unsigned int i = 0; i < tl.traff.size(); i++) {
//		for (unsigned int j = 0; j < tl.traff[i].laneset.size(); j++) {
//			cout<<tl.traff[i].laneset[j].id<<endl;
//		}
//	}

if(nodeId!=0){
	for(unsigned int i=0;i<tl.traff.size();i++){
		for(unsigned int j=0;j<tl.traff[i].laneset.size();j++){
			int s=tl.traff[i].laneset[j].nodeSource;
			int d=tl.traff[i].laneset[j].nodeDest;
			//cout<<"source"<<s<<endl;
			//cout<<"sink"<<d<<endl;
			//cout<<"i"<<i<<endl;
			if(vlt.empty())vlt.push_back(tl.traff[i].laneset[j]);
			else{
				bool b=false;
				for(unsigned int k=0;k<vlt.size();k++){
					int ss=vlt[k].nodeSource;
					int dd=vlt[k].nodeDest;
					if(ss==s&&dd==d)b=true;
				}
				if(!b)vlt.push_back(tl.traff[i].laneset[j]);
			}
		}
		//traRa.traff[i].laneset=vlt;
	}
	traL.laneset=vlt;
	traRa.traff.push_back(traL);
	traRa.traff.clear();
	for(unsigned int i=0;i<tl.traff.size();i++){
		if(traRa.traff.empty())traRa.traff.push_back(tl.traff[i]);
		else{
			bool b=false;
			for(unsigned int j=0;j<traRa.traff.size();j++){
				int nodepri=tl.traff[i].laneset[0].nodeSource;
				int nodeseg=traRa.traff[j].laneset[0].nodeSource;
				int nodepri2=tl.traff[i].laneset[0].nodeDest;
				int nodeseg2=traRa.traff[j].laneset[0].nodeDest;
				if(nodepri==nodeseg&&nodepri2==nodeseg2)b=true;
			}
			if(!b)traRa.traff.push_back(tl.traff[i]);
		}
	}


	tl=traRa;

	/*for(unsigned int i=0;i<traRa.traff.size();i++){
		for(unsigned int j=0;j<traRa.traff[i].laneset.size();j++){
			cout<<"s:"<<traRa.traff[i].laneset[j].nodeSource<<endl;
			cout<<"d:"<<traRa.traff[i].laneset[j].nodeDest<<endl;
			cout<<"traff:"<<i<<endl;
		}
	}*/

	//tl=traRa;
	straux<<"\t\t<traffic_light>"<<endl;
	straux<<"\t\t\t<traffic_light_id> "<<getID()<<" </traffic_light_id>"<<endl;
	straux<<"\t\t\t<located_at_node> "<<nodeId<<" </located_at_node>"<<endl;
	straux<<"\t\t\t<signalplans>"<<endl;
	straux<<"\t\t\t\t<signalplan>"<<endl;
	straux<<"\t\t\t\t\t<signalplan_id> "<<1<<" </signalplan_id>"<<endl;
	straux<<"\t\t\t\t\t<additional_info>  </additional_info>"<<endl;
	straux<<"\t\t\t\t\t<phases>"<<endl;
	int tempo=0;
	int cont=1;
	int numPhases=0;
	int numPhases2=0;
	bool boo=false;
	bool boo2=false;
	for(unsigned int i=0;i<tl.traff.size();i++){
		int nodeD=tl.traff[i].laneset[0].nodeDest;
//		if(nodeId==370660500){
//			cout<<"aki"<<endl;
//		}
		if(nodeD==nodeId){
			straux<<"\t\t\t\t\t\t<phase>"<<endl;
			straux<<"\t\t\t\t\t\t\t<phase_id> "<<cont<<" </phase_id>"<<endl;
			if(tempo!=0)tempo+=1;
			straux<<"\t\t\t\t\t\t\t<iteration_start> "<<tempo<<" </iteration_start>"<<endl;
			tempo=(tempo+(getDuracaoCiclo()/contaTLRaito(tl, nodeId)))-1;
			straux<<"\t\t\t\t\t\t\t<iteration_end> "<<tempo<<" </iteration_end>"<<endl;
			straux<<"\t\t\t\t\t\t\t<directions>"<<endl;
			for(unsigned int b=0;b<tl.traff[i].laneset[0].lanes.v.size();b++){
				for(unsigned int c=0;c<tl.traff[i].laneset[0].vetor.v.size();c++){
					int laneset=tl.traff[i].laneset[0].vetor.v[c];
					int street=tl.traff[i].laneset[0].streetID;
					int nodeS=tl.traff[i].laneset[0].nodeSource;
					if(mesmaStreet(laneset, street, nodeId, nodeS, tl)){
						boo=true;
						numPhases++;
						straux<<"\t\t\t\t\t\t\t\t<direction>"<<endl;
						straux<<"\t\t\t\t\t\t\t\t\t<from_lane> "<<tl.traff[i].laneset[0].lanes.v[b]<<" </from_lane>"<<endl;
						straux<<"\t\t\t\t\t\t\t\t\t<to_laneset> "<<tl.traff[i].laneset[0].vetor.v[c]<<" </to_laneset>"<<endl;
						straux<<"\t\t\t\t\t\t\t\t</direction>"<<endl;
					}
				}
			}if(boo)cont++;
			numPhases=numPhases/tl.traff[i].laneset[0].lanes.v.size();
			straux<<"\t\t\t\t\t\t\t</directions>"<<endl;
			straux<<"\t\t\t\t\t\t</phase>"<<endl;

		}else if(tl.traff[i].laneset.size()>1){
			straux<<"\t\t\t\t\t\t<phase>"<<endl;
			straux<<"\t\t\t\t\t\t\t<phase_id> "<<cont<<" </phase_id>"<<endl;
			if(tempo!=0)tempo+=1;
			straux<<"\t\t\t\t\t\t\t<iteration_start> "<<tempo<<" </iteration_start>"<<endl;
			tempo=(tempo+(getDuracaoCiclo()/contaTLRaito(tl, nodeId)))-1;
			straux<<"\t\t\t\t\t\t\t<iteration_end> "<<tempo<<" </iteration_end>"<<endl;
			straux<<"\t\t\t\t\t\t\t<directions>"<<endl;
			for(unsigned int b=0;b<tl.traff[i].laneset[1].lanes.v.size();b++){
				for(unsigned int c=0;c<tl.traff[i].laneset[1].vetor.v.size();c++){
					int laneset=tl.traff[i].laneset[1].vetor.v[c];
					int street=tl.traff[i].laneset[1].streetID;
					int nodeS=tl.traff[i].laneset[1].nodeSource;
					if(mesmaStreet(laneset, street, nodeId, nodeS, tl)){
						boo2=true;
						numPhases2++;
						straux<<"\t\t\t\t\t\t\t\t<direction>"<<endl;
						straux<<"\t\t\t\t\t\t\t\t\t<from_lane> "<<tl.traff[i].laneset[1].lanes.v[b]<<" </from_lane>"<<endl;
						straux<<"\t\t\t\t\t\t\t\t\t<to_laneset> "<<tl.traff[i].laneset[1].vetor.v[c]<<" </to_laneset>"<<endl;
						straux<<"\t\t\t\t\t\t\t\t</direction>"<<endl;
					}
				}
			}if(boo2)cont++;
			numPhases2=numPhases2/tl.traff[i].laneset[1].lanes.v.size();
			straux<<"\t\t\t\t\t\t\t</directions>"<<endl;
			straux<<"\t\t\t\t\t\t</phase>"<<endl;
		}
	}
	straux<<"\t\t\t\t\t</phases>"<<endl;
	straux<<"\t\t\t\t</signalplan>"<<endl;
	straux<<"\t\t\t</signalplans>"<<endl;
	straux<<"\t\t</traffic_light>"<<endl;

	if(cont>1){
		numTL++;
		str<<straux.str();
	}
	/*if((boolean[0]==true)&&(boolean[1]==true)&&(numPhases+numPhases2)>2){
		numTL++;
		str<<straux.str();
	}*/
	return str.str();
}else return "";
}

node leNet::getNode(simulation s, int id){
	node n;
	for(unsigned int a=0;a<s.nodes.size();a++){
		if(id==s.nodes[a].id){
			n=s.nodes[a];
			a=s.nodes.size();
		}
	}
	return n;
}

void leNet::mostraStreet(vector<street> streets){
	stringstream strs;
	cout<<"Street_id:"<<streets[0].id<<endl;
	cout<<"Street_name:"<<streets[0].name<<endl;
	cout<<"Sections_id:"<<streets[0].secti[1].section_id<<endl;
	cout<<"Laneset_id:"<<streets[0].secti[1].lsets[0].laneset_id<<endl;
	cout<<"Lane_id:"<<streets[0].secti[1].lsets[0].lanes[0].lane_id<<endl;
	/*for (unsigned int i = 0; i < streets[0].secti[0].lsets[0].destinations.size(); i++){
		if(i==0)strs<<streets[0].secti[0].lsets[0].destinations[i];
		else strs<<" "<<streets[0].secti[0].lsets[0].destinations[i];
	}
	cout<<"Destinations:"<<strs.str()<<endl;
	*/
	for(unsigned int a=0;a<streets.size();a++){
		cout<<"Street_id: "<<streets[a].id<<endl;
		for(unsigned int b=0;b<streets[a].secti.size();b++){
			for(unsigned int c=0;c<streets[a].secti[b].lsets.size();c++){
				cout<<"\tnodes: "<<streets[a].secti[b].lsets[c].startNode<<" "<<streets[a].secti[b].lsets[c].endNode<<endl;
			}
		}
	}
}

void leNet::mostraNos(simulation s){
	vector<node> n=s.nodes;
	for(unsigned int a=0;a<n.size();a++){
		cout<<"id: "<<n[a].id<<" x: "<<n[a].x<<" y: "<<n[a].y<<endl;
	}
	//teste
	for(unsigned int i=0;i<s.streets.size();i++){
		//file<<"Street_id: "<<streets[0].id<<endl;
		for(unsigned int j=0;j<s.streets[i].secti.size();j++){
			//file<<"\tsection_id: "<<streets[i].secti[j].section_id<<endl;
			for(unsigned int k=0;k<s.streets[i].secti[j].lsets.size();k++){
				//file<<"\t\tlaneset_id: "<<streets[i].secti[j].lsets[k].laneset_id<<endl;
				cout<<"node start: "<<s.streets[i].secti[j].lsets[k].startNode<<endl;
				cout<<"noe_end: "<<s.streets[i].secti[j].lsets[k].endNode<<endl;
			}
		}
	}

	//fim teste
}
/*
void leNet::mostraDirections(vector<street> streets){
	stringstream strs;
	ofstream file("out.txt");

	//teste
	stringstream str;
	for(unsigned int a=0;a<streets[0].traf.nodes.size();a++){
		if(streets[0].traf.nodes[a].id!=-1){
			cout<<"Start: "<<streets[0].traf.nodes[a].id<<endl;
			for(unsigned int b=0;streets[0].traf.nodes[a].des.size();b++){
				cout<<"End: "<<streets[0].traf.nodes[a].des[b]<<endl;
			}
		}
	}
	//fim teste!!!

	for(unsigned int i=0;i<streets.size();i++){
		//file<<"Street_id: "<<streets[0].id<<endl;
		for(unsigned int j=0;j<streets[i].secti.size();j++){
			//file<<"\tsection_id: "<<streets[i].secti[j].section_id<<endl;
			for(unsigned int k=0;k<streets[i].secti[j].lsets.size();k++){
				//file<<"\t\tlaneset_id: "<<streets[i].secti[j].lsets[k].laneset_id<<endl;
				file<<"node start: "<<streets[i].secti[j].lsets[k].endNode<<endl;
				for(unsigned int l=0;l<streets[i].secti[j].lsets[k].lanes.size();l++){
					//file<<"\t\t\tlane_id: "<<streets[i].secti[j].lsets[k].lanes[l].lane_id<<endl;
					file<<"\tfrom_lane: "<<streets[i].secti[j].lsets[k].lanes[l].lane_id<<endl;
					for(unsigned int m=0;m<streets[i].secti[j].lsets[k].destinations.size();m++){
						//file<<"\t\t\t\tdestination: "<<streets[i].secti[j].lsets[k].destinations[m]<<endl;
						file<<"\t\tto_laneset: "<<streets[i].secti[j].lsets[k].destinations[m]<<endl;
					}
				}
			}
		}
	}
}
*/
