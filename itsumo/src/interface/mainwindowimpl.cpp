#include "mainwindowimpl.h"
#include "choosedialogimpl.h"

#include <qlineedit.h>
#include <qpair.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qtable.h>
#include <qmessagebox.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qinputdialog.h>
#include <qpopupmenu.h>
#include <qerrormessage.h>
#include <qtabwidget.h>
#include <qbuttongroup.h>

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
using std::cout;
using std::endl;

using namespace std;

//aqui

// includes
#include <string.h>
#include <unistd.h>
// defines
#define MAX_FILENAME_LEN 512

//RELATIVE PATHS:
// The number of characters at the start of an absolute filename.  e.g. in DOS,
// absolute filenames start with "X:\" so this value should be 3, in UNIX they start
// with "\" so this value should be 1.
// source: http://www.codeguru.com/Cpp/misc/misc/fileanddirectorynaming/article.php/c263/
#define ABSOLUTE_NAME_START 3

// set this to '\\' for DOS or '/' for UNIX
#define SLASH '/'

char homePath[MAX_FILENAME_LEN];
	
char* GetRelativeFilename(char *currentDirectory, char *absoluteFilename)
{
	// declarations - put here so this should work in a C compiler
	int afMarker = 0, rfMarker = 0;
	int cdLen = 0, afLen = 0;
	int i = 0;
	int levels = 0;
	static char relativeFilename[MAX_FILENAME_LEN+1];

	cdLen = strlen(currentDirectory);
	afLen = strlen(absoluteFilename);

	// make sure the names are not too long or too short
	if(cdLen > MAX_FILENAME_LEN || cdLen < ABSOLUTE_NAME_START+1 ||
		afLen > MAX_FILENAME_LEN || afLen < ABSOLUTE_NAME_START+1)
	{
		return NULL;
	}

	// Handle DOS names that are on different drives:
	if(currentDirectory[0] != absoluteFilename[0])
	{
		// not on the same drive, so only absolute filename will do
		strcpy(relativeFilename, absoluteFilename);
		return relativeFilename;
	}

	// they are on the same drive, find out how much of the current directory
	// is in the absolute filename
	i = ABSOLUTE_NAME_START;
	while(i < afLen && i < cdLen && currentDirectory[i] == absoluteFilename[i])
	{
		i++;
	}

	if(i == cdLen && (absoluteFilename[i] == SLASH || absoluteFilename[i-1] == SLASH))
	{
		// the whole current directory name is in the file name,
		// so we just trim off the current directory name to get the
		// current file name.
		if(absoluteFilename[i] == SLASH)
		{
			// a directory name might have a trailing slash but a relative
			// file name should not have a leading one...
			i++;
		}

		strcpy(relativeFilename, &absoluteFilename[i]);
		return relativeFilename;
	}


	// The file is not in a child directory of the current directory, so we
	// need to step back the appropriate number of parent directories by
	// using "..\"s.  First find out how many levels deeper we are than the
	// common directory
	afMarker = i;
	levels = 1;

	// count the number of directory levels we have to go up to get to the
	// common directory
	while(i < cdLen)
	{
		i++;
		if(currentDirectory[i] == SLASH)
		{
			// make sure it's not a trailing slash
			i++;
			if(currentDirectory[i] != '\0')
			{
				levels++;
			}
		}
	}

	// move the absolute filename marker back to the start of the directory name
	// that it has stopped in.
	while(afMarker > 0 && absoluteFilename[afMarker-1] != SLASH)
	{
		afMarker--;
	}

	// check that the result will not be too long
	if(levels * 3 + afLen - afMarker > MAX_FILENAME_LEN)
	{
		return NULL;
	}

	// add the appropriate number of "..\"s.
	rfMarker = 0;
	for(i = 0; i < levels; i++)
	{
		relativeFilename[rfMarker++] = '.';
		relativeFilename[rfMarker++] = '.';
		relativeFilename[rfMarker++] = SLASH;
	}

	// copy the rest of the filename into the result string
	strcpy(&relativeFilename[rfMarker], &absoluteFilename[afMarker]);

	return relativeFilename;
}


MainWindowImpl::MainWindowImpl( QWidget* parent, const char* name, WFlags fl )
    : MainWindow( parent, name, fl )
{

    //connects
	fh_ = new SimFileHandler();
	nh_ = new NetHandler();
	ah_ = new AgentHandler();
	
	move(20, 20);
	
	//gets the current working directory to calculate relative paths
	getcwd(homePath, MAX_FILENAME_LEN);
	
	driverTable->horizontalHeader()->hide();
	driverTable->verticalHeader()->hide();
	driverTable->setLeftMargin( 0 );
	driverTable->setTopMargin( 0 );

	sensorTable->horizontalHeader()->hide();
	sensorTable->verticalHeader()->hide();
	sensorTable->setLeftMargin( 0 );
	sensorTable->setTopMargin( 0 );
	
	templateTable->horizontalHeader()->hide();
	templateTable->verticalHeader()->hide();
	templateTable->setLeftMargin( 0 );
	templateTable->setTopMargin( 0 );

	table4->setColumnWidth(2, 350);

	listDrivers->setColumnWidth(0, listDrivers->width() + 6);

	listTemplates_();

	tabWidget2->removePage(tabWidget2->page(1));
	
	//hide contents
	table6->hide();
	table7->hide();

	fileNew_();	
}


MainWindowImpl::~MainWindowImpl()
{
	fh_->~SimFileHandler();
}


