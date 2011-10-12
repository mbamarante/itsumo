#include "filehandler.h"

#include <qstringlist.h>
#include <qfile.h>
#include <string>
#include <qpoint.h>
#include <qmessagebox.h>
#include <qdir.h>

#include "../tiny/tinyxml.h"

#include <iostream>

using namespace std;

FileHandler::FileHandler()
	: QDomDocument()
{
	loaded_ = false;

	if (confLoad())
	{
		loaded_ = true;
	}
}

FileHandler::~FileHandler()
{
}


///Load the configuration file that defines templates for drivers.
int FileHandler::confLoad()
{
	//open the file
	QFile f( "src/interface/drivers.xml" ); //todo: outside it
    if ( !f.open( IO_ReadOnly ) )
        return 0;
    if ( !setContent( &f ) ) {
        f.close();
        return 0;
    }
    f.close();

	QDomNode n = firstChild();
	n = n.firstChild();

	//DRIVERS
	if (!n.isNull())
	{
		QDomNode p;//properties of drivers
		QDomElement de;//property element

		QString option = "";

		while (!n.isNull())
		{
			XmlDriver driver;
			XmlDriverOptions options;

			driver.name = n.namedItem("name").toElement().text();
			driver.nick = n.namedItem("nick").toElement().text();
			driver.description = n.namedItem("description").toElement().text();
			
			p = n.firstChild();
						
			while (!p.isNull())
			{
				de = p.toElement();

				if ( (de.tagName() == "option") )
				{
					options.push_back(qMakePair(de.text(), option));
				}
				
				p = p.nextSibling();
			}
			
			driver.options = options;
			
			confDrvList_.push_back(driver);
			
			n = n.nextSibling();
		}
	}

	return 1;
}


///Save the configuration file that defines templates for drivers.
int FileHandler::confSave()
{
	QDomText t;
	QString aux = "";

	setContent(aux);

	//general
	QDomElement root = createElement( "drivers" );
    appendChild( root );

	if (confDrvList_.isEmpty())
	{
		t = createTextNode( "" );
		root.appendChild( t );
	}
	else //populate
	{
		QDomElement driver;
		QDomElement opn ;
		QDomElement on;
		
		XmlDriverList::iterator itD;
		for (itD = confDrvList_.begin(); itD != confDrvList_.end(); itD++)
		{
			driver = createElement( "driver" );
			root.appendChild(driver);

			//name
			opn = createElement( "name" );
			driver.appendChild(opn);
			t = createTextNode( itD->name );
			opn.appendChild( t );

			//nick
			opn = createElement( "nick" );
			driver.appendChild(opn);
			t = createTextNode( itD->nick );
			opn.appendChild( t );

			//description
			opn = createElement( "description" );
			driver.appendChild(opn);
			t = createTextNode( itD->description );
			opn.appendChild( t );

			//options
			XmlDriverOptions options = itD->options;

			if (options.isEmpty())
			{
				t = createTextNode( "" );
				driver.appendChild( t );
			}
			else
			{
				QDomElement temp;//rever

				XmlDriverOptions::iterator itOp;
				for (itOp = options.begin(); itOp != options.end(); itOp++)
				{
					QString p = itOp->first;
					//option
					temp = createElement( "option" );
					driver.appendChild(temp);
					t = createTextNode( itOp->first );
					temp.appendChild( t );
				}
			}

		}
	}
    QStringList xml(toString());


	QFile f( "src/interface/drivers.xml" );
    if ( f.open( IO_WriteOnly ) ) 
	{
        QTextStream stream( &f );
        for ( QStringList::Iterator it = xml.begin(); it != xml.end(); ++it )
            stream << *it << "\n";
        f.close();
    }
	else return 0;

	return 1;;
}


///Return names for available template drivers.
///\param list: vector os names.
int FileHandler::confDrivers(XmlDrivers& list)
{
	XmlDriverList::iterator it;
	
	for (it = confDrvList_.begin(); it != confDrvList_.end(); it++)
	{
		list.push_back(it->nick);
	}
	return 1;
}

///Insert a template driver.
///\param name: name of template. 
int FileHandler::insertTemplateDriver(const QString& name)
{
	XmlDriver driver;
	XmlDriverList::iterator it;
	driver.name = name;

	for (it = confDrvList_.begin(); it != confDrvList_.end(); it++)
	{
		QString object = it->nick;
		object.replace(" ", "");
		if(object == name)
			return 0;
	}

	confDrvList_.push_back(driver);

	return 1;
}

///Remove a template driver.
///\param name: name of template. 
int FileHandler::removeTemplateDriver(const QString& name)
{
	XmlDriverList::iterator it;
	
	bool found = false;
	for (it = confDrvList_.begin(); it != confDrvList_.end(); it++)
	{
		if(it->nick == name)
		{
			found = true;
			break;
		}
	}

	if (found)
		confDrvList_.erase(it);

	return 1;
}


///Return the template structure of driver.
int FileHandler::getTemplateDriver(const QString& name, XmlDriver& driver)
{
	XmlDriverList::iterator it;
	
	for (it = confDrvList_.begin(); it != confDrvList_.end(); it++)
	{
		if(it->nick == name)
			driver = *it;
	}
	return 1;
}


///Add or remove an option from the list.
///\param driver: name of model.
///\param option: name of option.
int FileHandler::editTemplateOption(const int& op, const QString& driver, const QPair<QString, QString>& option)
{
	QString aux;

	XmlDriverList::iterator it;
	XmlDriverOptions::iterator itOp;
	XmlDriverOptions::iterator itFound=NULL;
	
	for (it = confDrvList_.begin(); it != confDrvList_.end(); it++)
	{
		QString object = it->nick;
		object.replace(" ", "");
		if( (object == driver) || (driver == it->nick) )
		{
			bool exists = false;

			if (option.first == "name")
			{
				it->name = option.second;
			}
			else if (option.first == "description")
			{
				it->description = option.second;
			}
			else
			{
				for (itOp=it->options.begin(); itOp!=it->options.end(); itOp++)
				{
					QString objectProp = itOp->first;
					object.replace(" ", "");
					if(objectProp == option.first)
					{
						exists = true;
						itFound = itOp;
						break;
					}
				}

				if ((op == 1) && (exists))//remove
				{
					it->options.erase(itFound);	
				}
				else if (op == 3)//edit
				{
					if (!exists)
					{
						QString empty = "";
						it->options.push_back(qMakePair(option.second, empty));
					}
					else
						itFound->second = option.second;
					
				}
			}
		}
	}

	return 1;
}


//*********************************************************************
//*********************************************************************
//*********************************************************************


