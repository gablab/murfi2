/****************************************************************************
** Meta object code from reading C++ file 'FrMainWindow.h'
**
** Created: Wed 13. Aug 00:10:10 2008
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
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x0a,
      48,   14,   13,   13, 0x0a,
      74,   14,   13,   13, 0x0a,
     101,   13,   13,   13, 0x0a,
     118,   13,   13,   13, 0x0a,
     135,   13,   13,   13, 0x0a,
     152,   13,   13,   13, 0x0a,
     167,   13,   13,   13, 0x0a,
     182,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrMainWindow[] = {
    "FrMainWindow\0\0value\0brightnessValueChanged(int)\0"
    "contrastValueChanged(int)\0"
    "thresholdValueChanged(int)\0tool1Triggered()\0"
    "tool2Triggered()\0tool3Triggered()\0"
    "mode1Clicked()\0mode2Clicked()\0"
    "mode3Clicked()\0"
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
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: brightnessValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: contrastValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: thresholdValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: tool1Triggered(); break;
        case 4: tool2Triggered(); break;
        case 5: tool3Triggered(); break;
        case 6: mode1Clicked(); break;
        case 7: mode2Clicked(); break;
        case 8: mode3Clicked(); break;
        }
        _id -= 9;
    }
    return _id;
}
