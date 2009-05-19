/****************************************************************************
** Meta object code from reading C++ file 'FrMainWindow.h'
**
** Created: Mon May 18 12:31:44 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FrMainWindow/FrMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrMainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   14,   13,   13, 0x0a,
      42,   14,   13,   13, 0x0a,
      66,   14,   13,   13, 0x0a,
      96,   14,   13,   13, 0x0a,
     125,   14,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrMainWindow[] = {
    "FrMainWindow\0\0id\0OnBookmarkChanged(ulong)\0"
    "OnBookmarkDelete(ulong)\0"
    "OnGraphBookmarkChanged(ulong)\0"
    "OnGraphBookmarkDelete(ulong)\0"
    "OnLayerSelected(ulong)\0"
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
        case 0: OnBookmarkChanged((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 1: OnBookmarkDelete((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 2: OnGraphBookmarkChanged((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 3: OnGraphBookmarkDelete((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 4: OnLayerSelected((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
