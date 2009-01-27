/****************************************************************************
** Meta object code from reading C++ file 'FrCreateRoiDialog.h'
**
** Created: Tue 27. Jan 17:30:29 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrCreateRoiDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrCreateRoiDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrCreateRoiDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      23,   19,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrCreateRoiDialog[] = {
    "FrCreateRoiDialog\0\0idx\0OnActionIndexChanged(int)\0"
};

const QMetaObject FrCreateRoiDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FrCreateRoiDialog,
      qt_meta_data_FrCreateRoiDialog, 0 }
};

const QMetaObject *FrCreateRoiDialog::metaObject() const
{
    return &staticMetaObject;
}

void *FrCreateRoiDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrCreateRoiDialog))
        return static_cast<void*>(const_cast< FrCreateRoiDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int FrCreateRoiDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnActionIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