SimFileHandler::SimFileHandler()
    : FileHandler()
{
  dList_ = static_cast<unsigned int>(NULL);
  sList_ = static_cast<unsigned int>(NULL);
  configFile_ = "";

}


SimFileHandler::~SimFileHandler()
{
}

///Load the XML structure of the XML file.
///\param file: name of te file to be loaded.
///\return: status of operation.
int SimFileHandler::load( const QString& file )
{
	//open the file
	QFile f( file );
    if ( !f.open( IO_ReadOnly ) )
        return 0;
    if ( !setContent( &f ) ) {
        f.close();
        return 0;
    }
    f.close();

	configFile_ = file;

	QDomNode dn; //drivers node
	QDomNode sn; //sensors node

	QDomNode n = firstChild();

	dn = n.namedItem("drivers");
	sn = n.namedItem("sensors");

	QDomElement e;
	
	simConf.netFile = n.namedItem("file").toElement().text();
	simConf.steps = n.namedItem("steps").toElement().text().toInt();
	simConf.defDec = n.namedItem("default_deceleration").toElement().text().toDouble();
	simConf.sensorInterval = n.namedItem("sensor_interval").toElement().text().toInt();
	simConf.agentInterval = n.namedItem("agent_interval").toElement().text().toInt();
	simConf.carMaxSpeed = n.namedItem("car_max_speed").toElement().text().toInt();
	simConf.cellSize = n.namedItem("cell_size").toElement().text().toInt();
	simConf.iterationLength = n.namedItem("iteration_length").toElement().text().toInt();


	//DRIVERS
	if (!dn.isNull())
	{
		dn = dn.firstChild();

		while (!dn.isNull())
		{
			XmlDriver driver;
			XmlDriverOptions options;
			
			QDomNode op;//option node
			QDomElement ope;//option element
			
			driver.name = dn.namedItem("name").toElement().text();
			driver.nick = dn.namedItem("nick").toElement().text();
			driver.number = dn.namedItem("number").toElement().text().toInt();
			driver.state = dn.namedItem("state").toElement().text();
			driver.debug = dn.namedItem("debug").toElement().text();

			if (dn.namedItem("options").hasChildNodes())
			{
				op = dn.namedItem("options").firstChild();
				while (!op.isNull())
				{
					ope = op.toElement();
					options.push_back(qMakePair(ope.tagName(), ope.text()));
					op = op.nextSibling();
				}
			}

			if (dn.namedItem("routes").hasChildNodes())
			{
				op = dn.namedItem("routes").firstChild();
				while (!op.isNull())
				{
					Route r;
					r.laneset = op.namedItem("laneset").toElement().text().toInt();
										
					driver.route.push_back(r);
					op = op.nextSibling();
				}
			}

			driver.options = options;
			
			dList_.push_back(driver);
			
			dn = dn.nextSibling();
		}
	}


	//SENSORS
	if (!sn.isNull())
	{
		sn = sn.firstChild();

		while (!sn.isNull())
		{
			XmlSensor sensor;
						
			sensor.name = sn.namedItem("name").toElement().text();
			sensor.file = sn.namedItem("file").toElement().text();
			sensor.state = sn.namedItem("state").toElement().text();
			sensor.timetag = sn.namedItem("time_tag").toElement().text();
						
			sList_.push_back(sensor);
			
			sn = sn.nextSibling();
		}
	}

	return 1;
}

///Save the XML structure of the XML file.
///\param file: name of te file to be loaded.
///\return: status of operation.
int SimFileHandler::save(const QString& file)
{


  if ( (file == "") && (configFile_ == "") ) {
    return 0;
  }


	if ((file != "") && (file.right(4) != ".xml"))
		configFile_ = file + ".xml";
	else
        	configFile_ = file;


	QDomText t;
	QString aux = "";

	setContent(aux);

	//general
	QDomElement root = createElement( "config" );
    appendChild( root );

    QDomElement fn = createElement( "file" );
    root.appendChild( fn );

	t = createTextNode( simConf.netFile );
    fn.appendChild( t );

	QDomElement stn = createElement( "steps" );
    root.appendChild( stn );

	t = createTextNode( aux.setNum(simConf.steps) );
    stn.appendChild( t );

	QDomElement ddn = createElement( "default_deceleration" );
    root.appendChild( ddn );

	t = createTextNode( aux.setNum(simConf.defDec) );
    ddn.appendChild( t );

	QDomElement sin = createElement( "sensor_interval" );
    root.appendChild( sin );

        t = createTextNode( aux.setNum(simConf.sensorInterval) );
    sin.appendChild( t );

	QDomElement ain = createElement( "agent_interval" );
    root.appendChild( ain );

        t = createTextNode( aux.setNum(simConf.agentInterval) );
    ain.appendChild( t );
    
	QDomElement cms = createElement( "car_max_speed" );
    root.appendChild( cms );

        t = createTextNode( aux.setNum(simConf.carMaxSpeed) );
    cms.appendChild( t );
    
	QDomElement cs = createElement( "cell_size" );
    root.appendChild( cs );

        t = createTextNode( aux.setNum(simConf.cellSize) );
    cs.appendChild( t );
    
	QDomElement itl = createElement( "iteration_length" );
    root.appendChild( itl );

        t = createTextNode( aux.setNum(simConf.iterationLength) );
    itl.appendChild( t );

	//drivers
	QDomElement dn = createElement( "drivers" );
    root.appendChild( dn );

	if (dList_.isEmpty())
	{
		t = createTextNode( "" );
		dn.appendChild( t );
	}
	else //populate
	{
		QDomElement driver;
		QDomElement opn ;
		QDomElement on;
		QDomElement rn;
		
		XmlDriverList::iterator itD;
		for (itD = dList_.begin(); itD != dList_.end(); itD++)
		{
			driver = createElement( "driver" );
			dn.appendChild(driver);

			//name
			opn = createElement( "name" );
			driver.appendChild(opn);
			t = createTextNode( itD->name );
			opn.appendChild( t );

			//nick
			opn = createElement( "nick" );
			driver.appendChild(opn);
			t = createTextNode( itD->nick );
			opn.appendChild( t );

			//number
			opn = createElement( "number" );
			driver.appendChild(opn);
			t = createTextNode( aux.setNum(itD->number) );
			opn.appendChild( t );

			//state
			opn = createElement( "state" );
			driver.appendChild(opn);
			t = createTextNode( itD->state );
			opn.appendChild( t );

			//debug
			opn = createElement( "debug" );
			driver.appendChild(opn);
			t = createTextNode( itD->debug );
			opn.appendChild( t );

			//options
			on = createElement( "options" );
			driver.appendChild(on);

			XmlDriverOptions options = itD->options;

			if (options.isEmpty())
			{
				t = createTextNode( "" );
				on.appendChild( t );
			}
			else
			{
				QDomElement temp;//rever

				XmlDriverOptions::iterator itOp;
				for (itOp = options.begin(); itOp != options.end(); itOp++)
				{
					QString p = itOp->first;
					//option
					temp = createElement( itOp->first );
					on.appendChild(temp);
					t = createTextNode( itOp->second );
					
					//int legal = itOp->second.toInt();

					temp.appendChild( t );
				}
			}

			//routes
			rn = createElement( "routes" );
			driver.appendChild(rn);

			RouteList rl = itD->route;

			if (rl.isEmpty())
			{
				t = createTextNode( "" );
				rn.appendChild( t );
			}
			else
			{
				QDomElement temp;//rever
				QDomElement item;

				RouteList::iterator itRl;
				for (itRl = rl.begin(); itRl != rl.end(); itRl++)
				{
					temp = createElement("route");
					rn.appendChild(temp);

					item = createElement( "laneset" );
					t = createTextNode( aux.setNum(itRl->laneset) );
					item.appendChild( t );
					temp.appendChild(item);
				}
			}

		}
	}

	//sensors
	QDomElement sn = createElement( "sensors" );
    root.appendChild( sn );

	if (sList_.isEmpty())
	{
		t = createTextNode( "" );
		sn.appendChild( t );
	}
	else //populate
	{
		QDomElement sensor;
		QDomElement opn ;
		
		XmlSensorList::iterator itS;
		for (itS = sList_.begin(); itS != sList_.end(); itS++)
		{
			sensor = createElement( "sensor" );
			sn.appendChild(sensor);

			//name
			opn = createElement( "name" );
			sensor.appendChild(opn);
			t = createTextNode( itS->name );
			opn.appendChild( t );

			//file
			opn = createElement( "file" );
			sensor.appendChild(opn);
			t = createTextNode( itS->file );
			opn.appendChild( t );

			//state
			opn = createElement( "state" );
			sensor.appendChild(opn);
			t = createTextNode( itS->state );
			opn.appendChild( t );
			
			//timetag
			opn = createElement( "time_tag" );
			sensor.appendChild(opn);
			t = createTextNode( itS->timetag );
			opn.appendChild( t );
		}
	}

    QStringList xml(toString());


	QFile f( configFile_ );
    if ( f.open( IO_WriteOnly ) ) 
	{
        QTextStream stream( &f );
        for ( QStringList::Iterator it = xml.begin(); it != xml.end(); ++it )
            stream << *it << "\n";
        f.close();
    }
	else return 0;

	return 1;
}