void MainWindowImpl::fileNew_()
{
	//check saving here
	
	clearFields_(false);

	QString aux;

	lineSensorI->setText(aux.setNum(30));
	lineAgentI->setText(aux.setNum(600));
	lineSteps->setText(aux.setNum(3600));
	lineDecel->setText(aux.setNum(0));
	lineMaxSpeed->setText(aux.setNum(3));
        lineCellSize->setText(aux.setNum(5));
        lineIterationLength->setText(aux.setNum(1));
}

bool MainWindowImpl::fileSave_()
{
	fh_->simConf.steps = lineSteps->text().toInt();
	fh_->simConf.defDec = lineDecel->text().toDouble();
	fh_->simConf.sensorInterval = lineSensorI->text().toInt();
	fh_->simConf.agentInterval = lineAgentI->text().toInt();
	fh_->simConf.carMaxSpeed = lineMaxSpeed->text().toInt();
	fh_->simConf.cellSize = lineCellSize->text().toInt();
	fh_->simConf.iterationLength = lineIterationLength->text().toInt();


	if (!fh_->fill())
	{
		QString temp;

		temp = QFileDialog::getSaveFileName(
			"./",
			"Config File (*.xml)",
			this,
			"save file dialog",
			"Choose a name for config file" );
		
		if (!fh_->save(temp))
		{
			QMessageBox::warning(this, "Error", "No file to be saved.");
			return false;
		}
	}
	else
	{
	    //needs to pass configFile as parameter (Bruno jun 2007)
		if (!fh_->save(fh_->configFile()))
		{
			QMessageBox::warning(this, "Error", "No file to be saved.");
			return false;
		}
	}
	
	fh_->setConfigFile(GetRelativeFilename(homePath, const_cast<char *>(fh_->configFile().ascii())));
	saveTemplate_();
	fileNameLabel->setText(fh_->configFile());
	
	return true;

}

void MainWindowImpl::fileSaveAs_()
{
	QString temp;

	temp = QFileDialog::getSaveFileName(
			"./",
			"Config File (*.xml)",
			this,
			"save file dialog",
			"Choose a name for config file" );

	if (temp != "")
	{		
		if (!fh_->save(temp))
			QMessageBox::warning(this, "Error", "No file to be saved.");
	}
	else
		QMessageBox::warning(this, "Error", "No file to save.");
	
	fh_->setConfigFile(GetRelativeFilename(homePath, const_cast<char *>(fh_->configFile().ascii())));
	fileNameLabel->setText(fh_->configFile());
}

void MainWindowImpl::fileOpen_()
{
	fh_->localClear();

	QString s = QFileDialog::getOpenFileName(
                    "./",
                    "Config File (*.xml)",
                    this,
                    "open file dialog",
                    "Choose a config file" );
	
	if (fh_->load(s))
	{
		groupGeneral->setEnabled(true);
		groupDrivers->setEnabled(true);
		groupSensors->setEnabled(true);
		fileNameLabel->clear();
		
		fileNameLabel->setText(GetRelativeFilename(homePath, const_cast<char *>(s.ascii())));
		
		fh_->simConf.netFile = GetRelativeFilename(homePath, const_cast<char *>(fh_->simConf.netFile.ascii()));
		netNameLabel->setText(fh_->simConf.netFile);
		
		lineSteps->setText(s.setNum(fh_->simConf.steps));
		lineDecel->setText(s.setNum(fh_->simConf.defDec));
		lineSensorI->setText(s.setNum(fh_->simConf.sensorInterval));
		lineAgentI->setText(s.setNum(fh_->simConf.agentInterval));
		lineMaxSpeed->setText(s.setNum(fh_->simConf.carMaxSpeed));
		lineCellSize->setText(s.setNum(fh_->simConf.cellSize));
		lineIterationLength->setText(s.setNum(fh_->simConf.iterationLength));

		fh_->checkDriverTemplate();

		if (!nh_->load(fh_->simConf.netFile)) {
			QMessageBox::warning(this, "Error", "Unable to open the network.");
		}
			
		showItems_();
	}
	else
		QMessageBox::warning(this, "Error", "Could not open the file.");


	  

}


///Show drivers and sensors.
void MainWindowImpl::showItems_()
{
	XmlDrivers drivers; 
	fh_->drivers(drivers);
	XmlSensors sensors;
	fh_->sensors(sensors);
	
	XmlDrivers::iterator itDriver;
	XmlSensors::iterator itSensor;
	
	if (drivers.size() > 0)
	{
		comboDriver->clear();
		for (itDriver=drivers.begin(); itDriver!=drivers.end(); itDriver++)
		{
			comboDriver->insertItem(*itDriver);
		}
		driverSelected_(comboDriver->currentText());
	}
	else
		driverSelected_("");
	
	if (sensors.size() > 0)
	{
		comboSensor->clear();
		for (itSensor=sensors.begin(); itSensor!=sensors.end(); itSensor++)
		{
			comboSensor->insertItem(*itSensor);
		}
		sensorSelected_(comboSensor->currentText());
	}
	else
		sensorSelected_("");
}

///Clear widgets.
void MainWindowImpl::fileClose_()
{
	clearFields_();
}

