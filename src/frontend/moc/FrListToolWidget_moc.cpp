/****************************************************************************
** Meta object code from reading C++ file 'FrListToolWidget.h'
**
** Created: Mon 25. May 16:10:23 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrListToolWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrListToolWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrListToolWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   34,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrListToolWidget[] = {
    "FrListToolWidget\0\0ParamsChanged()\0"
    "current,previous\0"
    "ROIListItemChanged(QListWidgetItem*,QListWidgetItem*)\0"
};

const QMetaObject FrListToolWidget::staticMetaObject = {
    { &FrBaseToolWidget::staticMetaObject, qt_meta_stringdata_FrListToolWidget,
      qt_meta_data_FrListToolWidget, 0 }
};

const QMetaObject *FrListToolWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrListToolWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrListToolWidget))
	return static_cast<void*>(const_cast< FrListToolWidget*>(this));
    return FrBaseToolWidget::qt_metacast(_clname);
}

int FrListToolWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = FrBaseToolWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ParamsChanged(); break;
        case 1: ROIListItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void FrListToolWidget::ParamsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