///Get the names of the drivers on the file.
///\param list: list fo drivers names to be populated.
///\return: status of operation.
int SimFileHandler::drivers(XmlDrivers& list)
{
	XmlDriverList::iterator it;
	
	for (it = dList_.begin(); it != dList_.end(); it++)
	{
		list.push_back(it->nick);
	}
	return 1;
}

///Get the names of the drivers on the file.
///\param list: list fo drivers names to be populated.
///\return: status of operation.
int SimFileHandler::sensors(XmlSensors& list)
{
	XmlSensorList::iterator it;
	
	for (it = sList_.begin(); it != sList_.end(); it++)
	{
		list.push_back(it->name);
	}
	return 1;
}


///Get the driver properties.
///\param name: name of the driver.
///\param driver: structure os driver.
///\return: status of operation.
int SimFileHandler::getDriver(const QString& name, XmlDriver& driver)
{
	XmlDriverList::iterator it;
	
	for (it = dList_.begin(); it != dList_.end(); it++)
	{
		if(it->nick == name)
			driver = *it;
	}
	return 1;
}

///Get the sensor properties.
///\param name: name of the sensor.
///\param sensor: structure os sensor.
///\return: status of operation.
int SimFileHandler::getSensor(const QString& name, XmlSensor& sensor)
{
	XmlSensorList::iterator it;
	
	for (it = sList_.begin(); it != sList_.end(); it++)
	{
		if(it->name == name)
			sensor = *it;
	}
	return 1;
}


///Set the driver properties.
///\param name: name of the driver.
///\param driver: structure os driver.
///\return: status of operation.
int SimFileHandler::setDriver(QString name, XmlDriver driver)
{
	XmlDriverList::iterator it;

	bool found = false;
	bool removed = true;
	
	for (it = dList_.begin(); it != dList_.end(); it++)
	{
		if(it->nick == name)
		{
			found = true;
			break;
		}
	}

	if (found) 
	{
		removed =  removeDriver(name);
	}

	if (removed)	
	{
		dList_.push_back(driver);
	}

	return 1;
}

///Set the sensor properties.
///\param name: name of the sensor.
///\param sensor: structure os sensor.
///\return: status of operation.
int SimFileHandler::setSensor(QString name, XmlSensor sensor)
{
	XmlSensorList::iterator it;
	
	for (it = sList_.begin(); it != sList_.end(); it++)
	{
		QString object = it->name;
		object.replace(" ", "");
		if(object == name)
			removeSensor(object);
	}

	sList_.push_back(sensor);
	return 1;
}

///Clear the class.
void SimFileHandler::localClear()
{
	configFile_ = "";
	XmlDriverList newDriverList;
	XmlSensorList newSensorList;
	XmlSimConf newConf;

	dList_ = newDriverList;
	sList_ = newSensorList;
	simConf = newConf;
}


///Remove the driver from the list.
///\param driver: driver to be removed.
int SimFileHandler::removeDriver(QString driver)
{
	XmlDriverList::iterator it;
	bool found = false;
	
	for (it = dList_.begin(); it != dList_.end(); it++)
	{
		if(it->nick == driver)
		{
			found = true;
			dList_.erase(it);
			break;
		}
	}

	
	if (found)
		return 1;
	
	return 0;
}


///Remove the sensor from the list.
///\param sensor: sensor to be removed.
int SimFileHandler::removeSensor(QString sensor)
{
	XmlSensorList::iterator it;
	bool found = false;
	
	for (it = sList_.begin(); it != sList_.end(); it++)
	{
		if(it->name == sensor)
		{
			found = true;
                        sList_.erase(it);
			break;
		}
	}


	if (found)
		return 1;

	return 0;
}


///Insert a driver.
///\param driver: driver to be inserted.
int SimFileHandler::insertDriver(const QString& driver)
{
	XmlDriverList::iterator it;
	QString aux;
	
	for (it = dList_.begin(); it != dList_.end(); it++)
	{
		if(it->name == driver)
			return 0;
	}

	XmlDriver newD;

	//todo: check if driver model exists
	
	getTemplateDriver(driver, newD);
	newD.nick = driver + "_" + aux.setNum(dList_.size());
	newD.number = 1;
	newD.state = "ON";
	newD.debug = "OFF";

	dList_.push_back(newD);

	return 1;
}

