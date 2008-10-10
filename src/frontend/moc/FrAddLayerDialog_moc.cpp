/****************************************************************************
** Meta object code from reading C++ file 'FrAddLayerDialog.h'
**
** Created: Thu 9. Oct 23:08:31 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrAddLayerDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrAddLayerDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrAddLayerDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x0a,
      46,   17,   17,   17, 0x0a,
      72,   66,   17,   17, 0x0a,
     102,   66,   17,   17, 0x0a,
     133,   66,   17,   17, 0x0a,
     161,   66,   17,   17, 0x0a,
     190,   66,   17,   17, 0x0a,
     218,   66,   17,   17, 0x0a,
     247,   66,   17,   17, 0x0a,
     279,   66,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrAddLayerDialog[] = {
    "FrAddLayerDialog\0\0index\0onComboBoxChange(int)\0"
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

const QMetaObject FrAddLayerDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FrAddLayerDialog,
      qt_meta_data_FrAddLayerDialog, 0 }
};

const QMetaObject *FrAddLayerDialog::metaObject() const
{
    return &staticMetaObject;
}

void *FrAddLayerDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrAddLayerDialog))
	return static_cast<void*>(const_cast< FrAddLayerDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int FrAddLayerDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
