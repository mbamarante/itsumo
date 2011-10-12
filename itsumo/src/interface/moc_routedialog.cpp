/****************************************************************************
** RouteDialog meta object code from reading C++ file 'routedialog.h'
**
** Created: Wed Oct 12 20:19:10 2011
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "routedialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *RouteDialog::className() const
{
    return "RouteDialog";
}

QMetaObject *RouteDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RouteDialog( "RouteDialog", &RouteDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString RouteDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RouteDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RouteDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RouteDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RouteDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"ok_", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "from", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"fromChanged_", 1, param_slot_1 };
    static const QUMethod slot_2 = {"remove_", 0, 0 };
    static const QUMethod slot_3 = {"add_", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "pos", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"radioClicked_", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "to", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"toChanged_", 1, param_slot_5 };
    static const QUMethod slot_6 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "ok_()", &slot_0, QMetaData::Public },
	{ "fromChanged_(const QString&)", &slot_1, QMetaData::Public },
	{ "remove_()", &slot_2, QMetaData::Public },
	{ "add_()", &slot_3, QMetaData::Public },
	{ "radioClicked_(int)", &slot_4, QMetaData::Public },
	{ "toChanged_(const QString&)", &slot_5, QMetaData::Public },
	{ "languageChange()", &slot_6, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"RouteDialog", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RouteDialog.setMetaObject( metaObj );
    return metaObj;
}

void* RouteDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RouteDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool RouteDialog::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: ok_(); break;
    case 1: fromChanged_((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: remove_(); break;
    case 3: add_(); break;
    case 4: radioClicked_((int)static_QUType_int.get(_o+1)); break;
    case 5: toChanged_((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool RouteDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool RouteDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool RouteDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
