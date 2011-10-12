#include <qdom.h>
#include "drivertypes.h"
#include <iostream>
class QPoint;
class TiXmlElement;

//Handler class for input/output operation on driver configuration files.
class FileHandler: public QDomDocument
{
public:
    FileHandler();
    ~FileHandler();

	int confLoad();
	int confSave();

	int confDrivers(XmlDrivers& list);

	int insertTemplateDriver(const QString& name);

	int removeTemplateDriver(const QString& name);

	int getTemplateDriver(const QString& name, XmlDriver& driver);

	int editTemplateOption(const int& op, const QString& driver, const QPair<QString, QString>& option);

	bool isLoaded();


protected:

	XmlDriverList confDrvList_;
	bool loaded_;

};

inline bool FileHandler::isLoaded()
{
	return loaded_;
}


//*********************************************************************
//*********************************************************************
//*********************************************************************


///Handler class for input/output operation on simulation configuration files.
class SimFileHandler: public FileHandler
{    
public:
    SimFileHandler();
    ~SimFileHandler();

	XmlSimConf simConf;
	QString configFile();
	void setConfigFile(QString file);
	
	int load(const QString& file);

	int save(const QString& file = QString::null);

	int drivers(XmlDrivers& list);

	int sensors(XmlDrivers& list);

	int getDriver(const QString& name, XmlDriver& driver);

	int getSensor(const QString& name, XmlSensor& sensor);

	int setDriver(QString name, XmlDriver driver);

	int setSensor(QString name, XmlSensor sensor);
	
	void localClear();

	bool fill();

	int removeDriver(QString driver);

	int removeSensor(QString sensor);

	int insertDriver(const QString& driver);

	int insertSensor(const QString& sensor);

	int changeDriverProperty(const QString& driver, QPair<QString, QString> prop);

	int changeSensorProperty(const QString& sensor, QPair<QString, QString> prop);

	int checkDriverTemplate();

	int changeTemplateName(const QString& name);

	int changeDriversName(const QString& old, const QString& actual );


private:

	int driverInfo_(XmlDriverList& list);

	QString configFile_;

	XmlDriverList dList_;
	XmlSensorList sList_;
};

inline bool SimFileHandler::fill()
{
	if (configFile_ != "")
		return true;
	else
		return false;
}

inline void SimFileHandler::setConfigFile(QString file)
{
	configFile_ = file;	
}

inline QString SimFileHandler::configFile()
{
	return configFile_;
}


//*********************************************************************
//*********************************************************************
//*********************************************************************


//Handler class for input/output operation on network configuration files.
class NetHandler: public QDomDocument
{
public:
    NetHandler();
    ~NetHandler();

	QString toString();

	int load(const QString& file);

	bool isLoaded();

	LanesetList lanesets();
	
        QString netName();

	NodeList nodes();

	QPoint nodeLocation(const int& nodeId);

	QPoint xRange();
	QPoint yRange();

	QPair<QString, QString> lanesetNodes(const int& id);

	QValueVector<QString> nextNodes(const QString& from);

	int definedLaneset(const QString& from, const QString& to);

	TLList nodeToTls(TLList list);

	TLList tlToLaneset(TLList list);

	int nodeId_(const QString& name);

	Node getNode(int id);

private:

	QString nodeName_(const int& id);
	int parseNodes_(TiXmlElement* nodes, TiXmlElement* sources, TiXmlElement* sinks);
	int parseStreets_(TiXmlElement* streets);
	int parseTls_(TiXmlElement* tls);

	bool loaded_;
	QString netName_; ///<Network name.
	int netId_;		  ///<Network id.

	LanesetList lanesets_;
	NodeList nodes_;

};

inline QString NetHandler::netName()
{
        return netName_;
}

inline NodeList NetHandler::nodes()
{
	return nodes_;
}

inline LanesetList NetHandler::lanesets()
{
	return lanesets_;
}

inline bool NetHandler::isLoaded()
{
	return loaded_;
}


class AgentHandler: public QDomDocument
{
public:
	AgentHandler();
	~AgentHandler();

	int load(const QString& file);

	int save(const QString& file = QString::null);

	int getAgent(const QString& name, XmlAgent& agent);

	void changeLogStat(QString name, QString stat);
	
	int setAgentTls(QString name, TLList list);
	TLList agentTls(QString name);

	int addAgent(QString name, QString nickname);

	int changeAgentProperty(QString agentName, QPair<QString, QString> prop);

	int removeAgent(QString agent);

	bool loaded;
	QString agentFile;
	XmlAgentList al;
	
};
