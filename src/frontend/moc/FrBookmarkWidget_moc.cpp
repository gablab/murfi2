/****************************************************************************
** Meta object code from reading C++ file 'FrBookmarkWidget.h'
**
** Created: Thu 11. Sep 02:51:28 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Widgets/FrBookmarkWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FrBookmarkWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FrBookmarkWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x0a,
      41,   32,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FrBookmarkWidget[] = {
    "FrBookmarkWidget\0\0addBookmark()\0"
    "bookmark\0bookmarkClicked(FrBookmark&)\0"
};

const QMetaObject FrBookmarkWidget::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_FrBookmarkWidget,
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
    return QTabWidget::qt_metacast(_clname);
}

int FrBookmarkWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addBookmark(); break;
        case 1: bookmarkClicked((*reinterpret_cast< FrBookmark(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
