/****************************************************************************
** Meta object code from reading C++ file 'FrImageSettingsWidget.h'
**
** Created: Wed 28. Jan 18:45:43 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrImageSettingsWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrImageSettingsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrImageSettingsWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,
      44,   22,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      65,   59,   22,   22, 0x08,
      95,   22,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrImageSettingsWidget[] = {
    "FrImageSettingsWidget\0\0ImageParamsChanged()\0"
    "UpdateSignal()\0value\0OnSpinSliderValueChanged(int)\0"
    "OnUpdate()\0"
};

const QMetaObject FrImageSettingsWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrImageSettingsWidget,
      qt_meta_data_FrImageSettingsWidget, 0 }
};

const QMetaObject *FrImageSettingsWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrImageSettingsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrImageSettingsWidget))
	return static_cast<void*>(const_cast< FrImageSettingsWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrImageSettingsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ImageParamsChanged(); break;
        case 1: UpdateSignal(); break;
        case 2: OnSpinSliderValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: OnUpdate(); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void FrImageSettingsWidget::ImageParamsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void FrImageSettingsWidget::UpdateSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
