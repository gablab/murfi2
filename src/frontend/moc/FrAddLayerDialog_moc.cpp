/****************************************************************************
** Meta object code from reading C++ file 'FrAddLayerDialog.h'
**
** Created: Tue 7. Oct 20:01:10 2008
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
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x0a,
      46,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrAddLayerDialog[] = {
    "FrAddLayerDialog\0\0index\0onComboBoxChange(int)\0"
    "onColorBtnClicked()\0"
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
        }
        _id -= 2;
    }
    return _id;
}