void MainWindowImpl::listAgentsSelectionChanged_(QListBoxItem* agentSelected) {
	
	TLList tls;
	TLList::iterator it2;
	QString lights;
	QString aux;
	QStringList labels;
	
	table4->setNumRows(0);
	table4->setNumCols(0);
		
	if (agentSelected != NULL)
	{
		XmlAgent agent;
		
		ah_->getAgent(const_cast<char *>(agentSelected->text().ascii()), agent);
		//tls = ah_->agentTls(const_cast<char *>(agentSelected->text().ascii()));
		tls = agent.tls;
		
		table6->hide();
		table7->hide();
		
		table4->resize(QSize(420, 380));
		table4->setNumCols(2);
		table4->setNumRows(4);
		table4->setRowReadOnly(0, true);
		table4->setRowReadOnly(2, true);
		table4->setRowReadOnly(3, true);
		
		table4->setText(0, 0, "Agent Type");
		table4->setText(1, 0, "Agent Name");
		table4->setText(2, 0, "Log");
		table4->setText(3, 0, "TLs");
		
		if (agent.name == "Greedy")
		{
			table4->insertRows(table4->numRows(), 4);
			table4->setText(4, 0, "Memory Size");
			table4->setText(5, 0, "Alpha");
			table4->setText(6, 0, "Minimum Phase Size");
			table4->setText(7, 0, "Action");
			table4->setText(4, 1, agent.memsize);
			table4->setText(5, 1, agent.alpha);
			table4->setRowReadOnly(6, false);
			table4->setText(6, 1, agent.minphase);
			table4->setText(7, 1, agent.action);
			table4->setRowReadOnly(7, true);
		}
		else {
			table4->setRowReadOnly(7, false);
		}
		
		if (agent.name == "Q-learning")
		{
			table4->insertRows(table4->numRows(), 5);
			table4->setText(4, 0, "Memory Size");
			table4->setText(5, 0, "Number of States");
			table4->setText(6, 0, "Start Time");
			table4->setText(7, 0, "Learning Rate");
			table4->setText(8, 0, "Discount Rate");
			
			table4->setText(4, 1, agent.memsize);
			table4->setText(5, 1, agent.states);
			table4->setText(6, 1, agent.starttime);
			table4->setText(7, 1, agent.learningrate);
			table4->setText(8, 1, agent.discountrate);
		}
		
		if (agent.name == "DCOP")
		//change DCOP options
		{
			QStringList algorithmList;
			algorithmList << "ADOPT" << "DPOP" << "OPTAPO";
			
			QComboTableItem* comboAlgorithm = new QComboTableItem(table4, algorithmList);
			comboAlgorithm->setCurrentItem(agent.algorithm);
			
			table4->resize(QSize(420, 170));
			
			table4->insertRows(table4->numRows(), 3);
			
			table4->setText(4, 0, "Algorithm");
			table4->setText(5, 0, "Coordinated Neighbors");
			table4->setText(6, 0, "Alpha");
			
			table4->setItem(4, 1, comboAlgorithm);
			table4->setText(5, 1, agent.coordneighbors);
			table4->setText(6, 1, agent.alpha);
			table4->setRowReadOnly(5, true);
			
			table6->show();
			table6->setText(0, 0, agent.gammasync);
			table6->setText(1, 0, agent.gammaunsync);
			
			table7->show();
			table7->setColumnReadOnly(0, true);
			table7->setColumnReadOnly(1, true);
			table7->setText(0, 0, "Agree");
			table7->setText(0, 1, "Agree");
			table7->setText(0, 2, agent.tau_agr_agr);
			table7->setText(1, 0, "Not Agree");
			table7->setText(1, 1, "Agree");
			table7->setText(1, 2, agent.tau_not_agr);
			table7->setText(2, 0, "Agree");
			table7->setText(2, 1, "Not Agree");
			table7->setText(2, 2, agent.tau_agr_not);
			table7->setText(3, 0, "Not Agree");
			table7->setText(3, 1, "Not Agree");
			table7->setText(3, 2, agent.tau_not_not);
		}
		else {
			table4->setRowReadOnly(5, false);
		}
		
		if (agent.name == "supervised-learning")
		{
			table4->insertRows(table4->numRows(), 8);
			table4->setText(4, 0, "Memory Size");
			table4->setText(5, 0, "Number of States");
			table4->setText(6, 0, "Start Time");
			table4->setText(7, 0, "Learning Rate");
			table4->setText(8, 0, "Discount Rate");
			table4->setText(9, 0, "First Phase");
			table4->setText(10, 0, "Second Phase");
			table4->setText(11, 0, "Tau");
			
			table4->setText(4, 1, agent.memsize);
			table4->setText(5, 1, agent.states);
			table4->setText(6, 1, agent.starttime);
			table4->setText(7, 1, agent.learningrate);
			table4->setText(8, 1, agent.discountrate);
			table4->setText(9, 1, agent.firstphase);
			table4->setText(10, 1, agent.secondphase);
			table4->setText(11, 1, agent.tau);
		}
		
		for (it2 = tls.begin(); it2 != tls.end(); it2++)
			lights += aux.setNum(*it2) + ", ";
			
			
		lights.remove(lights.length() - 2, 2);
		
		table4->setText(0, 1, agent.name);
		table4->setText(1, 1, agent.nickname);
		table4->setText(2, 1, agent.log);
		table4->setText(3, 1, lights);
		
		table4->setColumnWidth( 0, 180 );
		table4->setColumnWidth( 1, 200 );
		table4->setColumnReadOnly(0, true);
		
		labels << "Options" << "Values";
		table4->setColumnLabels(labels);
	}
	
}

