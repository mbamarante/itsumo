/****************************************************************************
** MainWindowImpl meta object code from reading C++ file 'mainwindowimpl.h'
**
** Created: Wed Oct 12 20:19:08 2011
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "mainwindowimpl.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MainWindowImpl::className() const
{
    return "MainWindowImpl";
}

QMetaObject *MainWindowImpl::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MainWindowImpl( "MainWindowImpl", &MainWindowImpl::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MainWindowImpl::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainWindowImpl", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MainWindowImpl::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainWindowImpl", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MainWindowImpl::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MainWindow::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "driver", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"insertDriver", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "list", &static_QUType_ptr, "TLList", QUParameter::In }
    };
    static const QUMethod slot_1 = {"tlToRoute", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "list", &static_QUType_ptr, "RouteList", QUParameter::In }
    };
    static const QUMethod slot_2 = {"routeToTl", 1, param_slot_2 };
    static const QUMethod slot_3 = {"fileNew_", 0, 0 };
    static const QUMethod slot_4 = {"fileOpen_", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_5 = {"fileSave_", 1, param_slot_5 };
    static const QUMethod slot_6 = {"fileSaveAs_", 0, 0 };
    static const QUMethod slot_7 = {"fileClose_", 0, 0 };
    static const QUMethod slot_8 = {"chooseNet_", 0, 0 };
    static const QUMethod slot_9 = {"seeNet_", 0, 0 };
    static const QUMethod slot_10 = {"configRoute_", 0, 0 };
    static const QUMethod slot_11 = {"editDrivers_", 0, 0 };
    static const QUMethod slot_12 = {"helpContents", 0, 0 };
    static const QUMethod slot_13 = {"helpAbout", 0, 0 };
    static const QUMethod slot_14 = {"removeDriver_", 0, 0 };
    static const QUMethod slot_15 = {"removeSensor_", 0, 0 };
    static const QUMethod slot_16 = {"insertDriver_", 0, 0 };
    static const QUMethod slot_17 = {"insertSensor_", 0, 0 };
    static const QUParameter param_slot_18[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"driverSelected_", 1, param_slot_18 };
    static const QUParameter param_slot_19[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_19 = {"sensorSelected_", 1, param_slot_19 };
    static const QUMethod slot_20 = {"showItems_", 0, 0 };
    static const QUMethod slot_21 = {"clearFields_", 0, 0 };
    static const QUParameter param_slot_22[] = {
	{ "all", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_22 = {"clearFields_", 1, param_slot_22 };
    static const QUMethod slot_23 = {"listTemplates_", 0, 0 };
    static const QUParameter param_slot_24[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_24 = {"showTemplateStructure_", 1, param_slot_24 };
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
    static const QUMethod slot_27 = {"agentChanged_", 2, param_slot_27 };
    static const QUParameter param_slot_28[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_28 = {"gammaChanged_", 2, param_slot_28 };
    static const QUParameter param_slot_29[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_29 = {"tauChanged_", 2, param_slot_29 };
    static const QUParameter param_slot_30[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_30 = {"templateChanged_", 2, param_slot_30 };
    static const QUParameter param_slot_31[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_31 = {"tableDriverClicked_", 2, param_slot_31 };
    static const QUParameter param_slot_32[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_32 = {"tableSensorClicked_", 2, param_slot_32 };
    static const QUParameter param_slot_33[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In },
	{ "mouse", &static_QUType_int, 0, QUParameter::In },
	{ "point", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_33 = {"tableTemplateClicked_", 4, param_slot_33 };
    static const QUMethod slot_34 = {"removeOption_", 0, 0 };
    static const QUMethod slot_35 = {"addOption_", 0, 0 };
    static const QUMethod slot_36 = {"saveTemplate_", 0, 0 };
    static const QUMethod slot_37 = {"descriptionChanged_", 0, 0 };
    static const QUParameter param_slot_38[] = {
	{ "rl", &static_QUType_ptr, "RouteList", QUParameter::In }
    };
    static const QUMethod slot_38 = {"setDriverRoutes_", 1, param_slot_38 };
    static const QUMethod slot_39 = {"newTemplate_", 0, 0 };
    static const QUMethod slot_40 = {"removeTemplate_", 0, 0 };
    static const QUMethod slot_41 = {"runSimulation_", 0, 0 };
    static const QUMethod slot_42 = {"viewSimulation_", 0, 0 };
    static const QUParameter param_slot_43[] = {
	{ "file", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_43 = {"view3D_", 1, param_slot_43 };
    static const QUMethod slot_44 = {"chooseAgentsFile_", 0, 0 };
    static const QUMethod slot_45 = {"createNewNet_", 0, 0 };
    static const QUMethod slot_46 = {"runAgents_", 0, 0 };
    static const QUParameter param_slot_47[] = {
	{ "list", &static_QUType_ptr, "XmlAgentList", QUParameter::In }
    };
    static const QUMethod slot_47 = {"showAgents_", 1, param_slot_47 };
    static const QUMethod slot_48 = {"addAgent_", 0, 0 };
    static const QUParameter param_slot_49[] = {
	{ "agentSelected", &static_QUType_ptr, "QListBoxItem", QUParameter::In }
    };
    static const QUMethod slot_49 = {"listAgentsSelectionChanged_", 1, param_slot_49 };
    static const QUMethod slot_50 = {"removeAgent_", 0, 0 };
    static const QUMethod slot_51 = {"saveAgents_", 0, 0 };
    static const QUMethod slot_52 = {"saveAsAgents_", 0, 0 };
    static const QUParameter param_slot_53[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In },
	{ "mouse", &static_QUType_int, 0, QUParameter::In },
	{ "point", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_53 = {"tableAgentsClicked_", 4, param_slot_53 };
    static const QUParameter param_slot_54[] = {
	{ "list", &static_QUType_ptr, "RouteList", QUParameter::In }
    };
    static const QUMethod slot_54 = {"setNewAgent_", 1, param_slot_54 };
    static const QMetaData slot_tbl[] = {
	{ "insertDriver(QString)", &slot_0, QMetaData::Public },
	{ "tlToRoute(TLList)", &slot_1, QMetaData::Public },
	{ "routeToTl(RouteList)", &slot_2, QMetaData::Public },
	{ "fileNew_()", &slot_3, QMetaData::Private },
	{ "fileOpen_()", &slot_4, QMetaData::Private },
	{ "fileSave_()", &slot_5, QMetaData::Private },
	{ "fileSaveAs_()", &slot_6, QMetaData::Private },
	{ "fileClose_()", &slot_7, QMetaData::Private },
	{ "chooseNet_()", &slot_8, QMetaData::Private },
	{ "seeNet_()", &slot_9, QMetaData::Private },
	{ "configRoute_()", &slot_10, QMetaData::Private },
	{ "editDrivers_()", &slot_11, QMetaData::Private },
	{ "helpContents()", &slot_12, QMetaData::Private },
	{ "helpAbout()", &slot_13, QMetaData::Private },
	{ "removeDriver_()", &slot_14, QMetaData::Private },
	{ "removeSensor_()", &slot_15, QMetaData::Private },
	{ "insertDriver_()", &slot_16, QMetaData::Private },
	{ "insertSensor_()", &slot_17, QMetaData::Private },
	{ "driverSelected_(const QString&)", &slot_18, QMetaData::Private },
	{ "sensorSelected_(const QString&)", &slot_19, QMetaData::Private },
	{ "showItems_()", &slot_20, QMetaData::Private },
	{ "clearFields_()", &slot_21, QMetaData::Private },
	{ "clearFields_(bool)", &slot_22, QMetaData::Private },
	{ "listTemplates_()", &slot_23, QMetaData::Private },
	{ "showTemplateStructure_(QListViewItem*)", &slot_24, QMetaData::Private },
	{ "driverChanged_(int,int)", &slot_25, QMetaData::Private },
	{ "sensorChanged_(int,int)", &slot_26, QMetaData::Private },
	{ "agentChanged_(int,int)", &slot_27, QMetaData::Private },
	{ "gammaChanged_(int,int)", &slot_28, QMetaData::Private },
	{ "tauChanged_(int,int)", &slot_29, QMetaData::Private },
	{ "templateChanged_(int,int)", &slot_30, QMetaData::Private },
	{ "tableDriverClicked_(int,int)", &slot_31, QMetaData::Private },
	{ "tableSensorClicked_(int,int)", &slot_32, QMetaData::Private },
	{ "tableTemplateClicked_(int,int,int,const QPoint&)", &slot_33, QMetaData::Private },
	{ "removeOption_()", &slot_34, QMetaData::Private },
	{ "addOption_()", &slot_35, QMetaData::Private },
	{ "saveTemplate_()", &slot_36, QMetaData::Private },
	{ "descriptionChanged_()", &slot_37, QMetaData::Private },
	{ "setDriverRoutes_(RouteList)", &slot_38, QMetaData::Private },
	{ "newTemplate_()", &slot_39, QMetaData::Private },
	{ "removeTemplate_()", &slot_40, QMetaData::Private },
	{ "runSimulation_()", &slot_41, QMetaData::Private },
	{ "viewSimulation_()", &slot_42, QMetaData::Private },
	{ "view3D_(QString)", &slot_43, QMetaData::Private },
	{ "chooseAgentsFile_()", &slot_44, QMetaData::Private },
	{ "createNewNet_()", &slot_45, QMetaData::Private },
	{ "runAgents_()", &slot_46, QMetaData::Private },
	{ "showAgents_(XmlAgentList)", &slot_47, QMetaData::Private },
	{ "addAgent_()", &slot_48, QMetaData::Private },
	{ "listAgentsSelectionChanged_(QListBoxItem*)", &slot_49, QMetaData::Private },
	{ "removeAgent_()", &slot_50, QMetaData::Private },
	{ "saveAgents_()", &slot_51, QMetaData::Private },
	{ "saveAsAgents_()", &slot_52, QMetaData::Private },
	{ "tableAgentsClicked_(int,int,int,const QPoint&)", &slot_53, QMetaData::Private },
	{ "setNewAgent_(RouteList)", &slot_54, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MainWindowImpl", parentObject,
	slot_tbl, 55,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MainWindowImpl.setMetaObject( metaObj );
    return metaObj;
}

void* MainWindowImpl::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MainWindowImpl" ) )
	return this;
    return MainWindow::qt_cast( clname );
}

bool MainWindowImpl::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: insertDriver((QString)static_QUType_QString.get(_o+1)); break;
    case 1: tlToRoute((TLList)(*((TLList*)static_QUType_ptr.get(_o+1)))); break;
    case 2: routeToTl((RouteList)(*((RouteList*)static_QUType_ptr.get(_o+1)))); break;
    case 3: fileNew_(); break;
    case 4: fileOpen_(); break;
    case 5: static_QUType_bool.set(_o,fileSave_()); break;
    case 6: fileSaveAs_(); break;
    case 7: fileClose_(); break;
    case 8: chooseNet_(); break;
    case 9: seeNet_(); break;
    case 10: configRoute_(); break;
    case 11: editDrivers_(); break;
    case 12: helpContents(); break;
    case 13: helpAbout(); break;
    case 14: removeDriver_(); break;
    case 15: removeSensor_(); break;
    case 16: insertDriver_(); break;
    case 17: insertSensor_(); break;
    case 18: driverSelected_((const QString&)static_QUType_QString.get(_o+1)); break;
    case 19: sensorSelected_((const QString&)static_QUType_QString.get(_o+1)); break;
    case 20: showItems_(); break;
    case 21: clearFields_(); break;
    case 22: clearFields_((bool)static_QUType_bool.get(_o+1)); break;
    case 23: listTemplates_(); break;
    case 24: showTemplateStructure_((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 25: driverChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 26: sensorChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 27: agentChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 28: gammaChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 29: tauChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 30: templateChanged_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 31: tableDriverClicked_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 32: tableSensorClicked_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 33: tableTemplateClicked_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 34: removeOption_(); break;
    case 35: addOption_(); break;
    case 36: saveTemplate_(); break;
    case 37: descriptionChanged_(); break;
    case 38: setDriverRoutes_((RouteList)(*((RouteList*)static_QUType_ptr.get(_o+1)))); break;
    case 39: newTemplate_(); break;
    case 40: removeTemplate_(); break;
    case 41: runSimulation_(); break;
    case 42: viewSimulation_(); break;
    case 43: view3D_((QString)static_QUType_QString.get(_o+1)); break;
    case 44: chooseAgentsFile_(); break;
    case 45: createNewNet_(); break;
    case 46: runAgents_(); break;
    case 47: showAgents_((XmlAgentList)(*((XmlAgentList*)static_QUType_ptr.get(_o+1)))); break;
    case 48: addAgent_(); break;
    case 49: listAgentsSelectionChanged_((QListBoxItem*)static_QUType_ptr.get(_o+1)); break;
    case 50: removeAgent_(); break;
    case 51: saveAgents_(); break;
    case 52: saveAsAgents_(); break;
    case 53: tableAgentsClicked_((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+4))); break;
    case 54: setNewAgent_((RouteList)(*((RouteList*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return MainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MainWindowImpl::qt_emit( int _id, QUObject* _o )
{
    return MainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MainWindowImpl::qt_property( int id, int f, QVariant* v)
{
    return MainWindow::qt_property( id, f, v);
}

bool MainWindowImpl::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
