/****************************************************************************
** Meta object code from reading C++ file 'PlotController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PlotController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PlotController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PlotController_t {
    QByteArrayData data[10];
    char stringdata[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_PlotController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_PlotController_t qt_meta_stringdata_PlotController = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 10),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 3),
QT_MOC_LITERAL(4, 31, 15),
QT_MOC_LITERAL(5, 47, 12),
QT_MOC_LITERAL(6, 60, 5),
QT_MOC_LITERAL(7, 66, 16),
QT_MOC_LITERAL(8, 83, 18),
QT_MOC_LITERAL(9, 102, 12)
    },
    "PlotController\0handleData\0\0qid\0"
    "mouseMoveSignal\0QMouseEvent*\0event\0"
    "mousePressSignal\0mouseReleaseSignal\0"
    "beforeReplot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlotController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a,
       4,    1,   42,    2, 0x08,
       7,    1,   45,    2, 0x08,
       8,    1,   48,    2, 0x08,
       9,    0,   51,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,

       0        // eod
};

void PlotController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlotController *_t = static_cast<PlotController *>(_o);
        switch (_id) {
        case 0: _t->handleData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->mouseMoveSignal((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: _t->mousePressSignal((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->mouseReleaseSignal((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 4: _t->beforeReplot(); break;
        default: ;
        }
    }
}

const QMetaObject PlotController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PlotController.data,
      qt_meta_data_PlotController,  qt_static_metacall, 0, 0}
};


const QMetaObject *PlotController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlotController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PlotController.stringdata))
        return static_cast<void*>(const_cast< PlotController*>(this));
    return QObject::qt_metacast(_clname);
}

int PlotController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
