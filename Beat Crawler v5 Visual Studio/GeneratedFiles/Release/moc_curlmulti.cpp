/****************************************************************************
** Meta object code from reading C++ file 'curlmulti.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../curlmulti.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'curlmulti.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CurlMulti_t {
    QByteArrayData data[7];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CurlMulti_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CurlMulti_t qt_meta_stringdata_CurlMulti = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CurlMulti"
QT_MOC_LITERAL(1, 10, 16), // "curlMultiTimeout"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 15), // "socketReadyRead"
QT_MOC_LITERAL(4, 44, 16), // "socketDescriptor"
QT_MOC_LITERAL(5, 61, 16), // "socketReadyWrite"
QT_MOC_LITERAL(6, 78, 15) // "socketException"

    },
    "CurlMulti\0curlMultiTimeout\0\0socketReadyRead\0"
    "socketDescriptor\0socketReadyWrite\0"
    "socketException"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CurlMulti[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x09 /* Protected */,
       3,    1,   35,    2, 0x09 /* Protected */,
       5,    1,   38,    2, 0x09 /* Protected */,
       6,    1,   41,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,

       0        // eod
};

void CurlMulti::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CurlMulti *_t = static_cast<CurlMulti *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->curlMultiTimeout(); break;
        case 1: _t->socketReadyRead((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->socketReadyWrite((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->socketException((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CurlMulti::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CurlMulti.data,
      qt_meta_data_CurlMulti,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CurlMulti::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CurlMulti::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CurlMulti.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CurlMulti::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