///Insert a sensor.
///\param sensor: sensor to be inserted.
int SimFileHandler::insertSensor(const QString& sensor)
{
	XmlSensorList::iterator it;
	
	for (it = sList_.begin(); it != sList_.end(); it++)
	{
		QString object = it->name;
		object.replace(" ", "");
		if(object == sensor)
			return 0;
	}

	XmlSensor newS;

	QDir dir("output/");

	newS.name = sensor;
	newS.file = dir.absPath() + "/" + sensor + ".txt";
	newS.state = "ON";
	newS.timetag = "NO";

	sList_.push_back(newS);

	return 1;
}

///Change some property of a driver.
int SimFileHandler::changeDriverProperty(const QString& driver, QPair<QString, QString> prop)
{
	QString aux;

	//cout << "ill change from " << driver.ascii() << endl;

	XmlDriverList::iterator it;
	XmlDriverOptions::iterator itOp;
	
	for (it = dList_.begin(); it != dList_.end(); it++)
	{
		QString object = it->nick;
		object.replace(" ", "");
		if( (object == driver) || (driver == it->nick) )
		{
			//cout << "i found " << it->nick.ascii() << endl;

			QString item = prop.first;
			if (item == "name")
				it->name = prop.second;
			else if(item == "nick")
				it->nick = prop.second.toInt();
			else if(item == "number")
				it->number = prop.second.toInt();
			else if(item == "state")
				it->state = prop.second;
			else if(item == "debug")
				it->debug = prop.second;
			else
			{
				for (itOp=it->options.begin(); itOp!=it->options.end(); itOp++)
				{
					QString objectProp = itOp->first;
					object.replace(" ", "");
					if(objectProp == item)
					{
						itOp->second = prop.second;
						break;
					}
				}
			}
		}
	}

	return 1;
}

///Change some property of a sensor.
int SimFileHandler::changeSensorProperty(const QString& sensor, QPair<QString, QString> prop)
{
	XmlSensorList::iterator it;
	
	for (it = sList_.begin(); it != sList_.end(); it++)
	{
		QString object = it->name;
		object.replace(" ", "");
		if( (object == sensor) || (sensor == it->name) )
		{
			QString item = prop.first;
			if (item == "name")
				it->name = prop.second;
			else if(item == "file")
				it->file = prop.second;
			else if(item == "state")
				it->state = prop.second;
			else if (item == "timetag")
				it->timetag = prop.second;
		}
	}

	return 1;

}


///Check structures.
//needs optimization
int SimFileHandler::checkDriverTemplate()
{
	XmlDriverList::iterator itT;
	XmlDriverList::iterator itO;
	XmlDriverOptions temp;

	XmlDriverOptions::iterator itOp;
	XmlDriverOptions::iterator itOp2;
	
	for (itO = dList_.begin(); itO != dList_.end(); itO++)
	{
		for (itT = confDrvList_.begin(); itT!=confDrvList_.end(); itT++)
		{
			QString object = itO->nick;
			object.replace(" ", "");
			if( (object == itT->nick) || (itT->nick == itO->nick) )//driver == template
			{
				temp = itT->options;

				itOp = temp.begin();

				for (itOp2 = itO->options.begin(); itOp2!=itO->options.end(); itOp2++)
				{
					if ((itOp != NULL) && (itOp!=temp.end()) && (itOp2 != NULL))
					{
						if ( (itT->options.size() < itO->options.size())//option removed
							&& (itOp->first != itOp2->first) )
							itO->options.erase(itOp2);
						
						if (itOp2 != NULL)
						{
							itOp->second = itOp2->second;
							itOp++;
						}
						
					}
					
				}

				itO->options = temp;

				break;
			}
		}
	}

  
	return 1;
}


///Change names of templatesXdrivers.
///\param old: old name.
///\param actual: new name.
int SimFileHandler::changeDriversName(const QString& old, const QString& actual)
{
	XmlDriverList::iterator it;
	
	for (it = dList_.begin(); it != dList_.end(); it++)
	{
		QString object = it->nick;
		object.replace(" ", "");
		if( (object == old) || (old == it->name) )
		{
			it->name = actual;
		}
	}
	return 1;
}

//*********************************************************************
//*********************************************************************
//*********************************************************************


NetHandler::NetHandler()
	: QDomDocument()
{
	loaded_ = false;

}

NetHandler::~NetHandler()
{
}


///Load the configuration file that defines templates for drivers.
int NetHandler::load(const QString& file)
{
	TiXmlElement* simulation;
	
	TiXmlDocument doc( file.ascii() );
	bool loadOkay = doc.LoadFile();
	
	if ( !loadOkay )
    	{
		printf( "Could not load file. Error='%s'.\nExiting.\n", 
			doc.ErrorDesc());
		return 0;
    	}

	loaded_ = true;
	
	
	simulation = doc.FirstChildElement( "simulation" );
	

	TiXmlElement* nodes;   //node
	TiXmlElement* sources; //sources node
	TiXmlElement* sinks;   //sinks node
	TiXmlElement* streets; //streets node
	TiXmlElement* traffic_lights;

	nodes = simulation->FirstChildElement( "nodes" );
	sources = simulation->FirstChildElement( "sources" );
	sinks = simulation->FirstChildElement( "sinks" );
	streets = simulation->FirstChildElement( "streets" );
	traffic_lights = simulation->FirstChildElement( "traffic_lights" );

	
	netId_ = atoi(simulation->FirstChildElement( "network_id" )->FirstChild()->Value());
	netName_ = simulation->FirstChildElement( "network_name" )->FirstChild()->Value();

 	lanesets_.clear();
	nodes_.clear();

	parseNodes_(nodes, sources, sinks);
	parseTls_(traffic_lights);
	parseStreets_(streets);

	return 1;

}

