/****************************************************************************
** ChooseDialogImpl meta object code from reading C++ file 'choosedialogimpl.h'
**
** Created: Wed Oct 12 20:19:08 2011
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "choosedialogimpl.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ChooseDialogImpl::className() const
{
    return "ChooseDialogImpl";
}

QMetaObject *ChooseDialogImpl::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ChooseDialogImpl( "ChooseDialogImpl", &ChooseDialogImpl::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ChooseDialogImpl::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ChooseDialogImpl", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ChooseDialogImpl::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ChooseDialogImpl", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ChooseDialogImpl::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ChooseDialog::staticMetaObject();
    static const QUMethod slot_0 = {"ok", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "ok()", &slot_0, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "item", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"selected", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "selected(QString)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ChooseDialogImpl", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ChooseDialogImpl.setMetaObject( metaObj );
    return metaObj;
}

void* ChooseDialogImpl::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ChooseDialogImpl" ) )
	return this;
    return ChooseDialog::qt_cast( clname );
}

// SIGNAL selected
void ChooseDialogImpl::selected( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool ChooseDialogImpl::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: ok(); break;
    default:
	return ChooseDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ChooseDialogImpl::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: selected((QString)static_QUType_QString.get(_o+1)); break;
    default:
	return ChooseDialog::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ChooseDialogImpl::qt_property( int id, int f, QVariant* v)
{
    return ChooseDialog::qt_property( id, f, v);
}

bool ChooseDialogImpl::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
