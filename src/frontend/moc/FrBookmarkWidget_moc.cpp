/****************************************************************************
** Meta object code from reading C++ file 'FrBookmarkWidget.h'
**
** Created: Wed 12. Nov 23:53:38 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrBookmarkWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrBookmarkWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrBookmarkWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x05,
      39,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   17,   17,   17, 0x0a,
      87,   82,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrBookmarkWidget[] = {
    "FrBookmarkWidget\0\0tabID\0DeleteTab(int)\0"
    "CurrentChanged(int)\0OnCloseButtonClicked()\0"
    "page\0OnCurrentChanged(QWidget*)\0"
};

const QMetaObject FrBookmarkWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrBookmarkWidget,
      qt_meta_data_FrBookmarkWidget, 0 }
};

const QMetaObject *FrBookmarkWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrBookmarkWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrBookmarkWidget))
	return static_cast<void*>(const_cast< FrBookmarkWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrBookmarkWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: DeleteTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: CurrentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: OnCloseButtonClicked(); break;
        case 3: OnCurrentChanged((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void FrBookmarkWidget::DeleteTab(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FrBookmarkWidget::CurrentChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
