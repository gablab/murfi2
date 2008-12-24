/****************************************************************************
** Meta object code from reading C++ file 'FrQwtPlotWidget.h'
**
** Created: Wed Dec 24 17:47:44 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrQwtPlotWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrQwtPlotWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrQwtPlotWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   17,   16,   16, 0x05,
      50,   41,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      82,   76,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrQwtPlotWidget[] = {
    "FrQwtPlotWidget\0\0p\0pointClicked(QPointF)\0"
    "position\0markerPositionChange(int)\0"
    "point\0onPointClicked(QwtDoublePoint)\0"
};

const QMetaObject FrQwtPlotWidget::staticMetaObject = {
    { &QwtPlot::staticMetaObject, qt_meta_stringdata_FrQwtPlotWidget,
      qt_meta_data_FrQwtPlotWidget, 0 }
};

const QMetaObject *FrQwtPlotWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrQwtPlotWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrQwtPlotWidget))
	return static_cast<void*>(const_cast< FrQwtPlotWidget*>(this));
    return QwtPlot::qt_metacast(_clname);
}

int FrQwtPlotWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QwtPlot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: pointClicked((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 1: markerPositionChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: onPointClicked((*reinterpret_cast< const QwtDoublePoint(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FrQwtPlotWidget::pointClicked(QPointF _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FrQwtPlotWidget::markerPositionChange(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