//needs optimization
void MainWindowImpl::driverSelected_(const QString& string)
{
	QString aux;

	driverTable->setNumRows(0);
	driverTable->setNumCols(0);

	if (!string.isEmpty())
	{
		XmlDriver driver;
		
		fh_->getDriver(string, driver);
		
		int rows = 5 + driver.options.size();
		
		driverTable->setNumCols(2);
		driverTable->setColumnReadOnly(0, true);
		driverTable->setRowReadOnly(0, true);
		driverTable->setNumRows(rows);
		
		driverTable->setText(0, 0, "Driver Name");
		driverTable->setText(1, 0, "Driver Nickname");
		driverTable->setText(2, 0, "Number of Drivers");
		driverTable->setText(3, 0, "State");
		driverTable->setText(4, 0, "Debug");
		
		driverTable->setText(0, 1, driver.name);
		driverTable->setText(1, 1, driver.nick);
		driverTable->setText(2, 1, aux.setNum(driver.number));
		driverTable->setText(3, 1, driver.state);
		driverTable->setText(4, 1, driver.debug);
		
		driverTable->setColumnWidth( 0, 180 );
		driverTable->setColumnWidth( 1, 250 );
		
		int count = 5;
		
		XmlDriverOptions::iterator it;
		
		for (it = driver.options.begin(); it != driver.options.end(); it++)
		{
			driverTable->setText(count, 0, it->first);
			driverTable->setText(count, 1, it->second);
			
			count++;
		}
		
		if (driver.name == "fc")
			routeButton->setEnabled(true);
		else
			routeButton->setEnabled(false);
	}
}

void MainWindowImpl::sensorSelected_(const QString& string)
{
	QString aux;

	sensorTable->setNumRows(0);
	sensorTable->setNumCols(0);

	if (!string.isEmpty())
	{
		XmlSensor sensor;
		QString homePath_(homePath);	
			
		fh_->getSensor(string, sensor);
		
		sensorTable->setNumCols(2);
		sensorTable->setColumnReadOnly(0, true);
		sensorTable->setNumRows(4);
		
		sensorTable->setColumnWidth( 1, 200 );
		
		sensorTable->setText(0, 0, "Sensor Name");
		sensorTable->setText(1, 0, "Sensor File");
		sensorTable->setText(2, 0, "State");
		sensorTable->setText(3, 0, "Time Tag");
		
		//relative path aqui
		//homePath_ += sensor.file;
		//sensor.file = homePath_ + "/" + sensor.file; 
		//sensor.file = "../../" + sensor.file;
		sensor.file = GetRelativeFilename(const_cast<char *>(homePath_.ascii()), const_cast<char *>(sensor.file.ascii()));
		
		sensorTable->setText(0, 1, sensor.name);
		sensorTable->setText(1, 1, sensor.file);
		sensorTable->setText(2, 1, sensor.state);
		sensorTable->setText(3, 1, sensor.timetag);

		sensorTable->setColumnWidth( 0, 180 );
		sensorTable->setColumnWidth( 1, 250 );
	}

}

void MainWindowImpl::clearFields_(bool all)
{
	groupGeneral->setEnabled(!all);
	groupDrivers->setEnabled(!all);
	groupSensors->setEnabled(!all);

	fileNameLabel->setText("");
	netNameLabel->setText("");
	lineSteps->setText("");
	lineDecel->setText("");
	lineSensorI->setText("");
	lineAgentI->setText("");
	lineMaxSpeed->setText("");
	lineCellSize->setText("");
	lineIterationLength->setText("");

	driverTable->setNumRows(0);
	driverTable->setNumCols(0);

	sensorTable->setNumRows(0);
	sensorTable->setNumCols(0);

	fh_->localClear();

}

void MainWindowImpl::chooseNet_()
{
	QString s = QFileDialog::getOpenFileName(
                    "./",
                    "Network File (*.xml)",
                    this,
                    "open file dialog",
                    "Choose a network file" );

	fh_->simConf.netFile = s;

	if (nh_->load(fh_->simConf.netFile))
	{
		fh_->simConf.netFile = GetRelativeFilename(homePath, const_cast<char *>(fh_->simConf.netFile.ascii()));
		netNameLabel->setText(fh_->simConf.netFile);
	}
	else
		QMessageBox::warning(this, "Error", "Unable to open the network.");
}

void MainWindowImpl::removeDriver_()
{
	QString driver = comboDriver->currentText();

	fh_->removeDriver(driver);

	comboDriver->removeItem(comboDriver->currentItem());

	showItems_();
}

void MainWindowImpl::removeSensor_()
{
	QString sensor = comboSensor->currentText();

	fh_->removeSensor(sensor);

	comboSensor->removeItem(comboSensor->currentItem());

	showItems_();
}


void MainWindowImpl::insertDriver_()
{
	XmlDrivers list;

	fh_->confDrivers(list);

	ChooseDialogImpl* d = new ChooseDialogImpl("Choose a Driver Template", list);

	d->show();

	connect( d, SIGNAL(selected(QString)), this, SLOT(insertDriver(QString)) );
	
}

void MainWindowImpl::insertDriver(QString driver)
{
	if (fh_->insertDriver(driver))
	{
		QMessageBox::warning(this, "Insert Driver", "Driver included!");
		showItems_();
	}
	else
		QMessageBox::warning(this, "Insert Driver", "Driver is already included.");
}


