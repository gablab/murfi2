/****************************************************************************
** Meta object code from reading C++ file 'FrAction.h'
**
** Created: Tue Jun 9 12:53:49 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Utils/FrAction.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrAction.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrAction[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      21,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   47,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrAction[] = {
    "FrAction\0\0id,checked\0actionChecked(ulong,bool)\0"
    "checked\0OnPress(bool)\0"
};

const QMetaObject FrAction::staticMetaObject = {
    { &QAction::staticMetaObject, qt_meta_stringdata_FrAction,
      qt_meta_data_FrAction, 0 }
};

const QMetaObject *FrAction::metaObject() const
{
    return &staticMetaObject;
}

void *FrAction::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrAction))
        return static_cast<void*>(const_cast< FrAction*>(this));
    return QAction::qt_metacast(_clname);
}

int FrAction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAction::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: actionChecked((*reinterpret_cast< ulong(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: OnPress((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void FrAction::actionChecked(unsigned long _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
