/****************************************************************************
** Meta object code from reading C++ file 'mysocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../update1/mysocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mysocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MySocket_t {
    QByteArrayData data[32];
    char stringdata0[331];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MySocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MySocket_t qt_meta_stringdata_MySocket = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MySocket"
QT_MOC_LITERAL(1, 9, 13), // "sigRunMsgToUi"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 2), // "ip"
QT_MOC_LITERAL(4, 27, 3), // "cmd"
QT_MOC_LITERAL(5, 31, 3), // "msg"
QT_MOC_LITERAL(6, 35, 5), // "value"
QT_MOC_LITERAL(7, 41, 13), // "timeoutMethod"
QT_MOC_LITERAL(8, 55, 13), // "slotConnected"
QT_MOC_LITERAL(9, 69, 16), // "slotDisconnected"
QT_MOC_LITERAL(10, 86, 8), // "recvData"
QT_MOC_LITERAL(11, 95, 14), // "recvDataMethod"
QT_MOC_LITERAL(12, 110, 12), // "const uchar*"
QT_MOC_LITERAL(13, 123, 4), // "data"
QT_MOC_LITERAL(14, 128, 12), // "slotDataSent"
QT_MOC_LITERAL(15, 141, 4), // "type"
QT_MOC_LITERAL(16, 146, 7), // "variant"
QT_MOC_LITERAL(17, 154, 16), // "cmdConnectTarget"
QT_MOC_LITERAL(18, 171, 6), // "target"
QT_MOC_LITERAL(19, 178, 4), // "addr"
QT_MOC_LITERAL(20, 183, 14), // "cmdAppJumpBoot"
QT_MOC_LITERAL(21, 198, 19), // "bootEraseAppSectors"
QT_MOC_LITERAL(22, 218, 16), // "bootWriteAppdata"
QT_MOC_LITERAL(23, 235, 12), // "txframeIndex"
QT_MOC_LITERAL(24, 248, 13), // "bootJumpToApp"
QT_MOC_LITERAL(25, 262, 19), // "readBackLcformerApp"
QT_MOC_LITERAL(26, 282, 10), // "backUpFile"
QT_MOC_LITERAL(27, 293, 8), // "filename"
QT_MOC_LITERAL(28, 302, 11), // "tcpSendData"
QT_MOC_LITERAL(29, 314, 6), // "uchar*"
QT_MOC_LITERAL(30, 321, 5), // "txBuf"
QT_MOC_LITERAL(31, 327, 3) // "len"

    },
    "MySocket\0sigRunMsgToUi\0\0ip\0cmd\0msg\0"
    "value\0timeoutMethod\0slotConnected\0"
    "slotDisconnected\0recvData\0recvDataMethod\0"
    "const uchar*\0data\0slotDataSent\0type\0"
    "variant\0cmdConnectTarget\0target\0addr\0"
    "cmdAppJumpBoot\0bootEraseAppSectors\0"
    "bootWriteAppdata\0txframeIndex\0"
    "bootJumpToApp\0readBackLcformerApp\0"
    "backUpFile\0filename\0tcpSendData\0uchar*\0"
    "txBuf\0len"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MySocket[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   98,    2, 0x08 /* Private */,
       8,    0,   99,    2, 0x08 /* Private */,
       9,    0,  100,    2, 0x08 /* Private */,
      10,    0,  101,    2, 0x08 /* Private */,
      11,    1,  102,    2, 0x08 /* Private */,
      14,    3,  105,    2, 0x08 /* Private */,
      17,    2,  112,    2, 0x08 /* Private */,
      20,    2,  117,    2, 0x08 /* Private */,
      21,    2,  122,    2, 0x08 /* Private */,
      22,    3,  127,    2, 0x08 /* Private */,
      24,    2,  134,    2, 0x08 /* Private */,
      25,    0,  139,    2, 0x08 /* Private */,
      26,    1,  140,    2, 0x08 /* Private */,
      28,    2,  143,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UShort, QMetaType::QStringList, QMetaType::UShort,    3,    4,    5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::UShort, QMetaType::UChar, QMetaType::QVariant,    4,   15,   16,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar,   18,   19,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar,   18,   19,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar,   18,   19,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar, QMetaType::UShort,   18,   19,   23,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar,   18,   19,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void, 0x80000000 | 29, QMetaType::UShort,   30,   31,

       0        // eod
};

void MySocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MySocket *_t = static_cast<MySocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigRunMsgToUi((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3])),(*reinterpret_cast< ushort(*)>(_a[4]))); break;
        case 1: _t->timeoutMethod(); break;
        case 2: _t->slotConnected(); break;
        case 3: _t->slotDisconnected(); break;
        case 4: _t->recvData(); break;
        case 5: _t->recvDataMethod((*reinterpret_cast< const uchar*(*)>(_a[1]))); break;
        case 6: _t->slotDataSent((*reinterpret_cast< ushort(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 7: _t->cmdConnectTarget((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        case 8: _t->cmdAppJumpBoot((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        case 9: _t->bootEraseAppSectors((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        case 10: _t->bootWriteAppdata((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3]))); break;
        case 11: _t->bootJumpToApp((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        case 12: _t->readBackLcformerApp(); break;
        case 13: _t->backUpFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->tcpSendData((*reinterpret_cast< uchar*(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MySocket::*_t)(QString , ushort , QStringList , ushort );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MySocket::sigRunMsgToUi)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MySocket::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_MySocket.data,
      qt_meta_data_MySocket,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MySocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MySocket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MySocket.stringdata0))
        return static_cast<void*>(this);
    return QTcpSocket::qt_metacast(_clname);
}

int MySocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void MySocket::sigRunMsgToUi(QString _t1, ushort _t2, QStringList _t3, ushort _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
