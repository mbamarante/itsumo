/****************************************************************************
** RouteDialogImpl meta object code from reading C++ file 'routedialogimpl.h'
**
** Created: Wed Oct 12 20:19:09 2011
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "routedialogimpl.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#include <qvariant.h>
const char *RouteDialogImpl::className() const
{
    return "RouteDialogImpl";
}

QMetaObject *RouteDialogImpl::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RouteDialogImpl( "RouteDialogImpl", &RouteDialogImpl::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString RouteDialogImpl::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RouteDialogImpl", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RouteDialogImpl::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RouteDialogImpl", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RouteDialogImpl::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = RouteDialog::staticMetaObject();
    static const QUMethod slot_0 = {"show_", 0, 0 };
    static const QUMethod slot_1 = {"ok_", 0, 0 };
    static const QUMethod slot_2 = {"add_", 0, 0 };
    static const QUMethod slot_3 = {"remove_", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "to", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"toChanged_", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "from", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"fromChanged_", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "pos", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"radioClicked_", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "r", &static_QUType_ptr, "Route", QUParameter::In }
    };
    static const QUMethod slot_7 = {"exists_", 2, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ 0, &static_QUType_QVariant, "\x0e", QUParameter::Out },
	{ "p", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_8 = {"realPoint_", 2, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "from", &static_QUType_QString, 0, QUParameter::In },
	{ "to", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"redrawNet_", 2, param_slot_9 };
    static const QMetaData slot_tbl[] = {
	{ "show_()", &slot_0, QMetaData::Private },
	{ "ok_()", &slot_1, QMetaData::Private },
	{ "add_()", &slot_2, QMetaData::Private },
	{ "remove_()", &slot_3, QMetaData::Private },
	{ "toChanged_(const QString&)", &slot_4, QMetaData::Private },
	{ "fromChanged_(const QString&)", &slot_5, QMetaData::Private },
	{ "radioClicked_(int)", &slot_6, QMetaData::Private },
	{ "exists_(const Route&)", &slot_7, QMetaData::Private },
	{ "realPoint_(QPoint)", &slot_8, QMetaData::Private },
	{ "redrawNet_(const QString&,const QString&)", &slot_9, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "rl", &static_QUType_ptr, "RouteList", QUParameter::In }
    };
    static const QUMethod signal_0 = {"done", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "done(RouteList)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"RouteDialogImpl", parentObject,
	slot_tbl, 10,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RouteDialogImpl.setMetaObject( metaObj );
    return metaObj;
}

void* RouteDialogImpl::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RouteDialogImpl" ) )
	return this;
    return RouteDialog::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL done
void RouteDialogImpl::done( RouteList t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool RouteDialogImpl::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: show_(); break;
    case 1: ok_(); break;
    case 2: add_(); break;
    case 3: remove_(); break;
    case 4: toChanged_((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: fromChanged_((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: radioClicked_((int)static_QUType_int.get(_o+1)); break;
    case 7: static_QUType_bool.set(_o,exists_((const Route&)*((const Route*)static_QUType_ptr.get(_o+1)))); break;
    case 8: static_QUType_QVariant.set(_o,QVariant(realPoint_((QPoint)(*((QPoint*)static_QUType_ptr.get(_o+1)))))); break;
    case 9: redrawNet_((const QString&)static_QUType_QString.get(_o+1),(const QString&)static_QUType_QString.get(_o+2)); break;
    default:
	return RouteDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool RouteDialogImpl::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: done((RouteList)(*((RouteList*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return RouteDialog::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool RouteDialogImpl::qt_property( int id, int f, QVariant* v)
{
    return RouteDialog::qt_property( id, f, v);
}

bool RouteDialogImpl::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
