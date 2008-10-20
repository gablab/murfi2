/****************************************************************************
** Meta object code from reading C++ file 'FrMainWindow.h'
**
** Created: Mon 20. Oct 17:01:35 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FrMainWindow/FrMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrMainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   14,   13,   13, 0x0a,
      40,   14,   13,   13, 0x0a,
      62,   14,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrMainWindow[] = {
    "FrMainWindow\0\0id\0OnBookmarkChanged(int)\0"
    "OnBookmarkDelete(int)\0OnLayerSelected(int)\0"
};

const QMetaObject FrMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FrMainWindow,
      qt_meta_data_FrMainWindow, 0 }
};

const QMetaObject *FrMainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *FrMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrMainWindow))
	return static_cast<void*>(const_cast< FrMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FrMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnBookmarkChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: OnBookmarkDelete((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: OnLayerSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}
