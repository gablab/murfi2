/****************************************************************************
** Meta object code from reading C++ file 'FrGraphPaneWidget.h'
**
<<<<<<< .mine
** Created: Mon 22. Dec 17:50:11 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
=======
** Created: Fri 19. Dec 16:12:14 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
>>>>>>> .r361
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrGraphPaneWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrGraphPaneWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrGraphPaneWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      29,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   19,   18,   18, 0x08,
      81,   72,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrGraphPaneWidget[] = {
    "FrGraphPaneWidget\0\0timePoint\0"
    "TimePointChanged(int)\0OnPlayTimePoint(int)\0"
    "position\0OnGraphMarkerPositionChanged(int)\0"
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
        case 1: OnPlayTimePoint((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: OnGraphMarkerPositionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FrGraphPaneWidget::TimePointChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
