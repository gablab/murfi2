/****************************************************************************
** Meta object code from reading C++ file 'FrGraphBookmarkWidget.h'
**
** Created: Mon 4. May 16:33:57 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrGraphBookmarkWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrGraphBookmarkWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FrGraphBookmarkWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      29,   23,   22,   22, 0x05,
      46,   23,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      68,   22,   22,   22, 0x0a,
      96,   91,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrGraphBookmarkWidget[] = {
    "FrGraphBookmarkWidget\0\0tabID\0"
    "DeleteTab(ulong)\0CurrentChanged(ulong)\0"
    "OnCloseButtonClicked()\0page\0"
    "OnCurrentChanged(QWidget*)\0"
};

const QMetaObject FrGraphBookmarkWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FrGraphBookmarkWidget,
      qt_meta_data_FrGraphBookmarkWidget, 0 }
};

const QMetaObject *FrGraphBookmarkWidget::metaObject() const
{
    return &staticMetaObject;
}

void *FrGraphBookmarkWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FrGraphBookmarkWidget))
	return static_cast<void*>(const_cast< FrGraphBookmarkWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FrGraphBookmarkWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: DeleteTab((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 1: CurrentChanged((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 2: OnCloseButtonClicked(); break;
        case 3: OnCurrentChanged((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void FrGraphBookmarkWidget::DeleteTab(unsigned long _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FrGraphBookmarkWidget::CurrentChanged(unsigned long _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
