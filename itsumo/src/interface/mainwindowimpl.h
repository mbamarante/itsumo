#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

#include "mainwindow.h"
#include "routedialogimpl.h"

class SimFileHandler;
class FileHandler;
class NetHandler;

class MainWindowImpl : public MainWindow
{
    Q_OBJECT

public:

    MainWindowImpl( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~MainWindowImpl();
    
public slots:

	void insertDriver(QString driver);

	RouteList tlToRoute(TLList list);
	TLList routeToTl(RouteList list);

private slots:

    	void fileNew_();
	void fileOpen_();
	bool fileSave_();
	void fileSaveAs_();
	void fileClose_();
	void chooseNet_();
	void seeNet_();
	void configRoute_();

	void editDrivers_();

	void helpContents();
	void helpAbout();

	void removeDriver_();
	void removeSensor_();
	void insertDriver_();
	void insertSensor_();

	void driverSelected_(const QString& string);
	void sensorSelected_(const QString& string);

	void showItems_();
	void clearFields_(bool all = true);

	void listTemplates_();
	void showTemplateStructure_(QListViewItem* item);

	void driverChanged_(int row, int col);
	void sensorChanged_(int row, int col);
	void agentChanged_(int row, int col);
	void gammaChanged_(int row, int col);
	void tauChanged_(int row, int col);
	void templateChanged_(int row, int col);

	void tableDriverClicked_(int row, int col);
	void tableSensorClicked_(int row, int col);

	void tableTemplateClicked_(int row, int col, int mouse, const QPoint& point);

	void removeOption_();
	void addOption_();

	void saveTemplate_();

	void descriptionChanged_();

	void setDriverRoutes_(RouteList rl);

	void newTemplate_();
	void removeTemplate_();

	void runSimulation_();
	void viewSimulation_();

	void view3D_(QString file);

	void chooseAgentsFile_();

	void createNewNet_();

	void runAgents_();

	void showAgents_(XmlAgentList list);

	void addAgent_();
	void listAgentsSelectionChanged_(QListBoxItem* agentSelected);
	void removeAgent_();
	void saveAgents_();
	void saveAsAgents_();
	void tableAgentsClicked_(int row, int col, int mouse, const QPoint& point);
	void setNewAgent_(RouteList list);

private:

	NetHandler* nh_;	///<Net handler.
	SimFileHandler* fh_;	///<File handler.
	AgentHandler* ah_;	///<Agent Handler.
};

#endif // MAINWINDOWIMPL_H
