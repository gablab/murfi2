/****************************************************************************
** Meta object code from reading C++ file 'FrMainWindow.h'
**
** Created: Wed 27. Aug 15:59:30 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FrMainWindow/FrMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrMainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      39,   13,   13,   13, 0x0a,
      62,   13,   13,   13, 0x0a,
      86,   13,   13,   13, 0x0a,
     103,   13,   13,   13, 0x0a,
     120,   13,   13,   13, 0x0a,
     137,   13,   13,   13, 0x0a,
     152,   13,   13,   13, 0x0a,
     167,   13,   13,   13, 0x0a,
     188,  182,   13,   13, 0x0a,
     204,  182,   13,   13, 0x0a,
     225,   13,   13,   13, 0x0a,
     237,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrMainWindow[] = {
    "FrMainWindow\0\0brightnessValueChanged()\0"
    "contrastValueChanged()\0thresholdValueChanged()\0"
    "tool1Triggered()\0tool2Triggered()\0"
    "tool3Triggered()\0mode1Clicked()\0"
    "mode2Clicked()\0mode3Clicked()\0index\0"
    "tabChanged(int)\0bookmarkChanged(int)\0"
    "saveToTab()\0openImage()\0"
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
        case 0: brightnessValueChanged(); break;
        case 1: contrastValueChanged(); break;
        case 2: thresholdValueChanged(); break;
        case 3: tool1Triggered(); break;
        case 4: tool2Triggered(); break;
        case 5: tool3Triggered(); break;
        case 6: mode1Clicked(); break;
        case 7: mode2Clicked(); break;
        case 8: mode3Clicked(); break;
        case 9: tabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: bookmarkChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: saveToTab(); break;
        case 12: openImage(); break;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
