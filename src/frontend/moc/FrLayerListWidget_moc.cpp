/****************************************************************************
** Meta object code from reading C++ file 'FrLayerListWidget.h'
**
** Created: Mon Mar 2 12:19:42 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrLayerListWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrLayerListWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrLayerListWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      30,   18,   18,   18, 0x05,
      44,   18,   18,   18, 0x05,
      61,   58,   18,   18, 0x05,
      82,   18,   18,   18, 0x05,
      97,   18,   18,   18, 0x05,
     117,   18,   18,   18, 0x05,
     139,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     164,  156,   18,   18, 0x08,
     187,   18,   18,   18, 0x08,
     202,   18,   18,   18, 0x08,
     220,   18,   18,   18, 0x08,
     238,   58,   18,   18, 0x08,
     271,  265,   18,   18, 0x08,
     293,   18,   18,   18, 0x08,
     319,   18,   18,   18, 0x08,
     338,   18,   18,   18, 0x08,
     365,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrLayerListWidget[] = {
    "FrLayerListWidget\0\0NewLayer()\0"
    "DeleteLayer()\0ChangeLayer()\0id\0"
    "LayerSelected(ulong)\0UpdateSignal()\0"
    "ChangeLayerParams()\0ChangeLayerColormap()\0"
    "RoiToolChanged()\0row,col\0"
    "OnCellClicked(int,int)\0OnAddClicked()\0"
    "OnDeleteClicked()\0OnChangeClicked()\0"
    "OnVisibilityChanged(ulong)\0value\0"
    "OnOpacityChanged(int)\0OnColormapParamsChanged()\0"
    "OnRoiToolChanged()\0UpdateCurrentLayerParams()\0"
    "OnUpdate()\0"
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
        case 5: ChangeLayerParams(); break;
        case 6: ChangeLayerColormap(); break;
        case 7: RoiToolChanged(); break;
        case 8: OnCellClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: OnAddClicked(); break;
        case 10: OnDeleteClicked(); break;
        case 11: OnChangeClicked(); break;
        case 12: OnVisibilityChanged((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 13: OnOpacityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: OnColormapParamsChanged(); break;
        case 15: OnRoiToolChanged(); break;
        case 16: UpdateCurrentLayerParams(); break;
        case 17: OnUpdate(); break;
        }
        _id -= 18;
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
void FrLayerListWidget::ChangeLayerParams()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void FrLayerListWidget::ChangeLayerColormap()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void FrLayerListWidget::RoiToolChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
QT_END_MOC_NAMESPACE
