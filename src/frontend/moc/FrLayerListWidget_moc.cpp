/****************************************************************************
** Meta object code from reading C++ file 'FrLayerListWidget.h'
**
** Created: Thu 20. Nov 17:50:17 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrLayerListWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrLayerListWidget.h' doesn't include <QObject>."
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
      16,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      30,   18,   18,   18, 0x05,
      44,   18,   18,   18, 0x05,
      61,   58,   18,   18, 0x05,
      80,   18,   18,   18, 0x05,
     100,   18,   18,   18, 0x05,
     122,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     147,  139,   18,   18, 0x08,
     170,   18,   18,   18, 0x08,
     185,   18,   18,   18, 0x08,
     203,   18,   18,   18, 0x08,
     221,   58,   18,   18, 0x08,
     252,  246,   18,   18, 0x08,
     274,   18,   18,   18, 0x08,
     300,   18,   18,   18, 0x08,
     319,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrLayerListWidget[] = {
    "FrLayerListWidget\0\0NewLayer()\0"
    "DeleteLayer()\0ChangeLayer()\0id\0"
    "LayerSelected(int)\0ChangeLayerParams()\0"
    "ChangeLayerColormap()\0RoiToolChanged()\0"
    "row,col\0OnCellClicked(int,int)\0"
    "OnAddClicked()\0OnDeleteClicked()\0"
    "OnChangeClicked()\0OnVisibilityChanged(int)\0"
    "value\0OnOpacityChanged(int)\0"
    "OnColormapParamsChanged()\0OnRoiToolChanged()\0"
    "UpdateCurrentLayerParams()\0"
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
        case 0: NewLayer(); break;
        case 1: DeleteLayer(); break;
        case 2: ChangeLayer(); break;
        case 3: LayerSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: ChangeLayerParams(); break;
        case 5: ChangeLayerColormap(); break;
        case 6: RoiToolChanged(); break;
        case 7: OnCellClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: OnAddClicked(); break;
        case 9: OnDeleteClicked(); break;
        case 10: OnChangeClicked(); break;
        case 11: OnVisibilityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: OnOpacityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: OnColormapParamsChanged(); break;
        case 14: OnRoiToolChanged(); break;
        case 15: UpdateCurrentLayerParams(); break;
        }
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void FrLayerListWidget::NewLayer()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void FrLayerListWidget::DeleteLayer()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void FrLayerListWidget::ChangeLayer()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void FrLayerListWidget::LayerSelected(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FrLayerListWidget::ChangeLayerParams()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void FrLayerListWidget::ChangeLayerColormap()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void FrLayerListWidget::RoiToolChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
