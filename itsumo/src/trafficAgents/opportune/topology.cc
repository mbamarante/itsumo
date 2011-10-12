#include "topology.hh"
using namespace std;


#define OBLIGATORY 0
#define OPTIONAL 1

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

#define CHECK(element, mode) (check(element, #element, mode))
#define VALUE(element) (element->FirstChild()->Value())

bool topology::check(TiXmlElement *element_to_test, string element_name, int condition)
{
  if (element_to_test==0) {
    cout << "Tag '" << element_name << "' expected but not found!" << endl;
    cout << "Fatal error! Leaving." << endl;
    exit(1);
  }
    
  if (element_to_test->FirstChild()==0) {
    if (condition==OBLIGATORY) {
      cout << "Tag '" << element_name << "' must have a value!" << endl;
      cout << "Fatal error! Leaving." << endl;
      exit(1);
    } 
    else
      return false;
  }
  else {
    return true;
  }
}


topology::topology(char* fileName)
{	
	parse_network(fileName);
}

topology::~topology(){}


streetC::streetC(int sid, vector<sectionC*> sec){
	id = sid;
	sections = sec;
	updateNodes();
}


streetC::~streetC(){}


void streetC::updateNodes(){
	vector<sectionC*>::const_iterator sec;
	vector<lanesetC*>::const_iterator lse;
	for (sec = sections.begin() ; sec != sections.end(); sec++ ){
		for (lse = (*sec)->lanesets.begin() ; lse != (*sec)->lanesets.end(); lse++ ){
				if (  (int) count (nodes.begin(), nodes.end(), (*lse)->node_begin) == 0 ){
					nodes.push_back( (*lse)->node_begin );			
				}
			}
	}
}

sectionC::sectionC(int sid, vector<lanesetC*> ls ){
	id = sid;
	lanesets = ls;
}

sectionC::~sectionC(){}

lanesetC::lanesetC(int sid, vector<laneSimple*> ln , int begin, int end){
	id = sid;
	lanes = ln;
	node_begin= begin;
	node_end=end;
}

lanesetC::~lanesetC(){}

/*
 * Return the node with the given id 
 */
nodeSimple* topology::getNode(int id){
	vector<nodeSimple*>::iterator n;
	for (n = nodes.begin(); n != nodes.end(); n++) {
		if( (*n)->id  == id ){
			return (*n);
		}
	}	
	return NULL;
}

lanesetSimple* topology::getLaneset(int id){
	vector<lanesetSimple*>::iterator ls;
	for (ls = lanesets.begin(); ls != lanesets.end(); ls++) {
		if( (*ls)->id  == id ){
			return (*ls);
		}
	}	
	return NULL;
}

void topology::parse_signalplans(TiXmlElement *splans, tLight *tlight)
{
	TiXmlElement *signalplan;
	TiXmlElement *signalplan_id;
	TiXmlElement *additional_info;	

	for(signalplan = splans->FirstChildElement("signalplan"); signalplan; 
	signalplan = signalplan->NextSiblingElement("signalplan")) {

		signalplan_id = signalplan->FirstChildElement("signalplan_id");

		additional_info = signalplan->FirstChildElement("additional_info");
		string str_additional_info;
		if CHECK(additional_info, OPTIONAL) {
			str_additional_info = VALUE(additional_info);
		} else {
			str_additional_info = " ";
		}
		
		plan p = {atoi(VALUE(signalplan_id)), str_additional_info};
		//cout<<"plano id"<<p.id<<endl;
		tlight->plans.push_back(p);		
	}

}


void topology::parse_trafficlights(TiXmlElement *tlts)
{

	TiXmlElement *trafficlight;
	TiXmlElement *traffic_light_id;
	TiXmlElement *located_at_node;
	TiXmlElement *signalplans;

	for(trafficlight = tlts->FirstChildElement("traffic_light"); trafficlight; 
	trafficlight = trafficlight->NextSiblingElement("traffic_light")) {

		traffic_light_id = trafficlight->FirstChildElement("traffic_light_id");
		located_at_node = trafficlight->FirstChildElement("located_at_node");

		tLight *tl;
		tl = new tLight(atoi(VALUE(located_at_node)), atoi(VALUE(traffic_light_id)));

		signalplans = trafficlight->FirstChildElement("signalplans");
		
		//cout<<"node:"<<atoi(VALUE(located_at_node))<<" id:"<< atoi(VALUE(traffic_light_id))<<endl;
		
		parse_signalplans(signalplans, tl);
			
		
		trafficLights.push_back(tl);
	}
}


