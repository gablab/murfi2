/****************************************************************************
** Meta object code from reading C++ file 'PlotController.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "PlotController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PlotController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PlotController_t {
    QByteArrayData data[10];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PlotController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PlotController_t qt_meta_stringdata_PlotController = {
    {
QT_MOC_LITERAL(0, 0, 14), // "PlotController"
QT_MOC_LITERAL(1, 15, 10), // "handleData"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 3), // "qid"
QT_MOC_LITERAL(4, 31, 15), // "mouseMoveSignal"
QT_MOC_LITERAL(5, 47, 12), // "QMouseEvent*"
QT_MOC_LITERAL(6, 60, 5), // "event"
QT_MOC_LITERAL(7, 66, 16), // "mousePressSignal"
QT_MOC_LITERAL(8, 83, 18), // "mouseReleaseSignal"
QT_MOC_LITERAL(9, 102, 12) // "beforeReplot"

    },
    "PlotController\0handleData\0\0qid\0"
    "mouseMoveSignal\0QMouseEvent*\0event\0"
    "mousePressSignal\0mouseReleaseSignal\0"
    "beforeReplot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlotController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       4,    1,   42,    2, 0x08 /* Private */,
       7,    1,   45,    2, 0x08 /* Private */,
       8,    1,   48,    2, 0x08 /* Private */,
       9,    0,   51,    2, 0x08 /* Private */,

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
        auto *_t = static_cast<PlotController *>(_o);
        (void)_t;
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

QT_INIT_METAOBJECT const QMetaObject PlotController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_PlotController.data,
    qt_meta_data_PlotController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PlotController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlotController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PlotController.stringdata0))
        return static_cast<void*>(this);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