void MainWindowImpl::insertSensor_()
{
	bool ok;

	QStringList lst;
    lst << "lane_sensor" 
		<< "laneset_sensor" 
		<< "laneset_sensor_without_border_nodes"
		<< "laneset_densities_sensor" 
		<< "section_in_columns_sensor" 
		<< "section_in_columns_sensor_printing_names"
		<< "stopped_cars_in_lanesets"
		<< "total_stopped_cars_in_network"
		<< "total_stopped_cars_in_network_without_sources"
		<< "controlled_lanes_summary"
// 		<< "visualization_of_section_densities"
// 		<< "visualization_of_section_relative_densities"
		<< "visualization_of_stopped_cars_in_sections"
        << "visualization_of_laneset_densities"
        << "visualization_microscopic_level"
        
		;

    QString text = QInputDialog::getItem(
            "Insert Sensor", "Sensor name:", lst, 1, 
            QString::null, &ok, this );
    if ( ok && !text.isEmpty() ) 
	{
        // user entered something and pressed OK
		if (fh_->insertSensor(text))
		{
			QMessageBox::warning(this, "Insert Sensor", "Sensor included!");
			showItems_();
		}
		else
			QMessageBox::warning(this, "Insert Sensor", "Sensor is already included.");

    } else 
	{
        // user entered nothing or pressed Cancel
    }

}

void MainWindowImpl::driverChanged_(int row, int col)
{
	QString driver = comboDriver->currentText();

	QPair<QString, QString> prop;

	switch(row)
	{
	case 2: 
		{
			prop.first = "number";
			prop.second = driverTable->text(row, col);
			break;
		}
	//case 2: break;
	default:
		{
			prop.first = driverTable->text(row, col-1);
			prop.second = driverTable->text(row, col);
			break;
		}
	}

	fh_->changeDriverProperty(driver, prop);
}

void MainWindowImpl::sensorChanged_(int row, int col)
{
	QString sensor = comboSensor->currentText();

	QPair<QString, QString> prop;

	switch(row)
	{
	case 0: 
		{
			prop.first = "name";
			prop.second = sensorTable->text(row, col);
			break;
		}
	case 1: 
		{
			prop.first = "file";
			prop.second = sensorTable->text(row, col);
			break;
		}
	case 2: break;
	}

	fh_->changeSensorProperty(sensor, prop);

	showItems_();
}

void MainWindowImpl::agentChanged_(int row, int col)
{
	QPair<QString, QString> prop;
	int agentSelected;
	QString agentType;
	
	agentSelected = listAgents->currentItem();
	agentType = table4->text(0, 1);
	
	switch(row)
	{
		case 1:
			prop.first = "Agent Name";
			break;
		case 4:
			if (agentType == "Greedy")
				prop.first = "Memory Size";
			if (agentType == "Q-learning")
				prop.first = "Memory Size";
			if (agentType == "DCOP")
				prop.first = "Algorithm";
			if (agentType == "supervised-learning")
				prop.first = "Memory Size";
			break;
		case 5:
			if (agentType == "Greedy")
				prop.first = "Alpha";
			if (agentType == "Q-learning")
				prop.first = "Number of States";
			if (agentType == "supervised-learning")
				prop.first = "Number of States";
			break;
		case 6:
			if (agentType == "Greedy")
				prop.first = "Minimum Phase Size";
			if (agentType == "DCOP")
				prop.first = "Alpha";
			if (agentType == "Q-learning")
				prop.first = "Start Time";
			if (agentType == "supervised-learning")
				prop.first = "Start Time";
			break;
		case 7:
			if (agentType == "Q-learning")
				prop.first = "Learning Rate";
			if (agentType == "supervised-learning")
				prop.first = "Learning Rate";
			break;
		case 8:
			if (agentType == "Q-learning")
				prop.first = "Discount Rate";
			if (agentType == "supervised-learning")
				prop.first = "Discount Rate";
			break;
		case 9:
			if (agentType == "supervised-learning")
				prop.first = "First Phase";
			break;
		case 10:
			if (agentType == "supervised-learning")
				prop.first = "Second Phase";
			break;
		case 11:
			if (agentType == "supervised-learning")
				prop.first = "Tau";
			break;
		default: prop.first = "";
	}
	
	if (agentType == "DCOP" && row == 6)
		prop.second = table4->item(row, col)->text();
	else
		prop.second = table4->text(row, col);
	
	ah_->changeAgentProperty(listAgents->currentText(), prop);
	showAgents_(ah_->al);
	
	listAgents->setCurrentItem(agentSelected);
	
}

void MainWindowImpl::gammaChanged_(int row, int col)
{
	QPair<QString, QString> prop;
	int agentSelected;
	QString agentType;
	
	agentSelected = listAgents->currentItem();
	agentType = table4->text(0, 1);
	
	if (agentType == "DCOP")
	{
		switch(row)
		{
			case 0:
				prop.first = "Gamma Sync";
				break;
			case 1:
				prop.first = "Gamma Unsync";
				break;
			default: prop.first = "";
		}
	}
	
	prop.second = table6->text(row, col);
	ah_->changeAgentProperty(listAgents->currentText(), prop);
	showAgents_(ah_->al);
		
	listAgents->setCurrentItem(agentSelected);
}	

void MainWindowImpl::tauChanged_(int row, int col)
{
	QPair<QString, QString> prop;
	int agentSelected;
	QString agentType;
	
	agentSelected = listAgents->currentItem();
	agentType = table4->text(0, 1);
	
	if (agentType == "DCOP")
	{
		switch(row)
		{
			case 0:
				prop.first = "Tau Agr Agr";
				break;
			case 1:
				prop.first = "Tau Not Agr";
				break;
			case 2:
				prop.first = "Tau Agr Not";
				break;
			case 3:
				prop.first = "Tau Not Not";
				break;
			default: prop.first = "";
		}
	}
	
	prop.second = table7->text(row, col);
	ah_->changeAgentProperty(listAgents->currentText(), prop);
	showAgents_(ah_->al);
		
	listAgents->setCurrentItem(agentSelected);
}

