/****************************************************************************
** Meta object code from reading C++ file 'FrBaseToolWidget.h'
**
** Created: Tue 23. Dec 15:04:40 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrBaseToolWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrBaseToolWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrBaseToolWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_FrBaseToolWidget[] = {
    "FrBaseToolWidget\0\0ParamsChanged()\0"
};

const QMetaObject FrBaseToolWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrBaseToolWidget,
      qt_meta_data_FrBaseToolWidget, 0 }
};

const QMetaObject *FrBaseToolWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrBaseToolWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrBaseToolWidget))
	return static_cast<void*>(const_cast< FrBaseToolWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrBaseToolWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ParamsChanged(); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void FrBaseToolWidget::ParamsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