vector<laneSimple*>  topology::parse_lanes(TiXmlElement *les, int parentLanesetId) {
	TiXmlElement *lane;
	TiXmlElement *lane_id;
	vector<laneSimple*> reply;
	for (lane = les->FirstChildElement("lane"); lane; lane= lane->NextSiblingElement("lane")) {
		lane_id = lane->FirstChildElement("lane_id");
		laneSimple* ls = new laneSimple(atoi(VALUE(lane_id)), parentLanesetId);
		//cout<<"lane ID:"<< ls->id << endl;
		lanes.push_back(ls);
		reply.push_back(ls);
		//cout<<"- - - lane "<<atoi(VALUE(lane_id))<<endl;
	}
	return reply;
}


vector<lanesetC*> topology::parse_lanesets(TiXmlElement *lsets, int parentSectionId) {

	TiXmlElement *laneset;
	TiXmlElement *laneset_id;
	TiXmlElement *start_node, *end_node;
	TiXmlElement *ls;
	vector<lanesetC*> reply;
	
	for (laneset = lsets->FirstChildElement("laneset"); laneset; laneset= laneset->NextSiblingElement("laneset")) {
		laneset_id = laneset->FirstChildElement("laneset_id");
		start_node = laneset->FirstChildElement("start_node");
		end_node = laneset->FirstChildElement("end_node");

		lanesets.push_back((new lanesetSimple(atoi(VALUE(laneset_id)), parentSectionId, atoi(VALUE(start_node)),atoi(VALUE(end_node)) ) ));
		//cout<<"- - laneset "<<atoi(VALUE(laneset_id))<<endl;


		ls = laneset->FirstChildElement("lanes");
		
		reply.push_back( new lanesetC( atoi(VALUE(laneset_id)), parse_lanes(ls,atoi(VALUE(laneset_id))), atoi(VALUE(start_node)),atoi(VALUE(end_node)) ) );
	}
	return reply;
}

vector<sectionC*>  topology::parse_sections(TiXmlElement *secs, int parentStreetId){
  TiXmlElement *section;
  TiXmlElement *section_id;
  TiXmlElement *lsets;
  vector<sectionC*> reply;
  for(section = secs->FirstChildElement("section"); section; section = section->NextSiblingElement("section")) {
    section_id = section->FirstChildElement("section_id");
    sectionSimple* newSection = new sectionSimple(atoi(VALUE(section_id)),parentStreetId);  
    sections.push_back(newSection);
	//cout<<"- section "<<atoi(VALUE(section_id))<<endl;
    lsets = section->FirstChildElement("lanesets");
    reply.push_back(new sectionC( atoi(VALUE(section_id)), parse_lanesets(lsets, atoi(VALUE(section_id))) ));  
  }
  return reply;
}


void topology::parse_streets(TiXmlElement *sts){

  TiXmlElement *street;
  TiXmlElement *street_id;
  TiXmlElement *sections;

  for(street = sts->FirstChildElement("street"); street; street = street->NextSiblingElement("street")) {

    street_id = street->FirstChildElement("street_id");
    
 
    
	sections = street->FirstChildElement("sections");
	
	//adds the street to the streets id vector of the network
	streets.push_back( new streetC( atoi(VALUE(street_id)), parse_sections(sections, atoi(VALUE(street_id))) ) );
	
	//cout<<"street "<<atoi(VALUE(street_id))<<endl;
  }
}


void topology::parse_nodes(TiXmlElement *nds){

  TiXmlElement *nd;
  TiXmlElement *n_id;
  TiXmlElement *n_name;
  TiXmlElement *n_x;
  TiXmlElement *n_y;
  string name;
  for(nd = nds->FirstChildElement("node"); nd; nd = nd->NextSiblingElement("node")) {
	  n_id = nd->FirstChildElement("node_id");
	  n_name = nd->FirstChildElement("node_name");
	  n_x = nd->FirstChildElement("x_coord");
	  n_y = nd->FirstChildElement("y_coord");
	  name =  VALUE(n_name); 
	  nodeSimple* n = new nodeSimple(atoi(VALUE(n_id)),name, atoi(VALUE(n_x)),atoi(VALUE(n_y)));
	  nodes.push_back(n);        
  }
}



