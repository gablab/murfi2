/****************************************************************************
** Meta object code from reading C++ file 'FrGraphListWidget.h'
**
** Created: Tue 25. Nov 19:52:55 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrGraphListWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrGraphListWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrGraphListWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      22,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   19,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrGraphListWidget[] = {
    "FrGraphListWidget\0\0id\0GraphVisibilityChanged(int)\0"
    "OnGraphVisibilityChanged(int)\0"
};

const QMetaObject FrGraphListWidget::staticMetaObject = {
    { &QScrollArea::staticMetaObject, qt_meta_stringdata_FrGraphListWidget,
      qt_meta_data_FrGraphListWidget, 0 }
};

const QMetaObject *FrGraphListWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrGraphListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrGraphListWidget))
	return static_cast<void*>(const_cast< FrGraphListWidget*>(this));
    return QScrollArea::qt_metacast(_clname);
}

int FrGraphListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: GraphVisibilityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: OnGraphVisibilityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void FrGraphListWidget::GraphVisibilityChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
