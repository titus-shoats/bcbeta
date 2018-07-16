/****************************************************************************
** Meta object code from reading C++ file 'filereader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../filereader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filereader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FileReader_t {
    QByteArrayData data[12];
    char stringdata0[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FileReader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FileReader_t qt_meta_stringdata_FileReader = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FileReader"
QT_MOC_LITERAL(1, 11, 29), // "emitSenderFileReadKeywordList"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 13), // "workRequested"
QT_MOC_LITERAL(4, 56, 12), // "valueChanged"
QT_MOC_LITERAL(5, 69, 5), // "value"
QT_MOC_LITERAL(6, 75, 8), // "finished"
QT_MOC_LITERAL(7, 84, 16), // "receiverReadFile"
QT_MOC_LITERAL(8, 101, 8), // "fileName"
QT_MOC_LITERAL(9, 110, 28), // "receiverStopFileReaderThread"
QT_MOC_LITERAL(10, 139, 1), // "b"
QT_MOC_LITERAL(11, 141, 6) // "doWork"

    },
    "FileReader\0emitSenderFileReadKeywordList\0"
    "\0workRequested\0valueChanged\0value\0"
    "finished\0receiverReadFile\0fileName\0"
    "receiverStopFileReaderThread\0b\0doWork"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FileReader[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   49,    2, 0x06 /* Public */,
       3,    0,   56,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       6,    0,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   61,    2, 0x0a /* Public */,
       9,    1,   64,    2, 0x0a /* Public */,
      11,    0,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void,

       0        // eod
};

void FileReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FileReader *_t = static_cast<FileReader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->emitSenderFileReadKeywordList((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->workRequested(); break;
        case 2: _t->valueChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->finished(); break;
        case 4: _t->receiverReadFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->receiverStopFileReaderThread((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->doWork(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (FileReader::*_t)(QString , QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileReader::emitSenderFileReadKeywordList)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FileReader::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileReader::workRequested)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (FileReader::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileReader::valueChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (FileReader::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FileReader::finished)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject FileReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FileReader.data,
      qt_meta_data_FileReader,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FileReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileReader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FileReader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FileReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void FileReader::emitSenderFileReadKeywordList(QString _t1, QString _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FileReader::workRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void FileReader::valueChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FileReader::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
