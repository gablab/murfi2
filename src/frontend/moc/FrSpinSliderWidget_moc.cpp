/****************************************************************************
** Meta object code from reading C++ file 'FrSpinSliderWidget.h'
**
** Created: Fri 21. Nov 13:58:55 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrSpinSliderWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrSpinSliderWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrSpinSliderWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      29,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   20,   19,   19, 0x08,
      71,   20,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrSpinSliderWidget[] = {
    "FrSpinSliderWidget\0\0newValue\0"
    "ValueChanged(int)\0OnSpinValueChanged(int)\0"
    "OnSliderValueChanged(int)\0"
};

const QMetaObject FrSpinSliderWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrSpinSliderWidget,
      qt_meta_data_FrSpinSliderWidget, 0 }
};

const QMetaObject *FrSpinSliderWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrSpinSliderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrSpinSliderWidget))
	return static_cast<void*>(const_cast< FrSpinSliderWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrSpinSliderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: OnSpinValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: OnSliderValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FrSpinSliderWidget::ValueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