//needs optimization
void MainWindowImpl::tableDriverClicked_(int row, int col)
{
	QString state = driverTable->text(row, col);
	state.replace(" ", "");

	if ( (row==3) && (col==1) ) //state clicked
	{
		if (state == "ON")
			driverTable->setText(row, col, "OFF");
		else if (state == "OFF")
			driverTable->setText(row, col, "ON");
		else 
			driverTable->setText(row, col, "OFF");

		state = "state";
		QString value = driverTable->text(row, col);
		QPair<QString, QString> prop = qMakePair(state, value);
		
		fh_->changeDriverProperty(comboDriver->currentText(), prop);
	}
	if ( (row==4) && (col==1) ) //debug clicked
	{
		if (state == "ON")
			driverTable->setText(row, col, "OFF");
		else if (state == "OFF")
			driverTable->setText(row, col, "ON");
		else 
			driverTable->setText(row, col, "OFF");

		state = "debug";
		QString value = driverTable->text(row, col);
		QPair<QString, QString> prop = qMakePair(state, value);
		
		fh_->changeDriverProperty(comboDriver->currentText(), prop);
	}
}

void MainWindowImpl::tableSensorClicked_(int row, int col)
{
	QString state = sensorTable->text(row, col);
	state.replace(" ", "");
	
	if ( (row==2) && (col==1) ) //state clicked
	{
		if (state == "ON")
			sensorTable->setText(row, col, "OFF");
		else if (state == "OFF")
			sensorTable->setText(row, col, "ON");
		else 
			sensorTable->setText(row, col, "OFF");

		state = "state";
		QString value = sensorTable->text(row, col);
		QPair<QString, QString> prop = qMakePair(state, value);
		
		fh_->changeSensorProperty(comboSensor->currentText(), prop);
	}
	
	if ( (row==3) && (col==1) ) //timetag clicked
	{
		if (state == "YES")
			sensorTable->setText(row, col, "NO");
		else if (state == "NO")
			sensorTable->setText(row, col, "YES");
		else
			sensorTable->setText(row, col, "NO");
		
		state = "timetag";
		QString value = sensorTable->text(row, col);
		QPair<QString, QString> prop = qMakePair(state, value);
		
		fh_->changeSensorProperty(comboSensor->currentText(), prop);
		
	}
}


void MainWindowImpl::seeNet_()
{
	RouteDialogImpl* rd;
	rd = new RouteDialogImpl(1, nh_);
	
	rd->buttonGroup1->hide();
	rd->groupBox4->hide();
	rd->comboFrom->hide();
	rd->comboTo->hide();
	rd->resize(580, 400);

	rd->show();

}

void MainWindowImpl::editDrivers_()
{
	QWidget* drivers = tabWidget2->page(1);

	drivers->show();
}

void MainWindowImpl::configRoute_()
{
	if (nh_->isLoaded())
	{
		XmlDriver driver;

		fh_->getDriver(comboDriver->currentText(), driver);

		RouteDialogImpl* rd = new RouteDialogImpl(0, nh_, driver.route);

		connect(rd, SIGNAL(done(RouteList)),
			this, SLOT(setDriverRoutes_(RouteList)));
		
		rd->show();
	}
	else
		QMessageBox::warning(this, "Warning", "Select a network file first.");
	
}

void MainWindowImpl::setDriverRoutes_(RouteList rl)
{
	XmlDriver driver;

	fh_->getDriver(comboDriver->currentText(), driver);

	driver.route = rl;

	fh_->setDriver(comboDriver->currentText(), driver);
}

void MainWindowImpl::runSimulation_()
{
	QString homePath_(homePath);
	
	if (fileSave_())
	{
		if (ah_->loaded)
		{
			runAgents_();
		}
		//aff123
		
		QString cmd = "cd src/basic; ./itsumo " + homePath_ + "/" + fh_->configFile();
		
		system(cmd);
		
		QMessageBox::information(this, "Done", "You can now visualize the results.");
	}
	else {
		QMessageBox::warning(this, "Warning", "Problem saving config file.");
	}
}

void MainWindowImpl::runAgents_()
{
	QString cmd = "echo Agents Start!; cd src/trafficAgents/; ./start_agents " + ah_->agentFile + " &";

	QString temp = "echo " + cmd;
	//system(temp);
	system(cmd);
}


void MainWindowImpl::viewSimulation_()
{
	QString s = QFileDialog::getOpenFileName(
                    "./",
                    "Sensor File (*.*)",
                    this,
                    "open file dialog",
                    "Choose a sensor log  file" );

	if (s != "")
		view3D_(s);
}

void MainWindowImpl::chooseAgentsFile_()
{
        QString s = QFileDialog::getOpenFileName(
                    "./",
                    "Agents File (*.xml)",
                    this,
                    "open file dialog",
                    "Choose an agents configuration file" );

	if (s != "")
	{
		if (ah_->load(s))
		{
			showAgents_(ah_->al);
		}
		else
		{
			ah_->loaded = false;
			ah_->agentFile = "";
		}
		
	}
	else
	{
		ah_->loaded = false;
		ah_->agentFile = "";
	}

	lineEdit3->setText(s);
}

void MainWindowImpl::showAgents_(XmlAgentList list)
{
	XmlAgentList::iterator it;

	int row = 0;
	
	listAgents->clear();

	for (it = list.begin(); it != list.end(); it++)	
	{	
		listAgents->insertItem(it->nickname, -1);
		row++;
	}
		
}

void MainWindowImpl::view3D_(QString file)
{
	QString cmd = "cd src/visual3D/; ./visual " + file ;
	system(cmd);
}