///Parse the node structure setting sources and sinks.
///\param nodes: nodes structure.
///\param sources: sources structure.
///\param sinks: sinks structure.
///\return: status of operation.
int NetHandler::parseNodes_(TiXmlElement* nodes, TiXmlElement* sources, TiXmlElement* sinks)
{

  
  
	QValueList<int> sourcesIds;
	QValueList<int> sinksIds;

	//sources
	TiXmlElement* source;
	for(source = sources->FirstChildElement("source"); source; source = source->NextSiblingElement("source")) 
	{
		sourcesIds.push_back(atoi(source->FirstChildElement("located_at_node")->FirstChild()->Value()));
	}

	//sinks
	TiXmlElement* sink;
	for(sink = sinks->FirstChildElement("sink"); sink; sink = sink->NextSiblingElement("sink")) 
	{
		sinksIds.push_back(atoi(sink->FirstChildElement("located_at_node")->FirstChild()->Value()));
	}

	//nodes
	TiXmlElement* node;
	for(node = nodes->FirstChildElement("node"); node; node = node->NextSiblingElement("node")) 
	{
		Node nd;

		nd.id = atoi(node->FirstChildElement("node_id")->FirstChild()->Value());
		nd.hastl = false;
		nd.name = node->FirstChildElement("node_name")->FirstChild()->Value();
		nd.x = atoi(node->FirstChildElement("x_coord")->FirstChild()->Value());
		nd.y = atoi(node->FirstChildElement("y_coord")->FirstChild()->Value());

		if (sourcesIds.contains(nd.id))
			nd.type = SOURCE;
		else if (sinksIds.contains(nd.id))
			nd.type = SINK;
		else
			nd.type = NODE;

		nodes_.push_back(nd);
	}

	return 1;
}

///Parse the node structure setting sources and sinks.
///\param streets: streets structure.
///\return: status of operation.
int NetHandler::parseStreets_(TiXmlElement* streets)
{
	TiXmlElement* sections;
	TiXmlElement* lanesets;
	TiXmlElement* turnings;

	//streets
	TiXmlElement* street;
	for(street = streets->FirstChildElement("street"); street; street = street->NextSiblingElement("street")) 
	{
		//sections
		sections = street->FirstChildElement("sections");

		TiXmlElement* section;
		section = sections->FirstChildElement("section");
		for(section = sections->FirstChildElement("section"); section; section = section->NextSiblingElement("section")) 
		{
			//lanesets
			lanesets = section->FirstChildElement("lanesets");
			TiXmlElement* laneset;
			for(laneset = lanesets->FirstChildElement("laneset"); laneset; laneset = laneset->NextSiblingElement("laneset")) 
			{
				Laneset ls;

				ls.id = atoi(laneset->FirstChildElement("laneset_id")->FirstChild()->Value());
				ls.begin = atoi(laneset->FirstChildElement("start_node")->FirstChild()->Value());
				ls.end = atoi(laneset->FirstChildElement("end_node")->FirstChild()->Value());
				ls.sectionId = atoi(section->FirstChildElement("section_id")->FirstChild()->Value());
				ls.sectionName = atoi(section->FirstChildElement("section_name")->FirstChild()->Value());

				turnings = laneset->FirstChildElement("turning_probabilities");
				TiXmlElement* turning;
				for(turning = turnings->FirstChildElement("turning"); turning; turning = turning->NextSiblingElement("turning")) 
				{
					Destination dir;

					dir.id = atoi(turning->FirstChildElement("destination_laneset")->FirstChild()->Value());
					dir.prob = atoi(turning->FirstChildElement("probability")->FirstChild()->Value());

					ls.destinations.push_back(dir);
				}

				lanesets_.push_back(ls);
			}
		}
	}


	return 1;
}

int NetHandler::parseTls_(TiXmlElement* tls)
{
	//tls
	TiXmlElement* tl;
	for(tl = tls->FirstChildElement("traffic_light"); tl; tl = tl->NextSiblingElement("traffic_light")) 
	{
		int tlid = atoi(tl->FirstChildElement("traffic_light_id")->FirstChild()->Value());
		int nodeid = atoi(tl->FirstChildElement("located_at_node")->FirstChild()->Value());

		NodeList::iterator it;
		for (it = nodes_.begin(); it != nodes_.end(); it++)
		{
			if (it->id == nodeid)
			{
				it->tlid = tlid;
				it->hastl = true;
				break;
			}
		}
	}


	return 1;
}

///Returns a string with the structure of the net.
//not complete
QString NetHandler::toString()
{
	QString ans;
	QString aux;

	LanesetList::iterator it;

	for (it = lanesets_.begin(); it != lanesets_.end(); it++)
	{
		ans += "Laneset: " + aux.setNum(it->id) + "\n";
	}

	return ans;
}

TLList NetHandler::nodeToTls(TLList list)
{	
	TLList ans;

	NodeList::iterator it;
	TLList::iterator it2;

	for (it2 = list.begin(); it2 != list.end(); it2++)
		for (it = nodes_.begin(); it != nodes_.end(); it++)
		{
			if ( (it->id == *it2) && (it->hastl) )
			{
				ans.push_back(it->tlid);
			}
		}
	return ans;
}

TLList NetHandler::tlToLaneset(TLList list)
{	
	TLList ans;

	LanesetList::iterator it;
	TLList::iterator it2;

	for (it2 = list.begin(); it2 != list.end(); it2++)
		for (it = lanesets_.begin(); it != lanesets_.end(); it++)
		{
			if ( (getNode(it->begin).tlid == *it2))
			{
				ans.push_back(it->id);
				break;
			}
		}
	return ans;
}

Node NetHandler::getNode(int id)
{
	Node ans;
	NodeList::iterator it;

	for (it = nodes_.begin(); it != nodes_.end(); it++)
	{
		if (it->id == id)
		{
			ans = *it;
		}
	}
	return ans;
}

QPoint NetHandler::nodeLocation(const int& nodeId)
{
	NodeList::iterator it;

	QPoint ans;

	for (it=nodes_.begin(); it!=nodes_.end(); it++)
	{
		if (it->id == nodeId)
		{
			QPoint point(it->x, it->y);
			ans = point;
		}
	}

	return ans;
}

QPoint NetHandler::xRange()
{
	NodeList::iterator it;

	int max = 0;
	int min = 100000;

	for (it=nodes_.begin(); it!=nodes_.end(); it++)
	{
		if (it->x > max)
			max = it->x;
		if (it->x < min)
			min = it->x;
	}

	QPoint range(min,max);
	return range;
}

QPoint NetHandler::yRange()
{
	NodeList::iterator it;

	int max = 0;
	int min = 100000;

	for (it=nodes_.begin(); it!=nodes_.end(); it++)
	{
		if (it->y > max)
			max = it->y;
		if (it->y < min)
			min = it->y;
	}

	QPoint range(min,max);
	return range;
}

QPair<QString, QString> NetHandler::lanesetNodes(const int& id)
{
	QPair<QString, QString> ans;

	LanesetList::iterator it;

	for (it = lanesets_.begin(); it != lanesets_.end(); it++)
	{
		if (it->id == id)
		ans = qMakePair(nodeName_(it->begin), nodeName_(it->end));
	}

	return ans;
}

QString NetHandler::nodeName_(const int& id)
{
	QString ans;

	NodeList::iterator it;

	for (it = nodes_.begin(); it != nodes_.end(); it++)
	{
		if (it->id == id)
			ans = it->name;
	}

	return ans;
}

