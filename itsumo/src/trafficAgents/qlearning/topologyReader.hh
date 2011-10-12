#ifndef TOPOLOGYREADER_H_
#define TOPOLOGYREADER_H_
#include "../../tiny/tinyxml.h"	
#include "../basic/common.hh"
#include "../../basic/utils.hh"

class sectionSimple 
{
public:
	int id;	
	int parentStreet;	
	int getParent();
	sectionSimple(int sid,int parentStreet);
	virtual ~sectionSimple();
};

class lanesetSimple
{
public:
	int id;
	int nodeBegin;
	int nodeEnd;
	int parentSection;
	int getParent();
	lanesetSimple(int lid,int parentSectionId, int begin, int end);
	virtual ~lanesetSimple();
};


class laneSimple
{
public:
	int id;
	int parentLaneset;
	int getParent();
	laneSimple(int lid, int parent);
	virtual ~laneSimple();
};

class nodeSimple
{
public:
	int id;
	string name;
	int x_coord;
	int y_coord;
	nodeSimple(int nid,string nname,int n_x,int n_y);
	virtual ~nodeSimple();
	int relativePosition(nodeSimple *node);
};


struct plan
{
	int id;
	string info;
};

class tLight
{
public:
	int id;
	int node;
	vector<plan> plans;
	tLight(int nodeid, int t_id);	
	virtual ~tLight();
	
};

class topologyReader
{
private:
	void parse_signalplans(TiXmlElement *signalplans, tLight *tlight);
	void parse_trafficlights(TiXmlElement *tlts);
	void parse_lanes(TiXmlElement *les, int parentLanesetId);
	void parse_lanesets(TiXmlElement *lsets, int parentSectionId);
	void parse_sections(TiXmlElement *secs, int parentStreetId);
	void parse_streets(TiXmlElement *sts);
	void parse_nodes(TiXmlElement *nds);
	void parse_network(char* fileName);
public:
	topologyReader(char* fileName);
    int id;
    vector<tLight*> trafficLights;
    vector<int> streets;
    vector<sectionSimple*> sections;
    vector<lanesetSimple*> lanesets;
    vector<laneSimple*> lanes;   
    vector<nodeSimple*> nodes;   
	virtual ~topologyReader();	
	laneSimple* getLane(int lid);
	lanesetSimple* getLaneset(int id);
	int getTLIdAtNode(int node);
	tLight* getTLight(int id);
	nodeSimple* getNode(int id);
	int getTLightIdAtLane(int lid);
	int getBeginingNodeFromLane(int lid);
};

#endif /*TOPOLOGYREADER_H_*/
