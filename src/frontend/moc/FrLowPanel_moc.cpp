/****************************************************************************
** Meta object code from reading C++ file 'FrLowPanel.h'
**
** Created: Tue 12. Aug 22:37:08 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FrToolsPanel/FrLowPanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrLowPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrLowPanel[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x05,
      46,   12,   11,   11, 0x05,
      72,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      99,   11,   11,   11, 0x0a,
     133,   11,   11,   11, 0x0a,
     165,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrLowPanel[] = {
    "FrLowPanel\0\0value\0brightnessValueChanged(int)\0"
    "contrastValueChanged(int)\0"
    "thresholdValueChanged(int)\0"
    "brightnessSliderPositionChanged()\0"
    "contrastSliderPositionChanged()\0"
    "thresholdSliderPositionChanged()\0"
};

const QMetaObject FrLowPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrLowPanel,
      qt_meta_data_FrLowPanel, 0 }
};

const QMetaObject *FrLowPanel::metaObject() const
{
    return &staticMetaObject;
}

void *FrLowPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrLowPanel))
	return static_cast<void*>(const_cast< FrLowPanel*>(this));
    if (!strcmp(_clname, "Ui::LowPanel"))
	return static_cast< Ui::LowPanel*>(const_cast< FrLowPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrLowPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: brightnessValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: contrastValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: thresholdValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: brightnessSliderPositionChanged(); break;
        case 4: contrastSliderPositionChanged(); break;
        case 5: thresholdSliderPositionChanged(); break;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void FrLowPanel::brightnessValueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FrLowPanel::contrastValueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FrLowPanel::thresholdValueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