int NetHandler::nodeId_(const QString& name)
{
	int ans=-1;

	NodeList::iterator it;

	for (it = nodes_.begin(); it != nodes_.end(); it++)
	{
		if (it->name == name)
			ans = it->id;
	}

	return ans;
}


QValueVector<QString> NetHandler::nextNodes(const QString& from)
{
	QValueVector<QString> ans;

	LanesetList::iterator it;

	for (it = lanesets_.begin(); it != lanesets_.end(); it++)
	{
		if (nodeName_(it->begin) == from)
			ans.push_back(nodeName_(it->end));
	}
	return ans;
}

int NetHandler::definedLaneset(const QString& from, const QString& to)
{
	int ans = -1;

	LanesetList::iterator it;

	for (it = lanesets_.begin(); it != lanesets_.end(); it++)
	{
		if ( (nodeName_(it->begin) == from) &&
			(nodeName_(it->end) == to) )
			ans = it->id;
	}
	return ans;
}


//*********************************************************************
//*********************************************************************
//*********************************************************************


AgentHandler::AgentHandler()
	: QDomDocument()
{
	loaded = false;

}

AgentHandler::~AgentHandler()
{
}

#define VALUE(element) (element->FirstChild()->Value())

int AgentHandler::load(const QString& file)
{
	TiXmlElement *agents;

	TiXmlDocument doc( file.ascii() );
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay )
	{
		printf( "Could not load file. Error='%s'.\nExiting.\n", 
				doc.ErrorDesc());
		exit(1);
	}

	agents = doc.FirstChildElement( "agents" );

	TiXmlElement *agent;
	TiXmlElement *name;
	TiXmlElement *nickname;
	
	TiXmlElement *memsize;
	TiXmlElement *alpha;
	TiXmlElement *action;
	TiXmlElement *minphase;
	TiXmlElement *states;
	TiXmlElement *algorithm;
	TiXmlElement *coordneighbors;
	TiXmlElement *gamma;
	TiXmlElement *starttime;
	TiXmlElement *learningrate;
	TiXmlElement *discountrate;
	TiXmlElement *firstphase;
	TiXmlElement *secondphase;
	TiXmlElement *tau; //same for DCOP tau's
	
	TiXmlElement *log;
	TiXmlElement *tl;
	TiXmlElement *tls;
	
	for(agent = agents->FirstChildElement("agent");agent; agent = agent->NextSiblingElement("agent")) 
	{
		XmlAgent la;

		name = agent->FirstChildElement("type");
		string agentName = VALUE(name);
		la.name = agentName;
		
		nickname = agent->FirstChildElement("name");
		string agentNickName = VALUE(nickname);
		la.nickname = agentNickName;
		
		//Greedy agents
		if (agentName == "Greedy")
		{
			memsize = agent->FirstChildElement("memsize");
			string agentMemSize = VALUE(memsize);
			la.memsize = agentMemSize;
			
			alpha = agent->FirstChildElement("alpha");
			string agentAlpha = VALUE(alpha);
			la.alpha = agentAlpha;
			
			minphase = agent->FirstChildElement("minphase");
			string agentMinPhase = VALUE(minphase);
			la.minphase = agentMinPhase;
			
			action = agent->FirstChildElement("action");
			string agentAction = VALUE(action);
			la.action = agentAction;
		}
		
		//Q-learning agents
		if (agentName == "Q-learning")
		{
			memsize = agent->FirstChildElement("memsize");
			string agentMemSize = VALUE(memsize);
			la.memsize = agentMemSize;
			
			states = agent->FirstChildElement("states");
			string agentStates = VALUE(states);
			la.states = agentStates;
			
			//start time
			starttime = agent->FirstChildElement("start_time");
			string agentStartTime = VALUE(starttime);
			la.starttime = agentStartTime;
			
			//learning rate
			learningrate = agent->FirstChildElement("learning_rate");
			string agentLearningRate = VALUE(learningrate);
			la.learningrate = agentLearningRate;
			
			//discount rate
			discountrate = agent->FirstChildElement("discount_rate");
			string agentDiscountRate = VALUE(discountrate);
			la.discountrate = agentDiscountRate;
				
		}
		
		//DCOP agents
		if (agentName == "DCOP")
		{
			//algorithm
			algorithm = agent->FirstChildElement("algorithm");
			string agentAlgorithm = VALUE(algorithm);
			la.algorithm = agentAlgorithm;
			
			//coordinated neighbors
			coordneighbors = agent->FirstChildElement("coordneighbors");
			string agentCoordNeighbors = VALUE(coordneighbors);
			la.coordneighbors = agentCoordNeighbors;
			
			//alpha
			alpha = agent->FirstChildElement("alpha");
			string agentAlpha = VALUE(alpha);
			la.alpha = agentAlpha;
			
			//gamma
			gamma = agent->FirstChildElement("gamma")->FirstChildElement("synchronized");
			string agentGamma = VALUE(gamma);
			la.gammasync = agentGamma;
			
			gamma = agent->FirstChildElement("gamma")->FirstChildElement("unsynchronized");
			agentGamma = VALUE(gamma);
			la.gammaunsync = agentGamma;
			
			//tau
			tau = agent->FirstChildElement("tau")->FirstChildElement("up_agree")->FirstChildElement("down_agree");
			string agentTau = VALUE(tau);
			la.tau_agr_agr = agentTau;
			
			tau = agent->FirstChildElement("tau")->FirstChildElement("up_agree")->FirstChildElement("down_not_agree");
			agentTau = VALUE(tau);
			la.tau_agr_not = agentTau;
			
			tau = agent->FirstChildElement("tau")->FirstChildElement("up_not_agree")->FirstChildElement("down_agree");
			agentTau = VALUE(tau);
			la.tau_not_agr = agentTau;
			
			tau = agent->FirstChildElement("tau")->FirstChildElement("up_not_agree")->FirstChildElement("down_not_agree");
			agentTau = VALUE(tau);
			la.tau_not_not = agentTau;
			
			//	TiXmlElement *tau;
		}
		
		
		//supervised learning agents
		if (agentName == "supervised-learning")
		{
			//memory size
			memsize = agent->FirstChildElement("memsize");
			string agentMemSize = VALUE(memsize);
			la.memsize = agentMemSize;
			
			//number of states
			states = agent->FirstChildElement("states");
			string agentStates = VALUE(states);
			la.states = agentStates;
			
			//start time
			starttime = agent->FirstChildElement("start_time");
			string agentStartTime = VALUE(starttime);
			la.starttime = agentStartTime;
			
			//learning rate
			learningrate = agent->FirstChildElement("learning_rate");
			string agentLearningRate = VALUE(learningrate);
			la.learningrate = agentLearningRate;
			
			//discount rate
			discountrate = agent->FirstChildElement("discount_rate");
			string agentDiscountRate = VALUE(discountrate);
			la.discountrate = agentDiscountRate;
			
			//first phase
			firstphase = agent->FirstChildElement("first_phase");
			string agentFirstPhase = VALUE(firstphase);
			la.firstphase = agentFirstPhase;
			
			//second phase
			secondphase = agent->FirstChildElement("second_phase");
			string agentSecondPhase = VALUE(secondphase);
			la.secondphase = agentSecondPhase;
			
			//tau
			tau = agent->FirstChildElement("tau");
			string agentTau = VALUE(tau);
			la.tau = agentTau;
		}
		
		log = agent->FirstChildElement("log");
		string agentLog;
		agentLog = static_cast<string> (VALUE(log));
		la.log = agentLog;
		
		tls = agent->FirstChildElement("trafficlights");
			
		TLList ids;
		if (tls != 0) 
		{	
			for(tl = tls->FirstChildElement("trafficlight"); tl;
				tl = tl->NextSiblingElement("trafficlight"))
			{	
				int id = atoi(VALUE(tl));
				ids.push_back(id);
			}
		}
		la.tls = ids;
		
		//insert agent into list
		al.push_back(la);
	}

	loaded = true;
	agentFile = file;
	return 1;
}

