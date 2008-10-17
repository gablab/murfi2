/****************************************************************************
** Meta object code from reading C++ file 'frlayerlistwidget.h'
**
** Created: Fri 17. Oct 01:48:57 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/frlayerlistwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'frlayerlistwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrLayerListWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      22,   19,   18,   18, 0x05,
      37,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      67,   56,   18,   18, 0x08,
      88,   56,   18,   18, 0x08,
     115,   56,  111,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrLayerListWidget[] = {
    "FrLayerListWidget\0\0id\0LayerEdit(int)\0"
    "LayerSelected(int)\0row,column\0"
    "OnLayerEdit(int,int)\0OnLayerSelect(int,int)\0"
    "int\0GetLayerID(int,int)\0"
};

const QMetaObject FrLayerListWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrLayerListWidget,
      qt_meta_data_FrLayerListWidget, 0 }
};

const QMetaObject *FrLayerListWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrLayerListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrLayerListWidget))
        return static_cast<void*>(const_cast< FrLayerListWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrLayerListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: LayerEdit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: LayerSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: OnLayerEdit((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: OnLayerSelect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: { int _r = GetLayerID((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void FrLayerListWidget::LayerEdit(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FrLayerListWidget::LayerSelected(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
