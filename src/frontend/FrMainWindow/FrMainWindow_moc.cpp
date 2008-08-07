/****************************************************************************
** Meta object code from reading C++ file 'frmainwindow.h'
**
** Created: Fri 8. Aug 00:47:26 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "frmainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'frmainwindow.h' doesn't include <QObject>."
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
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_FrMainWindow[] = {
    "FrMainWindow\0"
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
    if (!strcmp(_clname, "Ui::MainWindow"))
	return static_cast< Ui::MainWindow*>(const_cast< FrMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FrMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
