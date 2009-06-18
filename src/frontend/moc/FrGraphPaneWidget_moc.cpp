/****************************************************************************
** Meta object code from reading C++ file 'FrGraphPaneWidget.h'
**
** Created: Wed Jun 17 23:04:42 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrGraphPaneWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrGraphPaneWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrGraphPaneWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      26,   19,   18,   18, 0x05,
      51,   18,   18,   18, 0x05,
      69,   66,   18,   18, 0x05,
      84,   66,   18,   18, 0x05,
     117,  104,   18,   18, 0x05,
     158,  148,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     189,  180,   18,   18, 0x08,
     223,   18,   18,   18, 0x08,
     234,   18,   18,   18, 0x08,
     266,  255,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrGraphPaneWidget[] = {
    "FrGraphPaneWidget\0\0id,add\0"
    "GraphChanged(ulong,bool)\0UpdateSignal()\0"
    "id\0Clicked(ulong)\0DeleteWidget(ulong)\0"
    "gID,tsID,add\0GraphChanged(ulong,ulong,bool)\0"
    "timePoint\0TimePointChanged(int)\0"
    "position\0OnGraphMarkerPositionChanged(int)\0"
    "OnUpdate()\0OnDeleteBtnClicked()\0"
    "id,checked\0itemChecked(ulong,bool)\0"
};

const QMetaObject FrGraphPaneWidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_FrGraphPaneWidget,
      qt_meta_data_FrGraphPaneWidget, 0 }
};

const QMetaObject *FrGraphPaneWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrGraphPaneWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrGraphPaneWidget))
        return static_cast<void*>(const_cast< FrGraphPaneWidget*>(this));
    return QFrame::qt_metacast(_clname);
}

int FrGraphPaneWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: GraphChanged((*reinterpret_cast< ulong(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: UpdateSignal(); break;
        case 2: Clicked((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 3: DeleteWidget((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 4: GraphChanged((*reinterpret_cast< ulong(*)>(_a[1])),(*reinterpret_cast< ulong(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 5: TimePointChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: OnGraphMarkerPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: OnUpdate(); break;
        case 8: OnDeleteBtnClicked(); break;
        case 9: itemChecked((*reinterpret_cast< ulong(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void FrGraphPaneWidget::GraphChanged(unsigned long _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FrGraphPaneWidget::UpdateSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void FrGraphPaneWidget::Clicked(unsigned long _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FrGraphPaneWidget::DeleteWidget(unsigned long _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FrGraphPaneWidget::GraphChanged(unsigned long _t1, unsigned long _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void FrGraphPaneWidget::TimePointChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
