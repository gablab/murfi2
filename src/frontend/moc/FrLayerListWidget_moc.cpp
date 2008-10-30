/****************************************************************************
** Meta object code from reading C++ file 'frlayerlistwidget.h'
**
** Created: Wed 29. Oct 23:40:37 2008
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
      14,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      30,   18,   18,   18, 0x05,
      44,   18,   18,   18, 0x05,
      61,   58,   18,   18, 0x05,
      80,   18,   18,   18, 0x05,
     100,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     122,   18,   18,   18, 0x08,
     143,   18,   18,   18, 0x08,
     158,   18,   18,   18, 0x08,
     176,   18,   18,   18, 0x08,
     194,   58,   18,   18, 0x08,
     225,  219,   18,   18, 0x08,
     247,   18,   18,   18, 0x08,
     273,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrLayerListWidget[] = {
    "FrLayerListWidget\0\0NewLayer()\0"
    "DeleteLayer()\0ChangeLayer()\0id\0"
    "LayerSelected(int)\0ChangeLayerParams()\0"
    "ChangeLayerColormap()\0OnSelectionChanged()\0"
    "OnAddClicked()\0OnDeleteClicked()\0"
    "OnChangeClicked()\0OnVisibilityChanged(int)\0"
    "value\0OnOpacityChanged(int)\0"
    "OnColormapParamsChanged()\0"
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
        case 6: OnSelectionChanged(); break;
        case 7: OnAddClicked(); break;
        case 8: OnDeleteClicked(); break;
        case 9: OnChangeClicked(); break;
        case 10: OnVisibilityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: OnOpacityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: OnColormapParamsChanged(); break;
        case 13: UpdateCurrentLayerParams(); break;
        }
        _id -= 14;
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
