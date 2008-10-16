/****************************************************************************
** Meta object code from reading C++ file 'FrLayerDialog.h'
**
** Created: Thu 16. Oct 21:00:35 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrLayerDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrLayerDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrLayerDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x0a,
      43,   14,   14,   14, 0x0a,
      69,   63,   14,   14, 0x0a,
      99,   63,   14,   14, 0x0a,
     130,   63,   14,   14, 0x0a,
     158,   63,   14,   14, 0x0a,
     187,   63,   14,   14, 0x0a,
     215,   63,   14,   14, 0x0a,
     244,   63,   14,   14, 0x0a,
     276,   63,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrLayerDialog[] = {
    "FrLayerDialog\0\0index\0onComboBoxChange(int)\0"
    "onColorBtnClicked()\0value\0"
    "SetOpacitySliderPosition(int)\0"
    "SetOpacitySpinBoxPosition(int)\0"
    "SetPxMinSliderPosition(int)\0"
    "SetPxMinSpinBoxPosition(int)\0"
    "SetPxMaxSliderPosition(int)\0"
    "SetPxMaxSpinBoxPosition(int)\0"
    "SetThresholdSliderPosition(int)\0"
    "SetThresholdSpinBoxPosition(int)\0"
};

const QMetaObject FrLayerDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FrLayerDialog,
      qt_meta_data_FrLayerDialog, 0 }
};

const QMetaObject *FrLayerDialog::metaObject() const
{
    return &staticMetaObject;
}

void *FrLayerDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrLayerDialog))
        return static_cast<void*>(const_cast< FrLayerDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int FrLayerDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onComboBoxChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: onColorBtnClicked(); break;
        case 2: SetOpacitySliderPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: SetOpacitySpinBoxPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: SetPxMinSliderPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: SetPxMinSpinBoxPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: SetPxMaxSliderPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: SetPxMaxSpinBoxPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: SetThresholdSliderPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: SetThresholdSpinBoxPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
