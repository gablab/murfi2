/****************************************************************************
** Meta object code from reading C++ file 'FrLayerListWidget.h'
**
** Created: Mon 25. May 16:10:23 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrLayerListWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrLayerListWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrLayerListWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      30,   18,   18,   18, 0x05,
      44,   18,   18,   18, 0x05,
      61,   58,   18,   18, 0x05,
      82,   18,   18,   18, 0x05,
     101,   97,   18,   18, 0x05,
     126,   18,   18,   18, 0x05,
     146,   18,   18,   18, 0x05,
     168,   18,   18,   18, 0x05,
     185,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     211,  203,   18,   18, 0x08,
     234,   18,   18,   18, 0x08,
     249,   18,   18,   18, 0x08,
     267,   18,   18,   18, 0x08,
     285,   18,   18,   18, 0x08,
     304,   18,   18,   18, 0x08,
     325,   58,   18,   18, 0x08,
     358,  352,   18,   18, 0x08,
     380,   18,   18,   18, 0x08,
     406,   18,   18,   18, 0x08,
     425,   18,   18,   18, 0x08,
     451,   18,   18,   18, 0x08,
     478,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrLayerListWidget[] = {
    "FrLayerListWidget\0\0NewLayer()\0"
    "DeleteLayer()\0ChangeLayer()\0id\0"
    "LayerSelected(ulong)\0UpdateSignal()\0"
    "inc\0ChangeLayerPosition(int)\0"
    "ChangeLayerParams()\0ChangeLayerColormap()\0"
    "RoiToolChanged()\0ClearCurrentRoi()\0"
    "row,col\0OnCellClicked(int,int)\0"
    "OnAddClicked()\0OnDeleteClicked()\0"
    "OnChangeClicked()\0OnUpArrowClicked()\0"
    "OnDownArrowClicked()\0OnVisibilityChanged(ulong)\0"
    "value\0OnOpacityChanged(int)\0"
    "OnColormapParamsChanged()\0OnRoiToolChanged()\0"
    "OnClearCurrentRoiAction()\0"
    "UpdateCurrentLayerParams()\0OnUpdate()\0"
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
        case 3: LayerSelected((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 4: UpdateSignal(); break;
        case 5: ChangeLayerPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: ChangeLayerParams(); break;
        case 7: ChangeLayerColormap(); break;
        case 8: RoiToolChanged(); break;
        case 9: ClearCurrentRoi(); break;
        case 10: OnCellClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: OnAddClicked(); break;
        case 12: OnDeleteClicked(); break;
        case 13: OnChangeClicked(); break;
        case 14: OnUpArrowClicked(); break;
        case 15: OnDownArrowClicked(); break;
        case 16: OnVisibilityChanged((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 17: OnOpacityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: OnColormapParamsChanged(); break;
        case 19: OnRoiToolChanged(); break;
        case 20: OnClearCurrentRoiAction(); break;
        case 21: UpdateCurrentLayerParams(); break;
        case 22: OnUpdate(); break;
        }
        _id -= 23;
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
void FrLayerListWidget::LayerSelected(unsigned long _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FrLayerListWidget::UpdateSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void FrLayerListWidget::ChangeLayerPosition(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void FrLayerListWidget::ChangeLayerParams()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void FrLayerListWidget::ChangeLayerColormap()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void FrLayerListWidget::RoiToolChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void FrLayerListWidget::ClearCurrentRoi()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}
