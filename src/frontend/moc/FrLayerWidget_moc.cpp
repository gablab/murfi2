/****************************************************************************
** Meta object code from reading C++ file 'FrLayerWidget.h'
**
** Created: Sun 23. Nov 12:13:30 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrLayerWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrLayerWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrLayerWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      18,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FrLayerWidget[] = {
    "FrLayerWidget\0\0id\0VisibilityChanged(int)\0"
    "OnCheckBoxToggled()\0"
};

const QMetaObject FrLayerWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrLayerWidget,
      qt_meta_data_FrLayerWidget, 0 }
};

const QMetaObject *FrLayerWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrLayerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrLayerWidget))
	return static_cast<void*>(const_cast< FrLayerWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrLayerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: VisibilityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: OnCheckBoxToggled(); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void FrLayerWidget::VisibilityChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
