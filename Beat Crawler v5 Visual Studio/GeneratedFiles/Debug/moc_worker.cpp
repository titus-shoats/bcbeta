/****************************************************************************
** Meta object code from reading C++ file 'worker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../worker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'worker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Worker_t {
    QByteArrayData data[28];
    char stringdata0[380];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Worker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Worker_t qt_meta_stringdata_Worker = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Worker"
QT_MOC_LITERAL(1, 7, 13), // "workRequested"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 12), // "valueChanged"
QT_MOC_LITERAL(4, 35, 5), // "value"
QT_MOC_LITERAL(5, 41, 14), // "workRequested1"
QT_MOC_LITERAL(6, 56, 8), // "finished"
QT_MOC_LITERAL(7, 65, 14), // "emitParameters"
QT_MOC_LITERAL(8, 80, 13), // "emitEmailList"
QT_MOC_LITERAL(9, 94, 1), // "s"
QT_MOC_LITERAL(10, 96, 16), // "emitKeywordQueue"
QT_MOC_LITERAL(11, 113, 22), // "senderCurlResponseInfo"
QT_MOC_LITERAL(12, 136, 6), // "doWork"
QT_MOC_LITERAL(13, 143, 8), // "getParam"
QT_MOC_LITERAL(14, 152, 3), // "url"
QT_MOC_LITERAL(15, 156, 9), // "userAgent"
QT_MOC_LITERAL(16, 166, 15), // "QList<QString>*"
QT_MOC_LITERAL(17, 182, 12), // "proxyServers"
QT_MOC_LITERAL(18, 195, 12), // "getProxyFile"
QT_MOC_LITERAL(19, 208, 8), // "fileName"
QT_MOC_LITERAL(20, 217, 24), // "receiverEmptyProxyServer"
QT_MOC_LITERAL(21, 242, 7), // "isEmpty"
QT_MOC_LITERAL(22, 250, 26), // "receiverStopThreadCounters"
QT_MOC_LITERAL(23, 277, 17), // "stopThreadCounter"
QT_MOC_LITERAL(24, 295, 27), // "receiverStartThreadCounters"
QT_MOC_LITERAL(25, 323, 18), // "startThreadCounter"
QT_MOC_LITERAL(26, 342, 22), // "receiverHarvesterTimer"
QT_MOC_LITERAL(27, 365, 14) // "harvesterTimer"

    },
    "Worker\0workRequested\0\0valueChanged\0"
    "value\0workRequested1\0finished\0"
    "emitParameters\0emitEmailList\0s\0"
    "emitKeywordQueue\0senderCurlResponseInfo\0"
    "doWork\0getParam\0url\0userAgent\0"
    "QList<QString>*\0proxyServers\0getProxyFile\0"
    "fileName\0receiverEmptyProxyServer\0"
    "isEmpty\0receiverStopThreadCounters\0"
    "stopThreadCounter\0receiverStartThreadCounters\0"
    "startThreadCounter\0receiverHarvesterTimer\0"
    "harvesterTimer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Worker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x06 /* Public */,
       3,    1,   90,    2, 0x06 /* Public */,
       5,    0,   93,    2, 0x06 /* Public */,
       6,    0,   94,    2, 0x06 /* Public */,
       7,    0,   95,    2, 0x06 /* Public */,
       8,    1,   96,    2, 0x06 /* Public */,
      10,    0,   99,    2, 0x06 /* Public */,
      11,    1,  100,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  103,    2, 0x0a /* Public */,
      13,    3,  104,    2, 0x0a /* Public */,
      18,    1,  111,    2, 0x0a /* Public */,
      20,    1,  114,    2, 0x0a /* Public */,
      22,    1,  117,    2, 0x0a /* Public */,
      24,    1,  120,    2, 0x0a /* Public */,
      26,    1,  123,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 16,   14,   15,   17,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, QMetaType::Int,   27,

       0        // eod
};

void Worker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Worker *_t = static_cast<Worker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->workRequested(); break;
        case 1: _t->valueChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->workRequested1(); break;
        case 3: _t->finished(); break;
        case 4: _t->emitParameters(); break;
        case 5: _t->emitEmailList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->emitKeywordQueue(); break;
        case 7: _t->senderCurlResponseInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->doWork(); break;
        case 9: _t->getParam((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QList<QString>*(*)>(_a[3]))); break;
        case 10: _t->getProxyFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->receiverEmptyProxyServer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->receiverStopThreadCounters((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->receiverStartThreadCounters((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->receiverHarvesterTimer((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Worker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Worker::workRequested)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Worker::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Worker::valueChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Worker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Worker::workRequested1)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Worker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Worker::finished)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (Worker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Worker::emitParameters)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (Worker::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Worker::emitEmailList)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (Worker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Worker::emitKeywordQueue)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (Worker::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Worker::senderCurlResponseInfo)) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject Worker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Worker.data,
      qt_meta_data_Worker,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Worker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Worker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Worker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Worker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void Worker::workRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Worker::valueChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Worker::workRequested1()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Worker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Worker::emitParameters()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Worker::emitEmailList(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Worker::emitKeywordQueue()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void Worker::senderCurlResponseInfo(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
