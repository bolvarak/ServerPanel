/****************************************************************************
** Meta object code from reading C++ file 'ServerPanelService.h'
**
** Created: Sun May 27 19:23:20 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ServerPanelService/ServerPanelService.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ServerPanelService.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ServerPanelService[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x0a,
      39,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ServerPanelService[] = {
    "ServerPanelService\0\0acceptConnection()\0"
    "startRead()\0"
};

void ServerPanelService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ServerPanelService *_t = static_cast<ServerPanelService *>(_o);
        switch (_id) {
        case 0: _t->acceptConnection(); break;
        case 1: _t->startRead(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ServerPanelService::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ServerPanelService::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ServerPanelService,
      qt_meta_data_ServerPanelService, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ServerPanelService::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ServerPanelService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ServerPanelService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ServerPanelService))
        return static_cast<void*>(const_cast< ServerPanelService*>(this));
    return QObject::qt_metacast(_clname);
}

int ServerPanelService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
