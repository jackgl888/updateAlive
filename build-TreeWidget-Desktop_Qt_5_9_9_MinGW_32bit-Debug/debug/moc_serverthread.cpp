/****************************************************************************
** Meta object code from reading C++ file 'serverthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../update1/serverthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serverthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_serverThread_t {
    QByteArrayData data[14];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_serverThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_serverThread_t qt_meta_stringdata_serverThread = {
    {
QT_MOC_LITERAL(0, 0, 12), // "serverThread"
QT_MOC_LITERAL(1, 13, 13), // "disconnectTCP"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 2), // "ip"
QT_MOC_LITERAL(4, 31, 13), // "sendUpdateCmd"
QT_MOC_LITERAL(5, 45, 3), // "cmd"
QT_MOC_LITERAL(6, 49, 4), // "type"
QT_MOC_LITERAL(7, 54, 4), // "para"
QT_MOC_LITERAL(8, 59, 17), // "sigSendRunningMsg"
QT_MOC_LITERAL(9, 77, 3), // "msg"
QT_MOC_LITERAL(10, 81, 5), // "value"
QT_MOC_LITERAL(11, 87, 16), // "disconnectToHost"
QT_MOC_LITERAL(12, 104, 17), // "slotSendUpdateCmd"
QT_MOC_LITERAL(13, 122, 18) // "slotSendRunningMsg"

    },
    "serverThread\0disconnectTCP\0\0ip\0"
    "sendUpdateCmd\0cmd\0type\0para\0"
    "sigSendRunningMsg\0msg\0value\0"
    "disconnectToHost\0slotSendUpdateCmd\0"
    "slotSendRunningMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_serverThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    3,   47,    2, 0x06 /* Public */,
       8,    4,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   63,    2, 0x08 /* Private */,
      12,    3,   64,    2, 0x08 /* Private */,
      13,    4,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::UShort, QMetaType::UChar, QMetaType::QVariant,    5,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::UShort, QMetaType::QStringList, QMetaType::UShort,    3,    5,    9,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::UShort, QMetaType::UChar, QMetaType::QVariant,    5,    6,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::UShort, QMetaType::QStringList, QMetaType::UShort,    3,    5,    9,   10,

       0        // eod
};

void serverThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        serverThread *_t = static_cast<serverThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->disconnectTCP((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->sendUpdateCmd((*reinterpret_cast< ushort(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 2: _t->sigSendRunningMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3])),(*reinterpret_cast< ushort(*)>(_a[4]))); break;
        case 3: _t->disconnectToHost(); break;
        case 4: _t->slotSendUpdateCmd((*reinterpret_cast< ushort(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 5: _t->slotSendRunningMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3])),(*reinterpret_cast< ushort(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (serverThread::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&serverThread::disconnectTCP)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (serverThread::*_t)(ushort , uchar , QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&serverThread::sendUpdateCmd)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (serverThread::*_t)(QString , ushort , QStringList , ushort );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&serverThread::sigSendRunningMsg)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject serverThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_serverThread.data,
      qt_meta_data_serverThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *serverThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *serverThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_serverThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int serverThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void serverThread::disconnectTCP(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void serverThread::sendUpdateCmd(ushort _t1, uchar _t2, QVariant _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void serverThread::sigSendRunningMsg(QString _t1, ushort _t2, QStringList _t3, ushort _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