int AgentHandler::save(const QString& file)
{
	if (file != "")
		agentFile = file;

        if ( (file == "") && (agentFile == "") )
                return 0;

	if (agentFile.right(4) != ".xml")
		agentFile += ".xml";

        QDomText t;
        QString aux = "";

        setContent(aux);

        //general
        QDomElement root = createElement( "agents" );
    	appendChild( root );

	if (al.isEmpty())
        {
                t = createTextNode( "" );
                root.appendChild( t );
        }
        else //populate
        {
                QDomElement agent;
                QDomElement opn ;
                QDomElement on;
                QDomElement temp;
                
                XmlAgentList::iterator it;

                for (it = al.begin(); it != al.end(); it++)
                {
                        agent = createElement( "agent" );
                        root.appendChild(agent);

                        //name
                        opn = createElement( "type" );
                        agent.appendChild(opn);
                        t = createTextNode( it->name );
                        opn.appendChild( t );
						
                        //name
                        opn = createElement( "name" );
                        agent.appendChild(opn);
                        t = createTextNode( it->nickname );
                        opn.appendChild( t );
                        						
						//Greedy Agents
						if (it->name == "Greedy")
						{
							//memsize
							opn = createElement( "memsize" );
							agent.appendChild(opn);
							t = createTextNode( it->memsize );
							opn.appendChild( t );
							
							//alpha
							opn = createElement( "alpha" );
							agent.appendChild(opn);
							t = createTextNode( it->alpha );
							opn.appendChild( t );		

							//minphase
							opn = createElement( "minphase" );
							agent.appendChild(opn);
							t = createTextNode( it->minphase );
							opn.appendChild( t );
							
							opn = createElement( "action" );
							agent.appendChild(opn);
							t = createTextNode( it->action );
							opn.appendChild( t );
						}
						
						//Q-learning Agents
						if (it->name == "Q-learning")
						{
							//memsize
							opn = createElement( "memsize" );
							agent.appendChild(opn);
							t = createTextNode( it->memsize );
							opn.appendChild( t );
							
							//states
							opn = createElement( "states" );
							agent.appendChild(opn);
							t = createTextNode( it->states );
							opn.appendChild( t );				
							
							//start time
							opn = createElement ( "start_time" );
							agent.appendChild(opn);
							t = createTextNode( it->starttime );
							opn.appendChild( t );
							
							//learning rate
							opn = createElement ( "learning_rate" );
							agent.appendChild(opn);
							t = createTextNode( it->learningrate );
							opn.appendChild( t );
							
							//discount rate
							opn = createElement ( "discount_rate" );
							agent.appendChild(opn);
							t = createTextNode( it->discountrate );
							opn.appendChild( t );
							
						}
						
						//DCOP Agents
						if (it->name == "DCOP")
						{
							//algorithm
							opn = createElement( "algorithm" );
							agent.appendChild(opn);
							t = createTextNode( it->algorithm );
							opn.appendChild( t );

							//coordneighbors
							opn = createElement( "coordneighbors" );
							agent.appendChild(opn);
							t = createTextNode( it->coordneighbors );
							opn.appendChild( t );
							
							//alpha
							opn = createElement( "alpha" );
							agent.appendChild(opn);
							t = createTextNode( it->alpha );
							opn.appendChild( t );
							
							//gamma
							on = createElement( "gamma" );
							agent.appendChild(on);
							opn = createElement( "synchronized" );
							on.appendChild(opn);
							t = createTextNode( it->gammasync );
							opn.appendChild( t );
							
							opn = createElement( "unsynchronized" );
							on.appendChild(opn);
							t = createTextNode( it->gammaunsync );
							opn.appendChild( t );

							//tau
							temp = createElement( "tau" );
							agent.appendChild(temp);
							
							on = createElement( "up_agree" );
							temp.appendChild(on);
							
							opn = createElement( "down_agree" );
							on.appendChild(opn);
							t = createTextNode( it->tau_agr_agr );
							opn.appendChild( t );
							
							opn = createElement( "down_not_agree" );
							on.appendChild(opn);
							t = createTextNode( it->tau_agr_not );
							opn.appendChild( t );
							
							on = createElement( "up_not_agree" );
							temp.appendChild(on);
							
							opn = createElement( "down_agree" );
							on.appendChild(opn);
							t = createTextNode( it->tau_not_agr );
							opn.appendChild( t );
							
							opn = createElement( "down_not_agree" );
							on.appendChild(opn);
							t = createTextNode( it->tau_not_not );
							opn.appendChild( t );

						}
						
						//supervised-learning agents
						if (it->name == "supervised-learning")
						{
							//memsize
							opn = createElement( "memsize" );
							agent.appendChild(opn);
							t = createTextNode( it->memsize );
							opn.appendChild( t );
							
							//states
							opn = createElement( "states" );
							agent.appendChild(opn);
							t = createTextNode( it->states );
							opn.appendChild( t );				
							
							//start time
							opn = createElement ( "start_time" );
							agent.appendChild(opn);
							t = createTextNode( it->starttime );
							opn.appendChild( t );
							
							//learning rate
							opn = createElement ( "learning_rate" );
							agent.appendChild(opn);
							t = createTextNode( it->learningrate );
							opn.appendChild( t );
							
							//discount rate
							opn = createElement ( "discount_rate" );
							agent.appendChild(opn);
							t = createTextNode( it->discountrate );
							opn.appendChild( t );
							
							//first phase
							opn = createElement ( "first_phase" );
							agent.appendChild(opn);
							t = createTextNode( it->firstphase );
							opn.appendChild( t );
							
							//first phase
							opn = createElement ( "second_phase" );
							agent.appendChild(opn);
							t = createTextNode( it->secondphase );
							opn.appendChild( t );
							
							//tau
							opn = createElement ( "tau" );
							agent.appendChild(opn);
							t = createTextNode( it->tau );
							opn.appendChild( t );
							
						}			
						
                        //log
                        opn = createElement( "log" );
                        agent.appendChild(opn);
                        t = createTextNode( it->log );
                        opn.appendChild( t );						
						
						//tls
                        on = createElement( "trafficlights" );
                        agent.appendChild(on);

                        TLList tls = it->tls;

                        if (tls.isEmpty())
                        {
                                t = createTextNode( "" );
                                on.appendChild( t );
                        }
                        else
                        {
								QString aux;

                                TLList::iterator it;
                                for (it = tls.begin(); it != tls.end(); it++)
                                {
										temp = createElement( "trafficlight" );
                                        t = createTextNode( aux.setNum(*it) );
                                        temp.appendChild( t );
										on.appendChild( temp );
                                }
                        }
				}
	}


    QStringList xml(toString());

    QFile f( agentFile );
    if ( f.open( IO_WriteOnly ) )
        {
        QTextStream stream( &f );
        for ( QStringList::Iterator it = xml.begin(); it != xml.end(); ++it )
            stream << *it << "\n";
        f.close();
    }
    else return 0;

    return 1;
}

