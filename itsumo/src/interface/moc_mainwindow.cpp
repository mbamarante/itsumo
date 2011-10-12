/****************************************************************************
** MainWindow meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Oct 12 20:19:10 2011
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "mainwindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MainWindow::className() const
{
    return "MainWindow";
}

QMetaObject *MainWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MainWindow( "MainWindow", &MainWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MainWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MainWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MainWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"fileNew_", 0, 0 };
    static const QUMethod slot_1 = {"fileOpen_", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_2 = {"fileSave_", 1, param_slot_2 };
    static const QUMethod slot_3 = {"fileSaveAs_", 0, 0 };
    static const QUMethod slot_4 = {"filePrint", 0, 0 };
    static const QUMethod slot_5 = {"fileExit", 0, 0 };
    static const QUMethod slot_6 = {"editUndo", 0, 0 };
    static const QUMethod slot_7 = {"editRedo", 0, 0 };
    static const QUMethod slot_8 = {"editCut", 0, 0 };
    static const QUMethod slot_9 = {"editCopy", 0, 0 };
    static const QUMethod slot_10 = {"editPaste", 0, 0 };
    static const QUMethod slot_11 = {"editFind", 0, 0 };
    static const QUMethod slot_12 = {"helpIndex", 0, 0 };
    static const QUMethod slot_13 = {"helpContents", 0, 0 };
    static const QUMethod slot_14 = {"helpAbout", 0, 0 };
    static const QUMethod slot_15 = {"removeDriver_", 0, 0 };
    static const QUMethod slot_16 = {"insertDriver_", 0, 0 };
    static const QUMethod slot_17 = {"removeSensor_", 0, 0 };
    static const QUMethod slot_18 = {"insertSensor_", 0, 0 };
    static const QUParameter param_slot_19[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_19 = {"driverSelected_", 1, param_slot_19 };
    static const QUParameter param_slot_20[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"sensorSelected_", 1, param_slot_20 };
    static const QUMethod slot_21 = {"fileClose", 0, 0 };
    static const QUMethod slot_22 = {"saveTemplate_", 0, 0 };
    static const QUParameter param_slot_23[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_23 = {"showTemplateStructure_", 1, param_slot_23 };
    static const QUMethod slot_24 = {"listTemplates_", 0, 0 };
    static const QUParameter param_slot_25[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_25 = {"driverChanged_", 2, param_slot_25 };
    static const QUParameter param_slot_26[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_26 = {"sensorChanged_", 2, param_slot_26 };
    static const QUParameter param_slot_27[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_27 = {"tableDriverClicked_", 2, param_slot_27 };
    static const QUParameter param_slot_28[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_28 = {"tableSensorClicked_", 2, param_slot_28 };
    static const QUParameter param_slot_29[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In },
	{ "mouse", &static_QUType_int, 0, QUParameter::In },
	{ "point", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_29 = {"tableTemplateClicked_", 4, param_slot_29 };
    static const QUParameter param_slot_30[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_30 = {"templateChanged_", 2, param_slot_30 };
    static const QUMethod slot_31 = {"descriptionChanged_", 0, 0 };
    static const QUMethod slot_32 = {"configRoute_", 0, 0 };
    static const QUMethod slot_33 = {"seeNet_", 0, 0 };
    static const QUMethod slot_34 = {"chooseNet_", 0, 0 };
    static const QUMethod slot_35 = {"newTemplate_", 0, 0 };
    static const QUMethod slot_36 = {"removeTemplate_", 0, 0 };
    static const QUMethod slot_37 = {"createNewNet_", 0, 0 };
    static const QUMethod slot_38 = {"runSimulation_", 0, 0 };
    static const QUMethod slot_39 = {"viewSimulation_", 0, 0 };
    static const QUMethod slot_40 = {"chooseAgentsFile_", 0, 0 };
    static const QUMethod slot_41 = {"saveAgents_", 0, 0 };
    static const QUMethod slot_42 = {"addAgent_", 0, 0 };
    static const QUMethod slot_43 = {"removeAgent_", 0, 0 };
    static const QUMethod slot_44 = {"saveAsAgents_", 0, 0 };
    static const QUParameter param_slot_45[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In },
	{ "mouse", &static_QUType_int, 0, QUParameter::In },
	{ "point", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_45 = {"tableAgentsClicked_", 4, param_slot_45 };
    static const QUParameter param_slot_46[] = {
	{ "agentSelected", &static_QUType_ptr, "QListBoxItem", QUParameter::In }
    };
    static const QUMethod slot_46 = {"listAgentsSelectionChanged_", 1, param_slot_46 };
    static const QUParameter param_slot_47[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_47 = {"agentChanged_", 2, param_slot_47 };
    static const QUParameter param_slot_48[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_48 = {"gammaChanged_", 2, param_slot_48 };
    static const QUParameter param_slot_49[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_49 = {"tauChanged_", 2, param_slot_49 };
    static const QUMethod slot_50 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "fileNew_()", &slot_0, QMetaData::Public },
	{ "fileOpen_()", &slot_1, QMetaData::Public },
	{ "fileSave_()", &slot_2, QMetaData::Public },
	{ "fileSaveAs_()", &slot_3, QMetaData::Public },
	{ "filePrint()", &slot_4, QMetaData::Public },
	{ "fileExit()", &slot_5, QMetaData::Public },
	{ "editUndo()", &slot_6, QMetaData::Public },
	{ "editRedo()", &slot_7, QMetaData::Public },
	{ "editCut()", &slot_8, QMetaData::Public },
	{ "editCopy()", &slot_9, QMetaData::Public },
	{ "editPaste()", &slot_10, QMetaData::Public },
	{ "editFind()", &slot_11, QMetaData::Public },
	{ "helpIndex()", &slot_12, QMetaData::Public },
	{ "helpContents()", &slot_13, QMetaData::Public },
	{ "helpAbout()", &slot_14, QMetaData::Public },
	{ "removeDriver_()", &slot_15, QMetaData::Public },
	{ "insertDriver_()", &slot_16, QMetaData::Public },
	{ "removeSensor_()", &slot_17, QMetaData::Public },
	{ "insertSensor_()", &slot_18, QMetaData::Public },
	{ "driverSelected_(const QString&)", &slot_19, QMetaData::Public },
	{ "sensorSelected_(const QString&)", &slot_20, QMetaData::Public },
	{ "fileClose()", &slot_21, QMetaData::Public },
	{ "saveTemplate_()", &slot_22, QMetaData::Public },
	{ "showTemplateStructure_(QListViewItem*)", &slot_23, QMetaData::Public },
	{ "listTemplates_()", &slot_24, QMetaData::Public },
	{ "driverChanged_(int,int)", &slot_25, QMetaData::Public },
	{ "sensorChanged_(int,int)", &slot_26, QMetaData::Public },
	{ "tableDriverClicked_(int,int)", &slot_27, QMetaData::Public },
	{ "tableSensorClicked_(int,int)", &slot_28, QMetaData::Public },
	{ "tableTemplateClicked_(int,int,int,const QPoint&)", &slot_29, QMetaData::Public },
	{ "templateChanged_(int,int)", &slot_30, QMetaData::Public },
	{ "descriptionChanged_()", &slot_31, QMetaData::Public },
	{ "configRoute_()", &slot_32, QMetaData::Public },
	{ "seeNet_()", &slot_33, QMetaData::Public },
	{ "chooseNet_()", &slot_34, QMetaData::Public },
	{ "newTemplate_()", &slot_35, QMetaData::Public },
	{ "removeTemplate_()", &slot_36, QMetaData::Public },
	{ "createNewNet_()", &slot_37, QMetaData::Public },
	{ "runSimulation_()", &slot_38, QMetaData::Public },
	{ "viewSimulation_()", &slot_39, QMetaData::Public },
	{ "chooseAgentsFile_()", &slot_40, QMetaData::Public },
	{ "saveAgents_()", &slot_41, QMetaData::Public },
	{ "addAgent_()", &slot_42, QMetaData::Public },
	{ "removeAgent_()", &slot_43, QMetaData::Public },
	{ "saveAsAgents_()", &slot_44, QMetaData::Public },
	{ "tableAgentsClicked_(int,int,int,const QPoint&)", &slot_45, QMetaData::Public },
	{ "listAgentsSelectionChanged_(QListBoxItem*)", &slot_46, QMetaData::Public },
	{ "agentChanged_(int,int)", &slot_47, QMetaData::Public },
	{ "gammaChanged_(int,int)", &slot_48, QMetaData::Public },
	{ "tauChanged_(int,int)", &slot_49, QMetaData::Public },
	{ "languageChange()", &slot_50, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MainWindow", parentObject,
	slot_tbl, 51,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MainWindow.setMetaObject( metaObj );
    return metaObj;
}

void* MainWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MainWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool MainWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: fileNew_(); break;
    case 1: fileOpen_(); break;
    case 2: static_QUType_bool.set(_o,fileSave_()); break;
    case 3: fileSaveAs_(); break;
    case 4: filePrint(); break;
    case 5: fileExit(); break;
    case 6: editUndo(); break;
    case 7: editRedo(); break;
    case 8: editCut(); break;
    case 9: editCopy(); break;
    case 10: editPaste(); break;
    case 11: editFind(); break;
    case 12: helpIndex(); break;
    case 13: helpContents(); break;
    case 14: helpAbout(); break;
    case 15: removeDriver_(); break;
    case 16: insertDriver_(); break;
    case 17: removeSensor_(); break;
    case 18: insertSensor_(); break;
    case 19: driverSelected_((const QString&)static_QUType_QString.get(_o+1)); break;
    case 20: sensorSelected_((const QString&)static_QUType_QString.get(_o+1)); break;
    case 21: fileClose(); break;
    case 22: saveTemplate_(); break;
    case 23: showTemplateStructure_((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 24: listTemplates_(); break;
    case 25: driverChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 26: sensorChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 27: tableDriverClicked_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 28: tableSensorClicked_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 29: tableTemplateClicked_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 30: templateChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 31: descriptionChanged_(); break;
    case 32: configRoute_(); break;
    case 33: seeNet_(); break;
    case 34: chooseNet_(); break;
    case 35: newTemplate_(); break;
    case 36: removeTemplate_(); break;
    case 37: createNewNet_(); break;
    case 38: runSimulation_(); break;
    case 39: viewSimulation_(); break;
    case 40: chooseAgentsFile_(); break;
    case 41: saveAgents_(); break;
    case 42: addAgent_(); break;
    case 43: removeAgent_(); break;
    case 44: saveAsAgents_(); break;
    case 45: tableAgentsClicked_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 46: listAgentsSelectionChanged_((QListBoxItem*)static_QUType_ptr.get(_o+1)); break;
    case 47: agentChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 48: gammaChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 49: tauChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 50: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MainWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MainWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool MainWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