//// reads the Network XML file 
void topology::parse_network(char* fileName){
	TiXmlElement *network;
	TiXmlDocument doc(fileName );
	bool loadOkay = doc.LoadFile();
	if ( !loadOkay )
	{
		printf( "Could not load file. Error='%s'.\nExiting.\n", 
				doc.ErrorDesc());
		exit(1);
	}
	
	TiXmlElement *net_id;
	TiXmlElement *xmlstreets;
	TiXmlElement *xmltls;
	TiXmlElement *nds;
	
	network = doc.FirstChildElement("simulation");
	net_id = network->FirstChildElement("network_id");
	id = atoi(VALUE(net_id));
	//cout<<"net id:"<<id<<endl;
	
	nds = network->FirstChildElement("nodes");
	parse_nodes(nds);
	
	xmltls = network->FirstChildElement("traffic_lights");
	//cout<<"parse tl"<<endl;
	parse_trafficlights(xmltls);
	
	xmlstreets = network->FirstChildElement("streets");
	parse_streets(xmlstreets);		
}


/*
 * Return the lane with the given id
 */
laneSimple* topology::getLane(int lid){
	vector<laneSimple*>::iterator ln;	
	for (ln = lanes.begin(); ln!= lanes.end(); ln++) {	
		if( (*ln)->id == lid ){
			return (*ln);
		}
	}	
	cout<<"nao achou a lane! "<<endl;
	return NULL;
}


/*
 * Return the section with the given id
 */
sectionSimple* topology::getSection(int sid){
	vector<sectionSimple*>::iterator s;	
	for (s = sections.begin(); s!= sections.end(); s++) {	
		if( (*s)->id == sid ){
			return (*s);
		}
	}	
	cout<<"nao achou a section! "<<endl;
	return NULL;
}

/*
 * Return the TL id located at the given node, 
 * if there is no TL at node, return -1 
 */
int topology::getTLIdAtNode(int node){
	vector<tLight*>::iterator tl;
	cout<<"get TL id at node: "<<node<<endl;
	for (tl = trafficLights.begin(); tl != trafficLights.end(); tl++) {
		if( (*tl)->node  == node ){
			return (*tl)->id;
		}
	}	
	return (-1);
}

/*
 * Return the TL with the given id 
 */
tLight* topology::getTLight(int id){
	vector<tLight*>::iterator tl;
	for (tl = trafficLights.begin(); tl != trafficLights.end(); tl++) {
		if( (*tl)->id  == id ){
			return (*tl);
		}
	}	
	return NULL;
}



/*
 * Return the TL id at the begining of the given lane id 
 */
int topology::getTLightIdAtLane(int lid){
	vector<lanesetSimple*>::iterator lset;
	int ret = -1;
	laneSimple* lane = getLane(lid);
	int lsetid = (lane)->getParent();
	lanesetSimple* laneset = getLaneset(lsetid);
	int secId  = (laneset)->getParent();
	
	for (lset = lanesets.begin(); lset != lanesets.end(); lset++) {
		if((*lset)->id  == lsetid){
			ret = getTLIdAtNode((*lset)->nodeBegin);
			return(ret);
		}
	}			
	return (ret);
}
/*
 * Return the node id at the begining of the given lane id 
 */
int topology::getBeginingNodeFromLane(int lid){
	vector<lanesetSimple*>::iterator lset;
	int ret = -1;
	laneSimple* lane = getLane(lid);
	if (lane !=NULL){
		int lsetid = (lane)->getParent();
		for (lset = lanesets.begin(); lset != lanesets.end(); lset++) {
			if((*lset)->id  == lsetid){
				ret = (*lset)->nodeBegin;
				return(ret);
			}
		}			
	}
	return (ret);
}


/*
 * Returns the first TL id sending vehicles for the given lane id 
 */
int topology::getFirstTLightIdNeighbor(int nodeid){			
	int tlid = -1;	
	if (nodeid!=-1){
		vector<streetC*>::iterator str;			
		for (str = streets.begin(); str != streets.end(); str++) {
			vector<int>::iterator no;
			no = find((*str)->nodes.begin(), (*str)->nodes.end(), nodeid);
			if( no != (*str)->nodes.end()){
				cout<<"nodo:"<< (*no)<<endl;			
				while ( (no != (*str)->nodes.end()) && ((*no) != nodeid ) ){
					no++;
					tlid = getTLIdAtNode((*no));
					if ( tlid != -1 ){
						return tlid; 
					}																																		
				}			
			}
		}			
	}
	return (tlid);
}


