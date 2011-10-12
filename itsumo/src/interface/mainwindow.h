/****************************************************************************
** Form interface generated from reading ui file 'mainwindow.ui'
**
** Created: Wed Oct 12 20:18:54 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qmainwindow.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QTabWidget;
class QWidget;
class QGroupBox;
class QTable;
class QComboBox;
class QPushButton;
class QLabel;
class QLineEdit;
class QListView;
class QListViewItem;
class QTextEdit;
class QListBox;
class QListBoxItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~MainWindow();

    QTabWidget* tabWidget2;
    QWidget* tab;
    QGroupBox* groupSensors;
    QTable* sensorTable;
    QComboBox* comboSensor;
    QPushButton* insertSensorButton;
    QPushButton* removeSensorButton;
    QGroupBox* groupDrivers;
    QTable* driverTable;
    QComboBox* comboDriver;
    QPushButton* insertDriverButton;
    QPushButton* removeDriverButton;
    QPushButton* routeButton;
    QGroupBox* groupGeneral;
    QLabel* textLabel5;
    QLabel* textLabel1;
    QPushButton* pushButton14;
    QPushButton* seeButton;
    QPushButton* netChooseButton;
    QLabel* fileNameLabel;
    QLabel* netNameLabel;
    QLineEdit* lineSensorI;
    QLabel* textLabel2_2_2;
    QLineEdit* lineSteps;
    QLabel* textLabel4;
    QLabel* textLabel4_3;
    QLineEdit* lineIterationLength;
    QLineEdit* lineCellSize;
    QLabel* textLabel3;
    QLabel* textLabel4_2;
    QLineEdit* lineMaxSpeed;
    QLineEdit* lineDecel;
    QLineEdit* lineAgentI;
    QLabel* textLabel2_2;
    QLabel* textLabel1_4;
    QWidget* tab_2;
    QPushButton* buttonAddTemplate;
    QPushButton* buttonRemoveTemplate;
    QListView* listDrivers;
    QLabel* textLabel1_2;
    QTable* templateTable;
    QLabel* textLabel2;
    QTextEdit* textDescription;
    QWidget* TabPage;
    QPushButton* pushButton17;
    QPushButton* pushButton18;
    QPushButton* pushButton11;
    QLineEdit* lineEdit3;
    QLabel* textLabel1_3;
    QGroupBox* groupBox4;
    QPushButton* pushButton16;
    QPushButton* pushButton15;
    QListBox* listAgents;
    QTable* table7;
    QTable* table6;
    QTable* table4;
    QWidget* TabPage_2;
    QPushButton* pushButton13;
    QPushButton* pushButton12;
    QMenuBar *MenuBar;
    QPopupMenu *File;
    QPopupMenu *Help;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* fileSaveAsAction;
    QAction* filePrintAction;
    QAction* fileExitAction;
    QAction* editUndoAction;
    QAction* editRedoAction;
    QAction* editCutAction;
    QAction* editCopyAction;
    QAction* editPasteAction;
    QAction* editFindAction;
    QAction* helpContentsAction;
    QAction* helpIndexAction;
    QAction* helpAboutAction;
    QAction* editxxxAction;
    QAction* edityyyAction;
    QAction* fileCloseAction;
    QAction* Action;

public slots:
    virtual void fileNew_();
    virtual void fileOpen_();
    virtual bool fileSave_();
    virtual void fileSaveAs_();
    virtual void filePrint();
    virtual void fileExit();
    virtual void editUndo();
    virtual void editRedo();
    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editFind();
    virtual void helpIndex();
    virtual void helpContents();
    virtual void helpAbout();
    virtual void removeDriver_();
    virtual void insertDriver_();
    virtual void removeSensor_();
    virtual void insertSensor_();
    virtual void driverSelected_( const QString & string );
    virtual void sensorSelected_( const QString & string );
    virtual void fileClose();
    virtual void saveTemplate_();
    virtual void showTemplateStructure_( QListViewItem * item );
    virtual void listTemplates_();
    virtual void driverChanged_( int row, int col );
    virtual void sensorChanged_( int row, int col );
    virtual void tableDriverClicked_( int row, int col );
    virtual void tableSensorClicked_( int row, int col );
    virtual void tableTemplateClicked_( int row, int col, int mouse, const QPoint & point );
    virtual void templateChanged_( int row, int col );
    virtual void descriptionChanged_();
    virtual void configRoute_();
    virtual void seeNet_();
    virtual void chooseNet_();
    virtual void newTemplate_();
    virtual void removeTemplate_();
    virtual void createNewNet_();
    virtual void runSimulation_();
    virtual void viewSimulation_();
    virtual void chooseAgentsFile_();
    virtual void saveAgents_();
    virtual void addAgent_();
    virtual void removeAgent_();
    virtual void saveAsAgents_();
    virtual void tableAgentsClicked_( int row, int col, int mouse, const QPoint & point );
    virtual void listAgentsSelectionChanged_( QListBoxItem * agentSelected );
    virtual void agentChanged_( int, int );
    virtual void gammaChanged_( int, int );
    virtual void tauChanged_( int, int );

protected:
    QGridLayout* groupSensorsLayout;
    QVBoxLayout* layout15;
    QGridLayout* groupDriversLayout;
    QVBoxLayout* layout10;
    QGridLayout* layout9;
    QGridLayout* tabLayout;
    QSpacerItem* spacer12;
    QHBoxLayout* layout13;
    QSpacerItem* spacer11;
    QHBoxLayout* layout10_2;
    QVBoxLayout* layout9_2;
    QGridLayout* TabPageLayout;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;
    QPixmap image4;
    QPixmap image5;
    QPixmap image6;
    QPixmap image7;
    QPixmap image8;
    QPixmap image9;

};

#endif // MAINWINDOW_H