void MainWindowImpl::createNewNet_()
{
	QString homePath_(homePath);
	QString cmd;
		
	if (!fh_->simConf.netFile.isEmpty())	
    	cmd = "cd src/topologyEditor; ./edit " + homePath_ + "/" + fh_->simConf.netFile;
    else cmd = "cd src/topologyEditor; ./edit ";
	system(cmd);
}

void MainWindowImpl::helpContents()
{
	system("firefox docs/index.html &");
}

void MainWindowImpl::helpAbout()
{
	QMessageBox::information( this, "ITSUMO",
	"Intelligent Transportation System for Urban Mobility.\n\n"
	"MASLAB @ UFRGS." );
}

void MainWindowImpl::addAgent_()
{
	bool ok;
	QString agentNick;
//aqui	
	QStringList agentList;
	agentList << "Basic" << "Greedy" << "Q-learning" << "DCOP" << "supervised-learning";
	
 	//name
 	QString text = QInputDialog::getItem(
 		"New Agent", "Agent name:", agentList, 0, false,
 		 &ok, this);
 	
	if ( ok && !text.isEmpty() ) 
	{
		agentNick = QString("agent_" + text + "_%1").arg(ah_->al.size());
		
		ah_->addAgent(text, agentNick);
		showAgents_(ah_->al);
	} 
	else 
	{
        // user entered nothing or pressed Cancel
	}
	
}

void MainWindowImpl::removeAgent_()
{
	QString agent = listAgents->currentText();
	int currentAgent = listAgents->currentItem();
	
	//remove agent
	ah_->removeAgent(agent);
	
	listAgents->removeItem(currentAgent);
	//show new list
	showAgents_(ah_->al);
	
	//clear table
	table4->setNumRows(0);
	table4->setNumCols(0);
	
	//try to select previous agent in list
	listAgents->setCurrentItem(currentAgent-1);
}

void MainWindowImpl::saveAgents_()
{
	if (ah_->loaded == false)
		saveAsAgents_();
	else
	{
		ah_->save();
		QMessageBox::information(this, "Save", "Agents file saved.");
	}
	
}

void MainWindowImpl::saveAsAgents_()
{
	QString temp = QFileDialog::getSaveFileName(
			"./",
			"Agents File (*.xml)",
			this,
			"save file dialog",
			"Choose a name for agents file" );

	if (temp != "")
	{
		ah_->save(temp);
		lineEdit3->setText(temp);
		QMessageBox::information(this, "Save", "Agents file saved.");
	}
	else
	{
		// user entered nothing or pressed Cancel
	}
}

void MainWindowImpl::tableAgentsClicked_(int row, int col, int mouse, const QPoint& point)
{

  // just to avoid warnings
  mouse = 0;
  QString currentAgent = table4->text(0, 1);
  
	//table4->selectRow(row);

	if ( col == 1 && row == 2) //log clicked
	{
		QString logStat = table4->text(row, col);
		
		if (logStat == "on")
			table4->setText(row, col, "off");
		else if (logStat == "off")
			table4->setText(row, col, "on");
		else 
			table4->setText(row, col, "off");

		logStat = table4->text(row, col);
		
		QPair<QString, QString> prop = qMakePair(QString("Log"), logStat);
		
		ah_->changeAgentProperty(listAgents->currentText(), prop);
		
	}
	
	else if (col ==1 && row == 5 && currentAgent == "DCOP") //coordinated neighbors clicked in DCOP
	{
		QString coordNeighState = table4->text(row, col);

		if (coordNeighState == "yes")
			table4->setText(row, col, "no");
		else if (coordNeighState == "no")
			table4->setText(row, col, "yes");
		else 
			table4->setText(row, col, "no");
		
		coordNeighState = table4->text(row, col);
		
		QPair<QString, QString> prop = qMakePair(QString("Coordinated Neighbors"), coordNeighState);
		
		ah_->changeAgentProperty(listAgents->currentText(), prop);
	}
	
	else if (col == 1 && row == 7 && currentAgent == "Greedy") //action clicked ind Greedy
	{
		QString action = table4->text(row, col);
		
		if (action == "select_plan")
			table4->setText(row, col, "modify_plan");
		else if (action == "modify_plan")
			table4->setText(row, col, "select_plan");
		else
			table4->setText(row, col, "modify_plan");
		
		action = table4->text(row, col);
		
		QPair<QString, QString> prop = qMakePair(QString("Action"), action);
		
		ah_->changeAgentProperty(listAgents->currentText(), prop);
	}
		
	else if (col == 1 && row == 3) //tls clicked
	{
		QString aux;
		QString agent = listAgents->currentText();//table4->text(row, 0);
		RouteDialogImpl* rd;

		TLList teste = nh_->tlToLaneset(ah_->agentTls(agent));

		RouteList rl = tlToRoute(nh_->tlToLaneset(ah_->agentTls(agent)));

		rd = new RouteDialogImpl(2, nh_, rl);
	
		rd->buttonGroup1->hide();
		rd->textLabel2->hide();
		rd->groupBox4->setEnabled(true);
		rd->groupBox4->setTitle("Nodes");
		rd->comboTo->hide();
		rd->tableRoute->hideColumn(1);
		rd->tableRoute->hideColumn(2);
		rd->tableRoute->horizontalHeader()->setLabel( 0, tr( "Node Name" ) );

		rd->textLabel1->setText("Node");

		connect(rd, SIGNAL(done(RouteList)),
			this, SLOT(setNewAgent_(RouteList)));
	
		rd->show();
	}
}

