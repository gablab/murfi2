/****************************************************************************
** Meta object code from reading C++ file 'FrLayerDialog.h'
**
** Created: Thu 30. Oct 23:03:34 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrLayerDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrLayerDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrLayerDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrLayerDialog[] = {
    "FrLayerDialog\0\0OnBtnOKClicked()\0"
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
        case 0: OnBtnOKClicked(); break;
        }
        _id -= 1;
    }
    return _id;
}
