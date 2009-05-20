/****************************************************************************
** Meta object code from reading C++ file 'FrDataStoreDialog.h'
**
** Created: Wed May 20 10:16:10 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrDataStoreDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrDataStoreDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrDataStoreDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      31,   19,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrDataStoreDialog[] = {
    "FrDataStoreDialog\0\0item,column\0"
    "SelectID(QTreeWidgetItem*,int)\0"
};

const QMetaObject FrDataStoreDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FrDataStoreDialog,
      qt_meta_data_FrDataStoreDialog, 0 }
};

const QMetaObject *FrDataStoreDialog::metaObject() const
{
    return &staticMetaObject;
}

void *FrDataStoreDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrDataStoreDialog))
        return static_cast<void*>(const_cast< FrDataStoreDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int FrDataStoreDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SelectID((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
