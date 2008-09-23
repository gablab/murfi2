/****************************************************************************
** Meta object code from reading C++ file 'FrLowPanel.h'
**
** Created: Tue 23. Sep 20:14:11 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrLowPanel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrLowPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrLowPanel[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      46,   11,   11,   11, 0x0a,
      78,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrLowPanel[] = {
    "FrLowPanel\0\0brightnessSliderPositionChanged()\0"
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
        case 0: brightnessSliderPositionChanged(); break;
        case 1: contrastSliderPositionChanged(); break;
        case 2: thresholdSliderPositionChanged(); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