void MainWindowImpl::setNewAgent_(RouteList list)
{
	QString aux;
	TLList tls = routeToTl(list);
	tls = nh_->nodeToTls(tls);
	
	ah_->setAgentTls(listAgents->currentText(), tls);
	
	//showAgents_(ah_->al);
	listAgentsSelectionChanged_(listAgents->item(listAgents->currentItem()));

	if (list.size() > tls.size())
		QMessageBox::warning(this, "New Agent", "Nodes without traffic lights were removed.");
}



RouteList MainWindowImpl::tlToRoute(TLList list)
{
	RouteList ans;
	
	TLList::iterator it;

	for (it = list.begin(); it != list.end(); it++)
	{
		Route r;
		r.laneset = *it;
		
		ans.push_back(r);
	}

	return ans;
}

TLList MainWindowImpl::routeToTl(RouteList list)
{
	TLList ans;
	
	RouteList::iterator it;

	for (it = list.begin(); it != list.end(); it++)
	{
		ans.push_back(it->laneset);
	}

	return ans;
}

//********************************************************************* tab2


void MainWindowImpl::listTemplates_()
{
	listDrivers->clear();

	XmlDrivers drv;

	fh_->confDrivers(drv);

	XmlDrivers::iterator itDriver;

	for (itDriver=drv.begin(); itDriver!=drv.end(); itDriver++)
	{
		QListViewItem* item = new QListViewItem(listDrivers);

		item->setText(0, *itDriver);
	}
}

void MainWindowImpl::showTemplateStructure_(QListViewItem* item)
{
	//todo
	QString aux;

	if (item != NULL)
	{
		templateTable->setNumRows(0);
		templateTable->setNumCols(0);
		
		XmlDriver driver;
		
		fh_->getTemplateDriver(item->text(0), driver);
		
		int rows = 2 + driver.options.size();
		
		templateTable->setNumCols(2);
		templateTable->setColumnReadOnly(0, true);
		templateTable->setRowReadOnly(0, true);
		templateTable->setNumRows(rows);
		
		templateTable->setText(0, 0, "Driver Name");
		templateTable->setText(0, 1, driver.name);

		templateTable->setText(1, 0, "Driver Nickname");
		templateTable->setText(1, 1, driver.nick);
		
		
		int count = 2;
		
		XmlDriverOptions::iterator it;
		
		for (it = driver.options.begin(); it != driver.options.end(); it++)
		{
			templateTable->setText(count, 0, "option");
			templateTable->setText(count, 1, it->first);
			
			count++;
		}
		
		textDescription->setText(driver.description);
	}
	else //clear interface
	{
		templateTable->setNumRows(0);
		templateTable->setNumCols(0);

		textDescription->setText("");
	}

}


void MainWindowImpl::tableTemplateClicked_(int row, int col, int mouse, const QPoint& point)
{

  // just to avoid warnings
  col = 0;

	templateTable->selectRow(row);

	if (mouse == 2)
	{
		QPopupMenu *op = new QPopupMenu( this );
		op->insertItem( "Remove Option", this, SLOT(removeOption_()) );
		op->insertItem( "Add Option", this, SLOT(addOption_()) );

		int x = templateTable->x();
		int y = templateTable->y();

		QPoint correct(x, y);
		correct = QWidget::mapToGlobal(correct);

		correct.setX(correct.x() + point.x());
		correct.setY(correct.y() + point.y());

		op->popup(QWidget::mapToGlobal(correct));
	}
}


void MainWindowImpl::removeOption_()
{
	int row = templateTable->currentRow();
	QString op = templateTable->text(row, 1);

	if (row != 0)
	{
		templateTable->removeRow(row);
		
		QString empty = "";
		
		QPair<QString, QString> option = qMakePair(op, empty);
		
		fh_->editTemplateOption(1, listDrivers->currentItem()->text(0), option);

		fh_->checkDriverTemplate();

		showItems_();
	}
}

void MainWindowImpl::addOption_()
{
	templateTable->insertRows(templateTable->numRows());
	templateTable->setText(templateTable->numRows()-1, 0, "option");

}

void MainWindowImpl::templateChanged_(int row, int col)
{
	QString temp = listDrivers->currentItem()->text(0);

	QPair<QString, QString> prop;

	QString option = "option";
	QString value = templateTable->text(row, col);

	prop = qMakePair(option, value);

	fh_->editTemplateOption(3, temp, prop);

	if (row == 0) //change the name
	{
		fh_->changeDriversName(temp, value);
		listTemplates_();
	}

	fh_->checkDriverTemplate();

	showItems_();
}

void MainWindowImpl::saveTemplate_()
{
	fh_->confSave();
}

void MainWindowImpl::descriptionChanged_()
{
	QString temp = listDrivers->currentItem()->text(0);

	QPair<QString, QString> prop;

	QString option = "description";
	QString value = textDescription->text();

	prop = qMakePair(option, value);

	fh_->editTemplateOption(3, temp, prop);

}

void MainWindowImpl::newTemplate_()
{
	bool ok;
    QString text = QInputDialog::getText(
            "New Model", "Model name:", QLineEdit::Normal,
            QString::null, &ok, this );
    if ( ok && !text.isEmpty() ) 
	{
        // user entered something and pressed OK
		if (fh_->insertTemplateDriver(text))
		{
			QMessageBox::warning(this, "New Model", "Model included!");
			listTemplates_();
		}
		else
			QMessageBox::warning(this, "New Model", "Model is already included.");

    } else 
	{
        // user entered nothing or pressed Cancel
    }
}

void MainWindowImpl::removeTemplate_()
{
	QString model = listDrivers->currentItem()->text(0);

	if (!model.isEmpty())
	{
		fh_->removeTemplateDriver(model);
		listTemplates_();
	}

}

