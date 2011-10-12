#include <qstring.h>
#include <qvaluevector.h>
#include <qpair.h>

#define SOURCE 0
#define SINK 1
#define NODE 2

#define DestinationList QValueVector< Destination >

#define LanesetList QValueVector< Laneset >

#define NodeList QValueVector< Node >

#define RouteList QValueVector< Route >

struct Node
{
	int id;
	int tlid;
	bool hastl;//exist a traffic light?
	QString name;
	int x;
	int y;
	int type;
};

struct Destination
{
	int id;
	int prob;
};

struct Laneset
{
	int id;
	int sectionId;
	QString sectionName;

	DestinationList destinations;

	int begin;
	int end;
};


struct Route
{
	int laneset;//?
};


//////////////

#define XmlDriverOptions QValueVector< QPair <QString,QString> > 

struct XmlSimConf
{
	QString netFile;
	int steps;
	double defDec; //default deceleration
	int sensorInterval;
	int agentInterval;
	int carMaxSpeed;
	int iterationLength;
	int cellSize;
};

struct XmlDriver
{
	QString name;
	QString nick;
	QString className;
	int number;
	QString state;
	QString debug;
	QString description;
	XmlDriverOptions options;

	RouteList route;
};

struct XmlSensor
{
	QString name;
	QString file;
	QString state;
	QString timetag;
};

#define XmlDriverList QValueVector< XmlDriver >
#define XmlDrivers QValueVector< QString >

#define XmlSensorList QValueVector< XmlSensor >
#define XmlSensors QValueVector< QString >

//

#define TLList QValueVector< int >

struct XmlAgent
{
	QString name;
	QString nickname;
	QString log;
	QString memsize;
	QString alpha;
	QString minphase;
	QString states;
	QString algorithm;
	QString action;
	QString coordneighbors;
	QString gammasync;
	QString gammaunsync;
	QString starttime;
	QString learningrate;
	QString discountrate;
	QString firstphase;
	QString secondphase;
	QString tau;
	QString tau_agr_agr; //upstream agree, downstream agree
	QString tau_agr_not; //upstream agree, downstream not agree
	QString tau_not_agr; //upstream not agree, downstream agree
	QString tau_not_not; //upstream not agree, downstream not agree
	TLList tls;	
};

#define XmlAgentList QValueVector< XmlAgent >

