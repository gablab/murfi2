/****************************************************************************
** Meta object code from reading C++ file 'frlayerlistwidget.h'
**
** Created: Thu 23. Oct 12:35:47 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/frlayerlistwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'frlayerlistwidget.h' doesn't include <QObject>."
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
      20,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      30,   18,   18,   18, 0x05,
      44,   18,   18,   18, 0x05,
      61,   58,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      80,   18,   18,   18, 0x08,
     101,   18,   18,   18, 0x08,
     116,   18,   18,   18, 0x08,
     134,   18,   18,   18, 0x08,
     158,  152,   18,   18, 0x08,
     194,  183,   18,   18, 0x08,
     223,  217,   18,   18, 0x08,
     245,   18,   18,   18, 0x08,
     271,  265,   18,   18, 0x08,
     301,  265,   18,   18, 0x08,
     332,  265,   18,   18, 0x08,
     360,  265,   18,   18, 0x08,
     389,  265,   18,   18, 0x08,
     417,  265,   18,   18, 0x08,
     446,  265,   18,   18, 0x08,
     478,  265,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrLayerListWidget[] = {
    "FrLayerListWidget\0\0NewLayer()\0"
    "DeleteLayer()\0ChangeLayer()\0id\0"
    "LayerSelected(int)\0OnSelectionChanged()\0"
    "OnAddClicked()\0OnDeleteClicked()\0"
    "OnChangeClicked()\0state\0"
    "OnVisibilityChanged(int)\0row,column\0"
    "OnNameChanged(int,int)\0index\0"
    "onComboBoxChange(int)\0onColorBtnClicked()\0"
    "value\0SetOpacitySliderPosition(int)\0"
    "SetOpacitySpinBoxPosition(int)\0"
    "SetPxMinSliderPosition(int)\0"
    "SetPxMinSpinBoxPosition(int)\0"
    "SetPxMaxSliderPosition(int)\0"
    "SetPxMaxSpinBoxPosition(int)\0"
    "SetThresholdSliderPosition(int)\0"
    "SetThresholdSpinBoxPosition(int)\0"
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
        case 4: OnSelectionChanged(); break;
        case 5: OnAddClicked(); break;
        case 6: OnDeleteClicked(); break;
        case 7: OnChangeClicked(); break;
        case 8: OnVisibilityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: OnNameChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: onComboBoxChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: onColorBtnClicked(); break;
        case 12: SetOpacitySliderPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: SetOpacitySpinBoxPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: SetPxMinSliderPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: SetPxMinSpinBoxPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: SetPxMaxSliderPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: SetPxMaxSpinBoxPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: SetThresholdSliderPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: SetThresholdSpinBoxPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 20;
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