void AgentHandler::changeLogStat(QString name, QString stat)
{
	XmlAgentList::iterator it;
	//bool found = false;

	for (it = al.begin(); it != al.end(); it++)
	{
		if(it->name == name)
		{
			it->log = stat;
			break;
		}
	}
}

int AgentHandler::setAgentTls(QString name, TLList list)
{
	XmlAgentList::iterator it;
	//bool found = false;

	for (it = al.begin(); it != al.end(); it++)
	{
		if(it->nickname == name)
		{
			it->tls = list;
			break;
		}
	}

	return static_cast<unsigned int>(NULL);
}

TLList AgentHandler::agentTls(QString name)
{
	XmlAgentList::iterator it;
	//bool found = false;

	for (it = al.begin(); it != al.end(); it++)
	{
		if(it->nickname == name)
		{
			return it->tls;
		}
	}

	return static_cast<unsigned int>(NULL);
}

int AgentHandler::addAgent(QString name, QString nickname)
{
	XmlAgent agent;
	agent.name = name;
	agent.nickname = nickname;
	agent.log = "on";
	
	//initial values for Greedy agents
	if (name == "Greedy")
	{
		agent.memsize = "300";
		agent.alpha = "0.2";
		agent.minphase = "15";
		agent.action = "modify_plan";
	}
	
	//initial values for Q-Learning agents
	if (name == "Q-learning")
	{
		agent.memsize = "300";
		agent.starttime = "300";
		agent.states = "1";
		agent.learningrate = "0.5";
		agent.discountrate = "0.9";
	}
	
	//initial values for DCOP agents
	if (name == "DCOP")
	{
		agent.algorithm = "ADOPT";
		agent.coordneighbors = "yes";
		agent.alpha = "0.5";
		agent.gammasync = "0.5";
		agent.gammaunsync = "0.5";
		agent.tau_agr_agr = "0.5";
		agent.tau_agr_not = "0.5";
		agent.tau_not_agr = "0.5";
		agent.tau_not_not = "0.5";
	}
	
	//initial values for Supervised Learning agents
	if (name == "supervised-learning")
	{
		agent.memsize = "60";
		agent.states = "3";
		agent.starttime = "360";
		agent.learningrate = "1.0";
		agent.discountrate = "0.0";
		agent.firstphase = "6000";
		agent.secondphase = "12000";
		agent.tau = "0.1";
	}
	
	al.push_back(agent);

	return 1;
}

///Get the agent properties.
///\param name: name of the agent.
///\param driver: structure os agent.
///\return: status of operation.
int AgentHandler::getAgent(const QString& name, XmlAgent& agent)
{
	XmlAgentList::iterator it;
	
	for (it = al.begin(); it != al.end(); it++)
	{
		if(it->nickname == name)
			agent = *it;
	}
	return 1;
}

int AgentHandler::changeAgentProperty(QString agentName, QPair<QString, QString> prop) {
	
	
	XmlAgentList::iterator it;
	
	for (it = al.begin(); it != al.end(); it++)
	{
		if(it->nickname == agentName)
		{
			if (prop.first == "Agent Name")
				it->nickname = prop.second;
			if (prop.first == "Log")
				it->log = prop.second;
			if (prop.first == "Memory Size")
				it->memsize = prop.second;
			if (prop.first == "Alpha")
				it->alpha = prop.second;
			if (prop.first == "Action")
				it->action = prop.second;
			if (prop.first == "Minimum Phase Size")
				it->minphase = prop.second;
			if (prop.first == "Number of States")
				it->states = prop.second;
			if (prop.first == "Coordinated Neighbors")
				it->coordneighbors = prop.second;
			if (prop.first == "Algorithm")
				it->algorithm = prop.second;
			if (prop.first == "Gamma Sync")
				it->gammasync = prop.second;
			if (prop.first == "Gamma Unsync")
				it->gammaunsync = prop.second;
			if (prop.first == "Tau Agr Agr")
				it->tau_agr_agr = prop.second;
			if (prop.first == "Tau Not Agr")
				it->tau_not_agr = prop.second;
			if (prop.first == "Tau Agr Not")
				it->tau_agr_not = prop.second;
			if (prop.first == "Tau Not Not")
				it->tau_not_not = prop.second;
			if (prop.first == "Start Time")
				it->starttime = prop.second;
			if (prop.first == "Learning Rate")
				it->learningrate = prop.second;
			if (prop.first == "Discount Rate")
				it->discountrate = prop.second;
			if (prop.first == "First Phase")
				it->firstphase = prop.second;
			if (prop.first == "Second Phase")
				it->secondphase = prop.second;
			if (prop.first == "Tau")
				it->tau = prop.second;
		}
			
	}
	
	return 1;
}

//Remove the agent from the list.
///\param agent: agent to be removed.
int AgentHandler::removeAgent(QString agent)
{
	XmlAgentList::iterator it;
	bool found = false;
	
	for (it = al.begin(); it != al.end(); it++)
	{
		if(it->nickname == agent)
		{
			found = true;
			al.erase(it);
			break;
		}
	}

	if (found)
		return 1;
	
	return 0;
}

