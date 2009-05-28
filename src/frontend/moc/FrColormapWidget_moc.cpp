/****************************************************************************
** Meta object code from reading C++ file 'FrColormapWidget.h'
**
** Created: Mon 25. May 16:10:24 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrColormapWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrColormapWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrColormapWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      34,   17,   17,   17, 0x08,
      56,   17,   17,   17, 0x08,
      86,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrColormapWidget[] = {
    "FrColormapWidget\0\0ParamsChanged()\0"
    "OnComboBoxChange(int)\0"
    "OnSpinSliderValueChanged(int)\0"
    "OnBtnColorClicked()\0"
};

const QMetaObject FrColormapWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrColormapWidget,
      qt_meta_data_FrColormapWidget, 0 }
};

const QMetaObject *FrColormapWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrColormapWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrColormapWidget))
	return static_cast<void*>(const_cast< FrColormapWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrColormapWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ParamsChanged(); break;
        case 1: OnComboBoxChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: OnSpinSliderValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: OnBtnColorClicked(); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void FrColormapWidget::ParamsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
