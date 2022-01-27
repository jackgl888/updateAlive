/****************************************************************************
** Meta object code from reading C++ file 'myserial.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../update1/myserial.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myserial.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Myserial_t {
    QByteArrayData data[27];
    char stringdata0[277];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Myserial_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Myserial_t qt_meta_stringdata_Myserial = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Myserial"
QT_MOC_LITERAL(1, 9, 13), // "sigRunMsgToUi"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 2), // "ip"
QT_MOC_LITERAL(4, 27, 3), // "cmd"
QT_MOC_LITERAL(5, 31, 3), // "msg"
QT_MOC_LITERAL(6, 35, 5), // "value"
QT_MOC_LITERAL(7, 41, 13), // "timeoutMethod"
QT_MOC_LITERAL(8, 55, 8), // "recvData"
QT_MOC_LITERAL(9, 64, 14), // "recvDataMethod"
QT_MOC_LITERAL(10, 79, 12), // "const uchar*"
QT_MOC_LITERAL(11, 92, 4), // "data"
QT_MOC_LITERAL(12, 97, 12), // "slotDataSent"
QT_MOC_LITERAL(13, 110, 4), // "type"
QT_MOC_LITERAL(14, 115, 7), // "variant"
QT_MOC_LITERAL(15, 123, 15), // "conncetToLcBoot"
QT_MOC_LITERAL(16, 139, 6), // "target"
QT_MOC_LITERAL(17, 146, 4), // "addr"
QT_MOC_LITERAL(18, 151, 21), // "lcbootEraseAppSectors"
QT_MOC_LITERAL(19, 173, 26), // "lcBootReplyConnectAnalysis"
QT_MOC_LITERAL(20, 200, 18), // "lcbootWriteAppdata"
QT_MOC_LITERAL(21, 219, 12), // "txframeIndex"
QT_MOC_LITERAL(22, 232, 15), // "lcbootJumpToApp"
QT_MOC_LITERAL(23, 248, 11), // "tcpSendData"
QT_MOC_LITERAL(24, 260, 6), // "uchar*"
QT_MOC_LITERAL(25, 267, 5), // "txBuf"
QT_MOC_LITERAL(26, 273, 3) // "len"

    },
    "Myserial\0sigRunMsgToUi\0\0ip\0cmd\0msg\0"
    "value\0timeoutMethod\0recvData\0"
    "recvDataMethod\0const uchar*\0data\0"
    "slotDataSent\0type\0variant\0conncetToLcBoot\0"
    "target\0addr\0lcbootEraseAppSectors\0"
    "lcBootReplyConnectAnalysis\0"
    "lcbootWriteAppdata\0txframeIndex\0"
    "lcbootJumpToApp\0tcpSendData\0uchar*\0"
    "txBuf\0len"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Myserial[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   78,    2, 0x08 /* Private */,
       8,    0,   79,    2, 0x08 /* Private */,
       9,    1,   80,    2, 0x08 /* Private */,
      12,    3,   83,    2, 0x08 /* Private */,
      15,    2,   90,    2, 0x08 /* Private */,
      18,    2,   95,    2, 0x08 /* Private */,
      19,    1,  100,    2, 0x08 /* Private */,
      20,    3,  103,    2, 0x08 /* Private */,
      22,    2,  110,    2, 0x08 /* Private */,
      23,    2,  115,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UShort, QMetaType::QStringList, QMetaType::UShort,    3,    4,    5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::UShort, QMetaType::UChar, QMetaType::QVariant,    4,   13,   14,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar,   16,   17,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar,   16,   17,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar, QMetaType::UShort,   16,   17,   21,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar,   16,   17,
    QMetaType::Void, 0x80000000 | 24, QMetaType::UShort,   25,   26,

       0        // eod
};

void Myserial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Myserial *_t = static_cast<Myserial *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigRunMsgToUi((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< ushort(*)>(_a[2])),(*reinterpret_cast< QStringList(*)>(_a[3])),(*reinterpret_cast< ushort(*)>(_a[4]))); break;
        case 1: _t->timeoutMethod(); break;
        case 2: _t->recvData(); break;
        case 3: _t->recvDataMethod((*reinterpret_cast< const uchar*(*)>(_a[1]))); break;
        case 4: _t->slotDataSent((*reinterpret_cast< ushort(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        case 5: _t->conncetToLcBoot((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        case 6: _t->lcbootEraseAppSectors((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        case 7: _t->lcBootReplyConnectAnalysis((*reinterpret_cast< const uchar*(*)>(_a[1]))); break;
        case 8: _t->lcbootWriteAppdata((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< ushort(*)>(_a[3]))); break;
        case 9: _t->lcbootJumpToApp((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2]))); break;
        case 10: _t->tcpSendData((*reinterpret_cast< uchar*(*)>(_a[1])),(*reinterpret_cast< const ushort(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Myserial::*_t)(QString , ushort , QStringList , ushort );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Myserial::sigRunMsgToUi)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Myserial::staticMetaObject = {
    { &QSerialPort::staticMetaObject, qt_meta_stringdata_Myserial.data,
      qt_meta_data_Myserial,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Myserial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Myserial::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Myserial.stringdata0))
        return static_cast<void*>(this);
    return QSerialPort::qt_metacast(_clname);
}

int Myserial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSerialPort::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void Myserial::sigRunMsgToUi(QString _t1, ushort _t2, QStringList _t3, ushort _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
