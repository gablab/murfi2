/****************************************************************************
** Meta object code from reading C++ file 'FrGraphPaneWidget.h'
**
** Created: Fri Feb 6 15:34:37 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrGraphPaneWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrGraphPaneWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrGraphPaneWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      29,   19,   18,   18, 0x05,
      57,   51,   18,   18, 0x05,
      79,   18,   18,   18, 0x05,
      99,   18,   18,   18, 0x05,
     122,  115,   18,   18, 0x05,
     145,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     169,  160,   18,   18, 0x08,
     203,   18,   18,   18, 0x08,
     227,   18,   18,   18, 0x08,
     243,   18,   18,   18, 0x08,
     260,   18,   18,   18, 0x08,
     277,   18,   18,   18, 0x08,
     294,   18,   18,   18, 0x08,
     314,   18,   18,   18, 0x08,
     330,   18,   18,   18, 0x08,
     352,  341,   18,   18, 0x08,
     382,  374,   18,   18, 0x08,
     400,  374,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrGraphPaneWidget[] = {
    "FrGraphPaneWidget\0\0timePoint\0"
    "TimePointChanged(int)\0value\0"
    "LiveModeChanged(bool)\0PreviousTimePoint()\0"
    "NextTimePoint()\0id,add\0GraphChanged(int,bool)\0"
    "UpdateSignal()\0position\0"
    "OnGraphMarkerPositionChanged(int)\0"
    "OnLiveModeChanged(bool)\0OnPlayClicked()\0"
    "OnPauseClicked()\0OnResetClicked()\0"
    "OnPlayFinished()\0OnPreviousClicked()\0"
    "OnNextClicked()\0OnUpdate()\0id,checked\0"
    "itemChecked(int,bool)\0checked\0"
    "ShowRoiMean(bool)\0ShowRoiSTD(bool)\0"
};

const QMetaObject FrGraphPaneWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrGraphPaneWidget,
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
    return QWidget::qt_metacast(_clname);
}

int FrGraphPaneWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: TimePointChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: LiveModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: PreviousTimePoint(); break;
        case 3: NextTimePoint(); break;
        case 4: GraphChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: UpdateSignal(); break;
        case 6: OnGraphMarkerPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: OnLiveModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: OnPlayClicked(); break;
        case 9: OnPauseClicked(); break;
        case 10: OnResetClicked(); break;
        case 11: OnPlayFinished(); break;
        case 12: OnPreviousClicked(); break;
        case 13: OnNextClicked(); break;
        case 14: OnUpdate(); break;
        case 15: itemChecked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 16: ShowRoiMean((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: ShowRoiSTD((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void FrGraphPaneWidget::TimePointChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FrGraphPaneWidget::LiveModeChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FrGraphPaneWidget::PreviousTimePoint()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void FrGraphPaneWidget::NextTimePoint()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void FrGraphPaneWidget::GraphChanged(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void FrGraphPaneWidget::UpdateSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
